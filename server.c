#include <stdio.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdlib.h>
#include "ConexaoRawSocket.h"

int main(){
    int soquete = ConexaoRawSocket("eth0");

    unsigned char buff[200];
    buff[0] = 0b01111110;
    buff[1] = 15<<1;
    buff[1] |= 0b00000001; 
    buff[2] = 5<<4;
    buff[2] |= 2;
    buff[3] = 69;
    for (int i = 4; i < 200; i++)
        buff[i] = i;

    send(soquete, buff, 200, 0);



    printf("server ok\n");
    return 0;
}