#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26 // Para o alfabeto inglês
// todo node tem o tamnho do alfabeto

// Estrutura para um nó da Trie
typedef struct TrieNode
{
  struct TrieNode *filhos[ALPHABET_SIZE]; // Filhos da Trie
  bool fimDePalavra;                      // Indica se o nó é o fim de uma palavra
} TrieNode;

// Função para criar um novo nó da Trie
TrieNode *criarNo()
{
  TrieNode *novoNo = (TrieNode *)malloc(sizeof(TrieNode));
  novoNo->fimDePalavra = false; // Inicializa como não é o fim de uma palavra
  for (int i = 0; i < ALPHABET_SIZE; i++)
  {
    novoNo->filhos[i] = NULL; // Inicializa todos os filhos como NULL
  }
  return novoNo;
}

// funçao para inserir uma palavra na Trie
void inserirPalavra(TrieNode *raiz, const char *palavra)
{
  TrieNode *atual = raiz; // Começa a partir do nó raiz da Trie

  // Percorre cada caractere da palavra até encontrar o caractere nulo ('\0')
  for (int i = 0; palavra[i] != '\0'; i++)
  {
    int indice = palavra[i] - 'a'; // Calcula o índice da letra,
    // convertendo o caractere em um valor de 0 a 25 (para letras minúsculas)

    // Se o ponteiro para o filho correspondente à letra atual for NULL,
    // significa que esse caminho ainda não existe na Trie
    if (atual->filhos[indice] == NULL)
    {
      atual->filhos[indice] = criarNo(); // Cria um novo nó para essa letra
    }
    atual = atual->filhos[indice]; // Move para o próximo nó, ou seja,
                                   // desce para o filho correspondente à letra atual
  }
  atual->fimDePalavra = true; // Após inserir todos os caracteres,
                              // marca o último nó como o fim de uma palavra válida
}

// Função para verificar se uma palavra está na Trie
bool verificarPalavra(TrieNode *raiz, const char *palavra)
{
  TrieNode *atual = raiz;
  for (int i = 0; palavra[i] != '\0'; i++)
  { // navegar a char dada

    int indice = palavra[i] - 'a'; // Calcula o índice // indice sao caracters do alfabeto
    if (atual->filhos[indice] == NULL)
    {

      return false; // Palavra não encontrada
    }
    atual = atual->filhos[indice]; // Move para o próximo nó
  }
  return atual != NULL && atual->fimDePalavra; // Retorna true se for o fim da palavra
                                               // não sendo null e chegando no bool fim da palavra da struct
}

// Função para liberar a memória da Trie
void liberarTrie(TrieNode *raiz)
{
  for (int i = 0; i < ALPHABET_SIZE; i++)
  {
    if (raiz->filhos[i] != NULL)
    {
      liberarTrie(raiz->filhos[i]); // Libera recursivamente os filhos
    }
  }
  free(raiz); // Libera o nó atual
}

int main()
{
  TrieNode *raiz = criarNo(); // Cria a raiz da Trie
  // raiz eh vazia
  // ao inserir palavra comeca a partir da raiz

  // Inserindo palavras na Trie
  inserirPalavra(raiz, "chicago");
  inserirPalavra(raiz, "china");
  inserirPalavra(raiz, "sabia");
  inserirPalavra(raiz, "samurai");
  inserirPalavra(raiz, "caderno");
  inserirPalavra(raiz, "cadeado");
  inserirPalavra(raiz, "estrutura");

  // Verificando palavras
  char palavra[100];
  printf("Digite uma palavra para verificar: ");
  scanf("%s", palavra);

  if (verificarPalavra(raiz, palavra))
  {
    printf("A palavra '%s' esta na Trie.\n", palavra);
  }
  else
  {
    printf("A palavra '%s' NAO esta na Trie.\n", palavra);
  }

  // Liberar a memória da Trie
  liberarTrie(raiz);

  return 0;
}
