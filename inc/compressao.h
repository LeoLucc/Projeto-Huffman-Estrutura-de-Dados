#ifndef COMPRESSAO_H
#define COMPRESSAO_H

#include <stdio.h>
#include "lista.h"
#include "arvore.h"
#include "compressao.h"
#include <stdint.h>

/**
 * @brief Calcula o tamanho de um arquivo.
 *
 * Esta função determina o tamanho de um arquivo em bytes,
 * movendo o ponteiro de arquivo para o final e retornando
 * a posição atual.
 *
 * @param arquivo Ponteiro para o arquivo a ser lido.
 * @return Tamanho do arquivo em bytes.
 */
unsigned long long int calcularTamanho(FILE *arquivo);

/**
 * @brief Mapeia os bytes da árvore de Huffman para seus respectivos códigos.
 *
 * Esta função percorre a árvore de Huffman e associa cada byte
 * a um código binário, armazenando essas associações em uma tabela.
 *
 * @param tree_node Ponteiro para o nó da árvore atual.
 * @param table Tabela que armazena os códigos Huffman.
 * @param codigo_parafolha Código acumulado para o nó atual.
 */
void mapearBytesParaCodigos(node_t *tree_node, BitHuff table[], BitHuff codigo_parafolha);

/**
 * @brief Calcula o tamanho do "lixo" após a compressão.
 *
 * Esta função calcula quantos bits não foram utilizados no último byte
 * após a compressão, com base na frequência dos bytes e na tabela de
 * códigos Huffman.
 *
 * @param frequency Array contendo as frequências dos bytes.
 * @param table Tabela de códigos Huffman.
 * @return Número de bits que não foram utilizados no último byte.
 */
int tamanho_lixo(int frequency[], BitHuff table[]);

/**
 * @brief Escreve o cabeçalho no arquivo compactado.
 *
 * Esta função escreve os dois primeiros bytes no arquivo compactado,
 * que armazenam o tamanho do lixo e da árvore.
 *
 * @param file Ponteiro para o arquivo onde os dados serão escritos.
 * @param trashSize Tamanho do lixo em bits.
 * @param treeSize Tamanho da árvore de Huffman em bits.
 */
void escrever_no_Cabecalho_Arquivo_compac(FILE *file, int trashSize, int treeSize);

/**
 * @brief Escreve a estrutura da árvore de Huffman no arquivo compactado.
 *
 * Esta função grava a estrutura da árvore de Huffman no arquivo,
 * garantindo que ela possa ser reconstruída durante a descompressão.
 *
 * @param file Ponteiro para o arquivo onde a árvore será escrita.
 * @param arvore_b Ponteiro para a raiz da árvore de Huffman.
 */
void escrever_arvore(FILE *file, node_t *arvore_b);

/**
 * @brief Grava os códigos compactados no arquivo de saída.
 *
 * Esta função lê o arquivo de entrada byte por byte, converte cada
 * byte em seu código correspondente de Huffman e grava esses códigos
 * compactados no arquivo de saída.
 *
 * @param fileIn Ponteiro para o arquivo de entrada.
 * @param fileOut Ponteiro para o arquivo de saída.
 * @param table Tabela de códigos Huffman.
 * @param trashSize Tamanho do lixo em bits.
 */
void gravarCodigos(FILE *fileIn, FILE *fileOut, BitHuff table[], int trashSize);

#endif // COMPRESSAO_H
