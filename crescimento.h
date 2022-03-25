#pragma once

#ifndef CRESCIMENTO_H
#define CRESCIMENTO_H

#include <vector>
#include <stdlib.h> // para gerar n�meros aleat�rios
#include <time.h> // para gerar a 'random seed' baseada no tempo

#include "dados.h"
#include "matriz.h"

class Crescimento
{
public:

	//Construtores
	Crescimento();

	//Destrutor
	virtual ~Crescimento();

protected:

	//Fun��es

	void geraSeqAleat(vector<int> *); //Embaralha um veor de inteiros
};

#endif