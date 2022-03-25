#pragma once


#ifndef DADOS_H
#define DADOS_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;

class Dados
{
	public:
		Dados();
		
		virtual ~Dados();
		
		void setDados(const char*);

		int nx, ny, nz; 					//Numero de mallha nas direcoes x, y e z
		string mesh; 						//Tipo de malha usado
		string nucleationType;				//Tipo de nuclea��o
		string postNucleation;				//Tipo de nuclea��o durante o crescimento
		string inputFileName;				//Nome do arquivo de entrada
		string growthMethod;				//Tipo de crescimento
		
		string neighborsType; 				//Tipo de vizinhan�a
		
		int nNucleus;						//n�mero de n�cleos iniciais
		int growthType;						//Tipo de Crescimento do Cone Causal
		int criterio;						//Crit�rio de parada
		int parametro;						//Par�metro de parada
		string outputType;					//Tipo do arquivo de sa�da
		string outputFile; 					//Nome do arquivo de sa�da
		int percOut;						//Porcentagem inicial para gera��o de arquivos de sa�da
		int intOut;							//Intervalo percentual de gera��o de arquivos de sa�da
		
		bool switchSDPN;					//Determina se haver� Sa�da de Dados p�s-Nuclea��o
		bool switchCresc;					//Determina se haver� Crescimento
		bool switchSDF;						//Determina se haver� Sa�da de dados final
		bool switchVert;					//Determina se ser�o feitos os c�lculos de posi��o dos v�rtices
		
	private:
		
};

//Construtor de Dados incicializa cada membro com zeros


#endif
