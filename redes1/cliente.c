#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mensagem.h"
#include "mapa.h"
#include "rede.h"



int envia(img_mapa_t *m, unsigned char *mensagem, unsigned char *resposta){
    int flag = 1;
    char escolha;
    bool temTesouro = false;
    unsigned char nSeq = (getNSeq(mensagem) + 1) % 32;
    while (flag) {
        imprimeImgMapa(m);
        scanf("%1c\n", &escolha);
        switch (escolha) {
            case 'w':
                montaMensagem(mensagem, MOV_UP, nSeq, NULL, 0);
                break;
            case 's':
                montaMensagem(mensagem, MOV_DOWN, nSeq, NULL, 0);
                break;
            case 'd':
                montaMensagem(mensagem, MOV_RIGHT, nSeq, NULL, 0);
                break;
            case 'a':
                montaMensagem(mensagem, MOV_LEFT, nSeq, NULL, 0);
                break;
            default:
                break;
        }
        nSeq = (nSeq + 1) % 32;
        enviaMensEsperaResp(mensagem, resposta);
        if (getTipo(resposta) == ACK)
            continue;
        if (getTipo(resposta) == TEXTO || getTipo(resposta) == VIDEO ||
            getTipo(resposta) == IMAGEM){
            temTesouro = true;
            flag = 0;
        }
        switch (getTipo(mensagem)) {
            case MOV_UP:
                moveImgUp(m, temTesouro);
                break;
            case MOV_DOWN:
                moveImgDown(m, temTesouro);
                break;
            case MOV_LEFT:
                moveImgLeft(m, temTesouro);
                break;
            case MOV_RIGHT:
                moveImgRight(m, temTesouro);
                break;
        }
    }
    return flag;
}

//Terminar de tratar todos os tipos de mensagens e pensar em como fazer a troca
//de papeis nesse caso, tipo
//servidor mandou ACK 3 (Não tenho mais nada a transmitir) usando enviaMens...,
//cliente responde ACK 3 com trocaPapeis(), servidor
//trata a resposta do cliente e responde ACK 3 usando enviaResp... e fica
//esperando nova mensagem dentro da funcao recebe.
bool recebe(unsigned char *mensagem, unsigned char *resposta){
    unsigned char *nome = malloc(sizeof(unsigned char)*127);
    unsigned char tamNome;
    unsigned long long int tamArq;
    unsigned long long int espacoLivre;// = getEspacoLivre();
    // retorna quandos bytes tem disponivel no disco;
    unsigned char nSeqAux = 32;
    do {
        switch (getTipo(mensagem)) {
            case ACK:
                montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                trocaPapeis(mensagem, resposta);
                free(nome);
                return true;
                break;
            case START_GAME:
                montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                break;
            case TAMANHO:
                break;
            case DADOS:
                break;
            case TEXTO:
            case VIDEO:
            case IMAGEM:
                getDados(mensagem, nome);
                tamNome = getTam(mensagem);
                montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                break;
            case FIM_ARQ:
            case END_GAME:
                montaMensagem(resposta, END_GAME, getNSeq(mensagem), NULL, 0);
                trocaPapeis(mensagem, resposta);
                free(nome);
                return false;
        }
        nSeqAux = getNSeq(mensagem);
        do {
            enviaRespEsperaMens(mensagem, resposta);
        } while (getNSeq(mensagem) == nSeqAux);
    } while (1);
}

void rodaCliente(img_mapa_t *m, unsigned char *mensagem, unsigned char *resposta){
    int flag = 1;
    bool rodando = true;
    unsigned char nSeq = 0;
    unsigned char *dados = malloc(sizeof(unsigned char)*127);
    montaMensagem(mensagem, START_GAME, nSeq, NULL, 0);
    enviaMensEsperaResp(mensagem, resposta);
    getDados(resposta, dados);
    if (setPosInic(m, dados[0], dados[1], dados[2]))
        flag = 0;
    free(dados);
    while (rodando) {
        if (flag){
            flag = envia(m, mensagem, resposta);
            continue;
        } else {
            rodando = recebe(resposta, mensagem);
            flag = 1;
        }
    }
}
