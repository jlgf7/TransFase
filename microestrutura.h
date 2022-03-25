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

	//Métodos
	 
	 Grao getGrao(int ); //retorna um grão da lista de grãos de acordo com o id

	 void exibe(); //teste, poderá ser excluída posteriormente

	 vector<Ponto> listaVertices(); //Lista os vértices da microestrutura

private:

	//Métodos

	Ponto convertePonto(Ponto , Ponto , Matriz *); //Converte o Ponto para seu equivalente de fora da Matriz

	void encontraIntersecoes(Grao , Matriz * ); //Encontra os vértices

	void filtraPontos(vector<Ponto> *); //vector<Plano> *); //Elimina as interseções que não fazem parte do poliedro

	void limpaRepetidos(vector<Ponto> *); //Elimina pontos repetidos

	//Variáveis

	vector<Grao> listaDeGraos; //Armazena os grãos da matriz

	int numGraos; //Número de grãos da matriz

	vector<Ponto> pontosI; //Armazena o pontos de interseção dos grãos
	
	vector<Plano> planoFace; //Vector contendo os planos das faces

	vector<Ponto> vertices;
};


#endif
