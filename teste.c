// gcc teste.c ConexaoRawSocket.c -o teste

#include <stdio.h>
#include "ConexaoRawSocket.h"
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>

int main() {
    char escolha;
    while (1) {
        scanf("%2c", &escolha);
        printf("escolha %c\n", escolha);
    }
        



    /* printf("\nm manda r recebe ");
    char letra;

    scanf("%s", &letra);
    int soquete = ConexaoRawSocket("enp2s0");
    int x;
    char buff[131] = "oloco meu ai nao";
    
    if (letra == 'm') {
        printf("Enviando mensagem\n");
        send(soquete, buff, 131, 0);
    } else {
         do {
            printf("Recebe\n");
            x = recv(soquete, buff, 131, 0);
            puts(buff);
        } while (x != -1);
    } */
}