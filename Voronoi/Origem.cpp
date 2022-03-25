// Determinação de posição de Arestas e Vértices de Poliedros de Voronoi
// Autor: Jonathas Luis Groetares Ferreira - jonathasferreira@id.uff.br
// Conclusão: 30/05/2018

// Exemplo base:
// http://math.lbl.gov/voro++/examples/random_points/

// Como montar o projeto no Visual Studio:
// https://stackoverflow.com/questions/25219395/can-voro-be-compiled-in-visual-studio

// Como desabilitar alertas de erro
// Projeto>Propriedades>C/C++>Avançado>Desabilita avisos específicos> 4996;%(DisableSpecificWarnings)

//Visualizar no GNUPLOT
// splot "nucleos.gnu" u 2:3:4, "arestas.gnu" with lines

// !!! O arquivo de Leitura dos Núcleos (Nucleos.dat) deve estar na pasta "entrada" no diretório da Solução !!!
// !!! A saida será na pasta "saida" no diretório da Solução !!!

// Bibliotecas
#include <iostream>
#include <vector>
#include <fstream>	//arquivo
#include <iomanip>	//arquivo
#include <cstdlib>	//arquivo
#include <string>
#include <iterator>	//Separar string
#include <sstream>
#include <algorithm> //unique
#include <windows.h> //CreateDirectory
#include <math.h>



// Biblioteca Voro++
#include "src/voro++.hh"
using namespace voro;

// Namespace
using namespace std;

// Número de blocos no qual o container é dividido
const int n_x = 6, n_y = 6, n_z = 6;

//Função

//Separa uma string com espaços
vector<string> split(const string& str, char delim = ' ') 
{
	stringstream ss(str);
	string tok;
	vector<string> vec;
	while (getline(ss, tok, delim)) {
		if (!tok.empty())
			vec.push_back(tok);
	}
	return vec;
}

//Calcula a distância entre dois pontos
double distancia(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2), 0.5);
}


int main()
{
	//
	//Variáveis
	//
	
	int N; //Número de Núcleos
	int linhas, colunas, cotas; //Dimensão da Malha

	vector < vector <int> > nucleos; //Armazena as posições dos Núcleos
	vector <int> coord; //Auxiliar para criar o vector nucleos
	coord.resize(3, 0); //Inicialização com 3 posições: x y z

	//Dimensões do container
	double x_min, x_max;
	double y_min, y_max;
	double z_min, z_max;

	vector < vector <double> > arestas; //Armazena as posições das Arestas
	vector < double > auxAresta; //Auxiliar para criar o vector arestas
	auxAresta.resize(6, 0); //Inicialização

	vector < vector <double> > vertices; //Armazena as posições dos Vértices
	vector < double > auxVertices; //Auxiliar para criar o vector vértices
	auxVertices.resize(3, 0); //Inicialização 

	//
	//Leitura do Número de Núcleos e Dimensões da Matriz
	//

	//Variáveis para leitura
	const int Max = 100;
	char buff[Max];

	//Abre o arquivo de núcleos
	//!!! O arquivo de Leitura dos Núcleos deve estar na pasta "entrada" no diretório da Solução !!!
	ifstream nuc("entrada/Nucleos.dat");
	//Trata erros
	if (!nuc)
	{
		cerr << "The File Nucleos.dat can't be open!" << endl;
		getchar();
		exit(1);
	}

	nuc >> N; //Lê o valor de N
	nuc.getline(buff, Max);	//Pula para próxima linha do arquivo de texto

	nuc >> colunas >> linhas >> cotas; //Lê as dimensões da Malha

	nucleos.resize(N, coord); //Prepara o vector para receber as posições dos núcleos

	for (int i = 0; i < N; i++)
	{
		nuc.getline(buff, Max);	//Pula para próxima linha do arquivo de texto

		nuc >> nucleos[i][0] >> nucleos[i][1] >> nucleos[i][2]; //Lê as coordendas dos núcleos
	}

	nuc.out; //Fecha o arquivo de núcleos 

	//
	//Definição das Arestas
	//

	//Inicialização de variáveis
	x_min = y_min = z_min = 0;
	x_max = colunas - 1;
	y_max = linhas - 1;
	z_max = cotas - 1;

	//Cria um container com a geometria especificada
	//Condições de contorno periódicas nos três eixos (true, true, true)
	//8 partículas em cada bloco para inicialização (número arbitrário)
	container con(x_min, x_max, y_min, y_max, z_min, z_max, n_x, n_y, n_z,
		true, true, true, 8);

	//Adiciona Núcleos (partículas) ao container
	for (int i = 0; i < N; i++)
	{
		con.put(i, nucleos[i][0], nucleos[i][1], nucleos[i][2]);
	}

	//Cria diretório saida
	CreateDirectory("saida", NULL);

	//Saída núcleos em formato do Gnuplot
	con.draw_particles("saida/nucleos.gnu");

	// Saída das arestas em formato do Gnuplot
	con.draw_cells_gnuplot("saida/arestas.gnu");

	//
	//Interpretação das arestas
	//

	//As arestas são escritas de 2 a 2 em uma linha de 6 posições x1 y1 z1 x2 y2 z2
	
	//Abre o arquivo
	ifstream arqAresta("saida/arestas.gnu");
	//Trata erros
	if (!arqAresta)
	{
		cerr << "The File arestas.gnu can't be open!" << endl;
		getchar();
		exit(1);
	}

	//Variáveis para leitura
	string data;
	std::string::size_type sz;

	//Lê primeira linha
	getline(arqAresta, data);

	//Contadores
	int j = 0;
	int k = 1;

	//Vetor aux
	vector <string> aux(10, "");

	//Auxiliares
	double x, y, z;
	x = y = z = 0;

	while (!arqAresta.fail())
	{

		if (data != "")
		{
			//Separa a linha em 3 strings
			for (string tok : split(data)) {
				aux[j] = tok;
				j++;
			}
			j = 0;

			//Converte string para double
			x = stod(aux[0], &sz);
			y = stod(aux[1], &sz);
			z = stod(aux[2], &sz);

			//Vértices
			auxVertices[0] = x;
			auxVertices[1] = y;
			auxVertices[2] = z;

			vertices.push_back(auxVertices);

			//Determina a posição dos vértices nas arestas
			if (k == 1)
			{
				//Inicializa a próxima aresta
				auxAresta[0] = x;
				auxAresta[1] = y;
				auxAresta[2] = z;
				k++;
			}
			else
			{
				auxAresta[3] = x;
				auxAresta[4] = y;
				auxAresta[5] = z;

				//Completa a aresta
				arestas.push_back(auxAresta);

				//Inicializa a próxima aresta
				auxAresta[0] = x;
				auxAresta[1] = y;
				auxAresta[2] = z;
			}
		}
		else
		{
			k = 1;
		}

		getline(arqAresta, data);
	}

	//Fecha o arquivo arestas.gnu
	arqAresta.out;

	//
	//Determinar posições na malha que são arestas
	//

	//Parâmetro t
	double t = 0;
	double dist = 0; //Tamanho da aresta

	//Pontos pertencentes às arestas
	vector < vector <int> > pontosArestas;

	vector < int > auxP(3, 0); //Vetor Auxiliar

	for (int i = 0; i < arestas.size(); i++)
	{
		dist = distancia(arestas[i][0], arestas[i][1], arestas[i][2], arestas[i][3], arestas[i][4], arestas[i][5]);

		while (t <= 1)
		{
			auxP[0] = round(arestas[i][0] - t * (arestas[i][0] - arestas[i][3])); //x
			auxP[1] = round(arestas[i][1] - t * (arestas[i][1] - arestas[i][4])); //y
			auxP[2] = round(arestas[i][2] - t * (arestas[i][2] - arestas[i][5])); //z

			pontosArestas.push_back(auxP);

			t += 1.0 / dist;
		}

		t = 0; //Preparação para a próxima aresta
	}

	//Eliminar pontos repetidos
	//https://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector

	sort(pontosArestas.begin(), pontosArestas.end()); //Ordena o Vetor
	pontosArestas.erase(unique(pontosArestas.begin(), pontosArestas.end()), pontosArestas.end()); //Elimina Repetidos

	//
	//Escreve os pontos da aresta
	//

	//Cria o arquivo
	ofstream arqPontosArestas;
	arqPontosArestas.open("saida/PontosArestas.dat");

	//Escreve o Número de Pontos nas Arestas
	arqPontosArestas << pontosArestas.size() << endl;

	for (int i = 0; i < pontosArestas.size(); i++)
	{
		//Escreve as coordenadas
		arqPontosArestas << pontosArestas[i][0] << "\t" << pontosArestas[i][1] << "\t" << pontosArestas[i][2] << endl;
	}

	arqPontosArestas.close();

	//
	//Escreve Arquivo de Vértices
	//

	sort(vertices.begin(), vertices.end()); //Ordena o Vetor
	vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end()); //Elimina Repetidos

	//Cria o arquivo
	ofstream arqPontosVertices;
	arqPontosVertices.open("saida/PontosVertices.dat");

	//Escreve o número de Vértices
	arqPontosVertices << vertices.size() << endl;

	for (int i = 0; i < vertices.size(); i++)
	{
		arqPontosVertices << round(vertices[i][0]) << "\t" << round(vertices[i][1]) << "\t" << round(vertices[i][2]) << endl;
	}

	arqPontosVertices.close();

	//
	//Estatatísticas
	//

	//Id Vértices Arestas Faces
	con.print_custom("%i %w %g %s", "saida/Id_Vertices_Arestas_Faces.dat");

	system("pause");
	return 0;
}