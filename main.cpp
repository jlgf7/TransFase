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

//Paralelização
#include <omp.h>

using namespace std;

void compFortan(Matriz *); //Comparação com Fortran


int main()
{
	//Paralelização
	double omp_get_wtime(void);
	double start;
	double end, tempo;
	start = omp_get_wtime();
	
	//Inicialização para os métodos que necessitam de aleatoriedade
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
	//Realiza a Nucleação
	//

	cout << "Realizando nucleacao...\n\n";

	Nucleacao nucleos(&dados, &matriz);
	//compFortan(&matriz);

	cout << "Nucleacao concluida.\n\n";

	//
	//Saída com as posições dos núcleos
	//

	if ( dados.switchSDPN ) //Determina se haverá Saída de Dados pós-Nucleação
	{
		cout << "Escrevendo arquivo com as posicoes dos nucleos...\n\n";

		Saida saida1;
	
		saida1.saidaMicro(&dados, &matriz, "Inicio"); //Saída para visualização da Microestrutura
	}

	
	//
	//Realiza o Crescimento
	//

	if ( dados.switchCresc ) //Determina se haverá Crescimento
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

	Saida saida; //Objeto saída

	if ( dados.switchSDF ) //Determina se haverá Saída de dados final
	{

		//
		//Cálculo do Volume Médio de Grão
		//

		double vg, desvvg;

		vector<int> volumegrao;

		matriz.calcVolGrao(&dados, &vg, &desvvg, &volumegrao);

		cout << "Volume de Grao: " << vg << "\t";

		cout << "Desvio Padrao: " << desvvg << "\n\n";

		//
		//Escreve o Arquivo de Saída
		//

		cout << "Escrevendo arquivo de saida...\n";

		//Saida saida;
	
		saida.saidaMicro(&dados, &matriz, dados.outputFile); //Saída para visualização da Microestrutura
	
		saida.medidas(&matriz); //Saída com as medidas de estereologia quantitativa

		//Remove o primeiro elemento de volume de grão
		volumegrao.erase(volumegrao.begin());
	
		saida.escreveVetor(&volumegrao, "VG.dat"); //Saída com os volumes dos grãos

		saida.escreveNucleos(&dados, &matriz);

	}

	///////

	if ( dados.switchVert ) //Determina se serão feitos os cálculos de posição dos vértices
	{
		//Quantidade de faces
		vector<int> faces;
		double Sv, Vv;
		faces.resize(matriz.numNucleos(), 0);
		matriz.calculoSv( &Vv, &Sv, &faces);
		saida.escreveVetor(&faces, "Faces.dat"); //Saída com as faces


		cout << "Escrita concluida\n\n";

		cout << "As operacoes foram realizadas com sucesso.\n\n"; 

	

		/////////////////////////////////////////////////////////

		Microestrutura micro( &matriz );
	

		for (int i = 0; i < matriz.numNucleos(); i++) //Exibe número de Vértices e de Faces
		{
		
			cout << "\n" << i+1 << " vertices: " << (micro.getGrao(i+1)).numeroVertices() << "\tfaces: " << (micro.getGrao(i+1)).numeroFaces();
		}

		saida.escrevePontos( &(micro.listaVertices()), "Vertices.dat" );
		//saida.escreveVertices(&matriz, &micro, "p");

		///////////////////////////////////////////////////////////
	}


	//Paralelização
	
	// Contagem de tempo 
	end = omp_get_wtime();

	tempo = end - start;

	cout << "\n\nTempo de trabalho em segundos " << tempo << " segundos paralelizado\n";

	//Fim do Programa

	system("Pause");

	return 0;
}


