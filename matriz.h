#pragma once

#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>

#include <vector> //Biblioteca para uso de vetores

#include <math.h> //Biblioteca para operações matemáticas

#include "dados.h"



//Paralelização
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

	//Métodos

	//Define o estado inicial para determinada posição (posição, estado)
	void setState0(int pos, int sta); 

	//Define o estado atual para determinada posição (posição, estado)
	void setState1(int pos, int sta);

	//Retorna o estado inicial de determinada posição
	int getState0(int pos); 

	//Retorna o estado atual de determinada posição
	int getState1(int pos); 

	//Retorna o tamanho dos vetroes de estado
	int getTamVetor();

	//Retorna o vizinho para determinadas posição e direção
	int getVizinho(int pos, int dir);

	//
	//Métodos Relacionados ao Cone Causal
	//

	//Posição dos Núcleos
	void setPosNucleo(int endNucleo); //Insere o endereço do núcleo na matriz na próxima posição disponível no vetor

	int getPosNucleo(int pos); //Retorna o endereço do núcleo da matriz armazenado no vetor de endereços 
	//
	//void distNuc(); //Determina a menor distância entre os núcleos e as posições da Matriz
	//
	//double getDistancias(int i, int N); //Determina a menor distância entre um ponto e o núcleos

	//
	//Estereologia
	//

	//Retorna o Número de Grãos por Unidade de Volume
	double calculoNv();
	double calculoNa(int inicio, int fim, int direcao);

	//Retorna a fração volumétrica (Vv) transformada considerando todas as fases
	double calculoVv();

	//Retorna a fração de superfície transformada por volume total (Svnr) considerando todas as fases
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

	//Retorna o valor de uma posição em listaV
	double getListaV(int);

	//Retorna o valor de uma posição em listaS
	double getListaSvnr(int);

	double getListaSvr(int);

	//Curvatura

	//Cálculo da Na média é necessário para o cálculo de Hg

	double mediaNa(); //Calcula a média dos Na's dos planos da matriz nas três direções

	//Acrescenta um valor de Na ao vetor listaNa
	void pushBackListaNa(double);

	//Retorna o valor de uma posição em listaS
	double getListaNa(int);

	//Metalografia quantitativa

	//Volume de grão
	void calcVolGrao(const Dados *, double *, double *, vector<int> *);	//Faz os cálculos relativos ao volume de grão

	void defineDir2(); //Define a ordem de leitura nas direções Y e Z sem uso da matriz de vizinhos

	//Tempo (Equivale aos passos de transformação)
	void passTime(); //Passa uma unidade de tempo
	int getTime(); //Retorna o tempo

	//Funções para retornar as dimensões e características da matriz
	int getLinhas();
	int getColunas();
	int getCotas();	

	int numNucleos(); //Número de núcleos
	void atualNumNucleos(int ); //Altera o número de núcleos que de fato está em uso pelo programa 

	//Funções para o cálculo de interfaces

	//Retorna a quantidade de interfaces de determinado grão
	int getInterface(int ); 

	//Retorna Verdadeiro se houver interface entre os dois grãos
	bool getFacesComp(int, int );

	//void teste(Microestrutura *);

private:
	//Métodos
	virtual void setMalhaQuad(const Dados *);	//Malha Quadrangular

	void defineVizinhos(); //Preenche a Matriz de vizinhos

	void defineDir(); //Define a ordem de leitura nas direções Y e Z com uso da matriz de vizinhos

	//void defineDir2(); //Define a ordem de leitura nas direções Y e Z sem uso da matriz de vizinhos

	//
	//Métodos Relacionados ao Cone Causal
	//
	
	//Distâncias

	//double distInterna(int , int , int, int); //Calcula a Distância Convencional entre dois pontos da Matriz

	//void distNuc(); //Determina a menor distância entre os núcleos e as posições da Matriz

	//
	//Estereologia
	//

	//Retornam o valor da sequência de leitura nas direções Y e Z para fração de área
	
	int getdirY(int pos);
	
	int getdirZ(int pos);


	//Função para o cálculo de interfaces

	void setInterface(int ); //Adiciona mais um valor ao contador de interface de determinado grão
	
	//Função para o cálculo de número de faces

	void setFacesComp(int, int); //Adiciona verdadeiro (1) se houver interface entre dois grãos

	//
	//Variáveis
	//
	
	int linhas;
	int colunas;
	int cotas;
	int tamVetor;
	int Nucleos;

	int tempo; //Esta variável corresponde aos passos de transformação e não ao tempo real

	//vetores para associar posição da malha à matriz
	vector<double> posX; 
	vector<double> posY;
	vector<double> posZ;
	
	vector<int> state0; //vetor que armazena o estado inicial do nó na malha
	vector<int> state1;	//vetor que armazena o estado atual do nó na malha

	//Vetores para o cálculo de fração de área

	vector<int> dirY; //Ordem de leitura na direção Y
	vector<int> dirZ; //Ordem de leitura na direção Z

	///vector<int> stateY; //Estados ordenados segundo dirY
	//vector<int> stateZ; //Estados ordenados segundo dirZ

	int** vizinhos; //matriz de vizinhos para autômato celular

	vector<double> listaV; //Vetor para armazenar a variação da Vv durante a execução
	vector<double> listaSvnr; //Vetor para armazenar a variação da Svnr durante a execução
	vector<double> listaSvr; //Vetor para armazenar a variação da Svnr durante a execução
	vector<double> listaNa; //Vetor para armazenar a variação da Na durante a execução

	//Vetor para armazenar o endereço dos Núcleos
	vector<int> enderecoNucleos;

	//double **distancias; //Distâncias entre cada posição e os núcleos
	
	string metodoCrescimento; //String para armazenar "Automato" ou "Cone"

	//Variável para cálculo de número de interfaces

	vector<int> interfaces;

	//Variável para o cálculo de número de faces
	bool** facesComp;
};

#endif