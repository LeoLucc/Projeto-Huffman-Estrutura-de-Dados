#include <stdio.h>
#include "lista.h"
#include "arvore.h"
#include <stdbool.h>

bool ehFolha(node_t **head)
{
    return (*head)->left == NULL && (*head)->right == NULL;
}

/*
    monta a árvore de huffman com a lista ordenada
*/

node_t *arvoreHuff(node_t **head, int *tamLista)
{

    node_t *left, *right, *Pai;

    while (*tamLista > 1)
    {

        left = removeCabeca(head, tamLista);
        right = removeCabeca(head, tamLista);

        Pai = inserir_ord(head, left->freq + right->freq, '*', tamLista);
        Pai->left = left;
        Pai->right = right;
    }

    return Pai;
}
/*
    acha o tamanho da árvore
*/

int tamanhoArvore(node_t *no)
{
    if (no == NULL)
    {
        return 0;
    }

    // Recursivamente calcula a profundidade das subárvores esquerda e direita
    int esq = tamanhoArvore(no->left);
    int dir = tamanhoArvore(no->right);

    unsigned char x = no->byte; // Acessa diretamente o byte do nó

    // Caso especial para caracteres '*' e '\\', onde precisamos contabilizar um valor especial para nós folha
    if ((((x == '*') || (x == '\\')) && ehFolha(&no)))
    {
        return 2 + esq + dir; // Considera um ajuste especial na profundidade
    }

    // Para nós normais, retorna a maior profundidade entre os ramos esquerdo e direito
    return 1 + esq + dir;
}