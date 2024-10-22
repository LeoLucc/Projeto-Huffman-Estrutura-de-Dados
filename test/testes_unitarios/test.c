#include <stdio.h>
#include "list.h"
#include "tree.h"
#include "encode.h"
#include "decode.h"
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

// Função principal que executa o teste
int main()
{
    // Inicializa a biblioteca CUnit
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Cria um conjunto de testes
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("TestSuite_inserir_ord", 0, 0);

    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Adiciona o teste ao conjunto
    if (NULL == CU_add_test(pSuite, "test_inserir_ord", test_inserir_ord))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Executa os testes
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Limpa o registro
    CU_cleanup_registry();
    return CU_get_error();
}