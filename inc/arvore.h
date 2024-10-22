#include "lista.h"

#ifndef TREE_H
#define TREE_H

bool ehFolha(node_t **head);
node_t *arvoreHuff(node_t **head, int *tamLista);
int tamanhoArvore(node_t *no);
void print_filhos(node_t *raiz);

#endif