#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Cor; // Função para definir as cores dos nós da árvore

// Estrutura do nó da árvore
typedef struct Produto {
    int cod_prod; // chave de organização
    char nome_prod[100];
    int quant_estoque;
    float preco_uni;
    Cor cor;
    struct Produto *esq, *dir, *pai; 
} Produto;

Produto* cadastrar_produto(int cod_prod, char nome_prod[], int quant_estoque, float preco_uni){
    Produto* novo_prod = (Produto*)malloc(sizeof(Produto));
    novo_prod->cod_prod = cod_prod;
    strcpy(novo_prod->nome_prod, nome_prod);
    novo_prod->quant_estoque = quant_estoque;
    novo_prod->preco_uni = preco_uni;
    novo_prod->cor = RED; // Novo nó é sempre vermelho
    novo_prod->esq = novo_prod->dir = novo_prod->pai = NULL; // fazemos isso para evitar que o nó aponte para alguma posição inválida
    return novo_prod;
}

// ROTAÇÕES 
Produto* rotacaoEsquerda(Produto* raiz, Produto* x) {
    Produto* y = x->dir; // y agora é o filho direito de x	
    x->dir = y->esq; // o filho esquerdo de y se torna o filho direito de x
    if (y->esq != NULL){
        y->esq->pai = x; // o pai de y se torna o filho esquerdo de x, seria a troca de posições na árvore 
    }
    y->pai = x->pai; // o pai de y se torna o pai de x
    if (x->pai == NULL){
        raiz = y; // indica que x era a raiz da árvore, então y se torna a nova raiz
    } else if (x == x->pai->esq){
        x->pai->esq = y; // se x é o filho esquerdo de seu pai, y se torna o filho esquerdo de seu pai
    } else {
        x->pai->dir = y; // se x é o filho direito de seu pai, y se torna o filho direito de seu pai
    }
    y->esq = x; // x se torna o filho esquerdo de y
    x->pai = y; // o pai de x se torna y
    return raiz;
}
// segue a mesma lógica da rotação à esquerda, mas agora o nó y é o filho esquerdo de x
Produto* rotacaoDireita(Produto* raiz, Produto* y) {
    Produto* x = y->esq;
    y->esq = x->dir;
    if(x->dir != NULL){
        x->dir->pai = y;
    }    
    x->pai = y->pai;
    if(y->pai == NULL){
        raiz = x;
    } else if(y == y->pai->dir){
        y->pai->dir = x;
    } else {
        y->pai->esq = x;
    }
    x->dir = y;
    y->pai = x;
    return raiz;
}
// INSERÇÃO
Produto* inserirProduto(Produto* raiz, Produto* novo_prod) {
    if (raiz == NULL) {
        return novo_prod; // caso esteja vazio, o novo nó se torna a raiz 
    }

    if (novo_prod->cod_prod < raiz->cod_prod) {
        raiz->esq = inserirProduto(raiz->esq, novo_prod);
        raiz->esq->pai = raiz; // o pai do nó que foi inserido se torna a raiz
    } else if (novo_prod->cod_prod > raiz->cod_prod) {
        raiz->dir = inserirProduto(raiz->dir, novo_prod);
        raiz->dir->pai = raiz;
    }
    return raiz;
}

// CORREÇÃO DA INSERÇÃO
Produto* corrigirInsercao(Produto* raiz, Produto* no) {
    Produto* tio;
    while (no != raiz && no->pai->cor == RED) {
        if (no->pai == no->pai->pai->esq) {
            tio = no->pai->pai->dir; // tio é o filho direito do avô de no
            if (tio != NULL && tio->cor == RED) { // caso 1: tio vermelho
                no->pai->cor = BLACK; // o pai de no se torna preto
                tio->cor = BLACK; // o tio se torna preto
                no->pai->pai->cor = RED; // o avô de no se torna vermelho
                no = no->pai->pai; // sobe para o avô de no
            } else {
                if (no == no->pai->dir) { // caso 2: no é filho direito
                    no = no->pai;
                    raiz = rotacaoEsquerda(raiz, no); // rotação à esquerda
                }
                // caso 3: no é filho esquerdo
                no->pai->cor = BLACK; // o pai de no se torna preto
                no->pai->pai->cor = RED; // o avô de no se torna vermelho
                raiz = rotacaoDireita(raiz, no->pai->pai); // rotação à direita
            }
        } else {
            tio = no->pai->pai->esq; // tio é o filho esquerdo do avô de no

            if (tio != NULL && tio->cor == RED) { // caso 1: tio vermelho
                no->pai->cor = BLACK;
                tio->cor = BLACK;
                no->pai->pai->cor = RED;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esq) { // caso 2: no é filho esquerdo
                    no = no->pai;
                    raiz = rotacaoDireita(raiz, no); // rotação à direita
                }
                // caso 3: no é filho direito
                no->pai->cor = BLACK;
                no->pai->pai->cor = RED;
                raiz = rotacaoEsquerda(raiz, no->pai->pai); // rotação à esquerda

            }
        }
    }
    raiz->cor = BLACK; // a raiz sempre deve ser preta
    return raiz;
}

// Função para inserir
Produto* inserir(Produto* raiz, int cod_prod, char nome_prod[], int quant_estoque, float preco_uni) {
    Produto* novo_prod = cadastrar_produto(cod_prod, nome_prod, quant_estoque, preco_uni);
    raiz = inserirProduto(raiz, novo_prod); // insere o novo nó na árvore
    raiz = corrigirInsercao(raiz, novo_prod); // corrige a árvore após a inserção
    return raiz;
}

// Função para imprimir a árvore em ordem
void emOrdem(Produto* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq); 
        printf("Código: %d, Nome: %s, Quantidade: %d, Preço: %.2f\n Cor do nó: %s", raiz->cod_prod, raiz->nome_prod, raiz->quant_estoque, raiz->preco_uni, raiz->cor == RED ? "Vermelho" : "Preto"); // imprime o nó atual
        emOrdem(raiz->dir);
    }
}

Produto* FOLHA_NULA; // nó folha nulo, que representa o fim da árvore

// Inicializa a folha nula
Produto* criarFolhaNula() {
    Produto* folha = (Produto*)malloc(sizeof(Produto));
    folha->cor = BLACK; // a folha nula é sempre preta
    folha->esq = folha->dir = folha->pai = NULL; // inicializa os ponteiros como nulos
    return folha;
}
// BUSCA E REMOVER
Produto* buscarProduto(Produto* raiz, int cod_prod) {
    if(raiz == NULL || raiz->cod_prod == cod_prod) {
        return raiz; 
    }else if(cod_prod < raiz->cod_prod){
        return buscarProduto(raiz->esq, cod_prod); // busca na subárvore esquerda
    }else{
        return buscarProduto(raiz->dir, cod_prod); // busca na subárvore direita
    }
}

// Função para encaixar um nó no lugar removido
void transplantar(Produto** raiz, Produto* u, Produto* v) {
    if (u->pai == NULL) {
        *raiz = v; // se u é a raiz, v se torna a nova raiz
    } else if (u == u->pai->esq) {
        u->pai->esq = v; // se u é o filho esquerdo de seu pai, v se torna o filho esquerdo de seu pai
    } else {
        u->pai->dir = v; // se u é o filho direito de seu pai, v se torna o filho direito de seu pai
    }
    if (v != NULL) {
        v->pai = u->pai; // o pai de v se torna o pai de u
    }
}

// Função para encontrar o nó mínimo na árvore -> usado em nós que tem 2 filhos
Produto* minimo(Produto* no) {
    while (no->esq != NULL) {
        no = no->esq; // percorre para a esquerda até encontrar o nó mínimo
    }
    return no;
}

// Função para corrigir a árvore após a remoção
Produto* corrigirRemocao(Produto* raiz, Produto* x) {
    while (x != raiz && (x == NULL || x->cor == BLACK)) {
        if (x == x->pai->esq) {
            Produto* w = x->pai->dir; // irmão de x
            if (w->cor == RED) { // caso 1: irmão vermelho
                w->cor = BLACK; // o irmão se torna preto
                x->pai->cor = RED; // o pai se torna vermelho
                raiz = rotacaoEsquerda(raiz, x->pai); // rotação à esquerda
                w = x->pai->dir; // atualiza w para o novo irmão
            }

            if ((w->esq == NULL || w->esq->cor == BLACK) && (w->dir == NULL || w->dir->cor == BLACK)) { // caso 2: ambos os filhos do irmão são pretos
                w->cor = RED; // o irmão se torna vermelho
                x = x->pai; // sobe para o pai de x
            } else {
                if (w->dir == NULL || w->dir->cor == BLACK) { // caso 3: filho direito do irmão é preto
                    if (w->esq != NULL && w->esq->cor == RED) { // se o filho esquerdo do irmão é vermelho
                        w->esq->cor = BLACK; // o filho esquerdo do irmão se torna preto
                    }
                    w->cor = RED; // o irmão se torna vermelho
                    raiz = rotacaoDireita(raiz, w); // rotação à direita
                    w = x->pai->dir; // atualiza w para o novo irmão
                }
                // caso 4: filho direito do irmão é vermelho
                w->cor = x->pai->cor; // o irmão se torna a cor do pai de x
                x->pai->cor = BLACK; // o pai de x se torna preto
                if (w->dir != NULL) {
                    w->dir->cor = BLACK; // o filho direito do irmão se torna preto
                }
                raiz = rotacaoEsquerda(raiz, x->pai); // rotação à esquerda
                x = raiz; // atualiza x para a nova raiz
            }
        } else {
            // aqui vai se seguir a mesma lógica da parte da esquerda, mas agora o nó x é o filho direito
            Produto* w = x->pai->esq;
            if (w->cor == RED) { 
                w->cor = BLACK; 
                x->pai->cor = RED; 
                raiz = rotacaoDireita(raiz, x->pai); 
                w = x->pai->esq; 
            }
            if ((w->dir == NULL || w->dir->cor == BLACK) && (w->esq == NULL || w->esq->cor == BLACK)) { 
                w->cor = RED; 
                x = x->pai; 
            } else {
                if (w->esq == NULL || w->esq->cor == BLACK) { 
                    if (w->dir != NULL) {
                        w->dir->cor = BLACK; 
                    }
                    w->cor = RED; 
                    raiz = rotacaoEsquerda(raiz, w); 
                    w = x->pai->esq; 
                }
                w->cor = x->pai->cor; 
                x->pai->cor = BLACK; 
                if (w->esq != NULL) {
                    w->esq->cor = BLACK; 
                }
                raiz = rotacaoDireita(raiz, x->pai); 
                x = raiz;
            }
        }
    }
    if (x != NULL) {
        x->cor = BLACK; // a raiz sempre deve ser preta
    }

    return raiz;
}

// Função para remover um nó da árvore
Produto* remover(Produto* raiz, int cod_prod) {
    Produto* z = buscarProduto(raiz, cod_prod); 
    if (z == NULL) return raiz;

    Produto *y = z; // nó que vai ser removido
    Produto *x; // folha nula -> nó que vai substituir o nó removido
    Cor cor_original = y->cor; // cor original do nó removido

    if (z->esq == NULL) {
        x = z->dir;
        transplantar(&raiz, z, z->dir); // encaixa o nó na árvore
    } else if (z->dir == NULL) {
        x = z->esq;
        transplantar(&raiz, z, z->esq);
    } else {
        y = minimo(z->dir); // encontra o nó mínimo na subárvore direita
        cor_original = y->cor; // guarda a cor original do nó mínimo
        x = y->dir;

        if (y->pai == z) {
            if (x != NULL) {
                x->pai = y; // se o nó mínimo tem filho, atualiza o pai
            }
        } else {
            transplantar(&raiz, y, y->dir); // encaixa o nó mínimo na árvore
            y->dir = z->dir; // atualiza o filho direito do nó mínimo
            y->dir->pai = y; // atualiza o pai do filho direito do nó mínimo
        }

        transplantar(&raiz, z, y); // encaixa o nó a ser removido na árvore
        y->esq = z->esq; // atualiza o filho esquerdo do nó mínimo
        y->esq->pai = y; // atualiza o pai do filho esquerdo do nó mínimo
        y->cor = z->cor; // atualiza a cor do nó mínimo
    }

    free(z);

    if (cor_original == BLACK) { // se a cor original era preta, corrige a árvore
        raiz = corrigirRemocao(raiz, x);
    }

    return raiz;
}

void main(){
    Produto* raiz = NULL;
    int opcao, codigo;
    char nome[100];
    int quantidade;
    float preco;

    do {
        printf("\n=== MENU DE GERENCIAMENTO DE PRODUTOS ===\n");
        printf("1. Inserir novo produto\n");
        printf("2. Remover produto\n");
        printf("3. Buscar produto\n");
        printf("4. Exibir todos os produtos\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("\nDigite o codigo do produto: ");
                scanf("%d", &codigo);
                printf("Digite o nome do produto: ");
                getchar(); // Limpa o buffer
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0; // Remove o \n do final
                printf("Digite a quantidade em estoque: ");
                scanf("%d", &quantidade);
                printf("Digite o preco unitario: ");
                scanf("%f", &preco);
                
                raiz = inserir(raiz, codigo, nome, quantidade, preco);
                printf("\nProduto inserido com sucesso!\n");
                break;

            case 2:
                printf("\nDigite o codigo do produto a ser removido: ");
                scanf("%d", &codigo);
                raiz = remover(raiz, codigo);
                printf("\nProduto removido com sucesso!\n");
                break;

            case 3:
                printf("\nDigite o codigo do produto a ser buscado: ");
                scanf("%d", &codigo);
                Produto* encontrado = buscarProduto(raiz, codigo);
                if (encontrado != NULL) {
                    printf("\nProduto encontrado:\n");
                    printf("Codigo: %d\n", encontrado->cod_prod);
                    printf("Nome: %s\n", encontrado->nome_prod);
                    printf("Quantidade: %d\n", encontrado->quant_estoque);
                    printf("Preco: R$ %.2f\n", encontrado->preco_uni);
                } else {
                    printf("\nProduto nao encontrado!\n");
                }
                break;

            case 4:
                printf("\nLista de produtos:\n");
                emOrdem(raiz);
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }
    } while(opcao != 0);
}