# Software-de-processamento-de-imagens

Este projeto consiste no desenvolvimento de uma aplicação em C utilizando a biblioteca SDL (Simple DirectMedia Layer) em sua versão 3 para processamento de imagens.

O programa é capaz de carregar imagens, convertê-las para escala de cinza, analisar seu histograma, exibir informações estatísticas como brilho e contraste, equalizar o histograma e salvar a imagem.

## Funcionalidades implementadas

### 1. Carregamento da imagem

Suporte para formatos:
- PNG
- JPG
- BMP

Utilização da biblioteca SDL_image

Tratamento de erros:
- Arquivo inexistente
- Formato inválido

### 2. Conversão para escala de cinza

Verificação se a imagem já está em tons de cinza

Caso não esteja, conversão usando a fórmula: Y = 0.2125 * R + 0.7154 * G + 0.0721 * B

### 3. Interface Gráfica (GUI)

Janela principal:
- Exibe a imagem
- Ajusta o tamanho automaticamente
- Centralizada na tela

Janela secundária:
- Exibe o histograma da imagem
- Ao lado da janela principal

### 4. Histograma

Cálculo do histograma da imagem (256 níveis de intensidade)

Exibição gráfica na janela secundária

Cálculo de:
- Média de intensidade (brilho): escura, média, clara
- Desvio padrão (contraste): baixo, médio, alto

### 5. Equalização do histograma

Botão na interface gráfica

Alternância entre:
  - Imagem equalizada
  - Imagem original

Atualização dinâmica das janelas

Mudança visual do botão

### 6. Salvamento da imagem

Tecla S

Salva como: output_image.png

Sobrescreve caso já exista

## Controles

Teclas "S": salvar imagem

Botão para equalizar e ver imagem original

## Tecnologias utilizadas

Linguagem: C

Bibliotecas: SDL3, SDL_image, SDL_ttf

## Como compilar e executar

Pré-requisitos:
- SDL3 instalada
- SDL_image instalada
- SDL_ttf instalada

Compilação:

Utilize o debug do windows, no arquivo projeto.c.

OBS: Vai falhar, mas a ideia é usar apenas para gerar o executável de uma maneira mais fácil.


<img width="45" height="28" alt="image" src="https://github.com/user-attachments/assets/589ba541-f46c-4e89-98d9-78a11f8551c4" />


Execução:

No terminal rode

    projeto.exe assets/nomeDaSuaImagem.extaoDaSuaImagem
    
    Exemplo:
    
    projeto.exe assets/test.bmp

## Contribuições

Gabriel Barros Albertini: Participação em todas etapas, especialmente SDL_ttf e README.

Gustavo Luigi Chao Pinotti: Participação em todas etapas, especialmente centralização das janelas e função para salvar.

Rafael de Menezes Ros: Participação em todas etapas.

Vinicius Alves Marques: Participação em todas etapas, especialmente Carregamento de imagem, Conversão para escala de cinza, Interface Gráfica, Histograma, Equalização.

