#include "stdlib.h"
#include <stdbool.h>
#include "mensagem.h"
#include "rede.h"



bool aguardaResposta(unsigned char *resposta, int timeout){
    int x = 0;
    //seta o tempo de timeout sendo o tempo recebido
    do {
        //escutar a rede passando resposta
    } while (x != -1 && resposta[0] != MARCADOR_INI);
    //nao tenho certeza se e -1 ou que indica timeout
    //retorna true se deu timeout, false caso contrario
    return x;
}

void aguardaMensagem(unsigned char *mensagem){
    unsigned char nSeqAux = (getNSeq(mensagem) + 1) % 32;
    while (1) {
        do {
            //escuta a rede
        } while (mensagem[0] != MARCADOR_INI);
        if (verificaIntegridade(mensagem))
            return;
        montaMensagem(mensagem, NACK, nSeqAux, NULL, 0);
        //envia mensagem
    }
}

void enviaMensEsperaResp(unsigned char *mensagem, unsigned char *resposta){
    int timeout = 0; //colocar o tempo inicial
    int numTimeout = 0;
    bool timeOutFlag = false;
    do {
        //envia mensagem
        timeOutFlag = aguardaResposta(resposta, timeout);
        if (timeOutFlag) {
            numTimeout++;
            timeout *= 2;
        }
    } while (numTimeout < 15 && (timeOutFlag || !verificaIntegridade(resposta) || getTipo(resposta) == NACK));
    if (numTimeout == 15) {
        //erro, nao conseguiu resposta do outro lado termina o programa
    }
}

void enviaRespEsperaMens(unsigned char *mensagem, unsigned char *resposta){
    //envia resposta
    aguardaMensagem(mensagem);
}

void trocaPapeis(unsigned char *mensagem, unsigned char *resposta){
    do {
        enviaMensEsperaResp(resposta, mensagem);
    } while (getTipo(mensagem) != ACK);
}
