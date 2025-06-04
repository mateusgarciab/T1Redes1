#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include "mapa.h"
#include "servidor.h"
#include "cliente.h"

#include <dirent.h>
#include <string.h>
#define OBJETOS "objetos\0" 

/* char* obterExtensao(char* nomeArquivo) {
    char* ponto = strrchr(nomeArquivo, '.'); // Encontra o último ponto
    if (ponto)
        return ponto + 1; // Retorna a string a partir do ponto

    return NULL; // Retorna NULL se não houver ponto
} */

void abreArquivos(){
    DIR *dir;
    struct dirent *arquivo;

    dir = opendir(".");
    if (dir == NULL) {
        printf("Impossível abrir diretório"); 
        return 1;
    }
    int existeDir = 0;
    while ((arquivo = readdir(dir)) != NULL && !existeDir) {
/*         printf("%s\n", arquivo->d_name); 
        printf("%d\n", arquivo->d_type);  */
        // verifica nome e se é um diretorio
        if ((strcmp(arquivo->d_name, OBJETOS) == 0) && (arquivo->d_type == 4)) {
/*             printf("entrou\n"); */
            existeDir = 1;
        }
    }
    closedir(dir);

    char comando[100];
    strcat(comando, "xdg-open ");
    if (existeDir) {
        dir = opendir("./objetos");
        if (dir == NULL) {
            printf("Impossível abrir diretório");
            return 1;
        }
        while ((arquivo = readdir(dir)) != NULL) {
            printf("%s\n", arquivo->d_name); // Nome do arquivo
            /* printf(" %s\n", obterExtensao(arquivo->d_name)); */ // extensão
            if (arquivo->d_name[0] != '.') {
                strcat(comando, arquivo->d_name);
                /* puts(comando);
                system(comando);
                bzero(comando, 100); // limpa comando */
                strcat(comando, "xdg-open ./objetos/");
            } 
        }//xdg-open objetos/4.png
        closedir(dir);

    } else
        printf("Não existe diretorio objetos");
    return;
}

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
    int soquete = ConexaoRawSocket("eth0");;
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
