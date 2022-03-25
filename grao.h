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

	//Métodos

	vector<bool> estaNaFace(Matriz *); //Retorna o octante onde está o núcleo do grão

	bool getLocal(int ); //Retorna verdadeiro ou falso para cada face da matriz

	int getVizinhos(int ); //Retorna o Id dos vizinhos

	int numeroFaces(); //Retorna o número de faces

	void setVertice(Ponto); //Insere um vértice ao grão

	int numeroVertices(); //Retorna o número de vértices

	Ponto getVertice(int ); //Retorna um dos vértices 

	//Variáveis
		
	int id; //Identificador do grão (1 a N, onde N é o número de núcleos)

	Ponto nucleo; //Núcleo do grão

private:

	int nFaces; //Número de faces

	vector<int> vizinhos; //Id dos vizinhos

	vector<bool> local; //[0]: Frente //[1]: Direita //[2]: Trás //[3] Esquerda //[4] Baixo //[5] Cima

	vector<Ponto> vertices; //Armazena os vértices do grão
	
};

#endif