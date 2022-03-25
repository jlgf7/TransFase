#pragma once

#ifndef PLANO_H
#define PLANO_H

#include "ponto.h"
#include <iostream>

using namespace std;

class Plano
{
public:

	//Construtores
	Plano();

	Plano(Ponto , Ponto);

	Plano(Ponto, Ponto, Ponto);

	Plano(double, double, double, double);

	virtual ~Plano();

	//M�todos

	//Define os coeficientes
	void set(double, double, double, double);

	//Exibe na tela os coeficientes do plano
	void exibe();

	//Retorna o vetor normal ao plano
	Ponto normal();

	
	//Vari�veis

	//Equa��o do Plano
	double a, b, c, d;

private:

	
};

//Fun��es

//Retorna verdadeiro se a interse��o entre os tr�s planos for um Ponto
bool intersecaoUnica(Plano, Plano, Plano);

//Retorna o Ponto de Interse��o entre os Planos (considerando que haja interse��o)
Ponto intersecao(Plano, Plano, Plano);

#endif