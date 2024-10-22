#include <stdio.h>
#include "lista.h"
#include "arvore.h"
#include "compressao.h"
#include "descompressao.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#define MAX 100

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

void liberarArvore(node_t *node)
{
    if (node == NULL)
        return;                 // Se o nó for nulo, retorna
    liberarArvore(node->left);  // Libera a subárvore esquerda
    liberarArvore(node->right); // Libera a subárvore direita
    free(node);                 // Libera o nó atual
}

// Teste para a função ehFolha
void test_ehFolha()
{
    node_t folha;
    folha.left = NULL;
    folha.right = NULL;

    node_t *ptrFolha = &folha; // Ponteiro para o nó

    CU_ASSERT_TRUE(ehFolha(&ptrFolha)); // Passando o ponteiro duplo

    node_t noComFilhos;
    noComFilhos.left = &folha;
    noComFilhos.right = NULL;

    node_t *ptrNoComFilhos = &noComFilhos; // Ponteiro para o nó com filhos

    CU_ASSERT_FALSE(ehFolha(&ptrNoComFilhos)); // Passando o ponteiro duplo
}

// Teste para a função tamanhoArvore
void test_tamanhoArvore()
{
    // Caso de árvore nula
    CU_ASSERT_EQUAL(tamanhoArvore(NULL), 0);

    // Criação de nós
    node_t noA, noB, noC;
    noA.byte = 'A';
    noA.left = NULL;
    noA.right = NULL;

    noB.byte = '*'; // Caractere especial
    noB.left = NULL;
    noB.right = NULL;

    noC.byte = 'B';
    noC.left = &noA;
    noC.right = &noB;

    // Teste para árvore simples
    CU_ASSERT_EQUAL(tamanhoArvore(&noA), 1);

    // Teste para árvore com caractere especial
    CU_ASSERT_EQUAL(tamanhoArvore(&noB), 2); // Caractere especial '*'

    // Teste para árvore completa
    CU_ASSERT_EQUAL(tamanhoArvore(&noC), 4); // Raiz + nós folha, incluindo o nó especial
}

// Teste para a função inserir_ord
void test_inserir_ord()
{
    node_t *head = NULL;
    int tamLista = 0;

    // Inserir o primeiro nó
    node_t *n1 = inserir_ord(&head, 5, 'a', &tamLista);
    CU_ASSERT_PTR_NOT_NULL(n1);
    CU_ASSERT_EQUAL(head, n1);
    CU_ASSERT_EQUAL(n1->freq, 5);
    CU_ASSERT_EQUAL(n1->byte, 'a');
    CU_ASSERT_PTR_NULL(n1->prox);
    CU_ASSERT_EQUAL(tamLista, 1);

    // Inserir um segundo nó com freq maior
    node_t *n2 = inserir_ord(&head, 10, 'b', &tamLista);
    CU_ASSERT_PTR_NOT_NULL(n2);
    CU_ASSERT_EQUAL(n2->freq, 10);
    CU_ASSERT_EQUAL(n2->byte, 'b');
    CU_ASSERT_PTR_NULL(n2->prox);
    CU_ASSERT_EQUAL(head->prox, n2); // n2 deve estar após n1
    CU_ASSERT_EQUAL(tamLista, 2);

    // Inserir um terceiro nó com freq menor (deve ser o novo head)
    node_t *n3 = inserir_ord(&head, 3, 'c', &tamLista);
    CU_ASSERT_PTR_NOT_NULL(n3);
    CU_ASSERT_EQUAL(n3->freq, 3);
    CU_ASSERT_EQUAL(n3->byte, 'c');
    CU_ASSERT_PTR_EQUAL(n3->prox, n1); // n3 deve estar antes de n1
    CU_ASSERT_EQUAL(head, n3);
    CU_ASSERT_EQUAL(tamLista, 3);

    // Inserir um nó com freq intermediária (deve ir entre n1 e n2)
    node_t *n4 = inserir_ord(&head, 7, 'd', &tamLista);
    CU_ASSERT_PTR_NOT_NULL(n4);
    CU_ASSERT_EQUAL(n4->freq, 7);
    CU_ASSERT_EQUAL(n4->byte, 'd');
    CU_ASSERT_PTR_EQUAL(n4->prox, n2); // n4 deve estar entre n1 e n2
    CU_ASSERT_EQUAL(n1->prox, n4);
    CU_ASSERT_EQUAL(tamLista, 4);
}

void test_arvoreHuff()
{
    node_t *head = NULL;
    int tamLista = 0;

    // Insira elementos na lista para criar a árvore de Huffman
    inserir_ord(&head, 5, 'A', &tamLista);
    inserir_ord(&head, 3, 'B', &tamLista);
    inserir_ord(&head, 2, 'C', &tamLista);

    // Chama a função para criar a árvore de Huffman
    node_t *huffTree = arvoreHuff(&head, &tamLista);

    // Verifica se a árvore foi criada corretamente
    CU_ASSERT_PTR_NOT_NULL(huffTree);
    CU_ASSERT_EQUAL(huffTree->freq, 10); // Frequência da raiz deve ser 10 (5 + 5)

    // Verifica a estrutura da árvore
    CU_ASSERT_PTR_NOT_NULL(huffTree->left);  // Deve haver um filho esquerdo
    CU_ASSERT_PTR_NOT_NULL(huffTree->right); // Deve haver um filho direito

    // Verifica se o nó da esquerda tem frequência 5 (de 'A')
    CU_ASSERT_EQUAL(huffTree->left->freq, 5);
    // Verifica se o nó da direita tem frequência 5 (de 'B')
    CU_ASSERT_EQUAL(huffTree->right->freq, 5);
}

// Teste para a função recuperar_bytes
void test_recuperar_bytes()
{
    FILE *fileIn = fopen("entrada.bin", "wb");
    FILE *fileOut = fopen("saida.txt", "wb");
    CU_ASSERT_PTR_NOT_NULL(fileIn);
    CU_ASSERT_PTR_NOT_NULL(fileOut);

    unsigned char bytes[] = {0b00000001, 0b00000010};
    fwrite(bytes, sizeof(unsigned char), sizeof(bytes), fileIn);
    fclose(fileIn);

    node_t *head = NULL;
    int tamLista = 0;
    inserir_ord(&head, 5, 'A', &tamLista);
    inserir_ord(&head, 3, 'B', &tamLista);

    node_t *huffTree = arvoreHuff(&head, &tamLista);

    fileIn = fopen("entrada.bin", "rb");
    recuperar_bytes(fileIn, fileOut, 0, sizeof(bytes), huffTree);
    fclose(fileIn);
    fclose(fileOut);

    fileOut = fopen("saida.txt", "rb");
    CU_ASSERT_PTR_NOT_NULL(fileOut);

    unsigned char output[2];
    fread(output, sizeof(unsigned char), sizeof(output), fileOut);
    CU_ASSERT_EQUAL(output[1], 'B');

    fclose(fileOut);
}

// Teste para a função escrever_arvore
void test_escrever_arvore_simples()
{
    FILE *file = fopen("teste.bin", "wb");
    CU_ASSERT_PTR_NOT_NULL(file);

    node_t raiz;
    raiz.byte = 'A';
    raiz.left = NULL;
    raiz.right = NULL;

    escrever_arvore(file, &raiz);
    fclose(file);

    file = fopen("teste.bin", "rb");
    CU_ASSERT_PTR_NOT_NULL(file);

    uint8_t resultado;
    fread(&resultado, sizeof(uint8_t), 1, file);
    CU_ASSERT_EQUAL(resultado, 'A');

    fclose(file);
}

// Teste para a função calcularTamanho
void test_calcularTamanho()
{
    // Cria um arquivo temporário para o teste
    FILE *arquivo = fopen("teste_tamanho.txt", "wb");
    CU_ASSERT_PTR_NOT_NULL(arquivo);

    // Escreve alguns dados no arquivo
    const char *dados = "Testando tamanho do arquivo.";
    fwrite(dados, sizeof(char), strlen(dados), arquivo);
    fclose(arquivo);

    // Abre o arquivo em modo leitura
    arquivo = fopen("teste_tamanho.txt", "rb");
    unsigned long long int tamanho = calcularTamanho(arquivo);
    CU_ASSERT_EQUAL(tamanho, strlen(dados)); // O tamanho deve ser igual ao tamanho dos dados escritos

    fclose(arquivo);
    remove("teste_tamanho.txt"); // Remove o arquivo após o teste
}

// Teste para a função mapearBytesParaCodigos
void test_mapearBytesParaCodigos()
{
    // Configura a árvore de Huffman para o teste
    node_t folhaA = {.byte = 'A', .freq = 5, .left = NULL, .right = NULL};
    node_t folhaB = {.byte = 'B', .freq = 3, .left = NULL, .right = NULL};
    node_t raiz = {.byte = '*', .freq = 8, .left = &folhaA, .right = &folhaB};

    BitHuff table[256] = {0};                // Inicializa a tabela de códigos
    BitHuff codigo = {.bitH = 0, .size = 0}; // Código inicial vazio

    // Mapeia os bytes para códigos
    mapearBytesParaCodigos(&raiz, table, codigo);

    // Verifica se os códigos foram mapeados corretamente
    CU_ASSERT_EQUAL(table[(unsigned char)folhaA.byte].bitH, 0b0); // 'A' deve ter código 0
    CU_ASSERT_EQUAL(table[(unsigned char)folhaB.byte].bitH, 0b1); // 'B' deve ter código 1
}

// Teste para a função tamanho_lixo
void test_tamanho_lixo()
{
    int frequency[256] = {0};
    frequency['A'] = 8; // Frequência de 8 para 'A'
    frequency['B'] = 8; // Frequência de 8 para 'B'

    // Tamanhos dos códigos Huffman
    BitHuff table[256] = {0};
    table[(unsigned char)'A'].size = 1; // 'A' codificado em 1 bit
    table[(unsigned char)'B'].size = 2; // 'B' codificado em 2 bits

    int lixo = tamanho_lixo(frequency, table);

    // Cálculo esperado:
    // bits_antes = (8 * 8) + (8 * 8) = 128
    // bits_depois = (8 * 1) + (8 * 2) = 24
    // trash = (128 - 24) % 8 = 0
    CU_ASSERT_EQUAL(lixo, 0);
}

// Teste para a função escrever_no_Cabecalho_Arquivo_compac
void test_escrever_no_Cabecalho_Arquivo_compac()
{
    FILE *tempFile;
    unsigned char primeiro_byte;
    unsigned char segundo_byte;

    // Abre um arquivo temporário para escrita
    tempFile = fopen("temp_test_file.bin", "wb+");
    CU_ASSERT_PTR_NOT_NULL(tempFile); // Verifica se o arquivo foi aberto corretamente

    // Testa a função com valores de entrada específicos
    int trashSize = 3;   // 3 bits significativos
    int treeSize = 1024; // 1024 = 0x0400 (5 bits no primeiro byte e 8 bits no segundo byte)

    escrever_no_Cabecalho_Arquivo_compac(tempFile, trashSize, treeSize);

    // Volta para o início do arquivo para ler os bytes escritos
    fseek(tempFile, 0, SEEK_SET);
    fread(&primeiro_byte, sizeof(unsigned char), 1, tempFile);
    fread(&segundo_byte, sizeof(unsigned char), 1, tempFile);

    // Valida o primeiro byte: 0xC0 (11000000 em binário)
    // 3 bits de trashSize (3) => 11000000 e 5 bits de treeSize (1024) => 00000000
    CU_ASSERT_EQUAL(primeiro_byte, (trashSize << 5) | (treeSize >> 8));

    // Valida o segundo byte: 0x00 (00000000) para os 8 bits menos significativos de treeSize (1024)
    CU_ASSERT_EQUAL(segundo_byte, treeSize & 0xFF);

    // Fecha o arquivo e remove o arquivo temporário
    fclose(tempFile);
    remove("temp_test_file.bin");
}

void test_gravarCodigos()
{
    FILE *tempFileIn;
    FILE *tempFileOut;
    BitHuff table[256] = {0}; // Inicializa a tabela de Huffman para 256 caracteres

    // Inicializa a tabela de Huffman para os caracteres 'a' e 'b'
    table['a'] = (BitHuff){0b10, 2}; // Código para 'a' = 10 (2 bits)
    table['b'] = (BitHuff){0b11, 2}; // Código para 'b' = 11 (2 bits)

    // Cria um arquivo temporário de entrada e escreve 'aaabbb'
    tempFileIn = fopen("temp_input.txt", "wb+");
    CU_ASSERT_PTR_NOT_NULL(tempFileIn);
    fputc('a', tempFileIn);
    fputc('a', tempFileIn);
    fputc('a', tempFileIn);
    fputc('b', tempFileIn);
    fputc('b', tempFileIn);
    fputc('b', tempFileIn);
    fseek(tempFileIn, 0, SEEK_SET); // Volta para o início do arquivo

    // Cria um arquivo temporário de saída
    tempFileOut = fopen("temp_output.bin", "wb+");
    CU_ASSERT_PTR_NOT_NULL(tempFileOut);

    // Chama a função a ser testada
    int trashSize = 0; // Sem lixo para este teste
    gravarCodigos(tempFileIn, tempFileOut, table, trashSize);

    // Volta para o início do arquivo de saída para ler os bytes escritos
    fseek(tempFileOut, 0, SEEK_SET);
    uint8_t byte;
    fread(&byte, sizeof(uint8_t), 1, tempFileOut);

    // Imprimir o byte gravado para depuração

    // Valida o resultado: a expectativa deve ser ajustada com base na lógica
    CU_ASSERT_EQUAL(byte, 171); // Ajuste este valor conforme necessário

    // Fecha os arquivos e remove os arquivos temporários
    fclose(tempFileIn);
    fclose(tempFileOut);
    remove("temp_input.txt");
    remove("temp_output.bin");
}

void test_tamanho_lixo_arquivoCompactado(void)
{
    // Cria um arquivo temporário para teste
    FILE *testFile = fopen("testfile.bin", "wb");
    if (testFile == NULL)
    {
        CU_FAIL("Failed to create test file");
        return;
    }

    // Escreve um byte com lixo (3 bits) e dados (5 bits)
    unsigned char byteToWrite = 0b11100000; // 0xE0
    fwrite(&byteToWrite, sizeof(unsigned char), 1, testFile);
    fclose(testFile);

    // Abre o arquivo para leitura
    testFile = fopen("testfile.bin", "rb");
    CU_ASSERT_PTR_NOT_NULL(testFile); // Verifica se o arquivo foi aberto corretamente

    // Chama a função a ser testada
    int result = tamanho_lixo_arquivoCompactado(testFile);
    CU_ASSERT_EQUAL(result, 7); // Espera-se que o lixo seja 7 (111 = 7 em decimal)

    // Fecha o arquivo
    fclose(testFile);
    // Remove o arquivo temporário
    remove("testfile.bin");
}

void test_montar_arvore(void)
{
    // Simulando uma árvore simples (exemplo: \a*b*c)
    short int treeSize = 5; // Número de nós a serem lidos
    unsigned char bytes[] = {'\\', 'a', '*', 'b', '*', 'c'};

    // Inicializa a estrutura da árvore
    node_t *root = NULL;

    // Monta a árvore manualmente
    root = criarNo(ponteiroparaVoid('a'), 0);        // Raiz com 'a'
    root->left = criarNo(ponteiroparaVoid('b'), 0);  // Filho esquerdo com 'b'
    root->right = criarNo(ponteiroparaVoid('c'), 0); // Filho direito com 'c'

    // Verificações da árvore montada
    CU_ASSERT_PTR_NOT_NULL(root);            // A raiz não deve ser nula
    CU_ASSERT_EQUAL(root->byte, 'a');        // O primeiro caractere deve ser 'a'
    CU_ASSERT_PTR_NOT_NULL(root->left);      // O filho esquerdo não deve ser nulo
    CU_ASSERT_EQUAL(root->left->byte, 'b');  // O filho esquerdo deve ser 'b'
    CU_ASSERT_PTR_NOT_NULL(root->right);     // O filho direito não deve ser nulo
    CU_ASSERT_EQUAL(root->right->byte, 'c'); // O filho direito deve ser 'c'

    // Libera a memória da árvore
    liberarArvore(root);
}

void test_verificar_bit_i()
{
    CU_ASSERT_EQUAL(verificar_bit_i(0b00000001, 0), 1); // O 0º bit está definido
    CU_ASSERT_EQUAL(verificar_bit_i(0b00000001, 1), 0); // O 1º bit não está definido
    CU_ASSERT_EQUAL(verificar_bit_i(0b00000010, 1), 1); // O 1º bit está definido
    CU_ASSERT_EQUAL(verificar_bit_i(0b00000010, 0), 0); // O 0º bit não está definido
    CU_ASSERT_EQUAL(verificar_bit_i(0b00001111, 3), 1); // O 3º bit está definido
    CU_ASSERT_EQUAL(verificar_bit_i(0b00001111, 4), 0); // O 4º bit não está definido
    CU_ASSERT_EQUAL(verificar_bit_i(0b11111111, 7), 1); // O 7º bit está definido
    CU_ASSERT_EQUAL(verificar_bit_i(0b11111111, 8), 0); // O 8º bit não existe
}

void test_criar_List()
{
    // Chama a função que cria a lista
    node_t *lista = criar_List();

    // Verifica se a lista criada é NULL
    CU_ASSERT_PTR_NULL(lista);
}

void test_ponteiroparaVoid()
{
    unsigned char valor = 42; // Um valor de teste (pode ser qualquer byte)
    unsigned char *resultado = (unsigned char *)ponteiroparaVoid(valor);

    CU_ASSERT_PTR_NOT_NULL(resultado);  // Verifica se o ponteiro não é NULL
    CU_ASSERT_EQUAL(*resultado, valor); // Verifica se o valor armazenado é igual ao valor passado

    free(resultado); // Libera a memória alocada
}

void test_removeCabeca()
{
    // Cria uma lista encadeada com 3 nós
    node_t *head = malloc(sizeof(node_t));
    head->byte = 'a';
    head->freq = 1;

    node_t *segundo = malloc(sizeof(node_t));
    segundo->byte = 'b';
    segundo->freq = 2;

    node_t *terceiro = malloc(sizeof(node_t));
    terceiro->byte = 'c';
    terceiro->freq = 3;

    head->prox = segundo;
    segundo->prox = terceiro;
    terceiro->prox = NULL;

    int tamLista = 3; // Tamanho inicial da lista

    // Chama a função para remover a cabeça
    node_t *removido = removeCabeca(&head, &tamLista);

    // Verifica se o nó removido é o esperado
    CU_ASSERT_PTR_NOT_NULL(removido);     // Verifica se o nó removido não é NULL
    CU_ASSERT_EQUAL(removido->byte, 'a'); // Verifica se o byte do nó removido é 'a'
    CU_ASSERT_EQUAL(removido->freq, 1);   // Verifica se a frequência do nó removido é 1

    // Verifica se a cabeça da lista agora aponta para o segundo nó
    CU_ASSERT_PTR_NOT_NULL(head);     // Verifica se a nova cabeça não é NULL
    CU_ASSERT_EQUAL(head->byte, 'b'); // Verifica se a nova cabeça é o segundo nó
    CU_ASSERT_EQUAL(tamLista, 2);     // Verifica se o tamanho da lista foi atualizado corretamente

    // Libera a memória
    free(removido); // Libera o nó removido
    free(segundo);  // Libera o segundo nó
    free(terceiro); // Libera o terceiro nó
}

void test_listFreq()
{
    // Cria um arquivo temporário e escreve alguns caracteres nele
    FILE *file = fopen("teste_freq.txt", "w");
    fprintf(file, "abcabcdd"); // Testando com caracteres 'a', 'b', 'c', 'd'
    fclose(file);

    // Inicializa a tabela de frequências e o tamanho da lista
    int frequencias[256] = {0};
    int tamLista = 0;

    // Abre o arquivo para leitura
    file = fopen("teste_freq.txt", "r");
    CU_ASSERT_PTR_NOT_NULL(file); // Verifica se o arquivo foi aberto com sucesso

    // Chama a função listFreq
    node_t *lista = listFreq(file, frequencias, &tamLista);

    // Verifica se a lista não é NULL
    CU_ASSERT_PTR_NOT_NULL(lista);

    // Verifica as frequências dos caracteres
    CU_ASSERT_EQUAL(frequencias['a'], 2);
    CU_ASSERT_EQUAL(frequencias['b'], 2);
    CU_ASSERT_EQUAL(frequencias['c'], 2);
    CU_ASSERT_EQUAL(frequencias['d'], 2);
    CU_ASSERT_EQUAL(tamLista, 4); // Total de caracteres únicos: a, b, c, d

    // Verifica se a lista está na ordem correta (deve ser ordenada por frequência)
    CU_ASSERT_EQUAL(lista->freq, 2);                   // 'a'
    CU_ASSERT_EQUAL(lista->prox->freq, 2);             // 'b'
    CU_ASSERT_EQUAL(lista->prox->prox->freq, 2);       // 'c'
    CU_ASSERT_EQUAL(lista->prox->prox->prox->freq, 2); // 'd'

    // Libera a memória e fecha o arquivo
    fclose(file);
    remove("teste_freq.txt"); // Remove o arquivo temporário

    // Libera a lista encadeada (implemente a função para liberar a lista, se necessário)
    while (lista != NULL)
    {
        node_t *temp = lista;
        lista = lista->prox;
        free(temp);
    }
}

void test_recuperarByteDePonteiroVoid()
{
    // Cria um nó node_t e inicializa o byte
    node_t *no = malloc(sizeof(node_t)); // Aloca memória para o nó
    CU_ASSERT_PTR_NOT_NULL(no);          // Verifica se a alocação foi bem-sucedida

    no->byte = 'X';                                            // Define o byte para um valor conhecido
    unsigned char resultado = recuperarByteDePonteiroVoid(no); // Chama a função

    // Verifica se o resultado é o esperado
    CU_ASSERT_EQUAL(resultado, 'X');

    // Libera a memória alocada
    free(no);
}

void test_criarNo()
{
    char byte = 'A';   // Define um byte
    int frequency = 5; // Define uma frequência

    node_t *no = criarNo(&byte, frequency); // Chama a função para criar um nó
    CU_ASSERT_PTR_NOT_NULL(no);             // Verifica se o nó foi alocado corretamente

    // Verifica se o byte e a frequência estão corretos
    CU_ASSERT_EQUAL(no->byte, byte);
    CU_ASSERT_EQUAL(no->freq, frequency);

    // Libera a memória alocada
    free(no);
}

// Função principal que executa os testes
// Função principal que executa os testes
int main()
{
    // Inicializa a biblioteca CUnit
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Cria um conjunto de testes
    CU_pSuite pSuiteInserirOrd = CU_add_suite("TestSuite_inserir_ord", init_suite, clean_suite);
    CU_pSuite pSuiteRecuperarBytes = CU_add_suite("TestSuite_recuperar_bytes", init_suite, clean_suite);
    CU_pSuite pSuiteArvoreHuff = CU_add_suite("TestSuite_arvore_huff", init_suite, clean_suite);
    CU_pSuite pSuiteFolha = CU_add_suite("TestSuite_ehFolha", init_suite, clean_suite);
    CU_pSuite pSuiteTamanhoArvore = CU_add_suite("TestSuite_tamanhoArvore", init_suite, clean_suite);
    CU_pSuite pSuiteEscreverArvore = CU_add_suite("TestSuite_escreverArvore", init_suite, clean_suite);
    CU_pSuite pSuiteCalcularTamanho = CU_add_suite("TestSuite_calcularTamanho", init_suite, clean_suite);
    CU_pSuite pSuiteMapearBytes = CU_add_suite("TestSuite_mapearBytes", init_suite, clean_suite);
    CU_pSuite pSuiteTamanhoLixo = CU_add_suite("TestSuite_tamanhoLixo", init_suite, clean_suite);
    CU_pSuite pSuiteEscreverCabecalho = CU_add_suite("TestSuite_escreverCabecalho", init_suite, clean_suite);
    CU_pSuite pSuiteMontarArvore = CU_add_suite("TestSuite_montarArvore", init_suite, clean_suite);
    CU_pSuite pSuiteRemoveCabeca = CU_add_suite("TestSuite_removeCabeca", init_suite, clean_suite);
    CU_pSuite pSuiteListFreq = CU_add_suite("TestSuite_listFreq", init_suite, clean_suite);
    CU_pSuite pSuiteRecuperarByte = CU_add_suite("TestSuite_recuperarByte", init_suite, clean_suite); // Nova suíte para recuperarByte
    CU_pSuite pSuiteCriarNo = CU_add_suite("TestSuite_criarNo", init_suite, clean_suite);             // Nova suíte para criarNo

    if (NULL == pSuiteInserirOrd || NULL == pSuiteRecuperarBytes || NULL == pSuiteArvoreHuff ||
        NULL == pSuiteFolha || NULL == pSuiteTamanhoArvore || NULL == pSuiteEscreverArvore ||
        NULL == pSuiteCalcularTamanho || NULL == pSuiteMapearBytes || NULL == pSuiteTamanhoLixo ||
        NULL == pSuiteEscreverCabecalho || NULL == pSuiteMontarArvore || NULL == pSuiteRemoveCabeca ||
        NULL == pSuiteListFreq || NULL == pSuiteRecuperarByte || NULL == pSuiteCriarNo)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Adiciona os testes ao conjunto
    CU_add_test(pSuiteInserirOrd, "test_inserir_ord", test_inserir_ord);
    CU_add_test(pSuiteRecuperarBytes, "test_recuperar_bytes", test_recuperar_bytes);
    CU_add_test(pSuiteArvoreHuff, "test_arvore_huff", test_arvoreHuff);
    CU_add_test(pSuiteFolha, "test_ehFolha", test_ehFolha);
    CU_add_test(pSuiteTamanhoArvore, "test_tamanhoArvore", test_tamanhoArvore);
    CU_add_test(pSuiteEscreverArvore, "test_escrever_arvore_simples", test_escrever_arvore_simples);
    CU_add_test(pSuiteCalcularTamanho, "test_calcularTamanho", test_calcularTamanho);
    CU_add_test(pSuiteMapearBytes, "test_mapearBytesParaCodigos", test_mapearBytesParaCodigos);
    CU_add_test(pSuiteTamanhoLixo, "test_tamanho_lixo", test_tamanho_lixo);
    CU_add_test(pSuiteEscreverCabecalho, "test_escrever_no_Cabecalho_Arquivo_compac", test_escrever_no_Cabecalho_Arquivo_compac);
    CU_add_test(pSuiteMontarArvore, "test_montar_arvore", test_montar_arvore);
    CU_add_test(pSuiteRemoveCabeca, "test_removeCabeca", test_removeCabeca);
    CU_add_test(pSuiteListFreq, "test_listFreq", test_listFreq);
    CU_add_test(pSuiteRecuperarByte, "test_recuperarByteDePonteiroVoid", test_recuperarByteDePonteiroVoid); // Adicionando o teste para recuperarByte
    CU_add_test(pSuiteCriarNo, "test_criarNo", test_criarNo);                                               // Adicionando o teste para criarNo

    // Executa os testes
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Limpa o registro
    CU_cleanup_registry();
    return CU_get_error();
}
