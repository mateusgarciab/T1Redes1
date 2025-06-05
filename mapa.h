#ifndef MAPA_H
#define MAPA_H

#include <stdbool.h>

/* Exemplo de como é pra ficar, o 'F' e onde o jogador esta agora, os '*'
 * marcam por onde ele ja passou e os 'o' onde tinha tesouro, os "." casas que
 * ele nao passou ainda
*   |-----------------|
*   | * * F . . . . . |
*   | o * . . . . . . |
*   | . * . . . . . . |
*   | . * . . . . . . |
*   | . * * . . . . . |
*   | . . o . . . . . |
*   | . * * . . . . . |
*   | . . . . . . . . |
*   |-----------------|
*/
typedef struct IMAGEM_MAPA {
    unsigned char x;
    unsigned char y;
    char *M[8];
    char pos[64];
} img_mapa_t;

typedef struct TESOURO {
    unsigned char *nome;
    unsigned char tamNome;
    int tipo;
    unsigned long long int tamanho;
    bool achado;
} tesouro_t;

typedef struct POS {
    unsigned char linha;
    unsigned char coluna;
    tesouro_t *t;
    struct POS *up;
    struct POS *left;
    struct POS *down;
    struct POS *right;
} pos_t;

typedef struct MAPA {
    int tesourosRestantes;
    pos_t *posAtual;
    pos_t *posInicial;
    pos_t *mapa[8];
    pos_t posicoes[64];
} mapa_t;

/* Recebe um vetor tesouros do tipo tesouro_t de 8 posicoes aloca a estrutura
 * mapa_t e preenche os capos corretamente, destribui os tesouros 
 * aleatoriamente pelo mapa e sorteia onde o jogador vai estar
 * Retorna um ponteiro para o mapa criado ou NULL em caso de falha */
mapa_t *geraMapa(tesouro_t *tesouros);

/* Cria e retorna a estrutura img_mapa_t, para a representacao grafica do mapa
 * retona NULL em caso de falha */
img_mapa_t *geraImgMapa();

/* Recebe um ponteiro M para uma estrutura mapa_t e testa se o jogador nao esta
 * na borda do mapa, se nao estiver move a posicao atual uma casa para cima e
 * retorna true, retorna false caso contrario */
bool moveUp(mapa_t *M);

/* Recebe um ponteiro M para uma estrutura mapa_t e testa se o jogador nao esta
 * na borda do mapa, se nao estiver move a posicao atual uma casa para baixo e
 * retorna true, retorna false caso contrario */
bool moveDown(mapa_t *M);

/* Recebe um ponteiro M para uma estrutura mapa_t e testa se o jogador nao esta
 * na borda do mapa, se nao estiver move a posicao atual uma casa para esquerda
 * e retorna true, retorna false caso contrario */
bool moveLeft(mapa_t *M);

/* Recebe um ponteiro M para uma estrutura mapa_t e testa se o jogador nao esta
 * na borda do mapa, se nao estiver move a posicao atual uma casa para direita
 * e retorna true, retorna false caso contrario */
bool moveRight(mapa_t *M);

/* Recebe um ponteiro mapa para a estrutura img_mapa_t, dois unsigned char x e y
 * representando as coordenadas, respectivamente linha e coluna, da posicao
 * inicial do jogador e uma flag indicando se existe um tesouro nessa posicao,
 * preeenche a posicao x, y da representacao grafica do mapa com os simbolos
 * corretos comforme o exemplo, retorna a flag passada */
bool setPosInic(img_mapa_t *mapa, unsigned char x, unsigned char y, bool flag);

/* Recebe um ponteiro mapa para a estrutura img_mapa_t e uma flag indicando se
 * a casa acima da posicao atual possui um tesouro e move a posicao atual na
 * representacao grafica do mapa preenchendo as casas com os simbolos corretos
 * comforme o exemplo */
void moveImgUp(img_mapa_t *mapa, bool flag);

/* Recebe um ponteiro mapa para a estrutura img_mapa_t e uma flag indicando se
 * a casa abaixo da posicao atual possui um tesouro e move a posicao atual na
 * representacao grafica do mapa preenchendo as casas com os simbolos corretos
 * comforme o exemplo */
void moveImgDown(img_mapa_t *mapa, bool flag);

/* Recebe um ponteiro mapa para a estrutura img_mapa_t e uma flag indicando se
 * a casa a esquerda da posicao atual possui um tesouro e move a posicao atual
 * na representacao grafica do mapa preenchendo as casas com os simbolos
 * corretos comforme o exemplo */
void moveImgLeft(img_mapa_t *mapa, bool flag);

/* Recebe um ponteiro mapa para a estrutura img_mapa_t e uma flag indicando se
 * a casa a direita da posicao atual possui um tesouro e move a posicao atual
 * na representacao grafica do mapa preenchendo as casas com os simbolos
 * corretos comforme o exemplo */
void moveImgRight(img_mapa_t *mapa, bool flag);

/* Recebe um ponteiro mapa para a estrutura img_mapa_t e imprime a
 * representacao grafica do mapa */
void imprimeImgMapa(img_mapa_t *mapa);

#endif
