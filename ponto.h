#pragma once

#ifndef PONTO_H
#define PONTO_H

#include <iostream>
#include <math.h>


using namespace std;

class Ponto
{
public:
	//Construtores
	Ponto();

	Ponto(int, int, int, int );

	Ponto(double, double, double);

	//Destrutor
	virtual ~Ponto();

	//M�todos

	//Define o ponto (x,y,z)
	void set(double , double , double );

	void set(int, int, int, int);

	void oposto(); //Efetuta -P

	void exibe(); //Exibe o ponto na tela (m�todo para testes)
	
	//Vari�veis
	double x, y, z;

private:

	
};

//Fun��es para opera��es com pontos

//Efetua k*P
Ponto produto(double k, Ponto P); 

//Efetua P + Q
Ponto soma(Ponto P, Ponto Q); 

//Efetua P x Q 
Ponto produtoExterno(Ponto P, Ponto Q);

//Efetua P . Q
double produtoInterno(Ponto P, Ponto Q); 

//Efetua ||P||
double norma(Ponto P);

//Calcula a dist�ncia
double distancia(Ponto P, Ponto Q);

#endif