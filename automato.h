#pragma once

#ifndef AUTOMATO_H
#define AUTOMATO_H

#include "crescimento.h" //Esta Classe é herdeira de Crescimento

#include <stdlib.h> // para gerar números aleatórios
#include <time.h> // para gerar a 'random seed' baseada no tempo

#include <random> //Biblioteca para geração de números aleatórios

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

	//Funções

	//void geraSeqAleat(int *); //Gera um vetor com os números de 0 a 5 embaralhados
	void igualaEstados(Matriz *, int); //Iguala state0 a state1 ou vice-versa
	
};

#endif
