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

	//Métodos

	//Define o ponto (x,y,z)
	void set(double , double , double );

	void set(int, int, int, int);

	void oposto(); //Efetuta -P

	void exibe(); //Exibe o ponto na tela (método para testes)
	
	//Variáveis
	double x, y, z;

private:

	
};

//Funções para operações com pontos

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

//Calcula a distância
double distancia(Ponto P, Ponto Q);

#endif