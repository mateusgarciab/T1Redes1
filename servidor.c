#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"
#include "rede.h"
#include "mensagem.h"

int envia(tesouro_t *t, unsigned char *mensagem, unsigned char *resposta){
    int flag = 1;
    size_t i, aux;
    FILE *arq;
    unsigned char nSeq = getNSeq(mensagem);
    unsigned char *dados = malloc(sizeof(unsigned char)*127);
    if (getTipo(resposta) == START_GAME) {
        montaMensagem(mensagem, t->tipo, nSeq, t->nome, t->tamNome);
        nSeq = (nSeq +1) % 32;
        transmite(mensagem, resposta);
    }
    montaMensagem(mensagem, TAMANHO, nSeq, (unsigned char *)&t->tamanho, 8);
    nSeq = (nSeq +1) % 32;
    transmite(mensagem, resposta);
    if (getTipo(resposta) == ERRO) {
        getDados(mensagem, dados);
        if (dados[0]) {
            fprintf(stderr, "Espaço insuficiente para transferencia, encerrando programa\n");
            exit(1);
        }
        fprintf(stderr, "Sem permissão de acesso, encerrando programa\n");
        exit(1);
    }
    while ((i = fread(dados, sizeof(char), 126, arq))) {
        aux = montaMensagem(mensagem, DADOS, nSeq, dados, i);
        if (aux < i) {
            fseek(arq, -(i - aux), SEEK_CUR);
        }
        nSeq = (nSeq +1) % 32;
        transmite(mensagem, resposta);
    }
    return flag;
}

int recebe(mapa_t *M, unsigned char *mensagem, unsigned char *resposta){
    int flag = 1;
    unsigned char *aux = NULL;
    unsigned char numSeqAux = 32;
    unsigned char *dados = malloc(sizeof(unsigned char)*127);
    unsigned char *respostaAux = malloc(sizeof(unsigned char)*131);
    while (flag) {
        aguardaMensagem(mensagem);
        if (!verificaIntegridade(mensagem)) {
            montaMensagem(resposta, NACK, getNSeq(mensagem), NULL, 0);
            //envia resposta;
            continue;
        }
        if (getNSeq(mensagem) == numSeqAux) {
            //envia resposta anterior vulgo respostaAux
            continue;
        }
        switch (getTipo(mensagem)) {
            case ACK:
                flag = 0;
                break;
            case START_GAME:
                dados[0] = M->posInicial->linha;
                dados[1] = M->posInicial->coluna;
                if (M->posInicial->t)
                    dados[2] = 1;
                else
                    dados[2] = 0;
                montaMensagem(resposta, START_GAME, getNSeq(mensagem), dados, 3);
                //Envia resposta;
                numSeqAux = getNSeq(mensagem);
                unsigned char *aux = resposta;
                resposta = respostaAux;
                respostaAux = aux;
                break;
            case MOV_RIGHT:
                if (!moveRight(M)) {
                    montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                }else{
                    if (M->posAtual->t && !M->posAtual->t->achado) {
                        M->posAtual->t->achado = true;
                        montaMensagem(resposta, M->posAtual->t->tipo, getNSeq(mensagem), M->posAtual->t->nome, M->posAtual->t->tamNome);
                    }else{
                        montaMensagem(resposta, OK, getNSeq(mensagem), NULL, 0);
                    }
                }
                //Envia resposta;
                numSeqAux = getNSeq(mensagem);
                aux = resposta;
                resposta = respostaAux;
                respostaAux = aux;
                break;
            case MOV_UP:
                if (!moveUp(M)) {
                    montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                }else{
                    if (M->posAtual->t && !M->posAtual->t->achado) {
                        M->posAtual->t->achado = true;
                        montaMensagem(resposta, M->posAtual->t->tipo, getNSeq(mensagem), M->posAtual->t->nome, M->posAtual->t->tamNome);
                    }else{
                        montaMensagem(resposta, OK, getNSeq(mensagem), NULL, 0);
                    }
                }
                //Envia resposta;
                numSeqAux = getNSeq(mensagem);
                aux = resposta;
                resposta = respostaAux;
                respostaAux = aux;
                break;
            case MOV_DOWN:
                if (!moveDown(M)) {
                    montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                }else{
                    if (M->posAtual->t && !M->posAtual->t->achado) {
                        M->posAtual->t->achado = true;
                        montaMensagem(resposta, M->posAtual->t->tipo, getNSeq(mensagem), M->posAtual->t->nome, M->posAtual->t->tamNome);
                    }else{
                        montaMensagem(resposta, OK, getNSeq(mensagem), NULL, 0);
                    }
                }
                //Envia resposta;
                numSeqAux = getNSeq(mensagem);
                aux = resposta;
                resposta = respostaAux;
                respostaAux = aux;
                break;
            case MOV_LEFT:
                if (!moveLeft(M)) {
                    montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                }else{
                    if (M->posAtual->t && !M->posAtual->t->achado) {
                        M->posAtual->t->achado = true;
                        montaMensagem(resposta, M->posAtual->t->tipo, getNSeq(mensagem), M->posAtual->t->nome, M->posAtual->t->tamNome);
                    }else{
                        montaMensagem(resposta, OK, getNSeq(mensagem), NULL, 0);
                    }
                }
                //Envia resposta;
                numSeqAux = getNSeq(mensagem);
                aux = resposta;
                resposta = respostaAux;
                respostaAux = aux;
                break;
        }
    }
    aux = resposta;
    resposta = respostaAux;
    respostaAux = aux;
    free(respostaAux);
    free(dados);
    return flag;
}
