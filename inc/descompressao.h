#include "lista.h"
#include "arvore.h"
#include <string.h>
#include <stdlib.h>

#ifndef DECODE_H
#define DEDODE_H
/**
 * @brief Pega quantos bits não serão lidos(os três primeiros bits do arquivo)
 *
 * @param archive: Ponteiro para o arquivo compactado
 * @return O tamanho do lixo em inteiro
 */
int tamanho_lixo_arquivoCompactado(FILE *archive);
/**
 * @brief Pega o tamanho da árvore no arquivo(os 5 bits menos significativos do primeiro arquivo e o segundo byte inteiro)
 *
 * @param archive: Ponteiro para o arquivo compactado
 * @return O tamanho da árvore em inteiro
 */
int tamanho_arvore_arquivoCompactado(FILE *archive);
/**
 * @brief Pega a árvore no arquivo compactado
 *
 * @param archive: Ponteiro para o arquivo compactado
 * @param treeSize: Tamanho da árvore
 * @return Um ponteiro para a raiz da árvore que estava no arquivo
 */
node_t *montar_arvore(FILE *archive, short int *treeSize);
/**
 * @brief checa se o bit em determinada posição é 1
 *
 * @param c:caractere para checar a posição
 * @param i:posição que vai ser checada
 * @return retorna 1 caso esteja e 0 caso não esteja
 */
int verificar_bit_i(unsigned char c, int i);
/**
 * @brief Usa as informações do arquivo compactado para voltar ao arquivo original
 *
 * @param fileIn: Ponteiro para o arquivo compactado
 * @param fileOut: Ponteiro para o arquivo descompactado
 * @param trashSize: O tamanho do lixo
 * @param sizeFile: O tamanho do arquivo compactado
 * @param huffTree: A árvore de Huffmand
 */
void recuperar_bytes(FILE *fileIn, FILE *fileOut, short int trashSize, unsigned long long int sizeFile, node_t *huffTree);

#endif