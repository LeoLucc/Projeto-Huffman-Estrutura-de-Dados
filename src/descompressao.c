#include <stdio.h>
#include "lista.h"
#include "arvore.h"
#include "descompressao.h"

/*
    pega o tamanho do lixo que está nos 3 bits mais significativos
    do arquivo compactado
*/
int tamanho_lixo_arquivoCompactado(FILE *archive)
{
    if (archive == NULL)
    {
        return -1; // Retorna erro se o arquivo não puder ser lido
    }

    // Ler o primeiro byte do arquivo
    unsigned char byte;
    fread(&byte, sizeof(unsigned char), 1, archive);

    // Aplicar uma máscara para extrair apenas os 3 bits mais significativos
    int trashSize = byte & 0xE0; // 0xE0 é 11100000 em binário, que isola os 3 bits mais significativos

    // Deslocar os bits isolados de volta para a direita para obter o valor real
    trashSize = trashSize >> 5;

    // Retornar o ponteiro do arquivo para o início
    fseek(archive, 0, SEEK_SET);

    return trashSize;
}

/*
    pega o tamanho da árvore que está nos 5 bits menos significativos do primeiro
    byte e nos 8 bits do segundo byte
*/
int tamanho_arvore_arquivoCompactado(FILE *archive)
{
    unsigned char byte1, byte2;

    // leitura dos dois bytes do arquivo
    fread(&byte1, sizeof(unsigned char), 1, archive);
    fread(&byte2, sizeof(unsigned char), 1, archive);

    // considerando os 5 bits menos significativos do byte1
    byte1 = byte1 & 0x1F; // 0x1F (00011111 em binário) mantém apenas os 5 bits menos significativos

    // combinar os dois bytes para formar o tamanho da árvore
    short int treeSize = (byte1 << 8) | byte2;

    return treeSize;
}

/*
    Percorrendo Arvore de Huffman e monta ela sabendo que está em pré ordem
*/
node_t *montar_arvore(FILE *archive, short int *treeSize)
{
    unsigned char byte;
    node_t *huffTree = NULL;

    while (*treeSize > 0)
    {
        fread(&byte, sizeof(unsigned char), 1, archive);

        // Trata caractere especial (sempre folha)
        if (byte == '\\')
        {
            (*treeSize)--;
            fread(&byte, sizeof(unsigned char), 1, archive);
            huffTree = criarNo(ponteiroparaVoid(byte), 0);
            (*treeSize)--;
            return huffTree;
        }

        huffTree = criarNo(ponteiroparaVoid(byte), 0);
        (*treeSize)--;

        // Se for um nó intermediário
        if (byte == '*')
        {
            // Busca para a esquerda e para a direita
            huffTree->left = montar_arvore(archive, treeSize);
            huffTree->right = montar_arvore(archive, treeSize);
        }
        return huffTree;
    }

    return NULL; // Caso o tamanho da árvore seja zero
}

int verificar_bit_i(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    return mask & c;
}

/*
    Preencher arquivo descompactado
*/

void recuperar_bytes(FILE *fileIn, FILE *fileOut, short int trashSize, unsigned long long int sizeFile, node_t *huffTree)
{
    unsigned char byte;                           // Armazena o byte lido do arquivo
    node_t *aux = huffTree;                       // Ponteiro auxiliar para percorrer a árvore de Huffman
    int bitPosition = 0;                          // Controla a posição do bit dentro de cada byte
    int bitsToProcess = sizeFile * 8 - trashSize; // Número total de bits a processar, descontando o lixo no último byte

    // Itera enquanto houver bits para processar
    for (int bitIndex = 0; bitIndex < bitsToProcess; bitIndex++)
    {
        // A cada múltiplo de 8, lemos um novo byte
        if (bitIndex % 8 == 0)
        {
            fread(&byte, sizeof(unsigned char), 1, fileIn);
        }

        // Calcula a posição do bit no byte (bit mais significativo é 7, menos significativo é 0)
        bitPosition = 7 - (bitIndex % 8);

        // Navegação pela árvore de Huffman: vai para a direita se o bit estiver setado (1), para a esquerda se for 0
        if (verificar_bit_i(byte, bitPosition))
        {
            aux = aux->right; // Vai para a direita
        }
        else
        {
            aux = aux->left; // Vai para a esquerda
        }

        // Se encontrou uma folha na árvore, recupera o caractere correspondente e escreve no arquivo de saída
        if (ehFolha(&aux))
        {
            unsigned char caractere = recuperarByteDePonteiroVoid(aux); // Recupera o valor do nó folha
            fwrite(&caractere, sizeof(unsigned char), 1, fileOut);      // Escreve o caractere no arquivo de saída
            aux = huffTree;                                             // Volta o ponteiro para a raiz da árvore para decodificar a próxima sequência de bits
        }
    }
}