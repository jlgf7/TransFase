#include "saida.h"

//Construtores

Saida::Saida()
{
	
}



//Destrutor
Saida::~Saida()
{
}

//Métodos

void Saida::saidaMicro(const Dados *dados, Matriz *matriz, string nomeSaida)
{
	//Método que determina qual tipo de saída será usada para visualização da microestrutura 
	
	if (dados->outputType == "VTK") //Saída para Paraview
	{
		saidaVTK(dados, matriz, nomeSaida);
	}
	if (dados->outputType == "DAT") //Saída para Tecplot
	{
		saidaTecplot(dados, matriz, nomeSaida);
	}
}


void Saida::saidaTecplot(const Dados *dados, Matriz *matriz, string nomeSaida) 
{
	//Escreve arquivo ".dat" para Tecplot

	//Variáveis para o loop
	int linhas;
	int colunas;
	int cotas;
	int pos;

	//Extensão para Tecplot
	string ext;
	ext = ".dat";

	linhas = dados->ny; //Número de linhas
	colunas = dados->nx; //Número de colunas
	cotas = dados->nz; //Número de cotas
	pos = 0; //Posição da matriz de estado

	//Arquivo de saída

	//Recebe o nome do arquivo de saída
	nomeSaida = nomeSaida + ext;
	const char * arqSaida = nomeSaida.c_str();  //c_str() retorna um ponteiro para um array de caracteres

	//Cria o arquivo
	ofstream arquivo;
	arquivo.open (arqSaida);
	
	//Escreve o cabeçalho para o tecplot
	arquivo << "TITLE='Grid'\n"															//Título
			<< "VARIABLES='X','Y','Z','STATE'\n"										//Variáveis
			<< "ZONE\n"																	
			<< " T=\"\", I=" << colunas << " J=" << linhas << " K=" << cotas << " F=POINT\n";	//Dimensões da malha X, Y, Z

	for (int k = 0; k < cotas; k++)
	{
		for (int j = 0; j < linhas; j++)
		{
			for (int i = 0; i < colunas; i++)
			{
				//Escreve o estado de cada posição (x, y, z)
				arquivo << i << "\t" << j << "\t" << k << "\t" << matriz->getState0(pos) << "\n";

				pos++; //vai para próxima posição
			}
		}
	}

	//Fecha o arquivo
	arquivo.close();
}

void Saida::saidaVTK(const Dados *dados, Matriz *matriz, string nomeSaida)
{
	//Escreve arquivo ".vtk" para Paraview

	//Variáveis para o loop
	int linhas;
	int colunas;
	int cotas;

	//Tamanho do vetor de posição
	int tamanho;
	tamanho = matriz->getTamVetor();

	//contador
	int c = 0;

	//Extensão para VTK
	string ext;
	ext = ".vtk";

	linhas = dados->ny; //Número de linhas
	colunas = dados->nx; //Número de colunas
	cotas = dados->nz; //Número de cotas

	//Arquivo de saída

	//Recebe o nome do arquivo de saída
	nomeSaida = nomeSaida + ext;
	const char * arqSaida = nomeSaida.c_str();  //c_str() retorna um ponteiro para um array de caracteres

	//Cria o arquivo
	ofstream arquivo;
	arquivo.open (arqSaida);

	//Cabeçalho VTK
	arquivo << "# vtk DataFile Version 3.0\n"
			<< "vtk output\n"
			<< "ASCII\n"
			<< "DATASET STRUCTURED_GRID\n"
			<< "DIMENSIONS " << colunas << " " << linhas << " " << cotas << "\n"
			<< "POINTS " << tamanho << " int\n";

	for (int k = 0; k < cotas; k++)
	{
		for (int j = 0; j < linhas; j++)
		{
			for (int i = 0; i < colunas; i++)
			{
				//Escreve coordenadas (x, y, z)
				arquivo << i << "\t" << j << "\t" << k << "\t";

				//Pula linha após escrever 9 dígitos
				if (c % 3 == 2) arquivo << "\n";
				c++;
			}
		}
	}

	arquivo << "\nPOINT_DATA " << tamanho << "\n"
			<< "FIELD Estado 1\n"
			<< "State 1 " << tamanho << " int\n";

	for (int i = 0; i < tamanho; i++)
	{
		//Escreve estados da matriz
		arquivo << matriz->getState0(i) << "\t";

		//Pula linha após escrever 9 dígitos
		if (i % 9 == 8) arquivo << "\n";
	}

}


void Saida::medidas(Matriz *matriz) //Saída com os valores das medições de estereologia quantitativa realizadas no programa
{
	//Arquivo de saída

	//Recebe o nome dos arquivos de saída
	const char * arqSaida1 = "Vv.dat";  
	
	const char * arqSaida2 = "Svnr.dat";  

	const char * arqSaida3 = "Svr.dat"; 

	const char * arqSaida4 = "Na.dat";

	//Cria o arquivo
	ofstream arquivo1;
	arquivo1.open (arqSaida1);

	for (int i = 0; i < matriz->tamListaV(); i++)
	{
		arquivo1 << i << "\t" << matriz->getListaV(i) << "\n"; 
	}

	arquivo1.close();

	//Cria o arquivo
	ofstream arquivo2;
	arquivo2.open (arqSaida2);

	for (int i = 0; i < matriz->tamListaS(); i++)
	{
		arquivo2 << i << "\t" << matriz->getListaSvnr(i) << "\n"; 
	}

	arquivo2.close();

	//Cria o arquivo
	ofstream arquivo3;
	arquivo3.open (arqSaida3);

	for (int i = 0; i < matriz->tamListaS(); i++)
	{
		arquivo3 << i << "\t" << matriz->getListaSvr(i) << "\n"; 
	}

	arquivo3.close();

	//Cria o arquivo
	ofstream arquivo4;
	arquivo1.open (arqSaida4);

	for (int i = 0; i < matriz->tamListaS(); i++)
	{
		arquivo1 << i << "\t" << matriz->getListaNa(i) << "\n"; 
	}

	arquivo4.close();
}

void Saida::escreveVetor(vector<int> *vetor, string nome)
{
	//Arquivo de saída

	//Recebe o nome do arquivo de saída
	const char * arqSaida = nome.c_str();  //c_str() retorna um ponteiro para um array de caracteres

	//Cria o arquivo
	ofstream arquivo;
	arquivo.open (arqSaida);

	//Escreve o arquivo
	for (int i = 0; i < vetor->size(); i++)
	{
		arquivo << (*vetor)[i] << "\n";
	}

	//Fecha o arquivo
	arquivo.close();

}

void Saida::escrevePontos(vector<Ponto> *vetor, string nome)
{
	//Arquivo de saída

	
	//Recebe o nome do arquivo de saída
	const char * arqSaida = nome.c_str();  //c_str() retorna um ponteiro para um array de caracteres

	//Cria o arquivo
	ofstream arquivo;
	arquivo.open (arqSaida);

	

	//Escreve o arquivo
	for (int i = 0; i < vetor->size(); i++)
	{
		
		arquivo << ((*vetor)[i]).x << "\t" << ((*vetor)[i]).y << "\t" << ((*vetor)[i]).z << "\n";
	}

	//Fecha o arquivo
	arquivo.close();

}


void Saida::escreveNucleos(const Dados *dados, Matriz *matriz)
{
	//Variáveis
	int x, y, z;
	
	//Cria o arquivo
	ofstream arquivo;
	arquivo.open("Nucleos.dat");

	//Início da escrita
	arquivo << dados->nNucleus << endl;

	arquivo << dados->nx << "\t" << dados->ny << "\t" << dados->nz << endl;

	for (int i = 0; i < dados->nNucleus; i++)
	{
		//Conversão
		x = matriz->getPosNucleo(i) % dados->nx; 

		y = ((int) matriz->getPosNucleo(i) / dados->nx) % dados->ny;

		z = (int) matriz->getPosNucleo(i) / (dados->nx * dados->ny);

		//Escrita
		arquivo << x << "\t" << y << "\t" << z << endl;
	}

	//Fecha o arquivo
	arquivo.close();
}

//void Saida::escreveVertices(Matriz *matriz, Microestrutura *micro,  string nomeArquivo)
//{
//	int tamanho;
//
//	tamanho = matriz->numNucleos(); //Número de grãos
//
//	for (int i = 0; i < tamanho; i++)
//	{
//		for (int j = 0; j < (micro->getGrao(i+1)).numeroVertices(); j++)
//		{
//			cout << "\nGrao " << j << ":\n";
//			(micro->getGrao(i+1)).getVertice(j).exibe();
//			
//		}
//		
//	}
//}


