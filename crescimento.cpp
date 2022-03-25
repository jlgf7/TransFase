#include "crescimento.h"


Crescimento::Crescimento()
{
}


Crescimento::~Crescimento()
{
}

//Funções

void Crescimento::geraSeqAleat(vector<int> *vetor) //Embaralha o vetor
{
	/*
	Esta função embaralha um vetor de 0 a 5 usando o algoritmo de Fisher–Yates
	Passos:
		1) gerar uma lista ordenada de N elementos (o vetor já possui a lista)
		2) sortear um número k entre 1 e N-1;
		3) trocar as posições dos elementos nas posições k e N;
		4) decrementar N;
		5) repetir do passo 2 até que N seja igual a 0;
		6) o vetor original estará embaralhado.
	*/

	int N;
	

	N = vetor->size(); //Número de elementos


	int elementosRestantes = N;

	while (elementosRestantes > 0)
	{
		int k = rand() % (elementosRestantes); //passo 2

		//passo 3
		int tmp = (*vetor)[k];
        (*vetor)[k] = (*vetor)[elementosRestantes-1];
        (*vetor)[elementosRestantes-1] = tmp;
 
        elementosRestantes--; //passo 4
    
	}

	//cout << "\n" << (*vetor)[0] << (*vetor)[1] << (*vetor)[2] << (*vetor)[3] << (*vetor)[4] <<  "\n";

}
