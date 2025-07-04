#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servidor.h"
#include "mapa.h"
#include "rede.h"
#include "mensagem.h"

char *concatenaNomeTesouro(tesouro_t *t) {
    char nome[150];
    nome[0] = '\0';
    strcat(nome, "objetos/");
    strcat(nome, (char*)t->nome);

    return strdup(nome);
}

void sEnvia(int soquete, tesouro_t *t, unsigned char *mensagem, unsigned char *resposta){
    size_t i, aux;
    FILE *arq = fopen(concatenaNomeTesouro(t), "r");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", t->nome);
        exit(1);
    }
    unsigned char nSeq = (getNSeq(mensagem) + 1) % 32;
    unsigned char *dados = malloc(sizeof(unsigned char)*127);
    if (getTipo(resposta) == START_GAME) {
        montaMensagem(mensagem, t->tipo, nSeq, t->nome, t->tamNome);
        nSeq = (nSeq +1) % 32;
        enviaMensEsperaResp(soquete, mensagem, resposta);
    }
    montaMensagem(mensagem, TAMANHO, nSeq, (unsigned char *)&t->tamanho, 8);
    
    nSeq = (nSeq +1) % 32;
    enviaMensEsperaResp(soquete, mensagem, resposta);
    if (getTipo(resposta) == ERRO) {
        getDados(resposta, dados);
        if (dados[0]) {
            fprintf(stderr, "Espaço insuficiente para transferencia, encerrando programa\n");
            fclose(arq);
            exit(1);
        }
        fprintf(stderr, "Sem permissão de acesso, encerrando programa\n");
        fclose(arq);
        exit(1);
    }
    while ((i = fread(dados, sizeof(char), 126, arq))) {
        aux = montaMensagem(mensagem, DADOS, nSeq, dados, i);
        if (aux < i) {
            fseek(arq, (aux - i), SEEK_CUR);
        }
        nSeq = (nSeq + 1) % 32;
        enviaMensEsperaResp(soquete, mensagem, resposta);
    }
    free(dados);
    fclose(arq);
}

void sRecebe(int soquete, mapa_t *M, unsigned char *mensagem, unsigned char *resposta){
    bool flag = false;
    unsigned char *dados = malloc(sizeof(unsigned char)*127);
    unsigned char numSeqAux = 32;
    if (mensagem[0] != MARCADOR_INI) {
        montaMensagem(mensagem, NACK, 31, NULL, 0);
        aguardaMensagem(soquete, mensagem);
    }
    while (1) {
        switch (getTipo(mensagem)) {
            case ACK:
                montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                break;
            case START_GAME:
                dados[0] = M->posInicial->linha;
                dados[1] = M->posInicial->coluna;
                if (M->posInicial->t){
                    dados[2] = 1;
                    flag = true;
                }else{
                    dados[2] = 0;
                }
                montaMensagem(resposta, START_GAME, getNSeq(mensagem), dados, 3);
                break;
            case MOV_RIGHT:
                if (!moveRight(M)) {
                    montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                    break;
                }
                if (!M->posAtual->t || M->posAtual->t->achado) {
                    montaMensagem(resposta, OK, getNSeq(mensagem), NULL, 0);
                    break;
                }
                M->posAtual->t->achado = true;
                montaMensagem(resposta, M->posAtual->t->tipo, getNSeq(mensagem), M->posAtual->t->nome, M->posAtual->t->tamNome);
                flag = true;
                break;
            case MOV_UP:
                if (!moveUp(M)) {
                    montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                    break;
                }
                if (M->posAtual->t && !M->posAtual->t->achado) {
                    M->posAtual->t->achado = true;
                    montaMensagem(resposta, M->posAtual->t->tipo, getNSeq(mensagem), M->posAtual->t->nome, M->posAtual->t->tamNome);
                    flag = true;
                    break;
                }
                montaMensagem(resposta, OK, getNSeq(mensagem), NULL, 0);
                break;
            case MOV_DOWN:
                if (!moveDown(M)) {
                    montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                    break;
                }
                if (!M->posAtual->t || M->posAtual->t->achado) {
                    montaMensagem(resposta, OK, getNSeq(mensagem), NULL, 0);
                    break;
                }
                M->posAtual->t->achado = true;
                montaMensagem(resposta, M->posAtual->t->tipo, getNSeq(mensagem), M->posAtual->t->nome, M->posAtual->t->tamNome);
                flag = true;
                break;
            case MOV_LEFT:
                if (!moveLeft(M)) {
                    montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                    break;
                }
                if (!M->posAtual->t || M->posAtual->t->achado) {
                    montaMensagem(resposta, OK, getNSeq(mensagem), NULL, 0);
                    break;
                }
                M->posAtual->t->achado = true;
                montaMensagem(resposta, M->posAtual->t->tipo, getNSeq(mensagem), M->posAtual->t->nome, M->posAtual->t->tamNome);
                flag = true;
                break;
        }
        if (flag) {
            trocaPapeis(soquete, mensagem, resposta);
            free(dados);
            return;
        }
        numSeqAux = getNSeq(mensagem);
        do {
            enviaRespEsperaMens(soquete, mensagem, resposta);
        } while (getNSeq(mensagem) == numSeqAux);
    }
}

void rodaServidor(int soquete, mapa_t *M, unsigned char *mensagem, unsigned char *resposta){
    bool rodando = true;
    printf("[%d, %d]", (int) M->posAtual->linha, (int) M->posAtual->coluna);
    if (M->posAtual->t)
        printf("\tTesouro - %s", M->posAtual->t->nome);
    printf("\n");
    while (rodando) {
        sRecebe(soquete, M, mensagem, resposta);
        sEnvia(soquete, M->posAtual->t, mensagem, resposta);
        M->tesourosRestantes--;
        if (M->tesourosRestantes) {
            montaMensagem(mensagem, FIM_ARQ, (getNSeq(mensagem) + 1) % 32, NULL, 0);
        } else {
            montaMensagem(mensagem, END_GAME, (getNSeq(mensagem) + 1) % 32, NULL, 0);
            rodando = false;
        }
        enviaMensEsperaResp(soquete, mensagem, resposta);
        unsigned char *aux = mensagem;
        mensagem = resposta;
        resposta = aux;
    }
    printf("Todos os tesouros foram achados! Fim de Jogo.\n");
}
