#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StructRanking.h"

//Função que imprime os 10 primeiros elementos do Ranking astravés das funções primitivas 'primeiro' e 'proximo'
void imprime(Ranking *l){
  struct player x;
  int ok;
  int cont=1;

  printf("--------------RANKING--------------\n");

  ok=primeiro(&x, l);
  int linha=15-strlen(x.name); //variável para manter as informações em colunas
  
  if(ok==1){
    printf("%d. %s",cont,x.name);
    for(int i=1; i<=linha; i++){
      printf(".");
    }
    printf("          %d\n",x.points);
    ok=proximo(&x, l);
  } 
  
  while(ok==1 && cont<10){
    int linha=15-strlen(x.name);
    cont++;
    printf("%d. %s",cont,x.name);
    if(cont==10){
      for(int j=1; j<linha; j++)
        printf(".");
    }else{
      for(int i=1; i<=linha; i++){
        printf(".");
      }
    }
    printf("          %d\n",x.points);
    ok=proximo(&x, l);
  }
  printf("\n");
}