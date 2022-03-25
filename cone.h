#pragma once

#ifndef CONE_H
#define CONE_H

#include "crescimento.h" //Esta Classe é herdeira de Crescimento

#include <stdlib.h> // para gerar numeros aleatórios
#include <time.h> // para gerar a 'random seed' baseada no tempo
#include <math.h>; //Para uso de funções matemáticas
#include <vector>

#include <random> //Biblioteca para geração de números aleatórios

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

	//Métodos

	int nucleomaisproximo(vector<double> *); 

	void novaNuc(const Dados *, Matriz *);

	////Distâncias
	double calcDistancia(int , int , int , int, int ); //Distância convencional entre dois pontos

	void distNuc(Matriz *); //Determina a menor distância entre um ponto e o núcleos

	double getDistancias(int , int ); //Retorna a Distância entre a célula i e o núcleo N
	
	//Variáveis

	int tamanho; //Tamanho da matriz

	int Nucleos; //Número de Núcleos da Matriz

	vector<double> distancia; //Distância entre cada posição e os núcleo mais próximo
	vector<double> posCone; //Posição do Cone de 0 a N - 1
	
	
};

////Condições para determinação da fórmula da distância considerando as condições de contorno periódicas

int condInterno(int, int, double); //Retorna 0 se for usada a distância convencional e 1 para as demais

int condSinal(int, int); //Retorna 1 ou -1 segundo o critério

#endif // !CONE_H