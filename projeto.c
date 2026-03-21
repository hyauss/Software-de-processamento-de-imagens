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

enum constants
{
  DEFAULT_WINDOW_WIDTH = 640,
  DEFAULT_WINDOW_HEIGHT = 480,
};

typedef struct MyWindow MyWindow;
struct MyWindow
{
  SDL_Window *window;
  SDL_Renderer *renderer;
};

typedef struct MyImage MyImage;
struct MyImage
{
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_FRect rect;
};

//------------------------------------------------------------------------------
// Globals (argh!)
//------------------------------------------------------------------------------
static MyWindow g_window = { .window = NULL, .renderer = NULL };
static MyImage g_image = {
  .surface = NULL,
  .texture = NULL,
  .rect = { .x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 0.0f }
};

static MyWindow g_window2 = { .window = NULL, .renderer = NULL };
static MyImage g_image2 = {
  .surface = NULL,
  .texture = NULL,
  .rect = { .x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 0.0f }
};

//------------------------------------------------------------------------------
// Function declaration
//------------------------------------------------------------------------------
static bool MyWindow_initialize(MyWindow *window, const char *title, int width, int height, SDL_WindowFlags window_flags);
static void MyWindow_destroy(MyWindow *window);
static void MyImage_destroy(MyImage *image);

bool MyWindow_initialize(MyWindow *window, const char *title, int width, int height, SDL_WindowFlags window_flags)
{
  SDL_Log("\tMyWindow_initialize(%s, %d, %d)", title, width, height);

  if (!window)
  {
    SDL_Log("\t\t*** Erro: Janela/renderizador inválidos (window == NULL).");
    return false;
  }

  return SDL_CreateWindowAndRenderer(title, width, height, window_flags, &window->window, &window->renderer);
}

void MyWindow_destroy(MyWindow *window)
{
  SDL_Log(">>> MyWindow_destroy()");

  if (!window)
  {
    SDL_Log("\t*** Erro: Janela/renderizador inválidos (window == NULL).");
    SDL_Log("<<< MyWindow_destroy()");
    return;
  }

  SDL_Log("\tDestruindo MyWindow->renderer...");
  SDL_DestroyRenderer(window->renderer);
  window->renderer = NULL;

  SDL_Log("\tDestruindo MyWindow->window...");
  SDL_DestroyWindow(window->window);
  window->window = NULL;

  SDL_Log("<<< MyWindow_destroy()");
}

void MyImage_destroy(MyImage *image)
{
  SDL_Log(">>> MyImage_destroy()");

  if (!image)
  {
    SDL_Log("\t*** Erro: Imagem inválida (image == NULL).");
    SDL_Log("<<< MyImage_destroy()");
    return;
  }

  if (image->texture)
  {
    SDL_Log("\tDestruindo MyImage->texture...");
    SDL_DestroyTexture(image->texture);
    image->texture = NULL;
  }

  if (image->surface)
  {
    SDL_Log("\tDestruindo MyImage->surface...");
    SDL_DestroySurface(image->surface);
    image->surface = NULL;
  }

  SDL_Log("\tRedefinindo MyImage->rect...");
  image->rect.x = image->rect.y = image->rect.w = image->rect.h = 0.0f;

  SDL_Log("<<< MyImage_destroy()");
}

void load_rgba32(const char *filename, SDL_Renderer *renderer, MyImage *output_image)
{
  SDL_Log(">>> load_rgba32(\"%s\")", filename);

  if (!filename)
  {
    SDL_Log("\t*** Erro: Nome do arquivo inválido (filename == NULL).");
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  if (!renderer)
  {
    SDL_Log("\t*** Erro: Renderer inválido (renderer == NULL).");
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  if (!output_image)
  {
    SDL_Log("\t*** Erro: Imagem de saída inválida (output_image == NULL).");
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  MyImage_destroy(output_image);

  SDL_Log("\tCarregando imagem \"%s\" em uma superfície...", filename);
  SDL_Surface *surface = IMG_Load(filename);
  if (!surface)
  {
    SDL_Log("\t*** Erro ao carregar a imagem: %s", SDL_GetError());
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  SDL_Log("\tConvertendo superfície para formato RGBA32...");
  output_image->surface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
  SDL_DestroySurface(surface);
  if (!output_image->surface)
  {
    SDL_Log("\t*** Erro ao converter superfície para formato RGBA32: %s", SDL_GetError());
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  SDL_Log("\tCriando textura a partir da superfície...");
  output_image->texture = SDL_CreateTextureFromSurface(renderer, output_image->surface);
  if (!output_image->texture)
  {
    SDL_Log("\t*** Erro ao criar textura: %s", SDL_GetError());
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  SDL_Log("\tObtendo dimensões da textura...");
  SDL_GetTextureSize(output_image->texture, &output_image->rect.w, &output_image->rect.h);

  SDL_Log("<<< load_rgba32(\"%s\")", filename);
}

static void render_window(MyWindow *win, MyImage *img)
{
    SDL_SetRenderDrawColor(win->renderer, 128, 128, 128, 255);
    SDL_RenderClear(win->renderer);

    SDL_RenderTexture(win->renderer, img->texture, &img->rect, &img->rect);

    SDL_RenderPresent(win->renderer);
}

static void destroy_pair(MyWindow *win, MyImage *img)
{
    MyImage_destroy(img);
    MyWindow_destroy(win);
}

static void shutdown(void)
{
    SDL_Log(">>> shutdown()");

    destroy_pair(&g_window, &g_image);
    destroy_pair(&g_window2, &g_image2);

    SDL_Log("\tEncerrando SDL...");
    SDL_Quit();

    SDL_Log("<<< shutdown()");
}

static SDL_AppResult initialize(void)
{
  SDL_Log(">>> initialize()");

  SDL_Log("\tIniciando SDL...");
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("\t*** Erro ao iniciar a SDL: %s", SDL_GetError());
    SDL_Log("<<< initialize()");
    return SDL_APP_FAILURE;
  }

  SDL_Log("\tCriando janela e renderizador...");
  if (!MyWindow_initialize(&g_window, "Minha Janela", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0))
  {
    SDL_Log("\t*** Erro ao criar a janela e/ou renderizador: %s", SDL_GetError());
    SDL_Log("<<< initialize()");
    return SDL_APP_FAILURE;
  }

  SDL_Log("\tCriando janela e renderizador...");
  if (!MyWindow_initialize(&g_window2, "Minha Janela2", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0))
  {
    SDL_Log("\t*** Erro ao criar a janela e/ou renderizador: %s", SDL_GetError());
    SDL_Log("<<< initialize()");
    return SDL_APP_FAILURE;
  }

  SDL_Log("<<< initialize()");
  return SDL_APP_CONTINUE;
}

bool convertTonsDeCinza(SDL_Surface *surface)
{
    if (!surface)
    {
        SDL_Log("\t*** Erro: Imagem inválida (surface == NULL).");
        return false;
    }

    bool isGray = true;

    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);
    Uint32 *pixels = (Uint32 *)surface->pixels;

    Uint8 r, g, b;

    // 🔍 Verifica se já está em escala de cinza
    for (int y = 0; y < surface->h && isGray; y++)
    {
        for (int x = 0; x < surface->w && isGray; x++)
        {
            Uint32 pixel = pixels[y * surface->w + x];
            SDL_GetRGB(pixel, format, NULL, &r, &g, &b);

            if (!(r == g && g == b))
            {
                isGray = false;
            }
        }
    }

    SDL_Log("A imagem escolhida %s escala de cinza.",
            isGray ? "já está em" : "não está em");

    // 🎨 Se não for, converte
    if (!isGray)
    {
        SDL_Log("Convertendo a imagem para escala de cinza");

        SDL_LockSurface(surface);

        const size_t pixelCount = surface->w * surface->h;
        Uint8 a;

        for (size_t i = 0; i < pixelCount; ++i)
        {
            SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);

            Uint8 y = (Uint8)(0.2125 * r + 0.7154 * g + 0.0721 * b);

            pixels[i] = SDL_MapRGBA(format, NULL, y, y, y, a);
        }

        SDL_UnlockSurface(surface);
    }

    return isGray;
}

static void loop(void)
{
  SDL_Log(">>> loop()");

  // Para melhorar o uso da CPU (e consumo de energia), só atualizaremos o
  // conteúdo da janela se realmente for necessário. Nesse exemplo, isso
  // acontece quando invertemos os pixels da imagem.
  bool mustRefresh = false;
  render_window(&g_window2,&g_image2);
  render_window(&g_window, &g_image);

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

      case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_1 && !event.key.repeat)
        {
          convertTonsDeCinza(g_image.surface);
          mustRefresh = true;
        }
        break;
      }
    }

    if (mustRefresh)
    {
    render_window(&g_window, &g_image);
    mustRefresh = false;
    }
  }
  
  SDL_Log("<<< loop()");
}



int main(int argc, char *argv[])
{
  atexit(shutdown);

 if (initialize() == SDL_APP_FAILURE)
    return SDL_APP_FAILURE;

  load_rgba32(argv[1], g_window.renderer, &g_image);
  
  int imageWidth = (int)g_image.rect.w;
  int imageHeight = (int)g_image.rect.h;

  SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    if (!displayID) {
        fprintf(stderr, "Erro ao obter display principal: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(displayID);
    if (!mode) {
        fprintf(stderr, "Erro ao obter modo de display: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int screenWidth  = mode->w;
    int screenHeight = mode->h;

    printf("%d", screenHeight);
    printf("%d", screenWidth);

  // if (imageWidth <= DEFAULT_WINDOW_WIDTH || imageHeight <= DEFAULT_WINDOW_HEIGHT)
  // {
    // Obtém o tamanho da borda da janela. Neste exemplo, só queremos saber
    // o lado superior e o lado esquerdo, para posicionar a janela corretamente
    // (posicionar a janela na coordenada (0, 0) faria com que a borda do
    // programa ficasse fora da região da tela).
    int left = (screenWidth  - imageWidth) / 2;
    int top  = (screenHeight - imageHeight) / 2;

    int borderTop = 0, borderLeft = 0;
    SDL_GetWindowBordersSize(g_window.window, &borderTop, &borderLeft, NULL, NULL);
    left -= borderLeft;
    top  -= borderTop;

    SDL_Log("Redefinindo dimensões da janela, de (%d, %d) para (%d, %d), e alterando a posição para (%d, %d).",
      DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, imageWidth, imageHeight, left, top);

    SDL_SetWindowSize(g_window.window, imageWidth, imageHeight);
    SDL_SetWindowPosition(g_window.window, left, top);

    SDL_SyncWindow(g_window.window);
  // }
   
    SDL_SetWindowPosition(g_window2.window, left + borderLeft + imageWidth, top);

  loop();

  return 0;
}