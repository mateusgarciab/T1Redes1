#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "mapa.h"

void sEnvia(int soquete, tesouro_t *t, unsigned char *mensagem, unsigned char *resposta);

void sRecebe(int soquete, mapa_t *M, unsigned char *mensagem, unsigned char *resposta);

void rodaServidor(int soquete, mapa_t *M, unsigned char *mensagem, unsigned char *resposta);

#endif
