#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

#include "mapa.h"

int main() {
    tesouro_t t[8];
    for (int i = 0; i < 8; i++) {
        t[i].
    }
    mapa_t meuMapa = geraMapa()

   /* DIR *dir;
    struct dirent *arquivo;


    dir = opendir(".");
    if (dir == NULL) {
        perror("Impossível abrir diretório");
        return 1;
    }

    while ((arquivo = readdir(dir)) != NULL) {
        printf("%s\n", arquivo->d_name); // Nome do arquivo
        printf("%d\n", arquivo->d_type); // diretorio ou arquivo, 4 - diretorio  8 - arquivo "normal"
        printf("%d\n", arquivo->d_reclen); // Supostamente o tamnho, mas não sei do que 

    }

    closedir(dir);


    system("ls -l ./objetos"); */

    return 0;
}