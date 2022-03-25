#pragma once

#ifndef MICROESTRUTURA_H
#define MICROESTRUTURA_H

#include <vector>
#include <iostream> 

#include "grao.h"
#include "ponto.h"
#include "plano.h"
#include "cone.h" 
#include "matriz.h"



using namespace std;


class Microestrutura
{
public:

	//Construtores

	Microestrutura();

	Microestrutura(Matriz *);

	//Destrutor

	virtual ~Microestrutura();

	//M�todos
	 
	 Grao getGrao(int ); //retorna um gr�o da lista de gr�os de acordo com o id

	 void exibe(); //teste, poder� ser exclu�da posteriormente

	 vector<Ponto> listaVertices(); //Lista os v�rtices da microestrutura

private:

	//M�todos

	Ponto convertePonto(Ponto , Ponto , Matriz *); //Converte o Ponto para seu equivalente de fora da Matriz

	void encontraIntersecoes(Grao , Matriz * ); //Encontra os v�rtices

	void filtraPontos(vector<Ponto> *); //vector<Plano> *); //Elimina as interse��es que n�o fazem parte do poliedro

	void limpaRepetidos(vector<Ponto> *); //Elimina pontos repetidos

	//Vari�veis

	vector<Grao> listaDeGraos; //Armazena os gr�os da matriz

	int numGraos; //N�mero de gr�os da matriz

	vector<Ponto> pontosI; //Armazena o pontos de interse��o dos gr�os
	
	vector<Plano> planoFace; //Vector contendo os planos das faces

	vector<Ponto> vertices;
};


#endif
