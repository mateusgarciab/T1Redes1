#ifndef REDE_H
#define REDE_H

#include <stdbool.h>

bool aguardaResposta(int soquete, unsigned char *resposta, int timeout);

void aguardaMensagem(int soquete, unsigned char *mensagem);

void enviaMensEsperaResp(int soquete, unsigned char *mensagem, unsigned char *resposta);

void enviaRespEsperaMens(int soquete, unsigned char *mensagem, unsigned char *resposta);

void trocaPapeis(int soquete, unsigned char *mensagem, unsigned char *resposta);

#endif
