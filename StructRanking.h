#ifndef _STRUCTRANKING_H_
#define _STRUCTRANKING_H_

#include <stdio.h>
#include <stdlib.h>

//Definição do tipo player, onde serão armazenados o nome e pontuação do jogador
struct player{
  char name[15];
  int points;
};


//Definição do tipo Node
struct Node{
  struct player info; //local onde será armazenado a informção do jogador
  struct Node *next; //Ponteiro que apontará para o próximo elemento do Ranking
};

//Definindo um ponteiro do tipo Node como NodePtr
typedef struct Node* NodePtr;

//Definição do tipo Ranking
struct Rank{
  NodePtr first; //Ponteiro que apontará para 1º o elemento do tipo Node no Ranking
  NodePtr current; //Ponteiro que apontará para último elemento acessado do Ranking
};
typedef struct Rank Ranking;

//Protótipos das funções do Ranking
void cria(Ranking *l);
int vazia(Ranking *l);
int cheia(Ranking *l);
int insere(struct player j,  Ranking *l);
int retira(struct player j,  Ranking *l);
int primeiro(struct player *x,  Ranking *l);
int proximo(struct player *x,  Ranking *l);
int verifica(struct player x,  Ranking *l);

#endif