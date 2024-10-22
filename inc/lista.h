#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Cria uma lista encadeada vazia.
 *
 * Esta função inicializa uma lista encadeada com um único nó.
 *
 * @return Retorna NULL para indicar que a lista está vazia.
 */
node_t *criar_List();

/**
 * @brief Converte um byte para um ponteiro void.
 *
 * Esta função aloca memória para um byte e retorna um ponteiro void
 * para esse byte.
 *
 * @param byte O byte a ser convertido.
 * @return Retorna um ponteiro void para o byte alocado.
 */
void *ponteiroparaVoid(unsigned char byte);

/**
 * @brief Remove o nó da cabeça da lista.
 *
 * Esta função remove o primeiro nó da lista encadeada e atualiza a
 * cabeça da lista.
 *
 * @param head Ponteiro para o ponteiro da cabeça da lista.
 * @param tamLista Ponteiro para um inteiro que armazena o tamanho da lista.
 * @return Retorna o nó removido da cabeça da lista.
 */
node_t *removeCabeca(node_t **head, int *tamLista);

/**
 * @brief Insere um nó ordenadamente na lista encadeada.
 *
 * Esta função insere um novo nó na lista de acordo com a frequência,
 * mantendo a lista ordenada.
 *
 * @param head Ponteiro para o ponteiro da cabeça da lista.
 * @param freq Frequência do caractere a ser inserido.
 * @param byte Caractere a ser inserido.
 * @param tamLista Ponteiro para um inteiro que armazena o tamanho da lista.
 * @return Retorna o novo nó inserido.
 */
node_t *inserir_ord(node_t **head, int freq, char byte, int *tamLista);

/**
 * @brief Cria uma lista de frequência a partir de um arquivo.
 *
 * Esta função lê um arquivo e conta as frequências dos caracteres,
 * criando uma lista encadeada com os caracteres e suas frequências.
 *
 * @param arq Ponteiro para o arquivo a ser lido.
 * @param frequencias Array que armazena as frequências dos caracteres.
 * @param tamLista Ponteiro para um inteiro que armazena o tamanho da lista.
 * @return Retorna a cabeça da lista encadeada contendo os caracteres e suas frequências.
 */
node_t *listFreq(FILE *arq, int frequencias[256], int *tamLista);

/**
 * @brief Recupera um byte de um ponteiro void.
 *
 * Esta função recupera o byte armazenado em um ponteiro void.
 *
 * @param byte Ponteiro para o nó que contém o byte.
 * @return Retorna o byte recuperado.
 */
unsigned char recuperarByteDePonteiroVoid(node_t *byte);

/**
 * @brief Cria um novo nó.
 *
 * Esta função cria um novo nó e inicializa seus campos.
 *
 * @param byte Ponteiro void que contém o byte a ser armazenado.
 * @param frequency Frequência do caractere a ser armazenado.
 * @return Retorna um ponteiro para o novo nó criado.
 */
node_t *criarNo(void *byte, int frequency);

#endif // LISTA_H
