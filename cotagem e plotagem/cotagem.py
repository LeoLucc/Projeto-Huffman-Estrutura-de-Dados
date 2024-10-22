import random
import matplotlib.pyplot as plt

# Definição da Lista Encadeada
class Node:
    def __init__(self, data):
        self.data = data
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None

    def insert(self, data):
        new_node = Node(data)
        new_node.next = self.head
        self.head = new_node

    def search(self, key):
        current = self.head
        count = 0
        while current:
            count += 1
            if current.data == key:
                return count  # Retorna o número de operações de busca
            current = current.next
        return count  # Retorna o número total de operações (não encontrado)

# Definição da Árvore Binária
class TreeNode:
    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None

class BinaryTree:
    def __init__(self):
        self.root = None

    def insert(self, data):
        if not self.root:
            self.root = TreeNode(data)
        else:
            self._insert_recursive(self.root, data)

    def _insert_recursive(self, node, data):
        if data < node.data:
            if node.left is None:
                node.left = TreeNode(data)
            else:
                self._insert_recursive(node.left, data)
        else:
            if node.right is None:
                node.right = TreeNode(data)
            else:
                self._insert_recursive(node.right, data)

    def search(self, key):
        return self._search_recursive(self.root, key, 0)

    def _search_recursive(self, node, key, count):
        if node is None:
            return count  # Retorna o número total de operações (não encontrado)
        count += 1
        if node.data == key:
            return count  # Retorna o número de operações de busca
        elif key < node.data:
            return self._search_recursive(node.left, key, count)
        else:
            return self._search_recursive(node.right, key, count)

# Geração de números aleatórios e inserção nas estruturas
max_num = 20000  # Alterado para 20.000
sizes = list(range(1000, max_num + 1, 1000))  # Tamanhos de entrada de 1.000 a 20.000
comparisons_linked_list = []
comparisons_binary_tree = []

for size in sizes:
    random_numbers = [random.randint(0, 9999) for _ in range(size)]

    # Inserindo números na lista encadeada
    linked_list = LinkedList()
    for num in random_numbers:
        linked_list.insert(num)

    # Inserindo números na árvore binária
    binary_tree = BinaryTree()
    for num in random_numbers:
        binary_tree.insert(num)

    # Testando a busca e contando operações
    search_counts_linked_list = []
    search_counts_binary_tree = []

    for _ in range(100):  # Testar busca em 100 números aleatórios
        chave = random.randint(0, 9999)
        search_counts_linked_list.append(linked_list.search(chave))
        search_counts_binary_tree.append(binary_tree.search(chave))

    comparisons_linked_list.append(sum(search_counts_linked_list) / len(search_counts_linked_list))
    comparisons_binary_tree.append(sum(search_counts_binary_tree) / len(search_counts_binary_tree))

# Plotando os resultados
plt.figure(figsize=(10, 5))
plt.plot(sizes, comparisons_linked_list, label='Lista Encadeada', marker='o')
plt.plot(sizes, comparisons_binary_tree, label='Árvore Binária', marker='x')
plt.title('Comparação de Operações de Busca')
plt.xlabel('Tamanho da Entrada (números)')
plt.ylabel('Número Médio de Comparações')
plt.legend()
plt.grid()
plt.show()
