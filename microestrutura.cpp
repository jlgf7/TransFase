#include "microestrutura.h"

//Construtores

Microestrutura::Microestrutura(void)
{
}

Microestrutura::Microestrutura(Matriz *matriz)
{
	numGraos = matriz->numNucleos(); //N�mero de gr�os da matriz

	
	for (int i = 0; i < numGraos; i++)
	{
		listaDeGraos.push_back( Grao(i + 1, matriz) ); //Cria o gr�o de acordo com o identificador i+1
	}
	//@@@@@@
	for (int i = 0; i < numGraos; i++) //Determina os v�rtices de cada gr�o
	{
		encontraIntersecoes(listaDeGraos[i], matriz); //Encontra os Pontos de Interse��o dos Planos da Face do Gr�o
		//@encontraIntersecoes(listaDeGraos[0], matriz);

		filtraPontos(&pontosI); //Exclui os pontos repetidos e aqueles que n�o est�o no interior do poliedro

		for (int j = 0; j < pontosI.size(); j++)
		{
			listaDeGraos[i].setVertice(pontosI[j]); //Insere os v�rtices ao seu gr�o
			//@@@listaDeGraos[0].setVertice(pontosI[j]);

			vertices.push_back(pontosI[j]); //Armazena todos os v�rtices
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

//M�todos

Grao Microestrutura::getGrao(int id)
{
	return listaDeGraos[id - 1];
}


void Microestrutura::encontraIntersecoes(Grao grao, Matriz *matriz)
{
	
	int nVizinhos;

	bool graoEstaNoInterior, vizinhosEstaoNoInterior; //posi��o dos gr�o e seus vizinhos

	vector<Ponto> vizAjust; //Armazena os n�cleos dos vizinhos com ajuste de condi��o de contorno peri�dica

	vector<Ponto> normal, inter; //Vector contendo os pontos normais e as intese��es

	nVizinhos = grao.numeroFaces(); //n�mero de vizinhos
	
	graoEstaNoInterior = 1; //Assume que o gr�o est� no interior

	vizinhosEstaoNoInterior = 1; //Assume que seus vizinhos est�o no interior

	////
	////Inicializa��o do vetor de armazenamento de pontos e de planos
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

	//Convers�o dos pontos tendo em vista a condi��o de contorno peri�dica
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
		//centro do vizinho - centro do gr�o
		normal.push_back( soma( vizAjust[i], produto(-1, grao.nucleo) ) );

		//Define os vetores da interse��o (por enquanto � v�lido apenas para crescimento com velocidades iguais)
		//Ponto m�dio: 0.5 * (nucleo_vizinho + nucleo_grao)
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

	//Interse��es de 3 em 3
	for (int i = 0; i < nVizinhos; i++)
	{
		for (int j = i + 1; j < nVizinhos; j++)
		{
			for (int k = j + 1; k < nVizinhos; k++)
			{
				if ( intersecaoUnica( planoFace[i], planoFace[j], planoFace[k] ) ) //Se houver s� uma interse��o
				{
					pontosI.push_back( intersecao( planoFace[i], planoFace[j], planoFace[k] ) ); //Calcula a interse��o e insere no vetor pontoI
					
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
	int xG, yG, zG, xV, yV, zV; //Coordenadas do Gr�o e do Vizinho

	int i, j, k; //Auxiliares

	int col, lin, cot; //Dimens�es da Matriz

	int nx, ny, nz;

	col = matriz->getColunas();
	lin = matriz->getLinhas();
	cot = matriz->getCotas();
	
	

	//Separa��o dos valores
	xG = nucleoG.x; 

	xV = nucleoV.x;

	yG =nucleoG.y;

	yV = nucleoV.y;

	zG = nucleoG.z;

	zV = nucleoV.z;

	//Par�metros i, j, k para contempla��o das condi��es de contorno peri�dicas

	i = condInterno(xG, xV, (col)/2)*condSinal(xG, xV); //Par�metro para o eixo x

	j = condInterno(yG, yV, (lin)/2 )*condSinal(yG, yV); //Par�metro para o eixo y

	k = condInterno(zG, zV, (cot)/2 )*condSinal(zG, zV); //Par�metro para o eixo z


	//Convers�o

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

	//Eliminar pontos de fora do poliedro, mantendo apenas os v�rtices

	for (int i = 0; i < pontosI.size(); i++) //Para cada ponto
	{
		for (int j = 0; j < planoFace.size(); j++) //Para cada plano na face
		{
			if ( produtoInterno(pontosI[i], planoFace[j].normal()) > planoFace[j].d + 1) //!!Testa inequa��es dos planos (+1 fator para contemplar erros de precis�o na conta)
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
				pontos->erase(pontos->begin() + j); //Apaga a posi��o j + 1 do vetor
				j--;
			}
		}
	}
}

vector<Ponto> Microestrutura::listaVertices()
{
	return vertices;
}
