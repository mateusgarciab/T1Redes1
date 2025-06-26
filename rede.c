#include "stdlib.h"
#include <stdbool.h>
#include "mensagem.h"
#include "rede.h"

#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include "ConexaoRawSocket.h"
#include <stdio.h>

#define TAMBUFF 131

bool aguardaResposta(int soquete, unsigned char *resposta, int timeout){
    int x = 0;
    const int timeoutMillis = timeout;
    struct timeval timeOut = { .tv_sec = timeoutMillis / 1000, .tv_usec = (timeoutMillis % 1000) * 1000};
    setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeOut, sizeof(timeOut));
    
    do {
        x = recv(soquete, resposta, TAMBUFF, 0);
    } while (x != -1 && resposta[0] != MARCADOR_INI);
    if (x == -1)
        return true;
    return false;
}

void aguardaMensagem(int soquete, unsigned char *mensagem){
    unsigned char nSeqAux = (getNSeq(mensagem) + 1) % 32;
    int x = 0;
    while (1) {
        do {
            x = recv(soquete, mensagem, TAMBUFF, 0);
        } while (x == -1 || mensagem[0] != MARCADOR_INI);
        if (verificaIntegridade(mensagem))
            return;
        montaMensagem(mensagem, NACK, nSeqAux, NULL, 0);
        send(soquete, mensagem, TAMBUFF, 0);
    }
}

void enviaMensEsperaResp(int soquete, unsigned char *mensagem, unsigned char *resposta){
    int timeout = 20; // tempo inicial
    int numTimeout = 0;
    bool timeOutFlag = false;
    do {
        send(soquete, mensagem, TAMBUFF, 0);
        if (getTipo(mensagem) == END_GAME)
            return;
        timeOutFlag = aguardaResposta(soquete, resposta, timeout);
        if (timeOutFlag) {
            numTimeout++;
            timeout *= 2;
        }
    } while (numTimeout < 15 && (timeOutFlag || !verificaIntegridade(resposta) || getTipo(resposta) == NACK));
    if (numTimeout == 15) {
        exit(1);
    }
}

void enviaRespEsperaMens(int soquete, unsigned char *mensagem, unsigned char *resposta){
    send(soquete, resposta, TAMBUFF, 0);
    aguardaMensagem(soquete, mensagem);
}

void trocaPapeis(int soquete, unsigned char *mensagem, unsigned char *resposta){
    do {
        enviaMensEsperaResp(soquete, resposta, mensagem);
    } while (getTipo(mensagem) != ACK && getTipo(resposta) != END_GAME);
}
