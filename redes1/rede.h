#ifndef REDE_H
#define REDE_H

#include <stdbool.h>

bool aguardaResposta(unsigned char *resposta, int timeout);

void aguardaMensagem(unsigned char *mensagem);

void enviaMensEsperaResp(unsigned char *mensagem, unsigned char *resposta);

void enviaRespEsperaMens(unsigned char *mensagem, unsigned char *resposta);

void trocaPapeis(unsigned char *mensagem, unsigned char *resposta);

#endif
