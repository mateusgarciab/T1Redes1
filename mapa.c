#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mapa.h"



mapa_t *geraMapa(tesouro_t *tesouros){
    mapa_t *m = malloc(sizeof(mapa_t));
    if (!m)
        return NULL;
    for (int i = 0; i < 8; i++)
        m->mapa[i] = &m->posicoes[i*8];
    //Seta corretamente os ponteiros de cada posicao para seus vizinhos
    for (unsigned char i = 0; i < 8; i++) {
        for (unsigned char j = 0; j < 8; j++) {
            m->mapa[i][j].linha = i;
            m->mapa[i][j].coluna = j;
            if (i == 0){
                m->mapa[i][j].down = NULL;
            }else{
                m->mapa[i][j].down = &m->mapa[i-1][j];
            }
            if (i == 7){
                m->mapa[i][j].up = NULL;
            }else{
                m->mapa[i][j].up = &m->mapa[i+1][j];
            }
            if (j == 0){
                m->mapa[i][j].left = NULL;
            }else{
                m->mapa[i][j].left = &m->mapa[i][j-1];
            }
            if (j == 7){
                m->mapa[i][j].right = NULL;
            }else{
                m->mapa[i][j].right = &m->mapa[i][j+1];
            }
        }
    }
    //Distribui os tesouros aleatoriamente pelo mapa
    for (int i = 0; i < 8; i++) {
        int j, k;
        do {
            j = rand() % 8;
            k = rand() % 8;
        } while (m->mapa[j][k].t != NULL);
        m->mapa[j][k].t = &tesouros[i];
    }
    //Posiciona o jogador em uma posicao aleatoria no mapa
    m->posAtual = &m->mapa[rand() % 8][rand() % 8];
    m->posInicial = m->posAtual;
    return m;
}

bool moveUp(mapa_t *M){
    if (!M->posAtual->up)
        return false;
    M->posAtual->prox = M->posAtual->up;
    M->posAtual = M->posAtual->up;
    return true;
}

bool moveDown(mapa_t *M){
    if (!M->posAtual->down)
        return false;
    M->posAtual->prox = M->posAtual->down;
    M->posAtual = M->posAtual->down;
    return true;
}

bool moveLeft(mapa_t *M){
    if (!M->posAtual->left)
        return false;
    M->posAtual->prox = M->posAtual->left;
    M->posAtual = M->posAtual->left;
    return true;
}

bool moveRight(mapa_t *M){
    if (!M->posAtual->right)
        return false;
    M->posAtual->prox = M->posAtual->right;
    M->posAtual = M->posAtual->right;
    return true;
}

void imprimeCaminho(mapa_t *M){
    pos_t *aux = M->posInicial;
    while (aux) {
        printf("[%d, %d]", (int) aux->linha, (int) aux->coluna);
        if (aux->t)
            printf("\tTesouro");
        printf("\n");
    }
}

img_mapa_t *geraImgMapa(){
    img_mapa_t *mapaI = malloc(sizeof(img_mapa_t));
    if (!mapaI)
        return NULL;
    for (int i = 0; i < 64; i++)
        mapaI->pos[i] = '.';
    for (int i = 0; i < 8; i++)
        mapaI->M[i] = &mapaI->pos[i*8];
    return mapaI;
}

void moveImgUp(img_mapa_t *mapa, bool flag){
    if (mapa->M[mapa->x][mapa->y] == 'F')
        mapa->M[mapa->x][mapa->y] = '*';
    mapa->x++;
    if (flag)
        mapa->M[mapa->x][mapa->y] = 'o';
    else
        mapa->M[mapa->x][mapa->y] = 'F';
}

void moveImgDown(img_mapa_t *mapa, bool flag){
    if (mapa->M[mapa->x][mapa->y] == 'F')
        mapa->M[mapa->x][mapa->y] = '*';
    mapa->x--;
    if (flag)
        mapa->M[mapa->x][mapa->y] = 'o';
    else
        mapa->M[mapa->x][mapa->y] = 'F';
}

void moveImgLeft(img_mapa_t *mapa, bool flag){
    if (mapa->M[mapa->x][mapa->y] == 'F')
        mapa->M[mapa->x][mapa->y] = '*';
    mapa->y--;
    if (flag)
        mapa->M[mapa->x][mapa->y] = 'o';
    else
        mapa->M[mapa->x][mapa->y] = 'F';
}

void moveImgRight(img_mapa_t *mapa, bool flag){
    if (mapa->M[mapa->x][mapa->y] == 'F')
        mapa->M[mapa->x][mapa->y] = '*';
    mapa->y++;
    if (flag)
        mapa->M[mapa->x][mapa->y] = 'o';
    else
        mapa->M[mapa->x][mapa->y] = 'F';
}

void imprimeImgMapa(img_mapa_t *mapa){
    printf("|-----------------|");
    for (int i = 0; i < 64; i+=8) {
        printf("| ");
        printf("%c %c %c %c %c %c %c %c ", mapa->pos[i], mapa->pos[i+1],
                mapa->pos[i+2], mapa->pos[i+3], mapa->pos[i+4], mapa->pos[i+5],
                mapa->pos[i+6], mapa->pos[i+7]);
        printf("|\n");
    }
    printf("|-----------------|");
}
