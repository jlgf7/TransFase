#pragma once

#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>

#include <vector> //Biblioteca para uso de vetores

#include <math.h> //Biblioteca para opera��es matem�ticas

#include "dados.h"



//Paraleliza��o
#include <omp.h>

using namespace std;

class Matriz
{
public:
	//Construtores
	Matriz();
	Matriz(const Dados *);	//Argumento: Conjunto de dados. Passagem por ponteiro visando desempenho.

	//Destrutor
	virtual ~Matriz();

	//M�todos

	//Define o estado inicial para determinada posi��o (posi��o, estado)
	void setState0(int pos, int sta); 

	//Define o estado atual para determinada posi��o (posi��o, estado)
	void setState1(int pos, int sta);

	//Retorna o estado inicial de determinada posi��o
	int getState0(int pos); 

	//Retorna o estado atual de determinada posi��o
	int getState1(int pos); 

	//Retorna o tamanho dos vetroes de estado
	int getTamVetor();

	//Retorna o vizinho para determinadas posi��o e dire��o
	int getVizinho(int pos, int dir);

	//
	//M�todos Relacionados ao Cone Causal
	//

	//Posi��o dos N�cleos
	void setPosNucleo(int endNucleo); //Insere o endere�o do n�cleo na matriz na pr�xima posi��o dispon�vel no vetor

	int getPosNucleo(int pos); //Retorna o endere�o do n�cleo da matriz armazenado no vetor de endere�os 
	//
	//void distNuc(); //Determina a menor dist�ncia entre os n�cleos e as posi��es da Matriz
	//
	//double getDistancias(int i, int N); //Determina a menor dist�ncia entre um ponto e o n�cleos

	//
	//Estereologia
	//

	//Retorna o N�mero de Gr�os por Unidade de Volume
	double calculoNv();
	double calculoNa(int inicio, int fim, int direcao);

	//Retorna a fra��o volum�trica (Vv) transformada considerando todas as fases
	double calculoVv();

	//Retorna a fra��o de superf�cie transformada por volume total (Svnr) considerando todas as fases
	void calculoSv(double *, double *, vector<int> *);

	//Retorna o tamanho da lista de Vv
	int tamListaV();

	//Retorna o tamanho da lista de Svnr
	int tamListaS();

	//Acrescenta um valor de Vv ao vetor listaV
	void pushBackListaV(double);

	//Acrescenta um valor de Sv ao vetor listaS
	void pushBackListaSvnr(double);

	void pushBackListaSvr(double);

	//Retorna o valor de uma posi��o em listaV
	double getListaV(int);

	//Retorna o valor de uma posi��o em listaS
	double getListaSvnr(int);

	double getListaSvr(int);

	//Curvatura

	//C�lculo da Na m�dia � necess�rio para o c�lculo de Hg

	double mediaNa(); //Calcula a m�dia dos Na's dos planos da matriz nas tr�s dire��es

	//Acrescenta um valor de Na ao vetor listaNa
	void pushBackListaNa(double);

	//Retorna o valor de uma posi��o em listaS
	double getListaNa(int);

	//Metalografia quantitativa

	//Volume de gr�o
	void calcVolGrao(const Dados *, double *, double *, vector<int> *);	//Faz os c�lculos relativos ao volume de gr�o

	void defineDir2(); //Define a ordem de leitura nas dire��es Y e Z sem uso da matriz de vizinhos

	//Tempo (Equivale aos passos de transforma��o)
	void passTime(); //Passa uma unidade de tempo
	int getTime(); //Retorna o tempo

	//Fun��es para retornar as dimens�es e caracter�sticas da matriz
	int getLinhas();
	int getColunas();
	int getCotas();	

	int numNucleos(); //N�mero de n�cleos
	void atualNumNucleos(int ); //Altera o n�mero de n�cleos que de fato est� em uso pelo programa 

	//Fun��es para o c�lculo de interfaces

	//Retorna a quantidade de interfaces de determinado gr�o
	int getInterface(int ); 

	//Retorna Verdadeiro se houver interface entre os dois gr�os
	bool getFacesComp(int, int );

	//void teste(Microestrutura *);

private:
	//M�todos
	virtual void setMalhaQuad(const Dados *);	//Malha Quadrangular

	void defineVizinhos(); //Preenche a Matriz de vizinhos

	void defineDir(); //Define a ordem de leitura nas dire��es Y e Z com uso da matriz de vizinhos

	//void defineDir2(); //Define a ordem de leitura nas dire��es Y e Z sem uso da matriz de vizinhos

	//
	//M�todos Relacionados ao Cone Causal
	//
	
	//Dist�ncias

	//double distInterna(int , int , int, int); //Calcula a Dist�ncia Convencional entre dois pontos da Matriz

	//void distNuc(); //Determina a menor dist�ncia entre os n�cleos e as posi��es da Matriz

	//
	//Estereologia
	//

	//Retornam o valor da sequ�ncia de leitura nas dire��es Y e Z para fra��o de �rea
	
	int getdirY(int pos);
	
	int getdirZ(int pos);


	//Fun��o para o c�lculo de interfaces

	void setInterface(int ); //Adiciona mais um valor ao contador de interface de determinado gr�o
	
	//Fun��o para o c�lculo de n�mero de faces

	void setFacesComp(int, int); //Adiciona verdadeiro (1) se houver interface entre dois gr�os

	//
	//Vari�veis
	//
	
	int linhas;
	int colunas;
	int cotas;
	int tamVetor;
	int Nucleos;

	int tempo; //Esta vari�vel corresponde aos passos de transforma��o e n�o ao tempo real

	//vetores para associar posi��o da malha � matriz
	vector<double> posX; 
	vector<double> posY;
	vector<double> posZ;
	
	vector<int> state0; //vetor que armazena o estado inicial do n� na malha
	vector<int> state1;	//vetor que armazena o estado atual do n� na malha

	//Vetores para o c�lculo de fra��o de �rea

	vector<int> dirY; //Ordem de leitura na dire��o Y
	vector<int> dirZ; //Ordem de leitura na dire��o Z

	///vector<int> stateY; //Estados ordenados segundo dirY
	//vector<int> stateZ; //Estados ordenados segundo dirZ

	int** vizinhos; //matriz de vizinhos para aut�mato celular

	vector<double> listaV; //Vetor para armazenar a varia��o da Vv durante a execu��o
	vector<double> listaSvnr; //Vetor para armazenar a varia��o da Svnr durante a execu��o
	vector<double> listaSvr; //Vetor para armazenar a varia��o da Svnr durante a execu��o
	vector<double> listaNa; //Vetor para armazenar a varia��o da Na durante a execu��o

	//Vetor para armazenar o endere�o dos N�cleos
	vector<int> enderecoNucleos;

	//double **distancias; //Dist�ncias entre cada posi��o e os n�cleos
	
	string metodoCrescimento; //String para armazenar "Automato" ou "Cone"

	//Vari�vel para c�lculo de n�mero de interfaces

	vector<int> interfaces;

	//Vari�vel para o c�lculo de n�mero de faces
	bool** facesComp;
};

#endif