#include "mensagem.h"
#include <stdbool.h>
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
    do {
    //escuta a rede
    } while (mensagem[0] != MARCADOR_INI);
}

void transmite(unsigned char *mensagem, unsigned char *resposta){
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
