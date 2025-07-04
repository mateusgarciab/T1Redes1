#include "mensagem.h"
#include <stdbool.h>
#include <stdbool.h>
#include <stdio.h>

#define SEQ_INI_MASK 0b00000001
#define TIPO_MASK 0b00001111
#define TAM_MAX_DADOS 126
#define PADDING 0xff



void setMarcadorIni(unsigned char *mensagem){
    mensagem[0] = MARCADOR_INI;
}

unsigned char getTipo(unsigned char *mensagem){
    return mensagem[2] & TIPO_MASK;
}

void setTipo(unsigned char *mensagem, unsigned char tipo){
    mensagem[2] |= tipo;
}

unsigned char getNSeq(unsigned char *mensagem){
    return ((mensagem[1] & SEQ_INI_MASK) << 4) | (mensagem[2] >> 4);
}

void setNSeq(unsigned char *mensagem, unsigned char nSeq){
    if (nSeq > 15)
        mensagem[1] |= SEQ_INI_MASK;
    mensagem[2] = nSeq << 4;
}

unsigned char getTam(unsigned char *mensagem){
    return mensagem[1] >> 1;
}

void setTam(unsigned char *mensagem, unsigned char tam){
    mensagem[1] = tam << 1;
}

bool verificaIntegridade(unsigned char *mensagem){
    unsigned char soma = mensagem[1] + mensagem[2];
    unsigned char tam = getTam(mensagem);
    if (tam) {
        for (unsigned char i = 0; i < tam; i++)
            soma += mensagem[i+4];
    }
    return soma == mensagem[3];
}

void checkSum(unsigned char *mensagem){
    unsigned char soma = mensagem[1] + mensagem[2];
    unsigned char tam = getTam(mensagem);
    if (tam) {
        for (unsigned char i = 0; i < tam; i++)
            soma += mensagem[i+4];
    }
    mensagem[3] = soma;
}

int setDados(unsigned char *mensagem, unsigned char *dados, int tam){
    int tamInserido = 0;
    int numPadding = 0;
    for (int i = 0; (tamInserido < tam) && (i < TAM_MAX_DADOS); i++) {
        mensagem[i+4] = dados[i - numPadding];
        tamInserido++;
        if (dados[i - numPadding] == 0x81 || dados[i - numPadding] == 0x88) {
            i++;
            mensagem[i+4] = PADDING;
            numPadding++;
        }
    }
    if (tam < TAM_MAX_DADOS)
        tamInserido += numPadding;
    return tamInserido;
}

unsigned char getDados(unsigned char *mensagem, unsigned char *dados){
    unsigned char tam = getTam(mensagem);
    unsigned char numPadding = 0;
    unsigned char i;
    for (i = 0; i < tam; i++) {
        dados[i - numPadding] = mensagem[i+4];
        if (mensagem[i+4] == 0x81 || mensagem[i+4] == 0x88)  {
            i++;
            numPadding++;
        }
    }
    return i - numPadding;
}

int montaMensagem(unsigned char *mensagem, int tipo, unsigned char nSeq, void *dados, unsigned char tam){
    unsigned char dadosInseridos = 0;
    switch (tipo) {
        case ACK:
            setMarcadorIni(mensagem);
            setTam(mensagem, 0);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case NACK:
            setMarcadorIni(mensagem);
            setTam(mensagem, 0);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case OK:
            setMarcadorIni(mensagem);
            setTam(mensagem, 0);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case START_GAME:
            setMarcadorIni(mensagem);
            if (dados) {
                dadosInseridos = setDados(mensagem, (unsigned char *)dados, 3);
                setTam(mensagem, 3);
            }else{
                setTam(mensagem, 0);
            }
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case TAMANHO:
            setMarcadorIni(mensagem);
            dadosInseridos = setDados(mensagem, (unsigned char *)dados, 8);
            setTam(mensagem, 8);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case DADOS:
            setMarcadorIni(mensagem);
            dadosInseridos = setDados(mensagem, dados, tam);
            if (dadosInseridos > tam) {
                unsigned char aux = tam;
                tam = dadosInseridos;
                dadosInseridos = aux;
            }
            setTam(mensagem, tam);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case TEXTO:
            setMarcadorIni(mensagem);
            dadosInseridos = setDados(mensagem, dados, tam);
            setTam(mensagem, tam);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case VIDEO:
            setMarcadorIni(mensagem);
            dadosInseridos = setDados(mensagem, dados, tam);
            setTam(mensagem, tam);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case IMAGEM:
            setMarcadorIni(mensagem);
            dadosInseridos = setDados(mensagem, dados, tam);
            setTam(mensagem, tam);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case FIM_ARQ:
            setMarcadorIni(mensagem);
            setTam(mensagem, 0);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case MOV_RIGHT:
            setMarcadorIni(mensagem);
            setTam(mensagem, 0);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case MOV_UP:
            setMarcadorIni(mensagem);
            setTam(mensagem, 0);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case MOV_DOWN:
            setMarcadorIni(mensagem);
            setTam(mensagem, 0);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case MOV_LEFT:
            setMarcadorIni(mensagem);
            setTam(mensagem, 0);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case END_GAME:
            setMarcadorIni(mensagem);
            setTam(mensagem, 0);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
        case ERRO:
            setMarcadorIni(mensagem);
            setDados(mensagem, dados, 1);
            setTam(mensagem, 1);
            setNSeq(mensagem, nSeq);
            setTipo(mensagem, tipo);
            checkSum(mensagem);
            break;
    }
    return dadosInseridos;
}
