
#ifndef NUCLEACAO_H
#define NUCLEACAO_H

#include <iostream>
#include <random> //Biblioteca para geração de números aleatórios
#include <time.h>
#include <math.h>
//#include <windows.h>

#include "dados.h"
#include "matriz.h"

using namespace std;


class Nucleacao
{
public:
	//Construtores
	Nucleacao();
	Nucleacao(const Dados *, Matriz *); //Argumentos: Objetos Dados e Matriz

	//Destrutor
	virtual ~Nucleacao();

private:

	void uniforme(const Dados *, Matriz *); //Argumentos: Objetos Dados e Matriz

	void importar(const Dados *, Matriz *); //Argumentos: Objetos Dados e Matriz

	void periodica(const Dados *, Matriz *); //Argumentos: Objetos Dados e Matriz

	int arredondar(double ); //Arredonda um número para o inteiro mais próximo

	int convVet( int , int, int , const Dados *); // Converte coordenadas cartesianas para a coordenada vetorial
};



#endif
