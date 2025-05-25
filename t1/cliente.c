#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cliente.h"
#include "mensagem.h"
#include "mapa.h"
#include "rede.h"



int cEnvia(int soquete, img_mapa_t *m, unsigned char *mensagem, unsigned char *resposta){
    int flag = 1;
    char escolha;
    bool temTesouro = false;
    unsigned char nSeq = (getNSeq(mensagem) + 1) % 32;
    while (flag) {
        imprimeImgMapa(m);
        scanf("%1c", &escolha);
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
        enviaMensEsperaResp(soquete, mensagem, resposta);
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

bool cRecebe(int soquete, unsigned char *mensagem, unsigned char *resposta){
    unsigned char *nome = malloc(sizeof(unsigned char)*127);
    unsigned char *dados = malloc(sizeof(unsigned char)*127);
    FILE *arq;
    unsigned char tam;
    unsigned char tamNome;
    unsigned long long int tamArq;
    unsigned long long int espacoLivre;// = getEspacoLivre();
    // retorna quandos bytes tem disponivel no disco;
    unsigned char nSeqAux = 32;
    do {
        switch (getTipo(mensagem)) {
            case START_GAME:
                montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                break;
            case TAMANHO:
                getDados(mensagem, (unsigned char *)&tamArq);
                if (!((2 * tamArq) < espacoLivre)) {
                    dados[0] = 1;
                    montaMensagem(resposta, ERRO, getNSeq(mensagem), dados, 1);
                    break;
                }
                arq = fopen((char *)nome, "w+");
                if (!arq) {
                    fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome);
                    dados[0] = 0;
                    montaMensagem(resposta, ERRO, getNSeq(mensagem), dados, 1);
                    break;
                }
                montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                break;
            case DADOS:
                tam = getDados(mensagem, dados);
                fwrite(dados, sizeof(unsigned char), tam, arq);
                montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                break;
            case TEXTO:
            case VIDEO:
            case IMAGEM:
                tamNome = getDados(mensagem, nome);
                montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                break;
            case FIM_ARQ:
                montaMensagem(resposta, ACK, getNSeq(mensagem), NULL, 0);
                trocaPapeis(soquete, mensagem, resposta);
                fclose(arq);
                printf("Transferencia concluída, abrindo\n");
                //Executa/abre o arquivo
                free(nome);
                return true;
            case END_GAME:
                montaMensagem(resposta, END_GAME, getNSeq(mensagem), NULL, 0);
                trocaPapeis(soquete, mensagem, resposta);
                fclose(arq);
                printf("Transferencia concluída, abrindo\n");
                //Executa/abre o arquivo
                free(nome);
                return false;
        }
        nSeqAux = getNSeq(mensagem);
        do {
            enviaRespEsperaMens(soquete, mensagem, resposta);
        } while (getNSeq(mensagem) == nSeqAux);
    } while (1);
}

void rodaCliente(int soquete, img_mapa_t *m, unsigned char *mensagem, unsigned char *resposta){
    int flag = 1;
    bool rodando = true;
    unsigned char nSeq = 0;
    unsigned char *dados = malloc(sizeof(unsigned char)*127);
    montaMensagem(mensagem, START_GAME, nSeq, NULL, 0);
    enviaMensEsperaResp(soquete, mensagem, resposta);
    getDados(resposta, dados);
    if (setPosInic(m, dados[0], dados[1], dados[2]))
        flag = 0;
    free(dados);
    while (rodando) {
        if (flag){
            flag = cEnvia(soquete, m, mensagem, resposta);
            continue;
        } else {
            imprimeImgMapa(m);
            printf("Tesouro achado, aguardando transferencia\n");
            rodando = cRecebe(soquete, resposta, mensagem);
            flag = 1;
        }
    }
}
