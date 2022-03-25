#include "crescimento.h"


Crescimento::Crescimento()
{
}


Crescimento::~Crescimento()
{
}

//Fun��es

void Crescimento::geraSeqAleat(vector<int> *vetor) //Embaralha o vetor
{
	/*
	Esta fun��o embaralha um vetor de 0 a 5 usando o algoritmo de Fisher�Yates
	Passos:
		1) gerar uma lista ordenada de N elementos (o vetor j� possui a lista)
		2) sortear um n�mero k entre 1 e N-1;
		3) trocar as posi��es dos elementos nas posi��es k e N;
		4) decrementar N;
		5) repetir do passo 2 at� que N seja igual a 0;
		6) o vetor original estar� embaralhado.
	*/

	int N;
	

	N = vetor->size(); //N�mero de elementos


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
