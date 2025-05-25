#ifndef CLIENTE_H
#define CLIENTE_H

#include "mapa.h"

int cEnvia(int soquete, img_mapa_t *m, unsigned char *mensagem, unsigned char *resposta);

bool cRecebe(int soquete, unsigned char *mensagem, unsigned char *resposta);

void rodaCliente(int soquete, img_mapa_t *m, unsigned char *mensagem, unsigned char *resposta);

#endif
