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

	//Métodos

	//Define os coeficientes
	void set(double, double, double, double);

	//Exibe na tela os coeficientes do plano
	void exibe();

	//Retorna o vetor normal ao plano
	Ponto normal();

	
	//Variáveis

	//Equação do Plano
	double a, b, c, d;

private:

	
};

//Funções

//Retorna verdadeiro se a interseção entre os três planos for um Ponto
bool intersecaoUnica(Plano, Plano, Plano);

//Retorna o Ponto de Interseção entre os Planos (considerando que haja interseção)
Ponto intersecao(Plano, Plano, Plano);

#endif