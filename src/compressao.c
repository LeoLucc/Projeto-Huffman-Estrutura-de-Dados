#include <stdio.h>
#include "lista.h"
#include "arvore.h"
#include "compressao.h"
#include <stdint.h>

// tamanho do arquivo
// vai usar para calcular antes e depois de compactar

unsigned long long int calcularTamanho(FILE *arquivo)
{
    // Armazena a posição atual do cursor no arquivo

    unsigned long long int posicaoAtual = ftell(arquivo);

    // Move o ponteiro até o final do arquivo
    fseek(arquivo, 0, SEEK_END);

    // Captura a posição atual, que é o tamanho do arquivo
    unsigned long long int tamanho_do_arquivo = ftell(arquivo);

    // Retorna para a posição original do cursor no arquivo
    fseek(arquivo, posicaoAtual, SEEK_SET) != 0;

    return tamanho_do_arquivo;
}

/////////////////////////////////////////////////////////////////
/////////////

void mapearBytesParaCodigos(node_t *tree_node, BitHuff table[], BitHuff codigo_parafolha)
{
    // Verifica se o nó atual é uma folha da árvore
    // Uma folha representa um caractere que será codificado
    if (tree_node->left == NULL && tree_node->right == NULL)
    {

        // O nó é uma folha, então armazenamos o código acumulado na tabela

        // getByteFromVoidPointer(tree_node->byte) converte o byte do nó para um índice

        // table é um array que mapeia cada byte para seu respectivo código Huffman
        table[recuperarByteDePonteiroVoid(tree_node)] = codigo_parafolha;

        return; // n tem mais nodes entao retorna
    }

    // Aumenta o tamanho do código acumulado para o próximo nível da árvore
    // Isso é necessário, pois cada vez que descemos um nível na árvore, o código se torna um bit maior
    codigo_parafolha.size++;

    // switch para controlar o fluxo da navegação na árvore
    switch (1)
    {
    // Caso 1: Processa o filho esquerdo do nó atual
    case 1:
        // Verifica se o nó esquerdo não é nulo
        if (tree_node->left != NULL)
        {
            // Cria uma cópia do código acumulado para a chamada recursiva do filho esquerdo
            BitHuff leftCode = codigo_parafolha;
            // Desloca o código para a esquerda (multiplicação por 2 em binário)
            // Isso adiciona um 0 ao final do código, representando a escolha do caminho esquerdo
            leftCode.bitH <<= 1;
            // Chama recursivamente a função para processar o nó esquerdo
            mapearBytesParaCodigos(tree_node->left, table, leftCode);
        }
        // Não break aqui permite continuar para o próximo caso

    // Caso 2: Processa o filho direito do nó atual
    case 2:
        // Verifica se o nó direito não é nulo
        if (tree_node->right != NULL)
        {
            // Cria uma cópia do código acumulado para a chamada recursiva do filho direito
            BitHuff rightCode = codigo_parafolha;
            // Desloca o código para a esquerda e adiciona um 1 ao final do código
            // Isso representa a escolha do caminho direito
            rightCode.bitH = (rightCode.bitH << 1) | 1;
            // Chama recursivamente a função para processar o nó direito
            mapearBytesParaCodigos(tree_node->right, table, rightCode);
        }
        break; // Finaliza o switch
    }
}

/// /// /////////////////////////////////////////////////////////////////////////

int tamanho_lixo(int frequency[], BitHuff table[])
{
    long bits_antes = 0;
    long bits_depois = 0;

    int i = 0;
    while (i < 256)
    {
        if (frequency[i] != 0)
        {
            // bits_antes é a quantidade de bits original (sem compressao)
            bits_antes += frequency[i] * 8;
            // multiplicado por 8 pq i eh armazenado em um byte antes da compressao

            // bits_depois é a quantidade total de bits após a compressão
            // Usa o tamanho do código em huff armazenado em table[i].size
            bits_depois += frequency[i] * table[i].size;
        }
        i++;
    }

    // Calcula o "lixo" (bits não usados no último byte)

    // diferença entre bits_antes e bits_depois indica quantos bits foram "perdidos" ou não utilizados.

    int trash = (bits_antes - bits_depois) % 8;

    return trash;
}

/*
    Função que  cria e escreve os dos dois primeiros bytes
    que armazenam o tamanho do lixo e da árvore.
*/

void escrever_no_Cabecalho_Arquivo_compac(FILE *file, int trashSize, int treeSize)
{
    unsigned char primeiro_byte = 0x00; // Inicializa o primeiro byte com 0
    unsigned char segundo_byte = 0x00;  // Inicializa o segundo byte com 0

    // Primeiro byte:
    // - Pega os 3 bits mais significativos do tamanho do lixo (trashSize)
    // - Usa o operador << para deslocar o trashSize 5 bits para a esquerda, colocando-o nos 3 bits mais significativos do byte.
    primeiro_byte = primeiro_byte | (trashSize << 5);

    // - Agora, pega os 5 bits mais significativos do tamanho da árvore (treeSize)
    // - Usa o operador >> para deslocar 8 bits para a direita, movendo os 5 bits mais significativos para o lugar correto.
    primeiro_byte = primeiro_byte | (treeSize >> 8);

    // Segundo byte:
    // - Aqui colocamos os 8 bits menos significativos do tamanho da árvore
    // - Como os 8 bits menos significativos já estão no lugar certo, basta fazer uma operação OR simples.
    segundo_byte = segundo_byte | treeSize;

    // Escreve os dois bytes no arquivo
    fwrite(&primeiro_byte, sizeof(unsigned char), 1, file); // Escreve o primeiro byte
    fwrite(&segundo_byte, sizeof(unsigned char), 1, file);  // Escreve o segundo byte
}

/*
    setar a árvore no arquivo compactado
*/
///////////////////////////////////////////////////
///////////////////////

// escrever a estrutura da árvore de Huffman no arquivo compactado
// oque garante ser reconstruída durante a descompressão.

void escrever_arvore(FILE *file, node_t *arvore_b)
{
    if (arvore_b == NULL)
    { // se arvore for nula
        return;
    }
    if (arvore_b->left == NULL && arvore_b->right == NULL && (arvore_b->byte == '*' || arvore_b->byte == '\\'))
    {

        // uint8_t representa um inteiro sem sinal de 8 bits

        // criar um escape
        uint8_t scape = (uint8_t)'\\';

        fwrite(&scape, sizeof(uint8_t), 1, file); // file = arquivo_comprimido
    }
    uint8_t byte = arvore_b->byte;

    fwrite(&byte, sizeof(uint8_t), 1, file); // escreve em um arquivo comprimido

    // chama da raiz para os dois nos a direita e esqueda

    // funcoes escrrever_arvore_no_cabecalho
    // chamadas recursivamentes com o arquivo e arvore esquerda/direita
    // como argumentto
    escrever_arvore(file, arvore_b->left);

    // quando for null
    // vai a direita
    escrever_arvore(file, arvore_b->right);
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////

// Essa função lê o arquivo de entrada fileIn byte por byte,
// converte cada byte em seu código correspondente de Huffman (usando a tabela BitHuff)
//  e grava esses códigos compactados no arquivo de saída fileOut

void gravarCodigos(FILE *fileIn, FILE *fileOut, BitHuff table[], int trashSize)
{
    uint8_t acumulador_bits = 0; // Acumulador para armazenar os bits temporários (8 bits), que serão escritos no arquivo de saída.
    int tamanho_acumulador = 0;  // Contador para a quantidade de bits armazenados no acumulador, para saber quando ele está "cheio".
    int caractere;               // Variável para armazenar temporariamente cada caractere lido do arquivo de entrada.

    // Lê o arquivo de entrada byte por byte
    // A função 'getc' lê um caractere (byte) por vez do arquivo 'fileIn'. A leitura continua até atingir o final do arquivo (EOF).
    while ((caractere = getc(fileIn)) != EOF)
    {
        BitHuff entry = table[caractere]; // A tabela de Huffman ('table') contém o código binário (compressão) para cada caractere.

        // Aqui, 'entry' armazena o código binário e seu tamanho (em bits) correspondente ao caractere lido.

        // Percorre cada bit do código de Huffman correspondente ao caractere atual
        // Esse loop percorre o código de Huffman do último bit até o primeiro (do bit mais significativo ao menos significativo).
        for (int i = entry.size - 1; i >= 0; --i)
        {
            // Inserir o próximo bit no acumulador
            // A operação 'acumulador_bits << 1' desloca os bits do acumulador uma posição para a esquerda, abrindo espaço para o novo bit.
            // '((entry.bitH >> i) & 1)' extrai o i-ésimo bit do código Huffman (1 ou 0) e o adiciona no acumulador.
            acumulador_bits = (acumulador_bits << 1) | ((entry.bitH >> i) & 1);
            tamanho_acumulador++; // Incrementa o contador, indicando que mais um bit foi adicionado ao acumulador.

            // Quando o acumulador estiver cheio (8 bits completos), ele deve ser escrito no arquivo de saída.
            if (tamanho_acumulador == 8)
            {
                fwrite(&acumulador_bits, sizeof(uint8_t), 1, fileOut); // Escreve 1 byte (8 bits) no arquivo de saída 'fileOut'.
                acumulador_bits = 0;                                   // Reseta o acumulador para que ele possa armazenar novos bits.
                tamanho_acumulador = 0;                                // Reinicia o contador para os próximos bits a serem acumulados.
            }
        }
    }

    // Após ler todos os bytes do arquivo de entrada, pode haver bits restantes no acumulador que não foram escritos ainda.
    // Isso acontece quando o arquivo termina mas o acumulador não chegou a 8 bits completos.
    if (tamanho_acumulador > 0)
    {
        // Adiciona o "lixo" (bits extras que não fazem parte dos dados) ao final do último byte.
        // O 'trashSize' indica quantos desses bits finais são irrelevantes para a informação, necessários para completar o último byte.
        acumulador_bits <<= trashSize;

        // Escreve o último byte (com os bits restantes e os bits de "lixo") no arquivo de saída.
        fwrite(&acumulador_bits, sizeof(uint8_t), 1, fileOut);
    }
}
