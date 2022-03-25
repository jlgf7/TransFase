#pragma once

#ifndef AUTOMATO_H
#define AUTOMATO_H

#include "crescimento.h" //Esta Classe � herdeira de Crescimento

#include <stdlib.h> // para gerar n�meros aleat�rios
#include <time.h> // para gerar a 'random seed' baseada no tempo

#include <random> //Biblioteca para gera��o de n�meros aleat�rios

#include "dados.h"
#include "matriz.h"
#include "saida.h"


//teste
#include <iostream>
using namespace std;

class Automato : public Crescimento
{
public:
	//Construtores
	Automato();

	Automato(const Dados *, Matriz *); //Argumentos: Objetos Dados e Matriz

	//Destrutor
	virtual ~Automato();

private:

	//Fun��es

	//void geraSeqAleat(int *); //Gera um vetor com os n�meros de 0 a 5 embaralhados
	void igualaEstados(Matriz *, int); //Iguala state0 a state1 ou vice-versa
	
};

#endif
