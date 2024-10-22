#include <stdio.h>
#include "lista.h"
#include "arvore.h"
#include "compressao.h"
#include "descompressao.h"
#include <string.h>

#define MAX 100

int main()
{
    int i;
    printf("SEJA BEM VINDO A CODIFICAÇÃO DE HUFFMAN\n");
    printf("SELECIONE UMA DAS OPÇÕES A SEGUIR:\n");
    printf("1 - COMPACTAR ARQUIVO\n");
    printf("2 - DESCOMPACTAR ARQUIVO\n");
    printf("OPÇÃO: ");
    scanf("%d", &i);
    if (i == 1)
    {

        FILE *archive;

        char file_path[MAX]; // Array que armazena o nome do arquivo

        int currentSize = 0;

        printf("Digite o caminho da pasta do arquivo: ");

        scanf("%s", file_path);
        // Lista encadeada para armazenar as frequencias em ordem crescente
        node_t *list_freq = criar_List();

        archive = fopen(file_path, "rb");

        unsigned long long int tamanhoOriginal = calcularTamanho(archive);
        // chamar a funcao que mede o tamanho do arquivo para o arquivo selecionado
        // colocar em tamanhoOriginal

        strcat(file_path, ".huff");
        // Abre o arquivo para leitura
        FILE *archiveOut = fopen(file_path, "wb");

        if (archiveOut == NULL)
        {
            perror("Erro ao abrir o arquivo");
            return 1;
        }
        // Caso de erro na leitura do arquivo
        if (archive == NULL)
        {
            perror("Erro ao abrir o arquivo");
            return 1;
        }
        int frequencia[256] = {0};
        // na codificação ASCII, existem 256 caracteres possíveis (0 a 255).

        list_freq = listFreq(archive, frequencia, &currentSize);

        arvoreHuff(&list_freq, &currentSize);

        BitHuff bithuff, table[256];

        bithuff.bitH = 0;

        bithuff.size = 0;

        memset(table, 0, sizeof(BitHuff) * 256);

        mapearBytesParaCodigos(list_freq, table, bithuff);
        // cabeca da lista de freq. , table[256],

        escrever_no_Cabecalho_Arquivo_compac(archiveOut, tamanho_lixo(frequencia, table), tamanhoArvore(list_freq));
        // int frequencia[256] = {0};

        escrever_arvore(archiveOut, list_freq);

        fseek(archive, 0, SEEK_SET);

        gravarCodigos(archive, archiveOut, table, tamanho_lixo(frequencia, table));

        unsigned long long int tamanhoCompac = calcularTamanho(archiveOut);

        fclose(archive);

        fclose(archiveOut);

        printf("TAMANHO DO ARQUIVO ORIGINAL EM BYTES: %llu\n", tamanhoOriginal);

        printf("TAMANHO DO ARQUIVO COMPACTADO EM BYTES: %llu\n", tamanhoCompac);

        if (tamanhoOriginal > tamanhoCompac)
        {
            printf("A COMPRESSÃO SE DEMONSTROU EFICIENTE\n");
        }
        else
        {
            printf("A COMPRESSÃO SE DEMONSTROU INEFICIENTE\n");
        }
    }
    else if (i == 2)
    {
        FILE *archive;

        FILE *archiveOut;

        char file_path[MAX];

        printf("Digite o caminho da pasta do arquivo: ");

        scanf("%s", file_path);

        archive = fopen(file_path, "rb");

        char *remove = strrchr(file_path, '.');

        if (remove != NULL)
        {
            *remove = '\0';
        }
        archiveOut = fopen(file_path, "wb");

        short int trash = tamanho_lixo_arquivoCompactado(archive);

        short int treeSize = tamanho_arvore_arquivoCompactado(archive);

        unsigned long long int archiveSize = calcularTamanho(archive);

        archiveSize -= 2;

        archiveSize -= treeSize;

        node_t *huffTree = montar_arvore(archive, &treeSize);

        recuperar_bytes(archive, archiveOut, trash, archiveSize, huffTree);

        fclose(archive);

        fclose(archiveOut);
    }

    return 0;
}