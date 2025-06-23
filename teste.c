// gcc teste.c ConexaoRawSocket.c -o teste

#include <stdio.h>
#include "ConexaoRawSocket.h"
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <stdlib.h>
#include <net/if.h>
#include "cliente.h"
#include <sys/statvfs.h>

int main() {
    FILE *arq = fopen("objetos/2.mp4", "r");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", "2.mp4");
        exit(1);
    }
}