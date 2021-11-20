#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StructRanking.h"

//Função para criar um Ranking, iniciando-o vazio.
void cria(Ranking *l){
  l->first = NULL;
  l->current = NULL;
}

//Função que verifica se um Ranking está vazio
int vazia(Ranking *l){
  return (l->first == NULL && l->current==NULL) ? 1:0;
}

//Função que verifica se um Ranking  está cheia. Como o limite da Ranking é a quantidade de memória do pc, a função assume que a Ranking nunca estará cheio
int cheia(Ranking *l){
  return 0;
}


//Função que insere o elemento x no Ranking , caso esse elemento ainda não esteja no Ranking; se estiver, não insere e retorna false.
int insere(struct player j,  Ranking *l){
   NodePtr p, anterior, novo;
  int ok;

  p=l->first;
  anterior=NULL;

  while(p!=NULL && p->info.points > j.points){
    anterior = p;
    p = p->next;
  }

    if(p!=l->first){ //j é inserido no meio ou final da Ranking
      novo = malloc(sizeof(struct Node));
      novo->info = j;
      anterior->next = novo;
      novo->next = p;
      l->current = novo;
      novo = NULL;
      anterior = NULL;
      p = NULL;
     return 1;
    }else{ //j é inserido no início ou  como único elemento do Ranking
      novo = malloc(sizeof(struct Node));
      novo->info = j;
      novo->next = p;
      l->first = novo;
      l->current = l->first;
      novo = NULL;
      anterior = NULL;
      p = NULL;
      return 1;
    }
    return 0;

}

//Função que retira o elemento x da Ranking , caso esse elemento esteja na Ranking; se não estiver, não retira e retorna false.
int retira(struct player j, Ranking *l){
   NodePtr p, anterior;
  int ok;

  p=l->first;
  anterior=NULL;


  while(p!=NULL && p->info.points>j.points && strcmp(p->info.name, j.name)<=0){
    anterior = p;
    p = p->next;
  }

  if(p!=NULL && strcmp(p->info.name, j.name)==0 && p->info.points==j.points){
    ok = 1;
  }else{
    ok = 0;
  }

  if(ok==0){ //Caso j não esteja no Ranking, nada acontece
    return 0;
  }else{ //j está no Ranking e será retirado
    if(p!=l->first){ //j está no meio ou fim do Ranking  
      anterior->next = p->next;
      free(p);
      p = NULL;
      anterior = NULL;
    }else{ //j é o 1º ou único elemento do Ranking
      l->first = l->first->next;
      free(p);
      p = NULL;
    }
    return 1;
  }
}

//Função que retorna em x o primeiro elemento do Ranking; se o Ranking estiver vazio, retorna false.
int primeiro(struct player *x,  Ranking *l){
   Ranking *aux = malloc(sizeof( Ranking));
  aux = l;

  aux->current = aux->first;

  if(aux->current!=NULL){
    *x = aux->current->info;
    return 1;
  }else{
    return 0;
  }
}

//Função que retorna em x o valor do próxmo elemento do Ranking em relação ao atual valor de x, antes do início da função. Caso não tenha, retorna false, e indica que o valor inicial de x é o último do Ranking.
int proximo(struct player *x,  Ranking *l){
   Ranking *aux = malloc(sizeof( Ranking));
  aux = l;
  if(aux->current!=NULL){
    aux->current = aux->current->next;
  }

  if(aux->current!=NULL){
    *x = aux->current->info;
    return 1;
  }else{
    return 0;
  }
}


//Função que verifica se o elemento x está no Ranking. Coloca p apontando para o nó que contém x e anterior para o nó anterior ao que contém x.
int verifica(struct player x,  Ranking *l){
  struct player aux;
  int ok;

  primeiro(&aux,l);
  
  if(x.points==aux.points && x.name==aux.name){
    return 1;
  }else{
    ok=proximo(&aux,l);
    while(ok==1){
      if(x.points==aux.points && x.name==aux.name){
        return 1;
      }else{
        ok=proximo(&aux,l);
    }
  }
    return 0;
  }
}