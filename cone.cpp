#include "cone.h"


Cone::Cone(void)
{
}

Cone::Cone(const Dados *dados, Matriz *matriz) 
{
	
	double raio; //Raio do Cone Causal

	bool parar; //Crit�rio de parada do cone

	int passos; //Conta os passos 

	vector<int> sequencia; //Vetor com a sequ�ncia de leitura do n�cleos a serem analisados

	//Inicializa��o de vari�veis
	raio = 0;
	Nucleos = matriz->numNucleos();

	tamanho = matriz->getTamVetor();
	parar = 0;
	passos = 0;
	sequencia.resize(Nucleos, 0);

	distancia.resize(tamanho, 0);
	posCone.resize(tamanho, 0);

	distNuc(matriz); //Preenche o vetor de dist�ncias


	//Arquivos de sa�da gerados durante a execu��o
	Saida saida;
	double posicao;
	double intervalo;
	posicao = dados->percOut; //Recebe valor do arquivo de Entrada com a percentagem onde deve haver a primeira gera��o de arquivos
	intervalo = dados->intOut; //Recebe valor do arquivo de Entrada com o intervalo para gera��o de arquivos

	//Inicializa��o do vetor sequencia
	for (int i = 0; i < Nucleos; i++)
	{
		sequencia[i] = i;
	}

	//
	//Cone Causal
	//

	//Incremento do Raio
	raio++;

	while (! parar)
	{
		//Paraleliza��o
		#pragma omp parallel for
		for (int i = 0; i < tamanho; i++)
		{
			

			if (matriz->getState0(i) == 0) //Se a c�lula ainda n�o for transformada
			{
				if (distancia[i] <= raio) //Efetua a Transforma��o
				{
					matriz->setState0(i, matriz->getState0( matriz->getPosNucleo(posCone[i]))); //Analisar, talvez d� para simplificar
				}

				
			}
		}

		//Crescimento do raio
		raio++;

		//Realiza a contagem de passos e tempo
		passos++;
		matriz->passTime();

		//SA�DA DE DADOS

		//Atualiza as listas de Vv e Sv
		matriz->pushBackListaV(matriz->calculoVv());

		double Svnr, Svr;
	
		vector<int> Faces;

		Faces.resize(Nucleos,0);

		matriz->calculoSv(&Svnr, &Svr, &Faces);

		matriz->pushBackListaSvnr(Svnr);

		matriz->pushBackListaSvr(Svr);

		double Na; //Curvatura

		Na = matriz->mediaNa(); //Uso do Svnr para o c�lculo de Na

		matriz->pushBackListaNa(Na); //Acrescenta o valor � lista

		//SA�DA DE DADOS

		//Exibe Vv e Sv
		cout << "Tempo: " << matriz->getTime() << "\tVV\t" << matriz->getListaV(matriz->getTime())*100.0 
			<< "\tSVnr\t" << matriz->getListaSvnr(matriz->getTime()) << "\n";
		//O passo do Crescimento corresponde ao tempo = passos + 1
		
		
		//Sa�da de arquivo para microestrutura
		if (matriz->getListaV(matriz->getTime()) > posicao/100.0)
		{
			int nomePos = matriz->getListaV(matriz->getTime())*100; //Converte o valor da fra��o Transformada para um inteiro entre 0 e 100
			saida.saidaMicro(dados, matriz ,"etapa " + to_string( nomePos )); //############################
			posicao += intervalo;
		}

	
		//Decide parada
		if (dados->criterio == 1) //Fra��o transformada
		{
			//Multiplica por 100 pois o par�metro est� em porcentagem
			if (( matriz->getListaV(matriz->getTime()))*100.0 >= dados->parametro)
			{
				parar = true;
			}

		}
		if (dados->criterio == 2) //N�mero de passos
		{
			if (passos >= dados->parametro)
			{
				parar = true;
			}

		}

	} //!Fim do While



}


Cone::~Cone(void)
{
	
}


//M�todos

void Cone::distNuc(Matriz *matriz) //Determina a menor dist�ncia entre um ponto e o n�cleos
{
	
	vector<double> nAux; //Vetor auxiliar para armazenamnto tempor�rio das dist�ncias entre o ponto e os n�cleos
	nAux.resize(Nucleos, 0);
	
	
	
	for (int i = 0; i < tamanho; i++)
	{
		//Paraleliza��o
		#pragma omp parallel for
		for (int j = 0; j < Nucleos; j++)
		{
			nAux[j] = calcDistancia(i, matriz->getPosNucleo(j), matriz->getLinhas(), matriz->getColunas(), matriz->getCotas());
		}
		
		posCone[i] = nucleomaisproximo(&nAux); //Determina��o do N�cleo mais pr�ximo
		
		distancia[i] = nAux[ posCone[i] ]; //Dist�ncia at� o n�cleo mais pr�ximo
		
	}

	
}

//////////////////////////////////////////////////////


int Cone::nucleomaisproximo( vector<double> *vetor) //Determina qual o n�cleo mais pr�ximo
{
	double menordistancia; //armazena temporariamente a menor dist�ncia
	
	vector<int> cone;
	int t;	//tamanho do vetor cone

	menordistancia = (*vetor)[0]; //primeira posi��o
	cone.push_back(0);
	t = 1;

	int N;

	N = (*vetor).size();

	for (int j = 1; j < N; j++)
	{
		if ((*vetor)[j] < menordistancia) //Se houver um valor menor, substitua
		{
			menordistancia = (*vetor)[j];
			t = 1;
			while(cone.size() > 0)
			{
				cone.pop_back();
			}
			
			cone.push_back(j);
		}
		else if ((*vetor)[j] == menordistancia) //Se houver valor valor igual, acrescente ao vetor cone
		{
			cone.push_back(j);
			t++;
		}

	}

	if (t == 1) //Se n�o houver valores repetidos
	{
		return cone[0];
	}
	else
	{
		//Embaralha resultado para ter aleatoriedade em caso de dist�ncias iguais a mais de um n�cleo
		
		geraSeqAleat(&cone);

		return cone[0]; //Escolhe o primeiro valor embaralhado
		cout << "!";
	
	}

		
}



////////////////////////////////////


double Cone::calcDistancia(int ponto, int nucleo, int lin, int col, int cot) //Dist�ncia entre ponto e n�cleo
{
	int x1, y1, z1, x2, y2, z2;

	int i, j, k;

	//Conver�o de unidades
	x1 = ponto % col; //resto da divis�o da posi��o do vetor pelo n�mero de colunas

	x2 = nucleo % col;

	y1 = ((int) ponto / col) % lin; //resto da divis�o da "divis�o inteira da posi��o do vetor pelo n�mero de colunas" pelo n�mero de linhas

	y2 = ((int) nucleo / col) % lin;

	z1 = (int) ponto / (col*lin); //Divis�o inteira da posi��o do vetor pelo produto de linhas e colunas

	z2 = (int) nucleo / (col*lin);

	//Par�metros i, j, k para contempla��o das condi��es de contorno peri�dicas

	i = condInterno(x1, x2, (col)/2)*condSinal(x1, x2); //Par�metro para o eixo x

	j = condInterno(y1, y2, (lin)/2)*condSinal(y1, y2); //Par�metro para o eixo y

	k = condInterno(z1, z2, (cot)/2)*condSinal(z1, z2); //Par�metro para o eixo z

	return sqrt(pow(x1 - x2 + i*col, 2.0) + pow(y1 - y2 + j*lin, 2.0) + pow(z1 - z2 + k*cot, 2.0)); //C�lculo da dist�ncia entre dois pontos

}


///////////////////////////////////////////////////////////////////////

////Condi��es para determina��o da f�rmula da dist�ncia considerando as condi��es de contorno peri�dicas

int condInterno(int ponto, int nucleo, double raio) //Retorna 0 se for usada a dist�ncia convencional e 1 para as demais
{
	if ((abs(nucleo - ponto) - raio) <= 0) //verificar igual
	{
		return 0;
	}
	else
	{
		return 1;
	}

}

//////////////////////////////////////////////////////////////////////////

int condSinal(int ponto, int nucleo) //Retorna 1 ou -1 segundo o crit�rio
{
	if ((nucleo - ponto) >= 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}

}

/////////////////////////////////////////////////////////////////////////


//Nuclea��o durante o Crescimento

void Cone::novaNuc(const Dados *dados, Matriz *matriz)
{
	Nucleacao nuc(dados, matriz);

	//Como calcular raios diferentes para cada novo n�cleo???

}
