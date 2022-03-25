#include "plano.h"


//Construtores

Plano::Plano() //Construtor Padrão
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;
}

Plano::Plano(Ponto ponto, Ponto normal) //Construtor com ponto pertencente ao plano e vertor normal 
{
	//Coeficientes da equação do plano ax + by + cz = d
	
	a = normal.x;

	b = normal.y;

	c = normal.z;

	d = produtoInterno(ponto, normal);
}

Plano::Plano(Ponto P1, Ponto P2, Ponto P3) //Construtor com três pontos pertencentes ao plano
{
	Ponto P, Q, N;

	P = soma(P1, produto(-1, P2)); //um vetor pertencente ao plano

	Q = soma(P1, produto(-1, P3)); //outro vetor pertencente ao plano

	N = produtoExterno(P, Q); //vetor normal ao plano

	//Coeficientes da equação do plano ax + by + cz = d

	a = N.x;

	b = N.y;

	c = N.z;

	d = produtoInterno(P1, N);
}

Plano::Plano(double A, double B, double C, double D) //Construtor com os coeficienteas da equação do plano
{
	//Coeficientes da equação do plano ax + by + cz = d
	
	a = A;

	b = B;

	c= C;

	d = D;
}


//Destrutor

Plano::~Plano(void)
{
}

//Métodos

//Define os coeficientes
void Plano::set(double A, double B, double C, double D)
{
	//Coeficientes da equação do plano ax + by + cz = d
	
	a = A;

	b = B;

	c= C;

	d = D;
}

//Exibe na tela os coeficientes do plano
void Plano::exibe()
{
	cout << "\n" << a << "\t" << b << "\t" << c << "\t" << d << endl;
}


//Retorna o vetor normal ao plano
Ponto Plano::normal()
{
	Ponto P(a, b, c); //Vetor normal ao plano

	return P;
}

/////////////////////////////////////////////
//   Funções para operações com planos    //
///////////////////////////////////////////

//Retorna verdadeiro se a interseção entre os três planos for um Ponto
bool intersecaoUnica(Plano A, Plano B, Plano C)
{
	Ponto P, Q, R;

	P = A.normal(); //Vetor normal ao plano A

	Q = B.normal(); //Vetor normal ao plano B

	R = C.normal(); //Vetor normal ao plano C

	//Se o Determinante(P, Q, R) != 0 então só há uma solução
	
	if (produtoInterno(produtoExterno(P, Q), R) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//Retorna o Ponto de Interseção entre os Planos (considerando que haja interseção)
Ponto intersecao(Plano A, Plano B, Plano C)
{
	Ponto P, Q, R;

	double M, Mx, My, Mz;

	double X, Y, Z;

	P = A.normal(); //Vetor normal ao plano A

	Q = B.normal(); //Vetor normal ao plano B

	R = C.normal(); //Vetor normal ao plano C

	//Interseção pela regra de Cramer

	//Determinante M

	M = produtoInterno(produtoExterno(P, Q), R);

	//Determinante Mx

	P.set(A.d, A.b, A.c);

	Q.set(B.d, B.b, B.c);

	R.set(C.d, C.b, C.c);

	Mx = produtoInterno(produtoExterno(P, Q), R);

	//Determinante My

	P.set(A.a, A.d, A.c);

	Q.set(B.a, B.d, B.c);

	R.set(C.a, C.d, C.c);

	My = produtoInterno(produtoExterno(P, Q), R);

	//Determinante Mz

	P.set(A.a, A.b, A.d);

	Q.set(B.a, B.b, B.d);

	R.set(C.a, C.b, C.d);

	Mz = produtoInterno(produtoExterno(P, Q), R);

	//Solução

	X = Mx / M;

	Y = My / M;

	Z = Mz / M;

	Ponto S(X, Y, Z);

	return S;

}

