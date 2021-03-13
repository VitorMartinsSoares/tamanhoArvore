#include<stdio.h>
#include<stdlib.h>
#include <string.h>

typedef struct Record Record;
typedef struct Tree Tree;

struct Record{
	int key;
	int value;
};

struct Tree{
	Record reg;
	Tree *esq, *dir;
	int weight;
};

Tree* CreateTree(){
	return NULL;
}

void pesquisa(Tree **t, Tree **aux, Record r){

	if(*t == NULL){
		printf("[ERROR]: Node not found!");
		return;
	}

	if((*t)->reg.key > r.key){ pesquisa(&(*t)->esq, aux, r); return;}
	if((*t)->reg.key < r.key){ pesquisa(&(*t)->dir, aux, r); return;}

	*aux = *t;
}

int getWeight(Tree **t){
	if(*t == NULL)
		return -1;
	return (*t)->weight;
}

int getMaxWeight(int left, int right){
	if(left > right)
		return left;
	return right;
}

void rotacaoSimplesDireita(Tree **t){
	Tree *aux;
	aux = (*t)->esq;
	(*t)->esq = aux->dir;
	aux->dir = (*t);
	(*t)->weight = getMaxWeight(getWeight(&(*t)->esq), getWeight(&(*t)->dir)) + 1;
	aux->weight  = getMaxWeight(getWeight(&aux->esq), (*t)->weight) + 1;
	(*t) = aux;
}

void rotacaoSimplesEsquerda(Tree **t){
	Tree *aux;
	aux = (*t)->dir;
	(*t)->dir = aux->esq;
	aux->esq = (*t);
	(*t)->weight = getMaxWeight(getWeight(&(*t)->esq), getWeight(&(*t)->dir)) + 1;
	aux->weight  = getMaxWeight(getWeight(&aux->esq), (*t)->weight) + 1;
	(*t) = aux;
}

void rotacaoDuplaDireita(Tree **t){
	rotacaoSimplesEsquerda(&(*t)->esq);
	rotacaoSimplesDireita(t);
}

void rotacaoDuplaEsquerda(Tree **t){
	rotacaoSimplesDireita(&(*t)->dir);
	rotacaoSimplesEsquerda(t);
}

void insertTree(Tree **t, Record r)
{
  if(*t == NULL){
    *t = (Tree*)malloc(sizeof(Tree));
    (*t)->esq    = NULL; 
    (*t)->dir    = NULL; 
    (*t)->weight = 0;
    (*t)->reg    = r; 
  } else {
    
    if(r.key < (*t)->reg.key){
      insertTree(&(*t)->esq, r);
      if ((getWeight(&(*t)->esq) - getWeight(&(*t)->dir)) == 2){
      	if(r.key < (*t)->esq->reg.key)
      		rotacaoSimplesDireita(t);
      	else
      		rotacaoDuplaDireita(t);
      }
    }
    
    if(r.key > (*t)->reg.key){
      insertTree(&(*t)->dir, r);
      if ((getWeight(&(*t)->dir) - getWeight(&(*t)->esq)) == 2){
      	if(r.key > (*t)->dir->reg.key)
      		rotacaoSimplesEsquerda(t);
      	else
      		rotacaoDuplaEsquerda(t);
      }
    }
  
  }

  (*t)->weight = getMaxWeight(getWeight(&(*t)->esq), getWeight(&(*t)->dir)) + 1;
}

void antecessor(Tree **t, Tree *r){ 
	if ((*t)->dir != NULL){ 
		antecessor(&(*t)->dir, r);
		return;
  	}
  	r->reg = (*t)->reg;
  	r = *t; 
  	*t = (*t)->esq;
  	free(r);
} 


void rebalanceTree(Tree **t){
	int balance;
  	int left = 0;
  	int right = 0;

	balance = getWeight(&(*t)->esq) - getWeight(&(*t)->dir);
	if((*t)->esq)
		left = getWeight(&(*t)->esq->esq) - getWeight(&(*t)->esq->dir);
	if((*t)->dir)
		right = getWeight(&(*t)->dir->esq) - getWeight(&(*t)->dir->dir);

	printf("Raiz:%d, Filho esq:%d, Filho dir:%d\n", balance, left, right);

	if(balance == 2 && left >= 0)
		rotacaoSimplesDireita(t);
	if(balance == 2 && left < 0)
		rotacaoDuplaDireita(t);

	if(balance == -2 && right >= 0)
		rotacaoDuplaEsquerda(t);
	if(balance == -2 && right < 0)
		rotacaoSimplesEsquerda(t);
  	
  	
}

void removeTree(Tree **t, Tree **f, Record r){
	Tree *aux;
  	
  	if (*t == NULL){ 
  		printf("[ERROR]: Record not found!!!\n");
    	return;
  	}

  	if (r.key < (*t)->reg.key){ removeTree(&(*t)->esq, t, r); return;}
  	if (r.key > (*t)->reg.key){ removeTree(&(*t)->dir, t, r); return;}

  	if ((*t)->dir == NULL){ 
  		aux = *t;  
  		*t = (*t)->esq;
    	free(aux);
    	rebalanceTree(f);
    	return;
  	}

  	if ((*t)->esq != NULL){ 
  		antecessor(&(*t)->esq, *t); 
  		rebalanceTree(f);
  		return;
  	}

  	aux = *t;  
  	*t = (*t)->dir;
  	free(aux);
  	rebalanceTree(f);  	
  	
}

void showTreeInOrder(Tree *t)
{
  if(!(t == NULL)){
    showTreeInOrder(t->esq); 
    printf("%d ", t->reg.key);
    showTreeInOrder(t->dir); 
  }
}


int tamanhoMaximo=0;
void calcTamanho(Tree **t, int tam){
    tam+=1;
    for(int i=0;i<2;i++){
        if((*t)->esq == NULL){
            if((*t)->dir == NULL){
				if(tamanhoMaximo<tam){
					printf("%d\n",(*t)->reg.key);
					tamanhoMaximo = tam;
				}
            }else{
                calcTamanho(&(*t)->dir,tam);
            }
        }else if((*t)->dir == NULL){
            calcTamanho(&(*t)->esq,tam);
        }else{
            calcTamanho(&(*t)->esq,tam);
            calcTamanho(&(*t)->dir,tam);
        }
    }
}

int main(){
	char *teste;
	int reg[] = {6,4,8,5,2,3,1,7,9,15,12,11,13,14,10,16};
	Tree *raiz = CreateTree();
	Record r;
	for(int i=0; i<16; i++){
		r.key = reg[i];
		r.value = 1;
		insertTree(&raiz, r);
	}
	calcTamanho(&raiz,0);
	printf("O TAMANHO MAXIMO EH: %d", tamanhoMaximo);
	return 0;
}