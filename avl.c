#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura do nó da árvore AVL para armazenar usuários
typedef struct Usuario {
    char nome[100];
    int id;
    char email[100];
    int altura;
    struct Usuario *esq, *dir;
} Usuario;

// Converte uma string para minúsculo (para evitar duplicidade por caixa)
void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

// Retorna a altura de um nó
int altura(Usuario *n) {
    if (n == NULL) return 0;
    return n->altura;
}

// Retorna o maior entre dois valores
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Cria um novo usuário (nó)
Usuario* novoUsuario(char *nome, int id, char *email) {
    Usuario* u = (Usuario*)malloc(sizeof(Usuario));
    strcpy(u->nome, nome);
    u->id = id;
    strcpy(u->email, email);
    u->altura = 1;
    u->esq = u->dir = NULL;
    return u;
}

// Calcula o fator de balanceamento de um nó
int fatorBalanceamento(Usuario *n) {
    if (n == NULL) return 0;
    return altura(n->esq) - altura(n->dir);
}

// Rotação simples à direita
Usuario* rotacaoDireita(Usuario *y) {
    Usuario *x = y->esq;
    Usuario *T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    return x;
}

// Rotação simples à esquerda
Usuario* rotacaoEsquerda(Usuario *x) {
    Usuario *y = x->dir;
    Usuario *T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    return y;
}

// Insere um novo usuário na árvore AVL
Usuario* inserir(Usuario* raiz, char *nome, int id, char *email) {
    if (raiz == NULL) return novoUsuario(nome, id, email);

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = inserir(raiz->esq, nome, id, email);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = inserir(raiz->dir, nome, id, email);
    else
        return raiz; // nomes iguais não são permitidos

    // Atualiza altura e balanceia
    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int fb = fatorBalanceamento(raiz);

    // Rotação simples à direita
    if (fb > 1 && strcmp(nome, raiz->esq->nome) < 0)
        return rotacaoDireita(raiz);

    // Rotação simples à esquerda
    if (fb < -1 && strcmp(nome, raiz->dir->nome) > 0)
        return rotacaoEsquerda(raiz);

    // Rotação dupla esquerda-direita
    if (fb > 1 && strcmp(nome, raiz->esq->nome) > 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    // Rotação dupla direita-esquerda
    if (fb < -1 && strcmp(nome, raiz->dir->nome) < 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Retorna o nó com menor valor (usado na remoção)
Usuario* menorValor(Usuario* raiz) {
    Usuario* atual = raiz;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

// Remove um usuário da árvore AVL
Usuario* remover(Usuario* raiz, char *nome) {
    if (raiz == NULL) return raiz;

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = remover(raiz->esq, nome);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = remover(raiz->dir, nome);
    else {
        // Nó com um ou nenhum filho
        if ((raiz->esq == NULL) || (raiz->dir == NULL)) {
            Usuario *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;
            free(temp);
        } else {
            // Nó com dois filhos: pega o menor da subárvore direita
            Usuario* temp = menorValor(raiz->dir);
            strcpy(raiz->nome, temp->nome);
            raiz->id = temp->id;
            strcpy(raiz->email, temp->email);
            raiz->dir = remover(raiz->dir, temp->nome);
        }
    }

    if (raiz == NULL) return raiz;

    // Atualiza altura e balanceia
    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int fb = fatorBalanceamento(raiz);

    // Rotação simples à direita
    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);

    // Rotação dupla esquerda-direita
    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    // Rotação simples à esquerda
    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);

    // Rotação dupla direita-esquerda
    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Busca um usuário pelo nome
Usuario* buscar(Usuario* raiz, char *nome) {
    if (raiz == NULL) return NULL;
    int cmp = strcmp(nome, raiz->nome);
    if (cmp == 0) return raiz;
    if (cmp < 0) return buscar(raiz->esq, nome);
    return buscar(raiz->dir, nome);
}

// Lista todos os usuários em ordem alfabética
void listar(Usuario* raiz) {
    if (raiz != NULL) {
        listar(raiz->esq);
        printf("Nome: %s | ID: %d | Email: %s\n", raiz->nome, raiz->id, raiz->email);
        listar(raiz->dir);
    }
}

// Libera toda a memória da árvore
void liberar(Usuario* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esq);
        liberar(raiz->dir);
        free(raiz);
    }
}

// Função principal: menu de operações
int main() {
    Usuario* raiz = NULL;
    int opcao, id;
    char nome[100], email[100];

    do {
        printf("\n1-Cadastrar\n2-Remover\n3-Buscar\n4-Listar\n0-Sair\nOpcao: ");
        scanf("%d", &opcao);
        switch(opcao) {
            case 1:
                printf("Nome: "); scanf("%s", nome);
                toLowerStr(nome); // Converte para minúsculo
                printf("ID: "); scanf("%d", &id);
                printf("Email: "); scanf("%s", email);
                raiz = inserir(raiz, nome, id, email);
                break;
            case 2:
                printf("Nome para remover: "); scanf("%s", nome);
                toLowerStr(nome); // Converte para minúsculo
                raiz = remover(raiz, nome);
                break;
            case 3:
                printf("Nome para buscar: "); scanf("%s", nome);
                toLowerStr(nome); // Converte para minúsculo
                Usuario* u = buscar(raiz, nome);
                if (u)
                    printf("Encontrado: %s | ID: %d | Email: %s\n", u->nome, u->id, u->email);
                else
                    printf("Nao encontrado.\n");
                break;
            case 4:
                listar(raiz);
                break;
        }
    } while(opcao != 0);

    liberar(raiz); // Libera memória ao sair
    return 0;
}