#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da árvore AVL
typedef struct Node {
    int key; // Valor armazenado no nó
    struct Node *left; // Ponteiro para o filho à esquerda
    struct Node *right; // Ponteiro para o filho à direita
    int height; // Altura do nó
} Node;

// Cria um novo nó com uma chave específica
Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node)); // Aloca memória para o nó
    node->key = key; // Define a chave do nó
    node->left = NULL; // Inicialmente, o filho à esquerda é NULL
    node->right = NULL; // Inicialmente, o filho à direita é NULL
    node->height = 1; // Um novo nó é inicialmente uma folha, com altura 1
    return node;
}

// Retorna a altura de um nó; se o nó for NULL, retorna 0
int height(Node *N) {
    return (N == NULL) ? 0 : N->height;
}

// Calcula o fator de balanceamento de um nó
// O fator de balanceamento é a diferença entre a altura da subárvore esquerda e a altura da subárvore direita
int getBalance(Node *N) {
    return (N == NULL) ? 0 : height(N->left) - height(N->right);
}

// Rotaciona a subárvore à direita em torno do nó 'y'
// Usado para balancear a árvore quando há desbalanceamento no filho à esquerda
Node* rightRotate(Node *y) {
    Node *x = y->left; // 'x' é o filho à esquerda de 'y'
    Node *T2 = x->right; // 'T2' é o filho à direita de 'x'

    x->right = y; // 'y' se torna o filho à direita de 'x'
    y->left = T2; // 'T2' se torna o filho à esquerda de 'y'

    // Atualiza a altura dos nós 'y' e 'x'
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    return x; // Retorna o novo nó raiz da subárvore
}

// Rotaciona a subárvore à esquerda em torno do nó 'x'
// Usado para balancear a árvore quando há desbalanceamento no filho à direita
Node* leftRotate(Node *x) {
    Node *y = x->right; // 'y' é o filho à direita de 'x'
    Node *T2 = y->left; // 'T2' é o filho à esquerda de 'y'

    y->left = x; // 'x' se torna o filho à esquerda de 'y'
    x->right = T2; // 'T2' se torna o filho à direita de 'x'

    // Atualiza a altura dos nós 'x' e 'y'
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    return y; // Retorna o novo nó raiz da subárvore
}

// Insere um novo nó na árvore AVL
Node* insert(Node* node, int key) {
    if (node == NULL)
        return newNode(key); // Se a árvore estiver vazia, cria um novo nó

    // Insere a chave na subárvore esquerda ou direita conforme a comparação
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // Chaves iguais não são permitidas, então retorna o nó existente

    // Atualiza a altura do nó corrente
    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));

    // Obtém o fator de balanceamento do nó
    int balance = getBalance(node);

    // Caso 1 - Rotação simples à direita
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Caso 2 - Rotação simples à esquerda
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Caso 3 - Rotação dupla à esquerda e direita
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Caso 4 - Rotação dupla à direita e esquerda
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // Retorna o nó (ou o novo nó raiz após rotação)
}

// Encontra o nó com o valor mínimo na subárvore
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left; // Navega até o nó mais à esquerda
    return current;
}

// Remove um nó da árvore AVL
Node* deleteNode(Node* root, int key) {
    if (root == NULL)
        return root; // Se a árvore estiver vazia, retorna NULL

    // Navega até o nó a ser removido
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // Caso 1: Nó com apenas um filho ou sem filhos
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right; // Escolhe o filho único ou NULL

            if (temp == NULL) {
                temp = root; // Caso em que o nó a ser removido é uma folha
                root = NULL;
            } else
                *root = *temp; // Copia o conteúdo do nó filho para o nó a ser removido

            free(temp); // Libera a memória do nó removido
        } else {
            // Caso 2: Nó com dois filhos
            Node* temp = minValueNode(root->right); // Encontra o nó com o menor valor na subárvore direita
            root->key = temp->key; // Substitui o valor do nó a ser removido pelo valor do nó mínimo encontrado
            root->right = deleteNode(root->right, temp->key); // Remove o nó mínimo da subárvore direita
        }
    }

    if (root == NULL)
        return root; // Se a árvore ficou vazia, retorna NULL

    // Atualiza a altura do nó corrente
    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));

    // Obtém o fator de balanceamento do nó
    int balance = getBalance(root);

    // Caso 1 - Rotação simples à direita
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Caso 2 - Rotação dupla à esquerda e direita
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Caso 3 - Rotação simples à esquerda
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Caso 4 - Rotação dupla à direita e esquerda
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root; // Retorna o nó (ou o novo nó raiz após rotação)
}

// Imprime a árvore em ordem (esquerda, raiz, direita)
void inOrder(Node *root) {
    if (root != NULL) {
        inOrder(root->left); // Navega para a subárvore esquerda
        printf("%d ", root->key); // Imprime o valor do nó
        inOrder(root->right); // Navega para a subárvore direita
    }
}

// Função auxiliar para imprimir a árvore de forma visual
void printTree(Node *root, int space) {
    const int COUNT = 10; // Define a largura do espaço para cada nível
    if (root == NULL)
        return;

    space += COUNT; // Incrementa o espaço para o nível atual

    printTree(root->right, space); // Imprime o filho direito

    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" "); // Imprime espaços para alinhar a árvore
    printf("%d\n", root->key); // Imprime o valor do nó

    printTree(root->left, space); // Imprime o filho à esquerda
}

// Libera a memória da árvore
void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left); // Libera a memória da subárvore esquerda
        freeTree(root->right); // Libera a memória da subárvore direita
        free(root); // Libera a memória do nó atual
    }
}

// Função principal com menu interativo
int main() {
    Node* root = NULL; // Inicializa a árvore como vazia
    int choice, key;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Inserir um novo número\n");
        printf("2. Remover um número\n");
        printf("3. Imprimir árvore (em ordem)\n");
        printf("4. Imprimir árvore (visual)\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Digite o número para inserir: ");
                scanf("%d", &key);
                root = insert(root, key);
                printf("Número %d inserido.\n", key);
                break;

            case 2:
                printf("Digite o número para remover: ");
                scanf("%d", &key);
                root = deleteNode(root, key);
                printf("Número %d removido.\n", key);
                break;

            case 3:
                printf("Traversal em ordem da árvore AVL: ");
                inOrder(root);
                printf("\n");
                break;

            case 4:
                printf("Estrutura da árvore:\n");
                printTree(root, 0);
                break;

            case 5:
                // Libera a memória da árvore e sai
                freeTree(root);
                printf("Saindo...\n");
                return 0;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}