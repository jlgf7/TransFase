#include "grao.h"

//Construtores

Grao::Grao() //Construtor Padrão
{
	id = 0;

	nFaces = 0;

	local.resize(3, 0);
}

Grao::Grao(int ID, Matriz *matriz) //Contrutor a partir do identificador (1 a N, onde N é o número de núcleos)
{
	id = ID; //Atribuição do Id

	nFaces = 0; //inicialização

	//Ponto que armazena o núcleo do grão: posição do núcleo - linhas - colunas - cotas
	//id - 1 pois o vetor armazena o núcleos de 0 a N-1
	nucleo.set( matriz->getPosNucleo(id - 1), matriz->getLinhas(), matriz->getColunas(), matriz->getCotas() ); 

	int n;

	n = matriz->numNucleos(); //Número de núcleos

	for (int i = 1; i <= n; i++) //Determinação dos vizinhos
	{
		if (matriz->getFacesComp(id, i) ) //Se houver interface entre os grãos
		{
			nFaces++; //Contagem de faces

			vizinhos.push_back(i); //Armazenamento do Id dos vizinhos
		}
				
	}

	//Definição do local do grão
	local.resize(6, 0);

	local = estaNaFace(matriz);
}

//Destrutor

Grao::~Grao()
{
}

//Métodos

vector<bool> Grao::estaNaFace(Matriz *matriz)
{
	//
	//Retorna 0 ou 1 para cada face se estiver nela
	//
	//Identificação das faces
	//
	//[0]: Frente 
	//[3]: Esquerda
	//[5]: Cima 
	//

	int pos; //Posição do Núcleo

	int linhas, colunas, cotas; //dimensões da matriz

	linhas = matriz->getLinhas();
	colunas = matriz->getColunas();
	cotas = matriz->getCotas();
	
	//id - 1 pois o vetor armazena o núcleos de 0 a N-1
	pos = matriz->getPosNucleo(id - 1);

	if (matriz->getState0(pos - nucleo.z * linhas * colunas ) == id)
	{
		local[0] = 1; //Face da frente
	}

	if (matriz->getState0(pos + (colunas - nucleo.x - 1) ) == id)
	{
		local[1] = 1; //Face da direita
	}

	if (matriz->getState0(pos + (cotas - nucleo.z - 1) * linhas * colunas  ) == id)
	{
		local[2] = 1; //Face de trás
	}

	if (matriz->getState0(pos - nucleo.x ) == id)
	{
		local[3] = 1; //Face da esquerda
	}

	//cout << "!!! " << pos << "\t" << nucleo.y << "\t" << linhas << "\n";

	if (matriz->getState0(pos + (linhas - nucleo.y - 1) * linhas ) == id)
	{
		local[4] = 1; //Face de cima
	}
	
	if (matriz->getState0(pos - nucleo.y * linhas ) == id)
	{
		local[5] = 1; //Face de cima
	}


	return local;

}

//////////////////////////////////////////////////////////////////////////////////////

bool Grao::getLocal(int face) //Retorna verdadeiro ou falso para cada face da matriz
{
	return local[face];
}

//////////////////////////////////////////////////////////////////////////////////////

int Grao::getVizinhos(int i) //Retorna o Id dos vizinhos
{
	return vizinhos[i];
}

//////////////////////////////////////////////////////////////////////////////////////

int Grao::numeroFaces() //Retorna o número de faces
{
	
	return nFaces;
}

//////////////////////////////////////////////////////////////////////////////////////

void Grao::setVertice(Ponto vert) //Insere um vértice ao grão
{
	vertices.push_back(vert);
}

///////////////////////////////////////////////////////////////////////////////////////

int Grao::numeroVertices() //Retorna o número de vértices
{
	return vertices.size();
}

///////////////////////////////////////////////////////////////////////////////////////

Ponto Grao::getVertice(int i) //Retorna um dos vértices 
{
	return vertices[i];
}

