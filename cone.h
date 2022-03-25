#pragma once

#ifndef CONE_H
#define CONE_H

#include "crescimento.h" //Esta Classe � herdeira de Crescimento

#include <stdlib.h> // para gerar numeros aleat�rios
#include <time.h> // para gerar a 'random seed' baseada no tempo
#include <math.h>; //Para uso de fun��es matem�ticas
#include <vector>

#include <random> //Biblioteca para gera��o de n�meros aleat�rios

#include "dados.h"
#include "matriz.h"
#include "saida.h"
#include "nucleacao.h"


//teste
#include <iostream>
using namespace std;

class Cone : public Crescimento
{
public:

	//Construtores
	Cone();

	Cone(const Dados *, Matriz *);

	//Destrutor
	virtual ~Cone();
	
private:

	//M�todos

	int nucleomaisproximo(vector<double> *); 

	void novaNuc(const Dados *, Matriz *);

	////Dist�ncias
	double calcDistancia(int , int , int , int, int ); //Dist�ncia convencional entre dois pontos

	void distNuc(Matriz *); //Determina a menor dist�ncia entre um ponto e o n�cleos

	double getDistancias(int , int ); //Retorna a Dist�ncia entre a c�lula i e o n�cleo N
	
	//Vari�veis

	int tamanho; //Tamanho da matriz

	int Nucleos; //N�mero de N�cleos da Matriz

	vector<double> distancia; //Dist�ncia entre cada posi��o e os n�cleo mais pr�ximo
	vector<double> posCone; //Posi��o do Cone de 0 a N - 1
	
	
};

////Condi��es para determina��o da f�rmula da dist�ncia considerando as condi��es de contorno peri�dicas

int condInterno(int, int, double); //Retorna 0 se for usada a dist�ncia convencional e 1 para as demais

int condSinal(int, int); //Retorna 1 ou -1 segundo o crit�rio

#endif // !CONE_H