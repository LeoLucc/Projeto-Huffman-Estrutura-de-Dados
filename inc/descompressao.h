#ifndef DESCOMPRIMIR_H
#define DESCOMPRIMIR_H

#include <stdio.h>
#include "lista.h"
#include "arvore.h"

/**
 * @brief Obtém o tamanho do lixo do arquivo compactado.
 *
 * Esta função lê o primeiro byte do arquivo compactado e
 * extrai os 3 bits mais significativos para determinar
 * o tamanho do lixo.
 *
 * @param archive Ponteiro para o arquivo compactado.
 * @return Retorna o tamanho do lixo em bits, ou -1 se o arquivo não puder ser lido.
 */
int tamanho_lixo_arquivoCompactado(FILE *archive);

/**
 * @brief Obtém o tamanho da árvore do arquivo compactado.
 *
 * Esta função lê os dois primeiros bytes do arquivo compactado e
 * extrai os 5 bits menos significativos do primeiro byte e os 8 bits
 * do segundo byte para determinar o tamanho da árvore.
 *
 * @param archive Ponteiro para o arquivo compactado.
 * @return Retorna o tamanho da árvore em bits.
 */
int tamanho_arvore_arquivoCompactado(FILE *archive);

/**
 * @brief Monta a árvore de Huffman a partir do arquivo compactado.
 *
 * Esta função lê os dados do arquivo compactado em pré-ordem e monta
 * a árvore de Huffman de acordo com o tamanho fornecido.
 *
 * @param archive Ponteiro para o arquivo compactado.
 * @param treeSize Ponteiro para um inteiro que armazena o tamanho da árvore.
 * @return Retorna um ponteiro para a árvore de Huffman montada.
 */
node_t *montar_arvore(FILE *archive, short int *treeSize);

/**
 * @brief Verifica se o bit na posição 'i' do byte é 1.
 *
 * Esta função verifica se o bit na posição 'i' (0-7) de um byte
 * está definido como 1.
 *
 * @param c O byte a ser verificado.
 * @param i A posição do bit a ser verificado (0-7).
 * @return Retorna 1 se o bit estiver definido, caso contrário, retorna 0.
 */
int verificar_bit_i(unsigned char c, int i);

/**
 * @brief Recupera bytes do arquivo descompactado.
 *
 * Esta função lê os bits do arquivo compactado e escreve os
 * caracteres correspondentes no arquivo de saída usando a árvore
 * de Huffman.
 *
 * @param fileIn Ponteiro para o arquivo compactado.
 * @param fileOut Ponteiro para o arquivo de saída descompactado.
 * @param trashSize Tamanho do lixo no arquivo.
 * @param sizeFile Tamanho total do arquivo.
 * @param huffTree Ponteiro para a árvore de Huffman.
 */
void recuperar_bytes(FILE *fileIn, FILE *fileOut, short int trashSize, unsigned long long int sizeFile, node_t *huffTree);

#endif // DESCOMPRIMIR_H
