#include "stdlib.h" //n é <> ?
#include <stdbool.h>
#include "mensagem.h"
#include "rede.h"

#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>

#define TAMBUFF 200

bool aguardaResposta(int soquete, unsigned char *resposta, int timeout){
    int x = 0;
    //seta o tempo de timeout sendo o tempo recebido
    const int timeoutMillis = 3000;
    struct timeval timeOut = { .tv_sec = timeoutMillis / 1000, .tv_usec = (timeoutMillis % 1000) * 1000};
    setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeOut, sizeof(timeOut));
    unsigned char buff[TAMBUFF];
    
    do {
        //escutar a rede passando resposta
        x = recv(soquete, buff, TAMBUFF, 0);
    } while (x != -1 && resposta[0] != MARCADOR_INI);
    if (x == -1)
        return true;
    //nao tenho certeza se e -1 ou que indica timeout
    //retorna true se deu timeout, false caso contrario
    return false;
}

void aguardaMensagem(int soquete, unsigned char *mensagem){
    unsigned char nSeqAux = (getNSeq(mensagem) + 1) % 32;
    int x = 0;
    unsigned char buff[TAMBUFF];
    while (1) {
        do {
            //escuta a rede
            x = recv(soquete, buff, TAMBUFF, 0);
        } while (x == -1 || mensagem[0] != MARCADOR_INI);
        if (verificaIntegridade(mensagem))
            return;
        montaMensagem(mensagem, NACK, nSeqAux, NULL, 0);
        //envia mensagem
        send(soquete, buff, TAMBUFF, 0);
    }
}

void enviaMensEsperaResp(int soquete, unsigned char *mensagem, unsigned char *resposta){
    int timeout = 0; //colocar o tempo inicial
    int numTimeout = 0;
    bool timeOutFlag = false;
    do {
        //envia mensagem
        timeOutFlag = aguardaResposta(soquete, resposta, timeout);
        if (timeOutFlag) {
            numTimeout++;
            timeout *= 2;
        }
    } while (numTimeout < 15 && (timeOutFlag || !verificaIntegridade(resposta) || getTipo(resposta) == NACK));
    if (numTimeout == 15) {
        //erro, nao conseguiu resposta do outro lado termina o programa
    }
}

void enviaRespEsperaMens(int soquete, unsigned char *mensagem, unsigned char *resposta){
    //envia resposta
    aguardaMensagem(soquete, mensagem);
}

void trocaPapeis(int soquete, unsigned char *mensagem, unsigned char *resposta){
    do {
        enviaMensEsperaResp(soquete, resposta, mensagem);
    } while (getTipo(mensagem) != ACK);
}
