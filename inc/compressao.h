#include "lista.h"
#include "arvore.h"
#include <string.h>
#include <stdlib.h>

#ifndef ENCODE_H
#define ENDODE_H

typedef struct bithuff
{
    int bitH;
    int size;
} BitHuff;
/**
 * @brief Pega o tamanho de um árquivo
 *
 * @param archive: O ponteiro que aponta para o Arquivo
 * @return O tamanho do arquivo em bytes na maior variável do c unsigne long long int
 */
unsigned long long int calcularTamanho(FILE *archive);
/**
 * @brief Montar a Tabela que vai conter as informações dos bit que vão representar os caracteres(bits de Huffman)
 *
 * @param tree_node: A raiz da árvore de Huffman
 * @param table: A tabela do que vai conter as informações dos bit de Huffman
 * @param code: A variável que vai armazenar os bits de Huffman e seu tamanho até chegar em um nó folha
 */
void mapearBytesParaCodigos(node_t *tree_node, BitHuff table[], BitHuff code);
/**
 * @brief Retorna o tamanho do lixo no final do árquivo
 *
 * @param frequency: Uma tabela com a frequência de cada caractere
 * @param table: A tabela do que contém as informações dos bit de Huffman
 * @return A quantidade de bits que não serão lidos no ultimo byte em inteiro
 */
int tamanho_lixo(int frequency[], BitHuff table[]);
/**
 * @brief Seta o primeiro byte do arquivo compactado
 *
 * @param file: Um ponteiro para o arquivo compactado
 * @param trashsize: O tamanho do lixo
 * @param treeSize: O tamanho da árvore de Huffman
 */

void escrever_no_Cabecalho_Arquivo_compac(FILE *file, int tamanho_lixo, int treeSize);
/**
 * @brief Escreve o cabeçalho no arquivo compactado contendo o tamanho do lixo e o tamanho da árvore de Huffman
 *
 * O cabeçalho é composto por dois bytes. O primeiro byte armazena os 3 bits mais significativos
 * do tamanho do lixo e os 5 bits mais significativos do tamanho da árvore. O segundo byte armazena
 * os 8 bits restantes do tamanho da árvore.
 *
 * @param file: Um ponteiro para o arquivo compactado
 * @param tamanho_lixo: O número de bits de lixo no final dos dados compactados
 * @param treeSize: O tamanho da árvore de Huffman
 */

void escrever_arvore(FILE *file, node_t *bt);
/**
 * @brief Setar os bytes com os novos bits correspondentes de cada caractere
 *
 *
 * @param fileIn: Ponteiro para o árquivo original
 * @param fileOut: Ponteiro para o arquivo compactado
 * @param table: A tabela do que contém as informações dos bit de Huffman
 * @param trashsize: O tamanho do lixo
 */

void gravarCodigos(FILE *fileIn, FILE *fileOut, BitHuff table[], int trashsize);

#endif
