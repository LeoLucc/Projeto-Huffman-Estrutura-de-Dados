#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdbool.h>
#include "arvore.h"
#include "lista.h"

/**
 * @brief Verifica se um nó é uma folha da árvore.
 *
 * Esta função verifica se o nó fornecido é uma folha, ou seja,
 * se não possui filhos esquerdo ou direito.
 *
 * @param head Ponteiro para o ponteiro da raiz da árvore.
 * @return Retorna true se o nó for uma folha, caso contrário, retorna false.
 */
bool ehFolha(node_t **head);

/**
 * @brief Monta a árvore de Huffman a partir de uma lista ordenada.
 *
 * Esta função constrói a árvore de Huffman com base nos nós
 * fornecidos em uma lista ordenada. Os dois nós de menor
 * frequência são removidos da lista e um novo nó pai é criado.
 * O processo continua até que reste apenas um nó, que se torna
 * a raiz da árvore.
 *
 * @param head Ponteiro para o ponteiro da lista de nós a serem utilizados.
 * @param tamLista Ponteiro para um inteiro que irá armazenar o tamanho da lista.
 * @return Retorna o ponteiro para o nó raiz da árvore de Huffman.
 */
node_t *arvoreHuff(node_t **head, int *tamLista);

/**
 * @brief Calcula o tamanho da árvore.
 *
 * Esta função calcula e retorna o número de nós presentes na árvore,
 * considerando um ajuste especial para nós folha que têm caracteres
 * específicos ('*' e '\\').
 *
 * @param no Ponteiro para o nó que serve como raiz da árvore.
 * @return Retorna o número de nós na árvore.
 */
int tamanhoArvore(node_t *no);

#endif // TREE_H