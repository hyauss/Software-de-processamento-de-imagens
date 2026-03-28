# Nome do executável
TARGET = projeto

# Compilador
CC = gcc

# Caminhos das libs
SDL_PATH = C:/ComputacaoVisual/libs

# Includes
INCLUDES = -I$(SDL_PATH)/SDL3/include \
           -I$(SDL_PATH)/SDL3_image/include \
           -I$(SDL_PATH)/SDL3_ttf/include

# Libs
LIBS = -L$(SDL_PATH)/SDL3/lib \
       -L$(SDL_PATH)/SDL3_image/lib \
       -L$(SDL_PATH)/SDL3_ttf/lib \
       -lSDL3 -lSDL3_image -lSDL3_ttf -lm

# Flags
CFLAGS = -Wall -g $(INCLUDES)

# Arquivos fonte
SRC = projeto.c

# Regra padrão
all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

# Rodar o programa
run: all
	$(TARGET).exe assets/test.png

# Limpar
clean:
	del /f /q $(TARGET).exe