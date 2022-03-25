#pragma once

#ifndef SAIDA_H
#define SAIDA_H


#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <vector>

#include "dados.h"
#include "matriz.h"
#include "ponto.h"


using namespace std;

class Saida
{
public:
	//Construtores
	Saida();

	//Destrutor
	virtual ~Saida();

	//Métodos

	void saidaMicro(const Dados *, Matriz *, string); //Método que determina qual tipo de saída será usada para visualização da microestrutura 
	
	void saidaTecplot(const Dados *, Matriz *, string); //Saída padrão de arquivo escrito para TecPlot (daods, matriz, nome do arquivo de saida)

	void saidaVTK(const Dados *, Matriz *, string); //Saída padrão de arquivo escrito em arquivo VTK (daods, matriz, nome do arquivo de saida)

	void medidas(Matriz *); //Saída com os valores das medições realizadas no programa

	void escreveVetor(vector<int> *, string );

	void escrevePontos(vector<Ponto> *, string);

	void escreveNucleos(const Dados *, Matriz *);

private:
	
	int linhas;

	int colunas;

	int cotas;

};

#endif