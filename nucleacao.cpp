#include "nucleacao.h"
#include <iostream>

using namespace std;

//Construtores
Nucleacao::Nucleacao()
{
}

Nucleacao::Nucleacao(const Dados *dados, Matriz *matriz)
{
	//Define o tipo de malha conforme o escrito na entreda de dados
	if (dados->nucleationType == "Uniforme")
	{
		uniforme(dados, matriz);
	} 
	else if (dados->nucleationType == "Importar")
	{
		importar(dados, matriz);
	}
	else if (dados->nucleationType == "Periodica")
	{
		periodica(dados, matriz);
	}



}

//Destrutor
Nucleacao::~Nucleacao()
{
}

//Métodos
void Nucleacao::uniforme(const Dados *dados, Matriz *matriz)
{
	int pos=0;
	int tamanho_do_vetor;
	int i = 1;

	tamanho_do_vetor = matriz->getTamVetor();	//recebe o tamanho do vetor de estado

	//Geração de números aleatórios, distribuição uniforme. http://www.cplusplus.com/reference/random/uniform_int_distribution/
	mt19937 generator(time(NULL));
	//default_random_engine generator;
	uniform_int_distribution<int> distribution(0, tamanho_do_vetor - 1);
	
	//Faz o sorteio dos núcleos
	while (i <= matriz->numNucleos())
	{
		// Atribui valores aleatórios definindo a posição entre 0 e tamanho_do_vetor - 1
		pos = distribution(generator);
		
		pos = (int) pow(pos,1); //!!!!!!!!!!!!!!!!GAMBIARRA
		
		//testa se a posição escolhida já havia sido escolhida
		if (matriz->getState0(pos) == 0)
		{
			matriz->setState0(pos, i);	//Atribui o núcleo "i" à posição "pos"

			matriz->setPosNucleo(pos); //Salva o endereço do Núcleo

			i++;
		}
	}


	///Teste


	//Fim do teste

	//Passa o Tempo
	matriz->passTime();

	//Atualiza as listas de Vv e Sv
	matriz->pushBackListaV(matriz->calculoVv());

	double Svnr, Svr;
	
	vector<int> Faces;

	Faces.resize(matriz->numNucleos(),0);

	matriz->calculoSv(&Svnr, &Svr, &Faces);

	matriz->pushBackListaSvnr(Svnr);

	matriz->pushBackListaSvr(Svr);

	double Na; //Curvatura

	Na = matriz->mediaNa(); //Uso do Svnr para o cálculo de Na

	matriz->pushBackListaNa(Na); //Acrescenta o valor à lista
	
	//Exibe Vv e Sv
	//Toda Nucleação ocorre em tempo = 1
	
	cout << "Tempo: " << matriz->getTime() << "\tVV\t" << matriz->getListaV(1)*100.0 << "\tSVnr\t" << matriz->getListaSvnr(1) << "\n\n";
	
}

void Nucleacao::importar(const Dados *dados, Matriz *matriz)
{
	int tamanho;
	tamanho = matriz->getTamVetor(); //Tamanho de Matriz

	int c;
	c = 0; //contador

	int lixo, estado; //Variáveis para leitura do arquivo
	
	//Variáveis para leitura
	const int Max = 100;
	char buff[Max];
	
	//Abre o arquivo
	ifstream fcin(dados->inputFileName); //Nome do arquivo de entrada
	//Trata erros
	if (! fcin)
	{
		cerr << "The File can't be open!" << endl;
		exit(1);
	}

	//Testa o tipo de arquivo
	string formato;

	formato = dados->inputFileName;

	formato.erase(0, formato.length() - 3); //extrai todas menos as três últimas letras do nome
	
	//Matriz de Núcleos

	int **nucleos = new int*[matriz->numNucleos()]; 

	for (int i = 0; i < matriz->numNucleos(); i++)
	{
		nucleos[i] = new int[2];	//Armazena o Núcleo na primeira coluna e sua posição na segunda coluna
	}

	//Leitura

	//Se o formato for "dat"
	if (formato == "dat")
	{

		for (int i = 0; i < 3; i++) fcin.getline(buff, Max); 	//Pula 3 primeiras linhas do arquivo de texto

		//Escreve a matriz com base no arquivo
		for (int i = 0; i < tamanho; i++)
		{
			fcin.getline(buff, Max);	//Pula para próxima linha do arquivo de texto
			fcin >> lixo >> lixo >> lixo >> estado; //Lê o arquivo

			if (estado != 0)
			{
				matriz->setState0(i, estado); //Posição i e Estado estado
				nucleos[c][0] = estado; //estado
				nucleos[c][1] = i; //posição
				c++;
			}
		}

	}
	else if (formato == "vtk") //Se o formato vor "vtk"
	{
		for (int i = 0; i < 6 + tamanho/3 + 3; i++) fcin.getline(buff, Max); 	
		//6: Cabeçalho inicial | tamanho/3: 3 pares de coordenadas | 2: Informações intermediárias
				
		//Escreve a matriz com base no arquivo
		for (int i = 0; i < tamanho ; i++)
		{
			if (i % 9 == 0) //A cada 9 valores pula uma linha
			{
				fcin.getline(buff, Max);	//Pula para próxima linha do arquivo de texto
			}

			fcin >> estado; //Lê o arquivo

			if (estado != 0)
			{
				matriz->setState0(i, estado); //Posição i e Estado estado
				nucleos[c][0] = estado; //estado
				nucleos[c][1] = i; //posição
				c++;
			}
		}
	}
	else
	{
		cout << "\n! Formato " << formato << " incompativel para importacao\n";
		getchar();
		exit(1);
	}

	c = 0; //Zera o contador

	while (c < matriz->numNucleos()) //Enquanto a posição de todos os núcleos não for salva
	{
		for (int i = 0; i < matriz->numNucleos(); i++)
		{
			if (nucleos[i][0] == c + 1) //Se o estado for o mesmo da ordem de leitura
			{
				matriz->setPosNucleo( nucleos[i][1] ); //Salva a posição
				c++; //Incrementa o contador
				break; //Para o for
			}

		}
	}

	//Deleta a matriz
	for (int i = 0; i < matriz->numNucleos(); i++)
	{
		delete[] nucleos[i];	
	}

	delete[] nucleos;

	//Passa o Tempo
	matriz->passTime();

	//Atualiza as listas de Vv e Sv
	matriz->pushBackListaV(matriz->calculoVv());

	double Svnr, Svr;
	
	vector<int> Faces;

	Faces.resize(matriz->numNucleos(),0);

	matriz->calculoSv(&Svnr, &Svr, &Faces);

	matriz->pushBackListaSvnr(Svnr);

	matriz->pushBackListaSvr(Svr);

	double Na; //Curvatura

	Na = matriz->mediaNa(); //Uso do Svnr para o cálculo de Na

	matriz->pushBackListaNa(Na); //Acrescenta o valor à lista
	
	//Exibe Vv e Sv
	//Toda Nucleação ocorre em tempo = 1
	cout << "Tempo: " << matriz->getTime() << "\tVV\t" << matriz->getListaV(1)*100.0 << "\tSVnr\t" << matriz->getListaSvnr(1) << "\n\n";
}

void Nucleacao::periodica(const Dados *dados, Matriz *matriz)
{
	//Nucleação Periódica

	//Condições:
	//N : Número de Núcleos da Matriz deve ser um cubo perfeito
	//A matriz deve ser cúbica : linhas = colunas = cotas
	//O lado da matriz deve ser um múltiplo da raiz cúbica de N
	//O ideal é que cotas/raiz³(N) seja ímpar para que a vizualização seja mais simétrica

	int N;
	int linhas;
	int colunas;
	int cotas;
	int nd; //Número de núcleos em cada direção
	int dNuc; //Distância entre os núcleos
	int dI; //Distância inicial
	int px, py, pz; //Variáveis auxiliares para determinação das posições
	int iN; //Índice dos Núcleos

	N = dados->nNucleus;

	linhas = dados->ny;
	colunas = dados->nx;
	cotas = dados->nz; 

	nd = arredondar( pow( N , (1.0/3.0) ) ); //Raiz cúbica
	

	//Verificar se as condições foram atendidas

	//Cubo da parte inteira da raiz³ | Linhas, colunas ou cotas diferentes entre si | lado deve ser um múltiplo da raiz³ de N (% é o resto)
	
	if ( ( pow( nd, 3) != N ) || ( ( linhas != colunas ) || ( linhas != cotas ) || ( colunas != cotas ) )  || ( cotas % nd != 0) )
	{
		cout << "\n! O numero de nucleos ou o tamanho da matriz nao sao compativeis com este tipo de nucleacao\n";
		getchar();
		exit(0);
	}
	
		
	///Determinação das posições dos núcleos

	vector<vector<int>> posNuc(N, vector<int>(3)); //Posição dos núcleos: Matriz com N linhas e 3 colunas (para x, y, z)

	dNuc = cotas / nd; //Distância entre os núcleos

	dI = dNuc / 2; //Se dNuc for ímpar, o resultado é arrendondado para baixo

	//dI--; //Decrementa uma unidade devido à matriz começar em 0

	//Cálculo das Posições
	iN = 1; //Índice inicial
	 	

	for (int i = 0; i < nd; i++)
	{
		px = dI + dNuc * i; //Valor de X

		for (int j = 0; j < nd; j++)
		{
			py = dI + dNuc * j; //Valor de Y

			for (int k = 0; k < nd; k++)
			{
				pz = dI + dNuc * k; //Valor de X

				matriz->setState0( convVet(px, py, pz, dados), iN ); //Salva o valor de iN na posição convertida

				matriz->setPosNucleo( convVet(px, py, pz, dados) ); //Salva o endereço do Núcleo
				
				iN++; 
			}
		}
	}
	


	//Fim do teste

	//Passa o Tempo
	matriz->passTime();

	//Atualiza as listas de Vv e Sv
	matriz->pushBackListaV(matriz->calculoVv());

	double Svnr, Svr;

	vector<int> Faces;

	Faces.resize(matriz->numNucleos(), 0);

	matriz->calculoSv(&Svnr, &Svr, &Faces);

	matriz->pushBackListaSvnr(Svnr);

	matriz->pushBackListaSvr(Svr);

	double Na; //Curvatura

	Na = matriz->mediaNa(); //Uso do Svnr para o cálculo de Na

	matriz->pushBackListaNa(Na); //Acrescenta o valor à lista

								 //Exibe Vv e Sv
								 //Toda Nucleação ocorre em tempo = 1

	
}

//Função para arredondar
int Nucleacao::arredondar(double d)
{
	return static_cast<int> (d + 0.5);
}

//Converte para Coordendas vetoriais
int Nucleacao::convVet(int x, int y, int z, const Dados *dados)
{
	return x + ( y * dados->nx ) + ( z * dados->nx * dados->ny );
}
