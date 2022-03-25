#include "ponto.h"

//Contrutores

Ponto::Ponto(void) //Construtor padrão
{
	x = 0;

	y = 0;

	z = 0;
}

Ponto::Ponto(int ponto, int lin, int col, int cot) //Construtor para criação de ponto a partir de sua posição e das dimensões da matriz
{
	x = ponto % col; //resto da divisão da posição do vetor pelo número de colunas

	y = ((int) ponto / col) % lin; //resto da divisão da "divisão inteira da posição do vetor pelo número de colunas" pelo número de linhas

	z = (int) ponto / (col*lin); //Divisão inteira da posição do vetor pelo produto de linhas e colunas
}


Ponto::Ponto(double xx, double yy, double zz) //Construtor para criação de ponto a partir das coordenadas
{
	x = xx;

	y = yy;

	z = zz;
}

//Destrutor

Ponto::~Ponto(void)
{
}

//Métodos

//Define o ponto (x,y,z)
void Ponto::set(double X, double Y, double Z)
{
	x = X;

	y = Y;

	z = Z;
}

void Ponto::set(int ponto, int lin, int col, int cot) //Atribuição de ponto a partir de sua posição e das dimensões da matriz
{
	x = ponto % col; //resto da divisão da posição do vetor pelo número de colunas

	y = ((int) ponto / col) % lin; //resto da divisão da "divisão inteira da posição do vetor pelo número de colunas" pelo número de linhas

	z = (int) ponto / (col*lin); //Divisão inteira da posição do vetor pelo produto de linhas e colunas
}



void Ponto::oposto() //Efetuta -P
{
	x = - x;

	y = - y;

	z = - z;
}

void Ponto::exibe()
{
	cout << "\n" << x << "\t" << y << "\t" << z << endl;
}

/////////////////////////////////////////////
//   Funções para operações com pontos	  //
///////////////////////////////////////////

//Efetua k*P
Ponto produto(double k, Ponto P) 
{
	Ponto Q;

	Q.x = k * P.x;

	Q.y = k * P.y;

	Q.z = k * P.z;

	return Q;
}

//Efetua P + Q
Ponto soma(Ponto P, Ponto Q) 
{
	Ponto R;

	R.x = P.x + Q.x;

	R.y = P.y + Q.y;

	R.z = P.z + Q.z;

	return R;
}

//Efetua P x Q 
Ponto produtoExterno(Ponto P, Ponto Q) 
{
	Ponto R;
	
	R.x = P.y * Q.z - P.z * Q.y;

	R.y = - P.x * Q.z + P.z * Q.x;

	R.z = P.x * Q.y - P.y * Q.x;

	return R;
}

//Efetua P . Q
double produtoInterno(Ponto P, Ponto Q) 
{
	return P.x * Q.x + P.y * Q.y + P.z * Q.z;
}

//Efetua  ||P||
double norma(Ponto P) 
{
	return sqrt(pow(P.x, 2) + pow(P.y, 2) + pow(P.z, 2)); 
}

//Calcula a distância
double distancia(Ponto P, Ponto Q)
{
	return sqrt(pow(P.x - Q.x, 2) + pow(P.y - Q.y, 2) + pow(P.z - Q.z, 2)); 
}


