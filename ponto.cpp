#include "ponto.h"

//Contrutores

Ponto::Ponto(void) //Construtor padr�o
{
	x = 0;

	y = 0;

	z = 0;
}

Ponto::Ponto(int ponto, int lin, int col, int cot) //Construtor para cria��o de ponto a partir de sua posi��o e das dimens�es da matriz
{
	x = ponto % col; //resto da divis�o da posi��o do vetor pelo n�mero de colunas

	y = ((int) ponto / col) % lin; //resto da divis�o da "divis�o inteira da posi��o do vetor pelo n�mero de colunas" pelo n�mero de linhas

	z = (int) ponto / (col*lin); //Divis�o inteira da posi��o do vetor pelo produto de linhas e colunas
}


Ponto::Ponto(double xx, double yy, double zz) //Construtor para cria��o de ponto a partir das coordenadas
{
	x = xx;

	y = yy;

	z = zz;
}

//Destrutor

Ponto::~Ponto(void)
{
}

//M�todos

//Define o ponto (x,y,z)
void Ponto::set(double X, double Y, double Z)
{
	x = X;

	y = Y;

	z = Z;
}

void Ponto::set(int ponto, int lin, int col, int cot) //Atribui��o de ponto a partir de sua posi��o e das dimens�es da matriz
{
	x = ponto % col; //resto da divis�o da posi��o do vetor pelo n�mero de colunas

	y = ((int) ponto / col) % lin; //resto da divis�o da "divis�o inteira da posi��o do vetor pelo n�mero de colunas" pelo n�mero de linhas

	z = (int) ponto / (col*lin); //Divis�o inteira da posi��o do vetor pelo produto de linhas e colunas
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
//   Fun��es para opera��es com pontos	  //
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

//Calcula a dist�ncia
double distancia(Ponto P, Ponto Q)
{
	return sqrt(pow(P.x - Q.x, 2) + pow(P.y - Q.y, 2) + pow(P.z - Q.z, 2)); 
}


