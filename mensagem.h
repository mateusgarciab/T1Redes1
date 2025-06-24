/* As mensagens seguem o template abaixo:
 *
 * -------------------------------------------------
 * | MarIni | Taman | Seq | Tipo |  Sum   | Dados  |
 * |   1B   |  7b   | 5b  |  4b  |   1B   | 0-127B |
 * -------------------------------------------------
 * |               4 Bytes                |   nB   |
 * -------------------------------------------------
 *
 * Abaixo segue o que cada campo representa:
 *      MarIni:
 *          Marcador de inicio, 1 Byte (01111110) que representa o inico da
 *          mensagem
 *      Taman:
 *          Tamanho, 7 bits (0-127) que indicam quantos bytes entao no campo de
 *          dados
 *      Seq:
 *          Sequencia, 5 bits (0-31) que indicam o numero de sequencia da
 *          mensagem
 *      Tipo:
 *          Tipo, 4 bits (0-15) que indicam o tipo da mensagem, cada tipo e
 *          definido detalhadamente mais abaixo
 *      Sum:
 *          CheckSum 1 Byte que contem a soma (Byte a Byte) dos campos Taman,
 *          Seq, Tipo e Dados, para a verificacao de corretude da mensagem
 *      Dados:
 *          Dados, 0-127 Bytes contendo dados genericos
 *
 *
 * Segue abaixo as definições de tipo e o que eles representam
 * Codigo - Tipo:
 *    0 - ACK:
 *          Resposta positiva, mensagem correta
 *
 *    1 - NACK:
 *          Resposta negativa, mensagem incorreta
 *
 *    2 - OK:
 *          Resposta positiva, mensagem correta + fiz o que pedia
 *
 *    3 - START_GAME:
 *          Cliente - Envia vazia para iniciar o jogo
 *
 *          Servidor - Envia com a posição inical do jogador e uma flag
 *                     indicando se tem tesouro nessa posição, em resposta
 *                     a uma mensagem do tipo 3 recebida
 *
 *    4 - TAMANHO:
 *          Mensagem que contém o tamnho em bytes de um arquivo
 *  
 *    5 - DADOS:
 *          Mensagem que contém dados de algum arquivo
 *
 *    6 - TEXTO:
 *          OK + Mensagem contem o nome de um arquivo .txt
 *
 *    7 - VIDEO:
 *          OK + Mensagem contém o nome de um arquivo .mp4
 *
 *    8 - IMAGEM:
 *          Ok + Mensagem contémo nome de um arquivo .jpg
 *
 *    9 - FIM_ARQ:
 *          Mensagem sinaliza que o arquivo que estava sendo enviado acabou
 *
 *    10 - MOV_RIGHT:
 *          Mensagem sinaliza movimentação do jogador para a direita
 *
 *    11 - MOV_UP:
 *          Mensagem sinaliza movimentação do jogador para cima
 *
 *    12 - MOV_DOWN:
 *          Mensagem sinaliza movimentação do jogador para baixo
 *
 *    13 - MOV_LEFT:
 *          Mensagem sinaliza movimentação do jogador para a esquerda
 *
 *    14 - END_GAME:
 *          Mensagem sinaliza o fim do jogo, todos os tesouros foram achados
 *
 *    15 - ERRO:
 *          Contém um código que indica o erro,
 *          0 indica a fata de permissão
 *          1 indica espaço insuficiente
 */

#ifndef MENSAGEM_H
#define MENSAGEM_H

#include <stdbool.h>
#include <stdio.h>

#define MARCADOR_INI 0b01111110

#define ACK 0
#define NACK 1
#define OK 2
#define START_GAME 3
#define TAMANHO 4
#define DADOS 5
#define TEXTO 6
#define VIDEO 7
#define IMAGEM 8
#define FIM_ARQ 9
#define MOV_RIGHT 10
#define MOV_UP 11
#define MOV_DOWN 12
#define MOV_LEFT 13
#define END_GAME 14
#define ERRO 15

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes
 * seguindo o template definido no inicio desse arquivo e preenche o primeiro
 * campo (MarIni) com 01111110 */
void setMarcadorIni(unsigned char *mensagem);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes
 * seguindo o template definido no inicio desse arquivo e retorna o quarto
 * campo (Tipo) */
unsigned char getTipo(unsigned char *mensagem);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes e um
 * unsigned char tipo seguindo o template e as definicoes feitas no inicio
 * desse arquivo e preenche o quarto campo (Tipo) com o tipo recebido */
void setTipo(unsigned char *mensagem, unsigned char tipo);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes
 * seguindo o template definido no inicio desse arquivo e retorna o terceiro
 * campo (Seq) */
unsigned char getNSeq(unsigned char *mensagem);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes e um
 * unsigned char nSeq seguindo o template e as definicoes feitas no inicio
 * desse arquivo e preenche o terceiro campo (Seq) com o nSeq recebido */
void setNSeq(unsigned char *mensagem, unsigned char nSeq);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes
 * seguindo o template definido no inicio desse arquivo e retorna o segundo
 * campo (Taman) */
unsigned char getTam(unsigned char *mensagem);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes e um
 * unsigned char tam seguindo o template e as definicoes feitas no inicio desse
 * arquivo e preenche o segundo campo (Taman) com o tam recebido */
void setTam(unsigned char *mensagem, unsigned char tam);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes
 * seguindo o template definido no inicio desse arquivo, realiza o checkSum e
 * compara com o quinto campo (Sum) recebido, se forem iguais retorna true,
 * retorna false caso contrario */
bool verificaIntegridade(unsigned char *mensagem);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes
 * seguindo o template definido no inicio desse arquivo e realiza o
 * checkSum preenchendo o quinto campo (Sum) com o checkSum realizado */
void checkSum(unsigned char *mensagem);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes
 * seguindo o template definido no inicio desse arquivo e um ponteiro dados
 * para um vetor unsigned char de 127 bytes e copia o sexto campo (Dados) da
 * mensagem para o vetor dados recebido e retorna o numero de bytes copiados */
unsigned char getDados(unsigned char *mensagem, unsigned char *dados);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes
 * seguindo o template definido no inicio desse arquivo e um ponteiro dados
 * para um vetor unsigned char de 127 bytes alem de um int tam que representa
 * o numero de bytes uteis dentro do vetor dados, copia esses bytes
 * preenchendo o sexto campo (Dados) com os dados recebidos
 * Se tam < TAM_MAX_DADOS retorna um numero de bytes inseridos no sexto campo
 * da mensagem (numero de bytes de dados copiados mais o numero de paddings
 * inseridos), caso contrario retorna o numero de bytes de dados que foram
 * copiados */
int setDados(unsigned char *mensagem, unsigned char *dados, int tam);

int setDadosAux(unsigned char *mensagem, unsigned char *dados, int tam, FILE *arq);

/* Recebe um ponteiro mensagem para um vetor unsigned char de 131 bytes, um int
 * tipo, um unsigned char nSeq, um ponteiro generico dados e um unsigned char
 * tam seguindo o template e as definicoes feitas no inicio desse arquivo e com
 * base no tipo recebido realiza a chamada das funcoes acima na ordem
 * correta para preencher corretamente cada campo da mensagem com os dados
 * recebidos, seguindo a definicao do que cada tipo de mensagem representa
 * feita no inicio desse arquivo, retorna o numero de bytes de dados que foram
 * inseridos na mensagem */
int montaMensagem(unsigned char *mensagem, int tipo, unsigned char nSeq, void *dados, unsigned char tam);

#endif
