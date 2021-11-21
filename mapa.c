//Agrupamento, tudo o que relaciona mapa em um arquivo
#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "mapa.h"
#include <string.h>

//Funções para manipulação da dificuldade
void setDificuldade(int d){
  dificuldade = d;
}

int getDificuldade(){
  return dificuldade;
}

//Função pra fazer a leitura do arquivo onde está desenhado o mapa
void lemapa(MAPA* m){
  
	FILE* f;
  switch(dificuldade){
    case 1:
      f = fopen("mapaEasy.txt", "r");
      break;
    case 2:
      f = fopen("mapaMedium.txt", "r");
      break;
    case 3:
     f = fopen("mapaHard.txt", "r");
      break;
    default:
     printf("ERRO! Dificuldade inválida.\nPor padrão sua dificuldade foi selecinada como Medium.\n");
     f = fopen("mapaMedium.txt", "r");
  }
	
	if(f == 0) {
		printf("Erro na leitura do mapa");
		exit(1);
	}

	fscanf(f, "%d %d", &(m->linhas), &(m->colunas));
	alocamapa(m);
	
	for(int i = 0; i < m->linhas; i++){
		fscanf(f, "%s", m->matriz[i]);
	}

	fclose(f);
}

//Função para alocação do mapa
//Genérica (recebimento de ponteiros), funciona pra qualquer mapa (Função reutilizável)
void alocamapa(MAPA* m){
	m -> matriz = malloc(sizeof(char*) * m -> linhas);

  //sizeof retorna quantos bytes o tipo de variável precisa
  //Alocação de matriz dinamicamente
	for(int i = 0; i < m -> linhas; i++){
		m -> matriz[i] = malloc(sizeof(char) * m -> colunas + 1);
	}
}

//Função para gerar cópia do mapa
//Cópia do mapa onde eu uso uma pra ler e outra pra mudar
//Os fantasmas estarão nos mesmos lugares e não teremos problemas de colisão 
void copiamapa(MAPA* destino, MAPA* origem){
	destino -> linhas = origem -> linhas;
	destino -> colunas = origem -> colunas;
	alocamapa(destino);

  //Copia da matriz
	for(int i = 0; i < origem -> linhas; i++){
    //Uma string copia pra outra string (Copia até achar o /0)
		strcpy(destino -> matriz[i], origem -> matriz[i]);
	}
}

//Função para liberar o mapa alocado na memória
//"free (ptr) avisa ao sistema que o bloco de bytes apontado por ptr está disponível para reciclagem"
void liberamapa(MAPA* m){
	for(int i = 0; i < m -> linhas; i++) {
		free(m -> matriz[i]);
	}
	free(m -> matriz);
}

//Função genérica para busca de posição  no mapa -> Pode procurar posição de qualquer variável (heroi, fantasma)
int encontramapa(MAPA* m, POSICAO* p, char c){
	for(int i = 0; i < m -> linhas; i++) {
		for(int j = 0; j < m -> colunas; j++){
			if(m -> matriz[i][j] == c) {
				p -> x = i;
				p -> y = j;
				return 1;
			}
		}
	}
	return 0;
}

//Função para permitir andar no mapa caso não seja parede ou encontre personagem
int podeandar(MAPA* m, char personagem, int x, int y){
	return 
    //Pode andar se está em posição válida, se não é parede ou o heroi
		ehvalida(m, x, y) && 
		!ehparede(m, x, y) &&
		!ehpersonagem(m, personagem, x, y);
}

//Função para validar movimentação
int ehvalida(MAPA* m, int x, int y){
	//Não permite movimentar além das linhas
  if(x >= m -> linhas) return 0;
  //Não permite movimentar além das colunas
	if(y >= m -> colunas) return 0;

	return 1;	
}

//Função para definir o que é personagem
int ehpersonagem(MAPA* m, char personagem, int x, int y){
	return
		m->matriz[x][y] == personagem;
}

//Função para definir o que é parede, de modo que o personagem não possa ultrapassá-la
int ehparede(MAPA* m, int x, int y){
	return 
		m->matriz[x][y] == PAREDE_VERTICAL ||
		m->matriz[x][y] == PAREDE_HORIZONTAL;
}

//Função para movimentação no mapa
//Descobre o que tem na posição, manipula origem e destino
void andanomapa(MAPA* m, int xorigem, int yorigem, 
	int xdestino, int ydestino){

	char personagem = m -> matriz[xorigem][yorigem];
	m -> matriz[xdestino][ydestino] = personagem;
	m -> matriz[xorigem][yorigem] = VAZIO;
}