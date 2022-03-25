#pragma once

#ifndef GRAO_H
#define GRAO_H

#include "matriz.h"
#include "ponto.h"
#include "plano.h"

class Grao
{
public:
	
	//Construtores

	Grao();
	Grao(int, Matriz *);

	//Destrutor
	virtual ~Grao(void);

	//M�todos

	vector<bool> estaNaFace(Matriz *); //Retorna o octante onde est� o n�cleo do gr�o

	bool getLocal(int ); //Retorna verdadeiro ou falso para cada face da matriz

	int getVizinhos(int ); //Retorna o Id dos vizinhos

	int numeroFaces(); //Retorna o n�mero de faces

	void setVertice(Ponto); //Insere um v�rtice ao gr�o

	int numeroVertices(); //Retorna o n�mero de v�rtices

	Ponto getVertice(int ); //Retorna um dos v�rtices 

	//Vari�veis
		
	int id; //Identificador do gr�o (1 a N, onde N � o n�mero de n�cleos)

	Ponto nucleo; //N�cleo do gr�o

private:

	int nFaces; //N�mero de faces

	vector<int> vizinhos; //Id dos vizinhos

	vector<bool> local; //[0]: Frente //[1]: Direita //[2]: Tr�s //[3] Esquerda //[4] Baixo //[5] Cima

	vector<Ponto> vertices; //Armazena os v�rtices do gr�o
	
};

#endif