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
		string nucleationType;				//Tipo de nucleação
		string postNucleation;				//Tipo de nucleação durante o crescimento
		string inputFileName;				//Nome do arquivo de entrada
		string growthMethod;				//Tipo de crescimento
		
		string neighborsType; 				//Tipo de vizinhança
		
		int nNucleus;						//número de núcleos iniciais
		int growthType;						//Tipo de Crescimento do Cone Causal
		int criterio;						//Critério de parada
		int parametro;						//Parâmetro de parada
		string outputType;					//Tipo do arquivo de saída
		string outputFile; 					//Nome do arquivo de saída
		int percOut;						//Porcentagem inicial para geração de arquivos de saída
		int intOut;							//Intervalo percentual de geração de arquivos de saída
		
		bool switchSDPN;					//Determina se haverá Saída de Dados pós-Nucleação
		bool switchCresc;					//Determina se haverá Crescimento
		bool switchSDF;						//Determina se haverá Saída de dados final
		bool switchVert;					//Determina se serão feitos os cálculos de posição dos vértices
		
	private:
		
};

//Construtor de Dados incicializa cada membro com zeros


#endif
