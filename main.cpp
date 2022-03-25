#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <math.h>

#include "dados.h"
#include "matriz.h"
#include "nucleacao.h"
#include "cone.h"
#include "automato.h"
#include "saida.h"
#include "ponto.h"
#include "plano.h"
#include "grao.h"
#include "microestrutura.h"

//Paraleliza��o
#include <omp.h>

using namespace std;

void compFortan(Matriz *); //Compara��o com Fortran


int main()
{
	//Paraleliza��o
	double omp_get_wtime(void);
	double start;
	double end, tempo;
	start = omp_get_wtime();
	
	//Inicializa��o para os m�todos que necessitam de aleatoriedade
	srand (time (nullptr)); // Gera uma 'random seed' baseada no retorno da funcao time()
	
	//
	//Recebe os dados do arquivo
	//
	
	cout << "Abrindo o arquivo de Entrada e le os parametros...\n";

	Dados dados;
	const char* File = "Entrada_de_dados.dat";
	dados.setDados(File);

	cout << "Leitura concluida.\n\n";

	//
	//Cria a matriz 
	//

	cout << "Criando Malha\n";
	
	Matriz matriz(&dados); 

	cout << "Criacao concluida.\n\n";

	//Exibe Vv e Sv
	cout << "Tempo: " << matriz.getTime() << "\tVV\t" << matriz.getListaV(0)*100.0 << "\tSVnr\t" << matriz.getListaSvnr(0) << "\n\n";

	//
	//Realiza a Nuclea��o
	//

	cout << "Realizando nucleacao...\n\n";

	Nucleacao nucleos(&dados, &matriz);
	//compFortan(&matriz);

	cout << "Nucleacao concluida.\n\n";

	//
	//Sa�da com as posi��es dos n�cleos
	//

	if ( dados.switchSDPN ) //Determina se haver� Sa�da de Dados p�s-Nuclea��o
	{
		cout << "Escrevendo arquivo com as posicoes dos nucleos...\n\n";

		Saida saida1;
	
		saida1.saidaMicro(&dados, &matriz, "Inicio"); //Sa�da para visualiza��o da Microestrutura
	}

	
	//
	//Realiza o Crescimento
	//

	if ( dados.switchCresc ) //Determina se haver� Crescimento
	{
		cout << "Realizando crescimento...\n";


		if (dados.growthMethod == "Automato")
		{
			cout << "\n\nCrescimento via Automato Celular\n\n";

			Automato automato(&dados, &matriz);
		}
		else if (dados.growthMethod == "Cone")
		{
			cout << "\n\nCrescimento via Cone Causal\n\n";

			Cone cone(&dados, &matriz);
		}
	
		cout << "\nCrescimento concluido.\n\n";

	}

	Saida saida; //Objeto sa�da

	if ( dados.switchSDF ) //Determina se haver� Sa�da de dados final
	{

		//
		//C�lculo do Volume M�dio de Gr�o
		//

		double vg, desvvg;

		vector<int> volumegrao;

		matriz.calcVolGrao(&dados, &vg, &desvvg, &volumegrao);

		cout << "Volume de Grao: " << vg << "\t";

		cout << "Desvio Padrao: " << desvvg << "\n\n";

		//
		//Escreve o Arquivo de Sa�da
		//

		cout << "Escrevendo arquivo de saida...\n";

		//Saida saida;
	
		saida.saidaMicro(&dados, &matriz, dados.outputFile); //Sa�da para visualiza��o da Microestrutura
	
		saida.medidas(&matriz); //Sa�da com as medidas de estereologia quantitativa

		//Remove o primeiro elemento de volume de gr�o
		volumegrao.erase(volumegrao.begin());
	
		saida.escreveVetor(&volumegrao, "VG.dat"); //Sa�da com os volumes dos gr�os

		saida.escreveNucleos(&dados, &matriz);

	}

	///////

	if ( dados.switchVert ) //Determina se ser�o feitos os c�lculos de posi��o dos v�rtices
	{
		//Quantidade de faces
		vector<int> faces;
		double Sv, Vv;
		faces.resize(matriz.numNucleos(), 0);
		matriz.calculoSv( &Vv, &Sv, &faces);
		saida.escreveVetor(&faces, "Faces.dat"); //Sa�da com as faces


		cout << "Escrita concluida\n\n";

		cout << "As operacoes foram realizadas com sucesso.\n\n"; 

	

		/////////////////////////////////////////////////////////

		Microestrutura micro( &matriz );
	

		for (int i = 0; i < matriz.numNucleos(); i++) //Exibe n�mero de V�rtices e de Faces
		{
		
			cout << "\n" << i+1 << " vertices: " << (micro.getGrao(i+1)).numeroVertices() << "\tfaces: " << (micro.getGrao(i+1)).numeroFaces();
		}

		saida.escrevePontos( &(micro.listaVertices()), "Vertices.dat" );
		//saida.escreveVertices(&matriz, &micro, "p");

		///////////////////////////////////////////////////////////
	}


	//Paraleliza��o
	
	// Contagem de tempo 
	end = omp_get_wtime();

	tempo = end - start;

	cout << "\n\nTempo de trabalho em segundos " << tempo << " segundos paralelizado\n";

	//Fim do Programa

	system("Pause");

	return 0;
}


