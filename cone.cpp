#include "cone.h"


Cone::Cone(void)
{
}

Cone::Cone(const Dados *dados, Matriz *matriz) 
{
	
	double raio; //Raio do Cone Causal

	bool parar; //Critério de parada do cone

	int passos; //Conta os passos 

	vector<int> sequencia; //Vetor com a sequência de leitura do núcleos a serem analisados

	//Inicialização de variáveis
	raio = 0;
	Nucleos = matriz->numNucleos();

	tamanho = matriz->getTamVetor();
	parar = 0;
	passos = 0;
	sequencia.resize(Nucleos, 0);

	distancia.resize(tamanho, 0);
	posCone.resize(tamanho, 0);

	distNuc(matriz); //Preenche o vetor de distâncias


	//Arquivos de saída gerados durante a execução
	Saida saida;
	double posicao;
	double intervalo;
	posicao = dados->percOut; //Recebe valor do arquivo de Entrada com a percentagem onde deve haver a primeira geração de arquivos
	intervalo = dados->intOut; //Recebe valor do arquivo de Entrada com o intervalo para geração de arquivos

	//Inicialização do vetor sequencia
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
		//Paralelização
		#pragma omp parallel for
		for (int i = 0; i < tamanho; i++)
		{
			

			if (matriz->getState0(i) == 0) //Se a célula ainda não for transformada
			{
				if (distancia[i] <= raio) //Efetua a Transformação
				{
					matriz->setState0(i, matriz->getState0( matriz->getPosNucleo(posCone[i]))); //Analisar, talvez dê para simplificar
				}

				
			}
		}

		//Crescimento do raio
		raio++;

		//Realiza a contagem de passos e tempo
		passos++;
		matriz->passTime();

		//SAÍDA DE DADOS

		//Atualiza as listas de Vv e Sv
		matriz->pushBackListaV(matriz->calculoVv());

		double Svnr, Svr;
	
		vector<int> Faces;

		Faces.resize(Nucleos,0);

		matriz->calculoSv(&Svnr, &Svr, &Faces);

		matriz->pushBackListaSvnr(Svnr);

		matriz->pushBackListaSvr(Svr);

		double Na; //Curvatura

		Na = matriz->mediaNa(); //Uso do Svnr para o cálculo de Na

		matriz->pushBackListaNa(Na); //Acrescenta o valor à lista

		//SAÍDA DE DADOS

		//Exibe Vv e Sv
		cout << "Tempo: " << matriz->getTime() << "\tVV\t" << matriz->getListaV(matriz->getTime())*100.0 
			<< "\tSVnr\t" << matriz->getListaSvnr(matriz->getTime()) << "\n";
		//O passo do Crescimento corresponde ao tempo = passos + 1
		
		
		//Saída de arquivo para microestrutura
		if (matriz->getListaV(matriz->getTime()) > posicao/100.0)
		{
			int nomePos = matriz->getListaV(matriz->getTime())*100; //Converte o valor da fração Transformada para um inteiro entre 0 e 100
			saida.saidaMicro(dados, matriz ,"etapa " + to_string( nomePos )); //############################
			posicao += intervalo;
		}

	
		//Decide parada
		if (dados->criterio == 1) //Fração transformada
		{
			//Multiplica por 100 pois o parâmetro está em porcentagem
			if (( matriz->getListaV(matriz->getTime()))*100.0 >= dados->parametro)
			{
				parar = true;
			}

		}
		if (dados->criterio == 2) //Número de passos
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


//Métodos

void Cone::distNuc(Matriz *matriz) //Determina a menor distância entre um ponto e o núcleos
{
	
	vector<double> nAux; //Vetor auxiliar para armazenamnto temporário das distâncias entre o ponto e os núcleos
	nAux.resize(Nucleos, 0);
	
	
	
	for (int i = 0; i < tamanho; i++)
	{
		//Paralelização
		#pragma omp parallel for
		for (int j = 0; j < Nucleos; j++)
		{
			nAux[j] = calcDistancia(i, matriz->getPosNucleo(j), matriz->getLinhas(), matriz->getColunas(), matriz->getCotas());
		}
		
		posCone[i] = nucleomaisproximo(&nAux); //Determinação do Núcleo mais próximo
		
		distancia[i] = nAux[ posCone[i] ]; //Distância até o núcleo mais próximo
		
	}

	
}

//////////////////////////////////////////////////////


int Cone::nucleomaisproximo( vector<double> *vetor) //Determina qual o núcleo mais próximo
{
	double menordistancia; //armazena temporariamente a menor distância
	
	vector<int> cone;
	int t;	//tamanho do vetor cone

	menordistancia = (*vetor)[0]; //primeira posição
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

	if (t == 1) //Se não houver valores repetidos
	{
		return cone[0];
	}
	else
	{
		//Embaralha resultado para ter aleatoriedade em caso de distâncias iguais a mais de um núcleo
		
		geraSeqAleat(&cone);

		return cone[0]; //Escolhe o primeiro valor embaralhado
		cout << "!";
	
	}

		
}



////////////////////////////////////


double Cone::calcDistancia(int ponto, int nucleo, int lin, int col, int cot) //Distância entre ponto e núcleo
{
	int x1, y1, z1, x2, y2, z2;

	int i, j, k;

	//Converão de unidades
	x1 = ponto % col; //resto da divisão da posição do vetor pelo número de colunas

	x2 = nucleo % col;

	y1 = ((int) ponto / col) % lin; //resto da divisão da "divisão inteira da posição do vetor pelo número de colunas" pelo número de linhas

	y2 = ((int) nucleo / col) % lin;

	z1 = (int) ponto / (col*lin); //Divisão inteira da posição do vetor pelo produto de linhas e colunas

	z2 = (int) nucleo / (col*lin);

	//Parâmetros i, j, k para contemplação das condições de contorno periódicas

	i = condInterno(x1, x2, (col)/2)*condSinal(x1, x2); //Parâmetro para o eixo x

	j = condInterno(y1, y2, (lin)/2)*condSinal(y1, y2); //Parâmetro para o eixo y

	k = condInterno(z1, z2, (cot)/2)*condSinal(z1, z2); //Parâmetro para o eixo z

	return sqrt(pow(x1 - x2 + i*col, 2.0) + pow(y1 - y2 + j*lin, 2.0) + pow(z1 - z2 + k*cot, 2.0)); //Cálculo da distância entre dois pontos

}


///////////////////////////////////////////////////////////////////////

////Condições para determinação da fórmula da distância considerando as condições de contorno periódicas

int condInterno(int ponto, int nucleo, double raio) //Retorna 0 se for usada a distância convencional e 1 para as demais
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

int condSinal(int ponto, int nucleo) //Retorna 1 ou -1 segundo o critério
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


//Nucleação durante o Crescimento

void Cone::novaNuc(const Dados *dados, Matriz *matriz)
{
	Nucleacao nuc(dados, matriz);

	//Como calcular raios diferentes para cada novo núcleo???

}
