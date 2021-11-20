#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "fogefoge.h"
#include "mapa.h"
#include "ui.h"
#include "ImprimeRanking.h"

//Separar correlatos em arquivos separados auxilia na manutenção
//memset - limpa todos os dados em um espaço de memória

MAPA m;
POSICAO heroi;
int tempilula = 2;

//Função para definir quando o jogo acaba
int acabou(){
	POSICAO pos;

	int perdeu = !encontramapa(&m, &pos, HEROI);
	int ganhou = !encontramapa(&m, &pos, FANTASMA);

	return ganhou || perdeu;
}

//Função para limitar direções aceitas (Cima, Baixo, Esquerda e Direita)
int ehdirecao(char direcao){
	return
		direcao == ESQUERDA || 
		direcao == CIMA ||
		direcao == BAIXO ||
		direcao == DIREITA;
}

//Função para movimentação do herói
//Leva tempo constante pra rodar -> Leve pra rodar pois não depende do tamanho da matriz (Quanto maior a dependência, maior o tempo pra rodar)
void move(char direcao){
  //X e Y guardam posição
	int proximox = heroi.x;
	int proximoy = heroi.y;

  //Switch de movimentação a, s, d, w
	switch(direcao){
		case ESQUERDA:
      //mapa[x][y-1]
			proximoy--;
			break;
		case CIMA:
			proximox--;
			break;
		case BAIXO:
			proximox++;
			break;
		case DIREITA:
			proximoy++;
			break;
	}

  //Tem que ficar varrendo a matrix toda vez. Descartado. -> Variável pra guardar a posição do herói, função que procura a posição no mapa
  /*for(int i = 0; i < m.linhas; i++) {
		for(int j = 0; j < m.colunas; j++) {
			if(m.matriz[i][j] == '@') {
				x = i;
				y = j;
				break;
			}
		}
	}*/

	if(!podeandar(&m, HEROI, proximox, proximoy)) return;

	if(ehpersonagem(&m, PILULA, proximox, proximoy)){
    //Verificar se personagem ingeriu a pilula
		tempilula=1;
	}

	andanomapa(&m, heroi.x, heroi.y, proximox, proximoy);
	heroi.x = proximox;
	heroi.y = proximoy;
}

//Função para definir onde apresentar o fantasma, de modo randomico
int praondefantasmavai(int xatual, int yatual, 
	int* xdestino, int* ydestino){
    
  //Quatro possibilidades (Cima, Baixo, Esquerda e Direita)
	int opcoes[4][2] = { 
		{ xatual   , yatual+1 }, 
		{ xatual+1 , yatual   },  
		{ xatual   , yatual-1 }, 
		{ xatual-1 , yatual   }
	};

  //Definição de lugar randomico
	srand(time(0));
	for(int i = 0; i < 10; i++){
		int posicao = rand() % 4;

		if(podeandar(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1])){
			*xdestino = opcoes[posicao][0];
			*ydestino = opcoes[posicao][1];
			return 1;
		}
	}
	return 0;
}

//Funçãor responsável por andar o fantasma
void fantasmas(){
	MAPA copia;

	copiamapa(&copia, &m);

	for(int i = 0; i < copia.linhas; i++){
		for(int j = 0; j < copia.colunas; j++){
      //Se houver fantasma, temos de andar no mapa
			if(copia.matriz[i][j] == FANTASMA){
				int xdestino;
				int ydestino;

				int encontrou = praondefantasmavai(i, j, &xdestino, &ydestino);

				if(encontrou){
					andanomapa(&m, i, j, xdestino, ydestino);
				}
			}
		}
	}
  //Liberar pois caso contrário, todo movimento feito terá uma nova alocação de memória
	liberamapa(&copia);
}

//Recriar a função explodepilula(), que fará as 4 invocações necessárias
void explodepilula2(int x, int y, int somax, int somay, int qtd){
	if(qtd == 0) return;

  //Variáveis auxiliares
	int novox = x+somax;
	int novoy = y+somay;

	if(!ehvalida(&m, novox, novoy)) return;
	if(ehparede(&m, novox, novoy)) return;

	m.matriz[novox][novoy] = VAZIO;
	explodepilula2(novox, novoy, somax, somay, qtd-1);
}

//Função recursiva para permitir a explosão da pilula
void explodepilula(){
  //Atentar a como fazer uma função recursiva parar
	if(!tempilula) return;
	
  //Explodir o que tiver três casas ao redor do personagem
	explodepilula2(heroi.x, heroi.y, 0, 1, 3);
	explodepilula2(heroi.x, heroi.y, 0, -1, 3);
	explodepilula2(heroi.x, heroi.y, 1, 0, 3);
	explodepilula2(heroi.x, heroi.y, -1, 0, 3);
	
  //Zera a quantidade de pilulas após o uso da mesma
	tempilula = 0;
}


int main(){
	lemapa(&m);
	encontramapa(&m, &heroi, HEROI);

  FILE *rank;
  rank = fopen("ranking.txt","a");

  //Variáveis para manutenção do Ranking
  Ranking *r = malloc(sizeof(Ranking));
  cria(r);
  struct player p; //Struct que armazena nome e pontuação do jogador
  
   printf("-------------------FOGE-FOGE--------------------\n\n");
  printf("Player: ");
  scanf("%s",p.name);
  p.points=0;

	do{
		printf("Pílula: %s\n", (tempilula ? "SIM" : "NÃO"));
		imprimemapa(&m);

		char comando;
		scanf(" %c", &comando);

		if(ehdirecao(comando)) move(comando);
		if(comando == BOMBA) explodepilula();

		fantasmas();

    p.points += 100; //A cada movimento o jogador soma 100 pontos

    
	}while (!acabou());

  ///Salva no arquivo "ranking.txt" as inforção do ranking
  fwrite(&p,sizeof(struct player),1,rank);
  fclose(rank);

  printf("-------------GAME_OVER------------\n\n\n\n");

  //Dá a possibilidade do jogador ver o ranking
  printf("Quer ver o ranking?\n1(SIM) 0(NAO)   ");
  printf("\n");
  int x;
  scanf("%d",&x);
  if(x==1){
    
    fopen("ranking.txt","r");
    //Enquanto houver dados no arquivo "ranking.txt", eles serão inseridos em r (tipo Ranking)
    while(fread(&p,sizeof(struct player),1,rank)==1){
      insere(p,r);
    }
  }
  fclose(rank);
  imprime(r);
  free(r);

	liberamapa(&m);

  return 0;
}