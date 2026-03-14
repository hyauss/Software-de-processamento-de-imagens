// Copyright (c) 2026 Andre Kishimoto - https://kishimoto.com.br/
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

//------------------------------------------------------------------------------
void shutdown(void)
{
  SDL_Log("shutdown()");
  SDL_Quit();
}

//------------------------------------------------------------------------------

// void invert_image(SDL_Renderer *renderer, MyImage *image)
// {
//   SDL_Log(">>> invert_image()");

//   if (!renderer)
//   {
//     SDL_Log("\t*** Erro: Renderer inválido (renderer == NULL).");
//     SDL_Log("<<< invert_image()");
//     return;
//   }

//   if (!image || !image->surface)
//   {
//     SDL_Log("\t*** Erro: Imagem inválida (image == NULL ou image->surface == NULL).");
//     SDL_Log("<<< invert_image()");
//     return;
//   }

//   // Para acessar os pixels de uma superfície, precisamos chamar essa função.
//   SDL_LockSurface(image->surface);

//   const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(image->surface->format);
//   const size_t pixelCount = image->surface->w * image->surface->h;

//   Uint32 *pixels = (Uint32 *)image->surface->pixels;
//   Uint8 r = 0;
//   Uint8 g = 0;
//   Uint8 b = 0;
//   Uint8 a = 0;

//   for (size_t i = 0; i < pixelCount; ++i)
//   {
//     SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);

//     r = 255 - r;
//     g = 255 - g;
//     b = 255 - b;

//     pixels[i] = SDL_MapRGBA(format, NULL, r, g, b, a);
//   }

//   // Após manipularmos os pixels da superfície, liberamos a superfície.
//   SDL_UnlockSurface(image->surface);

//   // Atualizamos a textura a ser renderizada pelo SDL_Renderer, com base no
//   // novo conteúdo da superfície.
//   SDL_DestroyTexture(image->texture);
//   image->texture = SDL_CreateTextureFromSurface(renderer, image->surface);

//   SDL_Log("<<< invert_image()");
// }

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  atexit(shutdown);

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Erro ao iniciar a SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  const char *WINDOW_TITLE = "Software de processamento de imagens";
  const char *IMAGE_TEST_BMP = argv[1];
  const char *IMAGE_TEST_JPG = "assets/test.jpg";
  const char *IMAGE_TEST_PNG = "assets/test.png";
  enum constants
  {
    WINDOW_WIDTH = 640,
    WINDOW_HEIGHT = 480,
    WINDOW_TITLE_MAX_LENGTH = 64,
  };

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0,&window, &renderer))
  {
    SDL_Log("Erro ao criar a janela e/ou renderizador: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  char windowTitle[WINDOW_TITLE_MAX_LENGTH] = {0};

  SDL_Texture *texBmp = IMG_LoadTexture(renderer, IMAGE_TEST_BMP);
  if (!texBmp)
  {
    SDL_Log("Erro ao carregar a imagem '%s': %s", IMAGE_TEST_BMP, SDL_GetError());
  }
  SDL_FRect bmpRect =
      {
          .x = 0.0f,
          .y = 0.0f,
          .w = texBmp ? texBmp->w : 0.0f,
          .h = texBmp ? texBmp->h : 0.0f,
      };

  SDL_Texture *texJpg = IMG_LoadTexture(renderer, IMAGE_TEST_JPG);
  if (!texJpg)
  {
    SDL_Log("Erro ao carregar a imagem '%s': %s", IMAGE_TEST_JPG, SDL_GetError());
  }
  SDL_FRect jpgRect =
      {
          .x = bmpRect.x + bmpRect.w,
          .y = 0.0f,
          .w = texJpg ? texJpg->w : 0.0f,
          .h = texJpg ? texJpg->h : 0.0f,
      };

  SDL_Texture *texPng = IMG_LoadTexture(renderer, IMAGE_TEST_PNG);
  if (!texPng)
  {
    SDL_Log("Erro ao carregar a imagem '%s': %s", IMAGE_TEST_PNG, SDL_GetError());
  }
  SDL_FRect pngRect = {.x = jpgRect.x + jpgRect.w, .y = 0.0f};
  SDL_GetTextureSize(texPng, &pngRect.w, &pngRect.h);

  SDL_Surface *surface = IMG_Load(IMAGE_TEST_BMP);
  SDL_Surface *output_image = NULL;
  bool isGray = true;
  

  if (!surface)
  {
    SDL_Log("Erro ao carregar imagem: %s", SDL_GetError());
    return 1;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Log("Convertendo superfície para formato RGBA32...");

  output_image = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);

  SDL_DestroySurface(surface);
  surface = output_image;

  const SDL_PixelFormatDetails *pixelDetails = SDL_GetPixelFormatDetails(surface->format);

  if (!surface)
  {
    SDL_Log("Erro ao converter superfície: %s", SDL_GetError());
    return 1;
  }

  Uint8 r, g, b;

  for (int y = 0; y < surface->h && isGray; y++)
  {
    for (int x = 0; x < surface->w && isGray; x++)
    {

      Uint32 *pixels = (Uint32 *)surface->pixels;
      Uint32 pixel = pixels[y * surface->w + x];

      SDL_GetRGB(pixel, pixelDetails, NULL, &r, &g, &b);

      if (!(r == g && g == b))
      {
        isGray = false;
      }
    }
  }

  printf("A imagem escolhida é %d para escala de cinza.", isGray);

  if (isGray == false)
  {
    SDL_Log("Convertendo a imagem para escala de cinza");

    if (!renderer)
    {
      SDL_Log("\t*** Erro: Renderer inválido (renderer == NULL).");
      return SDL_APP_FAILURE;
    }

    if (!surface)
    {
      SDL_Log("\t*** Erro: Imagem inválida (surface == NULL).");
      return SDL_APP_FAILURE;
    }

    // Para acessar os pixels de uma superfície, precisamos chamar essa função.
    SDL_LockSurface(surface);

    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);
    const size_t pixelCount = surface->w * surface->h;

    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;

    for (size_t i = 0; i < pixelCount; ++i)
    {
      SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);
      Uint8 y = 0.2125 * r + 0.7154 * g + 0.0721 * b;
      r = y;
      g = y;
      b = y;

      pixels[i] = SDL_MapRGBA(format, NULL, r, g, b, a);
    }

    // Após manipularmos os pixels da superfície, liberamos a superfície.
    SDL_UnlockSurface(surface);

    // Atualizamos a textura a ser renderizada pelo SDL_Renderer, com base no
    // novo conteúdo da superfície.
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
  }

  SDL_FRect imageRect = {
      .x = 0,
      .y = 0,
      .w = surface->w,
      .h = surface->h
  };

SDL_Event event;
bool isRunning = true;
while (isRunning)
{
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_EVENT_QUIT:
      isRunning = false;
      break;

    case SDL_EVENT_MOUSE_MOTION:
      snprintf(windowTitle, WINDOW_TITLE_MAX_LENGTH, "%s (%.0f, %.0f)", WINDOW_TITLE, event.motion.x, event.motion.y);
      SDL_SetWindowTitle(window, windowTitle);
      break;
    }
  }

  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texBmp, NULL, &bmpRect);
  SDL_RenderTexture(renderer, texture, NULL, &imageRect);
  SDL_RenderTexture(renderer, texJpg, NULL, &jpgRect);
  SDL_RenderTexture(renderer, texPng, NULL, &pngRect);
  SDL_RenderPresent(renderer);
}

SDL_DestroyTexture(texBmp);
texBmp = NULL;
SDL_DestroyTexture(texJpg);
texJpg = NULL;
SDL_DestroyTexture(texPng);
texPng = NULL;

SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
renderer = NULL;
window = NULL;

return 0;
}