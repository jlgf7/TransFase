#include "matriz.h"

//Construtores
Matriz::Matriz()
{
}

Matriz::Matriz(const Dados *dados)
{
	linhas = dados->ny; //Número de linhas
	colunas = dados->nx; //Número de colunas
	cotas = dados->nz; //Número de cotas

	tamVetor = linhas*colunas*cotas; //Tamanho do Vetor

	Nucleos = dados->nNucleus; //Número de Núcleos Iniciais

	//Define o tipo de malha conforme o escrito na entreda de dados
	if (dados->mesh == "Malha_Quadrangular")
	{
		setMalhaQuad(dados); //???Por que não &dados (ver main.cpp): aqui ele é um endereço, lá não
	}

	metodoCrescimento = dados->growthMethod;

	//Se Autômato Celular
	
	if (metodoCrescimento == "Automato")
	{
		//Inicializa a matriz de vizinhos se o método escolhido for o Autômato Celular

		vizinhos = new int*[tamVetor]; 

		for (int i = 0; i < tamVetor; i++)
		{
			vizinhos[i] = new int[6];	//6 vizinhos para cada posição
		}

		defineVizinhos(); //Preenche a matriz de vizinhos

		defineDir(); //Define ordens auxiliares y e z

	}

	//Se Cone Causal

	if (metodoCrescimento == "Cone")
	{
		//Inicialização da matriz de distâncias

		//distancias = new double*[tamVetor]; //Número de posições na matriz
		//
		//for (int i = 0; i < tamVetor; i++)
		//{
		//
		//	distancias[i] = new double[Nucleos]; //Distância entre cada posicção e núcleo
		//		
		//}

		//distNuc(); //Preenche a matriz de Distâncias

		

		defineDir2(); //Define ordens auxiliares y e z pelo método sem uso da matriz de vizinhos
	}


	//Inicializa as listas de Vv e Sv

	pushBackListaV(0);
	pushBackListaSvnr(0);
	pushBackListaSvr(0);
	pushBackListaNa(0); 

	//Inicializa Tempo
	tempo = 0;

	//Inicializa matriz de faces compartilhadas entre os grãos

	facesComp = new bool*[Nucleos];

	for (int i = 0; i < Nucleos; i++)
	{
		facesComp[i] = new bool[Nucleos];
	}
	
	//Paralelização
	#pragma omp parallel for
	for (int i = 0; i < Nucleos; i++)
	{
		for (int j = 0; j < Nucleos; j++)
		{
			facesComp[i][j] = 0; //zera a matriz
		}
	}
	
}

//Destrutor
Matriz::~Matriz()
{
	if (metodoCrescimento == "Automato")
	{
		//Deleta a matriz de vizinhos
		for (int i = 0; i < tamVetor; i++)
		{
			delete[] vizinhos[i];	
		}

		delete[] vizinhos;
	}

	if (metodoCrescimento == "Cone")
	{
		//Deleta a matriz de distâncias
		/*for (int i = 0; i < tamVetor; i++)
		{
			delete[] distancias[i];	
		}

		delete[] distancias;*/
	}

	//Deleta a matriz de faces compartilhadas entre os grãos
		for (int i = 0; i < Nucleos; i++)
		{
			delete[] facesComp[i];	
		}

		delete[] facesComp;


}

//Métodos

void Matriz::setState0(int pos, int sta)	//Atribui um valor à deteminada posição em state0
{
	//??? incluir critério para não acessar posição errada
	state0[pos] = sta;
}

void Matriz::setState1(int pos, int sta)	//Atribui um valor à deteminada posição em state1
{
	//??? incluir critério para não acessar posição errada
	state1[pos] = sta;
}

int Matriz::getState0(int pos)	//Retorna o valor de deteminada posição em state0
{
	return state0[pos];
}

int Matriz::getState1(int pos)	//Retorna o valor de deteminada posição em state1
{
	return state1[pos];
}

void Matriz::setMalhaQuad(const Dados *dados)	//Constrói uma malha quadrangular
{
	int tamanho_do_vetor = getTamVetor();
	
	//Incializa e zera os vetores
	//redimensionamento do vetor para o tamanho (linhas*colunas*cotas) seguida de atribuição de valor Zero(0) para cada ponto
	//posX.resize(tamanho_do_vetor,0); 
	//posY.resize(tamanho_do_vetor,0);
	//posZ.resize(tamanho_do_vetor,0);
	state0.resize(tamanho_do_vetor,0);
	state1.resize(tamanho_do_vetor,0);
	dirY.resize(tamanho_do_vetor,0);
	dirZ.resize(tamanho_do_vetor,0);

	//Atribuir as posições da malha aos vetores posição (dx, dy, dz)
}

int Matriz::getTamVetor()	//Retorna o tamanho do vetor de estado
{
	return tamVetor;
}

//Posição dos Núcleos

void Matriz::setPosNucleo(int endNucleo) //Insere o endereço do núcleo na matriz na próxima posição disponível no vetor
{
	enderecoNucleos.push_back(endNucleo); 
}

int Matriz::getPosNucleo(int pos) //Retorna o endereço do núcleo da matriz armazenado no vetor de endereços
{
	//Pos: de 0 a NumeroDeNucleos - 1

	return enderecoNucleos[pos];
}

////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////


double Matriz::calculoVv()	//Retorna a fração volumétrica transformada considerando todas as fases
{
	int tamanho; //tamanho do vetor de estado
	int k; //contador de posições já transformadas

	k = 0; 

	tamanho = this->getTamVetor(); //recebe o tamanho do vetor de estado

	for (int i = 0; i < tamanho; i++)
	{
		if (this->getState0(i) != 0)
		{
			k++;
		}

	}

	return ((k*1.0)/(tamanho*1.0)); //cálculo da fração (*1.0 para converter para double)
}


//#####################################################################################


void Matriz::calculoSv(double *Svnr, double *Svr, vector<int> *Faces) //Retorna a fração de superfície transformada por volume total considerando todas as fases
{
	//Svnr: Interface recristalizada com não recristalizada
	//Svr: Interfaces recristalizadas
	//faces: Número de Faces de cada grão


	int tamanho; //tamanho do vetor de estado
	int n, m; //Contadores de fronteiras
	int soma; //Variável para somatório

	n = 0;
	m = 0;
	soma = 0;

	tamanho = getTamVetor(); //recebe o tamanho do vetor de estado

	//Zera interfaces
	interfaces.resize(Nucleos, 0);

	for (int i = 0; i < tamanho; i++) //Faz a varredura da Malha nas direções X, Y e Z
	{
		//Direção X
		
		if (i % colunas != 0) //Fora da extremidade esquerda
		{
			if (getState0(i) != getState0(i-1)) //Células diferentes
			{
				if ((getState0(i) == 0) || (getState0(i-1) == 0)) //Uma das células igual a zero
				{
					n++; //Incremento para Svnr
				}
				else //Se as duas são diferentes de zero, calcula interfaces
				{
					//Incrementa o número de interfaces para os dois grãos
					setInterface( getState0(i) ) ;
					setInterface( getState0(i - 1) );
					
					m++; //Incremento para Svr

					setFacesComp( getState0(i), getState0(i - 1) ); //Determinação do número de faces do grão
				}

			}
			
		}
		else
		{
			
			//Compara levando em consideração a condição de contorno periódica
			if (getState0(i) != getState0(i + (colunas - 1))) //Células diferentes 
			{

				if ((getState0(i) == 0) || (getState0(i + (colunas - 1)) == 0)) //Uma das células iguais a zero
				{
					n++; //Incremento para Svnr
				}
				else //Se as duas são diferentes de zero, calcula interfaces
				{
					//Incrementa o número de interfaces para os dois grãos
					setInterface( getState0(i) ) ;
					setInterface( getState0(i + (colunas - 1)) );

					m++; //Incremento para Svr

					setFacesComp( getState0(i), getState0(i + (colunas - 1)) ); //Determinação do número de faces do grão
				}
			}
				

		}

		 

		//Direção Y
		 
		if ((getdirY(i) % (linhas*colunas)) >= colunas) //Fora da extremidade superior
		{
			if (getState0(getdirY(i)) != getState0(getdirY(i-1))) //Células diferentes
			{
				if ((getState0(getdirY(i)) == 0) || (getState0(getdirY(i-1)) == 0)) //Uma das células igual a zero
				{
					n++; //Incremento para Svnr
				}
				else //Se as duas são diferentes de zero, calcula interfaces
				{
					//Incrementa o número de interfaces para os dois grãos
					setInterface( getState0( getdirY(i) ) ) ;
					setInterface( getState0( getdirY(i-1) ) );

					m++; //Incremento para Svr

					setFacesComp( getState0( getdirY(i) ), getState0( getdirY(i-1) ) ); //Determinação do número de faces do grão
				}

			}
			
		}
		else //Na extremidade superior
		{
						
			//Compara levando em consideração a condição de contorno periódica 
			if ((getState0(getdirY(i)) != getState0(getdirY(i) + colunas*(linhas - 1)))) //Células diferentes
			{
				if ((getState0(getdirY(i)) == 0) || (getState0(getdirY(i) + colunas*(linhas - 1)) == 0)) //Uma das células igual a zero
				{
					n++; //Incremento para Svnr
				}
				else //Se as duas são diferentes de zero, calcula interfaces
				{
					//Incrementa o número de interfaces para os dois grãos
					setInterface( getState0( getdirY(i) ) ) ;
					setInterface( getState0( getdirY(i) + colunas*(linhas - 1) ) );

					m++; //Incremento para Svr

					setFacesComp( getState0( getdirY(i) ), getState0( getdirY(i) + colunas*(linhas - 1) ) ); //Determinação do número de faces do grão
				}

			}
				
		}


		//Direção Z
		 
		if (getdirZ(i) >= linhas*colunas) //Fora da extremidade frontal
		{
			if (getState0(getdirZ(i)) != getState0(getdirZ(i-1))) //Células diferentes
			{
				if ((getState0(getdirZ(i)) == 0) || (getState0(getdirZ(i-1)) == 0)) //Uma das células igual a zero
				{
					n++; //Incremento para Svnr
				}
				else //Se as duas são diferentes de zero, calcula interfaces
				{
					//Incrementa o número de interfaces para os dois grãos
					setInterface( getState0( getdirZ(i) ) ) ;
					setInterface( getState0( getdirZ(i-1) ) );

					m++; //Incremento para Svr

					setFacesComp( getState0( getdirZ(i) ), getState0( getdirZ(i-1) ) ); //Determinação do número de faces do grão
				}

			}
			
		}
		else //Na extremidade frontal
		{
			if (getState0(getdirZ(i)) != getState0(getdirZ(i) + linhas*colunas*(cotas - 1))) //Células diferentes
			{
				if ((getState0(getdirZ(i)) == 0) || (getState0(getdirZ(i) + linhas*colunas*(cotas - 1)) == 0)) //Uma das células igual a zero
				{
					n++; //Incremento para Svnr
				}
				else //Se as duas são diferentes de zero, calcula interfaces
				{
					//Incrementa o número de interfaces para os dois grãos
					setInterface( getState0( getdirZ(i) ) ) ;
					setInterface( getState0( getdirZ(i) + linhas*colunas*(cotas - 1) ) );

					m++; //Incremento para Svr

					setFacesComp( getState0( getdirZ(i) ),  getState0( getdirZ(i) + linhas*colunas*(cotas - 1) ) ); //Determinação do número de faces do grão
				}
			}
				
		}

	}

	//Svnr

	*Svnr = ((n*1.0)/(tamanho*1.0)); //cálculo da fração (*1.0 para converter para double)

	//Svr
	/*for (int i = 0; i < Nucleos; i++)
	{
		soma += getInterface(i+1);
	}*/

	//*Svr = (double) soma/tamanho;
	*Svr = ((m*1.0)/(tamanho*1.0)); //cálculo da fração (*1.0 para converter para double)

	//Faces
	for (int i = 0; i < Nucleos; i++)
	{
		(*Faces)[i] = 0; //zera o vetor de faces
	}

	////

	for (int i = 0; i < Nucleos; i++)
	{
		for (int j = 0; j < Nucleos; j++)
		{
			if (facesComp[i][j] == 1) //Se houver interfaces entre os dois grãos
			{
				(*Faces)[i]++; //Soma uma face ao grão
			}
		}
	}
	

	//Fim da função
}

//#####################################################################################

//Retorna o Número de Grãos por Unidade de Volume
double Matriz::calculoNv()
{
	int tamanho; //tamanho do vetor de estado
	int k; //contador de grãos 
	vector<int> graos;

	graos.resize(1,0); //Inicialização do vetor
	k = 0;

	tamanho = this->getTamVetor(); //recebe o tamanho do vetor de estado

	for (int i = 0; i < tamanho; i++)
	{
		if (this->getState0(i) != 0) //Se o estado é diferente de 0
		{
			if (k == 0) //Primeiro grão encontrado
			{
				graos[0] = this->getState0(i);
				k++;
			}
			else //Outros grãos
			{
				int j = 0;

				for (j = 0; j < k; j++) //Testa se já foi encontrado
				{
					if (graos[j] == this->getState0(i))
					{
						break;
					}
				}

				if (j == k) //Se passar por todos e não for igual
				{
					graos.push_back(this->getState0(i)); //Adiciona grão à lista
					k++; //Aumenta a contagem
				}
			}


		}

	}

	

	return ((k*1.0)/(tamanho*1.0)); //cálculo da fração (*1.0 para converter para double)
}

//Início; Fim (<=); Direção 1: x, 2: y, 3: z
double Matriz::calculoNa(int inicio, int fim, int direcao)
{
	int tamanho; //tamanho do vetor de estado
	int k; //contador de grãos 
	vector<int> graos;

	graos.resize(1,0); //Inicialização do vetor
	k = 0;

	tamanho = this->getTamVetor(); //recebe o tamanho do vetor de estado

	for (int i = inicio; i <= fim; i++)
	{
		if (direcao == 1) //Direção x
		{
			
			if (this->getState0(i) != 0) //Se o estado é diferente de 0
			{
				if (k == 0) //Primeiro grão encontrado
				{
					graos[0] = this->getState0(i);
					k++;
				}
				else //Outros grãos
				{
					int j = 0;

					for (j = 0; j < k; j++) //Testa se já foi encontrado
					{
						if (graos[j] == this->getState0(i))
						{
							break;
						}
					}

					if (j == k) //Se passar por todos e não for igual
					{
						graos.push_back(this->getState0(i)); //Adiciona grão à lista
						k++; //Aumenta a contagem
					}
				}


			}
		} 
		else if (direcao == 2) //Direção y
		{
			if (this->getState0(getdirY(i)) != 0) //Se o estado é diferente de 0
			{
				if (k == 0) //Primeiro grão encontrado
				{
					graos[0] = this->getState0(getdirY(i));
					k++;
				}
				else //Outros grãos
				{
					int j = 0;

					for (j = 0; j < k; j++) //Testa se já foi encontrado
					{
						if (graos[j] == this->getState0(getdirY(i)))
						{
							break;
						}
					}

					if (j == k) //Se passar por todos e não for igual
					{
						graos.push_back(this->getState0(getdirY(i))); //Adiciona grão à lista
						k++; //Aumenta a contagem
					}
				}


			}
		}
		else if (direcao == 3) //Direção z
		{
			if (this->getState0(getdirZ(i)) != 0) //Se o estado é diferente de 0
			{
				if (k == 0) //Primeiro grão encontrado
				{
					graos[0] = this->getState0(getdirZ(i));
					k++;
				}
				else //Outros grãos
				{
					int j = 0;

					for (j = 0; j < k; j++) //Testa se já foi encontrado
					{
						if (graos[j] == this->getState0(getdirZ(i)))
						{
							break;
						}
					}

					if (j == k) //Se passar por todos e não for igual
					{
						graos.push_back(this->getState0(getdirZ(i))); //Adiciona grão à lista
						k++; //Aumenta a contagem
					}
				}


			}
		}

	}

	

	return ((k * 1.0)/((fim - inicio + 1.0) * 1.0)); //cálculo da fração (*1.0 para converter para double)
}


//#####################################################################################

//Curvaturas

double Matriz::mediaNa() //Curvatura Média dos Grãos
{
	double somaNa; //somar todos os Nv dos planos e dividir por tamanho (média dos Nv)

	double mediaNa; //média dos Nv's dos planos

	somaNa = 0;

	//Nv por plano é equivalente a Na

	//Direção X
	for (int i = 0; i < cotas; i++)
	{
		somaNa += calculoNa( i * linhas * colunas, (i + 1) * linhas * colunas - 1, 1);
	}

	//Direção Y
	for (int i = 0; i < colunas; i++)
	{
		somaNa += calculoNa( i * linhas * cotas, (i + 1) * linhas * cotas - 1, 1);
	}

	//Direção Z
	for (int i = 0; i < linhas; i++)
	{
		somaNa += calculoNa( i * colunas * cotas, (i + 1) * colunas * cotas - 1, 1);
	}
	
	mediaNa = somaNa / (linhas + colunas + cotas);
	
	return mediaNa;	

}




//#####################################################################################

//Funções para os tamanhos das listas

int Matriz::tamListaV() //Retorna o tamanho da lista de Vv
{
	return listaV.size();
}

int Matriz::tamListaS() //Retorna o tamanho da lista de Sv
{
	return listaSvnr.size();
}

//Funções para adiconar valores às listas

void Matriz::pushBackListaV(double V) //Acrescenta um valor de Vv ao vator listaV
{
	
	listaV.push_back(V);

}

void Matriz::pushBackListaSvnr(double S) //Acrescenta um valor de Svnr ao vator listaSvnr
{
	
	listaSvnr.push_back(S);
	
}

void Matriz::pushBackListaSvr(double S) //Acrescenta um valor de Svnr ao vator listaSvr
{
	
	listaSvr.push_back(S);
	
}

//Listas para as Curvaturas
void Matriz::pushBackListaNa(double C) //Acrescenta um valor de Na ao vetor listaNa
{
	listaNa.push_back(C);
}


	

//Funções para retornar valor das listas

double Matriz::getListaV(int pos) //Retorna o valor de uma posição em listaV
{
	return listaV[pos];
}

double Matriz::getListaSvnr(int pos) //Retorna o valor de uma posição em listaSvnr
{
	return listaSvnr[pos];
}

double Matriz::getListaSvr(int pos) //Retorna o valor de uma posição em listaSvr
{
	return listaSvr[pos];
}


double Matriz::getListaNa(int pos) //Retorna o valor de uma posição em listaS
{
	return listaNa[pos];
}


//Funções para dfinição de vizinhança e ordens de leitura

void Matriz::defineVizinhos()
{
	//Esta função cria a matriz com as posições dos vizinhos
	//
	//0: Frente (-z)
	//1: Direita (+x)
	//2: Trás (+z)
	//3: Esquerda (-x)
	//4: Baixo (+y)
	//5: Cima (-y)
	//

	int nCelulas; 

	nCelulas = tamVetor; //número de células (linhas) da matriz de vizinhos

	for (int i = 0; i < nCelulas; i++)
	{
		//Posição 0: Frente
		if (i >= linhas*colunas)	//Se estiver fora do plano frontal
		{
			vizinhos[i][0] = i - linhas*colunas;
		}
		else
		{
			vizinhos[i][0] = i + (linhas*colunas)*(cotas - 1);
		}

		//Posição 1: Direita
		if ((i % colunas) != (colunas - 1))	//Se estiver fora do plano mais a direita
		{
			vizinhos[i][1] = i + 1;
		}
		else
		{
			vizinhos[i][1] = i - (colunas - 1);
		}

		//Posição 2: Trás
		if (i < (nCelulas - linhas*colunas))	//Se estiver fora do plano traseiro
		{
			vizinhos[i][2] = i + linhas*colunas;
		}
		else
		{
			vizinhos[i][2] = i - (linhas*colunas)*(cotas - 1);
		}

		//Posição 3: Esquerda
		if ((i % colunas) != 0)	//Se estiver fora do plano mais a esquerda
		{
			vizinhos[i][3] = i - 1;
		}
		else
		{
			vizinhos[i][3] = i + (colunas - 1);
		}

		//Posição 4: Baixo
		if ((i % (linhas*colunas)) < (linhas*colunas - colunas))	//Se estiver fora do plano inferior
		{
			vizinhos[i][4] = i + colunas;
		}
		else
		{
			vizinhos[i][4] = i - (linhas - 1)*colunas;
		}

		//Posição 5: Cima
		if ((i % (linhas*colunas)) >= colunas)	//Se estiver fora do plano superior
		{
			vizinhos[i][5] = i - colunas;
		}
		else
		{
			vizinhos[i][5] = i + (linhas - 1)*colunas;
		}
	}
}

int Matriz::getVizinho(int pos, int dir)
{
	//Retorna o vizinho na posição pos e na direção dir (0 a 5)

	return vizinhos[pos][dir];
}

void Matriz::defineDir() //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	//Define a ordem de leitura nas direções Y e Z

	//variáveis auxiliares para determinação das posições

	int j; 
	int k;

	j = 0;
	k = 0;

	for (int i = 0; i < tamVetor; i++)
	{
		//Direção Y
		
		dirY[i] = j;

		if ( getVizinho(j, 4) > j ) //embaixo maior que de cima
		{
			j = getVizinho(j, 4);
		}
		else
		{
			if ( getVizinho(j, 4) < getVizinho(getVizinho(j, 4), 2) ) //No topo, lado direito maior que do lado esquerdo
			{
				j = getVizinho(getVizinho(j, 4), 2);
			}
			else //Se não, vai para trás
			{
				j = getVizinho(getVizinho(getVizinho(j, 4), 2), 1);
			}

		}

		//Direção Z

		dirZ[i] = k;

		if ( getVizinho(k, 2) > k )
		{
			k = getVizinho(k, 2);
		}
		else
		{
			if ( getVizinho(k, 2) < getVizinho(getVizinho(k, 2), 1) )
			{
				k = getVizinho(getVizinho(k, 2), 1);
			}
			else
			{
				k = getVizinho(k, 2) + 1;
			}

		}


	}

}


//######################################################################


void Matriz::defineDir2()
{
	//Define a ordem de leitura nas direções Y e Z sem uso da matriz de vizinhos
	
	int c;

	c = 0;

	//Direção Y

	for (int i = 0; i < colunas; i++)
	{
		for (int k = 0; k < cotas; k++)
		{
			for (int j = 0; j < linhas; j++)
			{
				dirY[c] = i + j*colunas + k*linhas*colunas;

				c++;

				
				
			}
		}

	}

	c = 0;
	
	//Direção Z

	for (int j = 0; j < linhas; j++)
	{
		for (int i = 0; i < colunas; i++)
		{
			for (int  k = 0; k < cotas; k++)
			{
				dirZ[c] = i + j*colunas + k*linhas*colunas;

				c++;
				
			}
		}

	}
}

	
//Retornam o valor da sequência de leitura nas direções Y e Z 

int Matriz::getdirY(int pos)
{
	return dirY[pos];
}


int Matriz::getdirZ(int pos)
{
	return dirZ[pos];
}

//Metalografia quantitativa

//Volume de Grão
//Cada posição do vetor volGrao[i] possui a quantidade de vezes que o o grão de número i aparece na matriz. 
//v[0] possui a quantidade de células não transformadas 
void Matriz::calcVolGrao(const Dados *dados, double *VG, double *desvVG, vector<int> *volGrao)
{
	int n;
	int tamanho;
	long long soma;

	tamanho = getTamVetor();

	n = numNucleos(); //Número de núcleos atual

	volGrao->resize(n + 1, 0); //o vetor terá n + 1 posições para também armazenar a quantidade de valores 0 no vetor de estado

	for (int i = 0; i < tamanho; i++)
	{
		(*volGrao)[getState0(i)]++; //
	}
	
	//Média dos Volumes

	soma = 0;


	for (int i = 1; i < n + 1; i++)
	{
		soma += (*volGrao)[i];
	}

	*VG = soma/n;

	//Desvio Padrão

	soma = 0;

	for (int i = 1; i < n + 1; i++)
	{
		soma += pow(((*volGrao)[i] - *VG),2.0);

	}

	*desvVG = sqrt((double) soma/(n-1.0));

}


//####################################################################

//Funções para o controle de Tempo (isto é, passos de transformação)

void Matriz::passTime() //Incrementa o Tempo
{
	tempo++;
}

int Matriz::getTime() //Retorna o Tempo
{
	return tempo;
}

//Funções para retornar as dimensões da matriz

int Matriz::getLinhas()
{
	return linhas;
}

int Matriz::getColunas()
{
	return colunas;
}

int Matriz::getCotas()
{
	return cotas;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void Matriz::setInterface(int grao) //Adiciona mais um valor ao contador de interface de determinado grão
{
	//A posição 0 refere-se ao grão 1 e assim por diante

	interfaces[grao - 1]++;
}

int Matriz::getInterface(int grao) //Adiciona mais um valor ao contador de interface de determinado grão
{
	//A posição 0 refere-se ao grão 1 e assim por diante
	
	return interfaces[grao - 1];
}

/////////////////////////////////////////////////////////////////////////////////////////

void Matriz::setFacesComp(int a, int b) //Adiciona verdadeiro (1) se houver interface entre dois grãos a e b;
{
	//o grão a está armazenado na posição a - 1

	facesComp[a - 1][b - 1] = 1; 

	facesComp[b - 1][a - 1] = 1;
}

bool Matriz::getFacesComp(int a, int b) //Retorna Verdadeiro se houver interface entre os dois grãos
{
	//o grão a está armazenado na posição a - 1

	return facesComp[a - 1][b - 1];
}

//////////////////////////////////////////////////////////////////////////////////////////

//Número de Núcleos
int Matriz::numNucleos()
{
	return Nucleos;
}

//Altera o número de núcleos da matriz
void Matriz::atualNumNucleos(int N)
{
	Nucleos = N;
}



//void Matriz::teste(Microestrutura *micro)
//{
//	int g;
//	g = 2+2;
//}
