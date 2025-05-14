#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Usuario {
    char nome[100];
    int id;
    char email[100];
    int altura;
    struct Usuario *esq, *dir;
} Usuario;

// Função para converter string para minúsculo
void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

int altura(Usuario *n) {
    if (n == NULL) return 0;
    return n->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Usuario* novoUsuario(char *nome, int id, char *email) {
    Usuario* u = (Usuario*)malloc(sizeof(Usuario));
    strcpy(u->nome, nome);
    u->id = id;
    strcpy(u->email, email);
    u->altura = 1;
    u->esq = u->dir = NULL;
    return u;
}

int fatorBalanceamento(Usuario *n) {
    if (n == NULL) return 0;
    return altura(n->esq) - altura(n->dir);
}

Usuario* rotacaoDireita(Usuario *y) {
    Usuario *x = y->esq;
    Usuario *T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    return x;
}

Usuario* rotacaoEsquerda(Usuario *x) {
    Usuario *y = x->dir;
    Usuario *T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    return y;
}

Usuario* inserir(Usuario* raiz, char *nome, int id, char *email) {
    if (raiz == NULL) return novoUsuario(nome, id, email);

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = inserir(raiz->esq, nome, id, email);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = inserir(raiz->dir, nome, id, email);
    else
        return raiz; // nomes iguais não são permitidos

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && strcmp(nome, raiz->esq->nome) < 0)
        return rotacaoDireita(raiz);

    if (fb < -1 && strcmp(nome, raiz->dir->nome) > 0)
        return rotacaoEsquerda(raiz);

    if (fb > 1 && strcmp(nome, raiz->esq->nome) > 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && strcmp(nome, raiz->dir->nome) < 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

Usuario* menorValor(Usuario* raiz) {
    Usuario* atual = raiz;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

Usuario* remover(Usuario* raiz, char *nome) {
    if (raiz == NULL) return raiz;

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = remover(raiz->esq, nome);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = remover(raiz->dir, nome);
    else {
        if ((raiz->esq == NULL) || (raiz->dir == NULL)) {
            Usuario *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;
            free(temp);
        } else {
            Usuario* temp = menorValor(raiz->dir);
            strcpy(raiz->nome, temp->nome);
            raiz->id = temp->id;
            strcpy(raiz->email, temp->email);
            raiz->dir = remover(raiz->dir, temp->nome);
        }
    }

    if (raiz == NULL) return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);

    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

Usuario* buscar(Usuario* raiz, char *nome) {
    if (raiz == NULL) return NULL;
    int cmp = strcmp(nome, raiz->nome);
    if (cmp == 0) return raiz;
    if (cmp < 0) return buscar(raiz->esq, nome);
    return buscar(raiz->dir, nome);
}

void listar(Usuario* raiz) {
    if (raiz != NULL) {
        listar(raiz->esq);
        printf("Nome: %s | ID: %d | Email: %s\n", raiz->nome, raiz->id, raiz->email);
        listar(raiz->dir);
    }
}

void liberar(Usuario* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esq);
        liberar(raiz->dir);
        free(raiz);
    }
}

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
                toLowerStr(nome);
                printf("ID: "); scanf("%d", &id);
                printf("Email: "); scanf("%s", email);
                raiz = inserir(raiz, nome, id, email);
                break;
            case 2:
                printf("Nome para remover: "); scanf("%s", nome);
                toLowerStr(nome);
                raiz = remover(raiz, nome);
                break;
            case 3:
                printf("Nome para buscar: "); scanf("%s", nome);
                toLowerStr(nome);
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

    liberar(raiz);
    return 0;
}