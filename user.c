#include <stdio.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdlib.h>
#include "ConexaoRawSocket.h"
#define INIT_MASK 0b01111110

int main(){
    int soquete = ConexaoRawSocket("eth0"); //eth0 enp2s0 lo:para loop back
    const int timeoutMillis = 3000;
    struct timeval timeOut = { .tv_sec = timeoutMillis / 1000, .tv_usec = (timeoutMillis % 1000) * 1000};
    setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeOut, sizeof(timeOut));
    unsigned char buff[200];
    int x = 0;
    //x = recv(soquete, buff, 200, 0);
    //printf("Deu pau %d", x);
    do{
        x = recv(soquete, buff, 200, 0);
        printf("Deu pau %d\n", x);
        //puts(buff);
    }while(x != -1 && buff[0] != INIT_MASK);
    unsigned char cabecalho[4];
    cabecalho[0] = buff[1] >> 1;
    cabecalho[1] = ((buff[1] & 0b00000001) << 4) | (buff[2] >> 4); 
    cabecalho[2] = buff[2] & 0b00001111;
    cabecalho[3] = buff[3];
    printf("tam = %d, seq = %d, tipo = %d, checkSum = %d\n", (unsigned int)cabecalho[0], (unsigned int)cabecalho[1],
                                                             (unsigned int)cabecalho[2], (unsigned int)cabecalho[3]);
    for(int i = 4; i < 200; i++)
        printf("%d ", (unsigned int)buff[i]);
    printf("user ok\n");
    //puts(buff);
    return 0;
}