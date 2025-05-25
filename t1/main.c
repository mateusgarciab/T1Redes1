#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include "mapa.h"
#include "servidor.h"
#include "cliente.h"



void iniciaServidor(int soquete, unsigned char *mensagem, unsigned char *resposta){
    tesouro_t *tesouros = malloc(sizeof(tesouro_t)*8);
    if (!tesouros) {
        fprintf(stderr, "Erro alocando memoria para nomes, encrrando\n");
        exit(1);
    }
    //Colocar aqui o codigo pra pegar os nomes, tamNome (acredito que e so dar
    //um strlen(nome)), tamanho e tipo dos arquivos pra preencher os campos de
    //cada estrutura tesouro, o campo achado é sempre false;
    mapa_t *mapa = geraMapa(tesouros);
    if (!mapa) {
        fprintf(stderr, "Erro ao carregar mapa, encerrando\n");
        exit(1);
    }
    rodaServidor(soquete, mapa, mensagem, resposta);
}

void inicializaCliente(int soquete, unsigned char *mensagem, unsigned char *resposta){
    unsigned char inicio;
    img_mapa_t *mapa = geraImgMapa();
    if (!mapa) {
        fprintf(stderr, "Erro gerando mapa, encerrando\n");
        exit(1);
    }
    printf("Use: 'w', 'a', 's', 'd' para mover-se pelo mapa\n");
    printf("O caractere 'F' no grid indica sua posicao atual\n");
    printf("'*' indica por onde ja passou\n");
    printf("'o' indica posições do grid onde um tesouro foi achado, note que");
    printf(" caso a posição atual contenha um tesouro ela sera o caractere");
    printf(" 'o' e não 'F'\n");
    printf("'.' indicam posições não passadas\n");
    printf("Exemplo grafico:\n\n");
    printf("|-----------------|\n");
    printf("| * * F . . . . . |\n");
    printf("| o * . . . . . . |\n");
    printf("| . * . . . . . . |\n");
    printf("| . * . . . . . . |\n");
    printf("| . * * . . . . . |\n");
    printf("| . . o . . . . . |\n");
    printf("| . * * . . . . . |\n");
    printf("| . . . . . . . . |\n");
    printf("|-----------------|\n");
    printf("Pressione qualquer tecla para começar: ");
    scanf("%1c", &inicio);
    rodaCliente(soquete, mapa, mensagem, resposta);
}




int main(int argc, char *argv[]){
    int soquete;
    //Cria o soquete
    unsigned char *mensagem = malloc(sizeof(unsigned char)*131);
    unsigned char *resposta = malloc(sizeof(unsigned char)*131);
    if (!mensagem || !resposta) {
        fprintf(stderr, "Erro ao alocar buffers, encerrando\n");
        exit(1);
    }
    switch (getopt(argc, argv, "sc")) {
        case 's':
            iniciaServidor(soquete, mensagem, resposta);
            break;
        case 'c':
            inicializaCliente(soquete, mensagem, resposta);
            break;
        default:
            fprintf(stderr, "Opção invalida, use -s para inicar no modo servidor ou -c para iniciar no modo cliente, encerrando\n");
            break;
    }
    free(mensagem);
    free(resposta);
    return 0;
}
