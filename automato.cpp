#include "automato.h"

//Construtores

Automato::Automato()
{
}

Automato::Automato(const Dados *dados, Matriz *matriz)
{


	//Arquivos de sa�da gerados durante a execu��o
	Saida saida;
	double posicao;
	double intervalo;
	posicao = dados->percOut; //Recebe valor do arquivo de Entrada com a percentagem onde deve haver a primeira gera��o de arquivos
	intervalo = dados->intOut; //Recebe valor do arquivo de Entrada com o intervalo para gera��o de arquivos
	

	
	//Recebe o tamanho do vetor de estado	
	int tamVetor;
	tamVetor = matriz->getTamVetor();
	
	//Vetor para sequencia aleat�ria de leitura dos 6 vizinhos
	vector<int> sequencia;
	
	//Inicializa��o do vetor de vizinhos
	for (int i = 0; i < 6; i++)
	{
		sequencia.push_back(i);
	}
	
	//
	//AUTOMATO CELULAR
	//

	//Crit�rio para parada do Aut�mato
	bool parar = false;

	//N�mero de passos
	int passos;
	passos = 0; //Sendo que passos = 1  ocorre no Tempo = 2 (Considerando que toda Nuclea��o ocorre em tempo = 1). 

	//Iguala State1 a State0
	igualaEstados(matriz, 1);

	
	//Se��o principal
	while (! parar)
	{
		for (int i = 0; i < tamVetor; i++)
		{
			if (matriz->getState0(i) != 0) //verifica se a c�lula n�o foi transformada e a pula caso tenha sido
			{
				continue;
			}

			geraSeqAleat(&sequencia); //Gera sequ�ncia aleat�ria para consultar vizinhos (fun��o herdada de Crescimento)

			

			for (int j = 0; j < 6; j++) //Analisa vizinhos
			{
				//Se o valor da posi��o analisada � diferente de zero ocorre transforma��o
				if (matriz->getState0(matriz->getVizinho(i,sequencia[j])) != 0) 
				{
					//A poi��o atual recebe o valor da posi��o que j� foi transformada
					matriz->setState1(i, matriz->getState0(matriz->getVizinho(i,sequencia[j]))); 
					
					break; //pula para pr�xima posi��o 
				}

			}
		}

		//Iguala State0 a State1
		igualaEstados(matriz, 0);

		//Incrementa contador de passos e tempo
		passos++;
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
			saida.saidaMicro(dados, matriz ,"etapa " + to_string( nomePos ));
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

		
	}
	//Fim da se��o principal

	//
	//FIM DO AUTOMATO CELULAR
	//


}

//Destrutor

Automato::~Automato()
{
}

//Fun��es

//Iguala estados: state0 = state1 ou state1 = state0;
void Automato::igualaEstados(Matriz *matriz, int a)
{
	if (a == 0)
	{
		for (int i = 0; i < matriz->getTamVetor(); i++)
		{
			matriz->setState0(i, matriz->getState1(i));
		}
	}
	else
	{
		for (int i = 0; i < matriz->getTamVetor(); i++)
		{
			matriz->setState1(i, matriz->getState0(i));
		}
	}

}

