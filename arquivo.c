#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define OBJETOS "objetos\0" /* com \0 ou não ? */

char* obterExtensao(char* nomeArquivo) {
    char* ponto = strrchr(nomeArquivo, '.'); // Encontra o último ponto
    if (ponto) {
        return ponto + 1; // Retorna a string a partir do ponto
    }

    return NULL; // Retorna NULL se não houver ponto
}

int main() {
    DIR *dir;
    struct dirent *arquivo;


    dir = opendir(".");
    if (dir == NULL) {
        printf("Impossível abrir diretório"); /* perror ? */
        return 1;
    }

    int existeDir = 0;
    while ((arquivo = readdir(dir)) != NULL && !existeDir) {
        printf("%s\n", arquivo->d_name); 
        printf("%d\n", arquivo->d_type); 

        // verifica nome e se é um diretorio
        if ((strcmp(arquivo->d_name, OBJETOS) == 0) && (arquivo->d_type == 4)) {
            printf("entrou\n");
            existeDir = 1;
        }
    }
    closedir(dir);

    if (existeDir) {
        dir = opendir("./objetos");
        if (dir == NULL) {
            printf("Impossível abrir diretório");
            return 1;
        }
    
        while ((arquivo = readdir(dir)) != NULL) {
            printf("%s\n", arquivo->d_name); // Nome do arquivo
            printf("%s\n", obterExtensao(arquivo->d_name)); // extensão
            system("nano objetos/1.txt"); // system executa comandos no terminal, usei o nano pq n sei usar o vi direito e não achei como abrir o editor de texto normal   xdg-open file-or-url    libreoffice --writer file.odt
        }
    
        closedir(dir);

    } else {
        printf("Não existe diretorio objetos");
    }
    return 0;
}