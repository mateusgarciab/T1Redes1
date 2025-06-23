#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include "mapa.h"
#include "servidor.h"
#include "cliente.h"

#include <dirent.h>
#include <string.h>
#define OBJETOS "objetos\0" 
#include "ConexaoRawSocket.h"

char* obterExtensao(char* nomeArquivo) {
    char* ponto = strrchr(nomeArquivo, '.'); // Encontra o último ponto
    if (ponto)
        return ponto + 1; // Retorna a string a partir do ponto

    return NULL; // Retorna NULL se não houver ponto
}

void pegaTesouros(tesouro_t *tesouros){
    DIR *dir;
    struct dirent *arquivo;
    struct stat dados;
    dir = opendir("./objetos");
    if (dir == NULL) {
        printf("Impossível abrir diretório");
        exit(1);
    }
    int i = 0; 
    char *aux;
    while ((arquivo = readdir(dir)) != NULL) {
        if (arquivo->d_name[0] != '.') {
            tesouros[i].nome = (unsigned char *)strdup(arquivo->d_name);
            puts(tesouros[i].nome);
            stat((char *)tesouros[i].nome, &dados);
            tesouros[i].tamanho = dados.st_size;
            tesouros[i].achado = false;
            aux = obterExtensao((char *)tesouros[i].nome);
            puts(aux);
            switch (aux[0]){
                case 't':
                    tesouros[i].tipo = 6;
                    break;
                case 'm':
                    tesouros[i].tipo = 7;
                    break;
                case 'j':
                    tesouros[i].tipo = 8;
                    break;
            }   
            printf("%d\n", tesouros[i].tipo);
            tesouros[i].tamNome = strlen((char *)tesouros[i].nome)+1;
            i++;
        } 
    }
    closedir(dir);
}

void iniciaServidor(int soquete, unsigned char *mensagem, unsigned char *resposta){
    tesouro_t *tesouros = malloc(sizeof(tesouro_t)*8);
    if (!tesouros) {
        fprintf(stderr, "Erro alocando memoria para nomes, encrrando\n");
        exit(1);
    }
    pegaTesouros(tesouros);
    printf("Tesouro pegos\n");
    mapa_t *mapa = geraMapa(tesouros);
    printf("Mapa gerado\n");
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
    int soquete = ConexaoRawSocket("enp2s0"); // enp4s0
    srand(time(NULL));
    //Cria o soquete
    unsigned char *mensagem = malloc(sizeof(unsigned char)*131);
    unsigned char *resposta = malloc(sizeof(unsigned char)*131);
    if (!mensagem || !resposta) {
        fprintf(stderr, "Erro ao alocar buffers, encerrando\n");
        exit(1);
    }
    switch (getopt(argc, argv, "sc")) {
        case 's':
            printf("Inicia servidor\n");
            iniciaServidor(soquete, mensagem, resposta);
            break;
        case 'c':
            printf("Inicia Cliente\n");
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
