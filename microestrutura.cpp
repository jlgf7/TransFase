#include "microestrutura.h"

//Construtores

Microestrutura::Microestrutura(void)
{
}

Microestrutura::Microestrutura(Matriz *matriz)
{
	numGraos = matriz->numNucleos(); //Número de grãos da matriz

	
	for (int i = 0; i < numGraos; i++)
	{
		listaDeGraos.push_back( Grao(i + 1, matriz) ); //Cria o grão de acordo com o identificador i+1
	}
	//@@@@@@
	for (int i = 0; i < numGraos; i++) //Determina os vértices de cada grão
	{
		encontraIntersecoes(listaDeGraos[i], matriz); //Encontra os Pontos de Interseção dos Planos da Face do Grão
		//@encontraIntersecoes(listaDeGraos[0], matriz);

		filtraPontos(&pontosI); //Exclui os pontos repetidos e aqueles que não estão no interior do poliedro

		for (int j = 0; j < pontosI.size(); j++)
		{
			listaDeGraos[i].setVertice(pontosI[j]); //Insere os vértices ao seu grão
			//@@@listaDeGraos[0].setVertice(pontosI[j]);

			vertices.push_back(pontosI[j]); //Armazena todos os vértices
		}
		
	}

	cout << "Tamanho " << vertices.size();
	
	limpaRepetidos(&vertices); //Remove repetidos

	cout << "Tamanho " << vertices.size();
}

//Destrutores

Microestrutura::~Microestrutura()
{
}

//Métodos

Grao Microestrutura::getGrao(int id)
{
	return listaDeGraos[id - 1];
}


void Microestrutura::encontraIntersecoes(Grao grao, Matriz *matriz)
{
	
	int nVizinhos;

	bool graoEstaNoInterior, vizinhosEstaoNoInterior; //posição dos grão e seus vizinhos

	vector<Ponto> vizAjust; //Armazena os núcleos dos vizinhos com ajuste de condição de contorno periódica

	vector<Ponto> normal, inter; //Vector contendo os pontos normais e as inteseções

	nVizinhos = grao.numeroFaces(); //número de vizinhos
	
	graoEstaNoInterior = 1; //Assume que o grão está no interior

	vizinhosEstaoNoInterior = 1; //Assume que seus vizinhos estão no interior

	////
	////Inicialização do vetor de armazenamento de pontos e de planos
	////
	pontosI.clear(); //Esvazia o vetor
	planoFace.clear(); //Esvazia o vetor

	/////////////////////////////////////////
	/*cout << "\nNucleo do grao!\n";
	grao.nucleo.exibe();

	cout << "\n______________\n";
	for (int i = 0; i < nVizinhos; i++)
	{
		cout << grao.getVizinhos(i) << "\n";
	}

	cout << "\n______________\n";*/

	//Conversão dos pontos tendo em vista a condição de contorno periódica
	for (int i = 0; i < nVizinhos; i++)
	{
		vizAjust.push_back( convertePonto( grao.nucleo, getGrao( grao.getVizinhos(i) ).nucleo , matriz ) );
		/////////////
		/*cout << "\n|vizinho";
		getGrao( grao.getVizinhos(i) ).nucleo.exibe();
		cout << "\n|vizinho ajustado"; 
		vizAjust[i].exibe();*/
		//////////////////////
	}

		
	for (int i = 0; i < nVizinhos; i++)
	{
		//Define os vetores normais
		//centro do vizinho - centro do grão
		normal.push_back( soma( vizAjust[i], produto(-1, grao.nucleo) ) );

		//Define os vetores da interseção (por enquanto é válido apenas para crescimento com velocidades iguais)
		//Ponto médio: 0.5 * (nucleo_vizinho + nucleo_grao)
		inter.push_back( produto( 0.5, soma( vizAjust[i] , grao.nucleo ) ) );

		//Define os planos
		planoFace.push_back( Plano( inter[i], normal[i] ) );
	}

	//Teste//////////////////////////////////////////////////////////////////////////////
		/*for (int i = 0; i < planoFace.size(); i++)
		{
			planoFace[i].exibe();
			inter[i].exibe();
		}
		cout << "------------------------";
		*/
	//Fim Teste////////////////////////////////////////////////////////////////////////////////

	//Interseções de 3 em 3
	for (int i = 0; i < nVizinhos; i++)
	{
		for (int j = i + 1; j < nVizinhos; j++)
		{
			for (int k = j + 1; k < nVizinhos; k++)
			{
				if ( intersecaoUnica( planoFace[i], planoFace[j], planoFace[k] ) ) //Se houver só uma interseção
				{
					pontosI.push_back( intersecao( planoFace[i], planoFace[j], planoFace[k] ) ); //Calcula a interseção e insere no vetor pontoI
					
				}
			}
		}
	}
	
	////////////////////
	/*for (int i = 0; i < pontosI.size(); i++)
	{
		pontosI[i].exibe();
	}*/

	///////////////////
}

void Microestrutura::exibe() //Teste, pode ser apagado
{
	for (int i = 0; i < pontosI.size(); i++)
	{
		pontosI[i].exibe();
	}
}

//Converte o Ponto para seu equivalente de fora da Matriz
Ponto Microestrutura::convertePonto(Ponto nucleoG, Ponto nucleoV, Matriz *matriz)
{
	int xG, yG, zG, xV, yV, zV; //Coordenadas do Grão e do Vizinho

	int i, j, k; //Auxiliares

	int col, lin, cot; //Dimensões da Matriz

	int nx, ny, nz;

	col = matriz->getColunas();
	lin = matriz->getLinhas();
	cot = matriz->getCotas();
	
	

	//Separação dos valores
	xG = nucleoG.x; 

	xV = nucleoV.x;

	yG =nucleoG.y;

	yV = nucleoV.y;

	zG = nucleoG.z;

	zV = nucleoV.z;

	//Parâmetros i, j, k para contemplação das condições de contorno periódicas

	i = condInterno(xG, xV, (col)/2)*condSinal(xG, xV); //Parâmetro para o eixo x

	j = condInterno(yG, yV, (lin)/2 )*condSinal(yG, yV); //Parâmetro para o eixo y

	k = condInterno(zG, zV, (cot)/2 )*condSinal(zG, zV); //Parâmetro para o eixo z


	//Conversão

	nx = xV - i*col;

	ny = yV - j*lin;

	nz = zV - k*cot;

	Ponto N(nx, ny, nz);
	
	return N;
}

///////////////////////////////////////////////////////////////////////////////////////////

void Microestrutura::filtraPontos(vector<Ponto> *pontos) //, vector<Plano> *planos)
{

	
	//Eliminar pontos repetidos

	limpaRepetidos(pontos);

	//Eliminar pontos de fora do poliedro, mantendo apenas os vértices

	for (int i = 0; i < pontosI.size(); i++) //Para cada ponto
	{
		for (int j = 0; j < planoFace.size(); j++) //Para cada plano na face
		{
			if ( produtoInterno(pontosI[i], planoFace[j].normal()) > planoFace[j].d + 1) //!!Testa inequações dos planos (+1 fator para contemplar erros de precisão na conta)
			{
				pontosI.erase(pontosI.begin() + i);
				i--;
				break;
			}
		}
	}


}

void Microestrutura::limpaRepetidos(vector<Ponto> *pontos) //Eliminar pontos repetidos
{
	for (int i = 0; i < pontos->size(); i++)
	{
		for (int j = i + 1; j < pontos->size(); j++)
		{
			if (((*pontos)[i].x == (*pontos)[j].x ) && ((*pontos)[i].y == (*pontos)[j].y ) && ((*pontos)[i].z == (*pontos)[j].z )) //Se encontrar ponto repetido
			{
				pontos->erase(pontos->begin() + j); //Apaga a posição j + 1 do vetor
				j--;
			}
		}
	}
}

vector<Ponto> Microestrutura::listaVertices()
{
	return vertices;
}
