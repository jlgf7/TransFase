#include "dados.h"
#include <string>


//Construtor Dados()
Dados::Dados()											
{
	//Inicializa variáveis
	nx = ny = nz = 0;
	mesh = "";
	neighborsType = "";
	nucleationType = "";
	nNucleus = 0;
	outputType = ""; 
	outputFile = ""; 
	percOut = intOut = 0;
}

//Destrutor Dados()
Dados::~Dados()
{
	
}

void Dados::setDados(const char* inputFile)
{	
	//Variáveis para leitura
	const int Max = 100;
	char buff[Max];
	
	//Abre o arquivo
	ifstream fcin(inputFile);
	//Trata erros
	if (! fcin)
	{
		cerr << "The File can't be open!" << endl;
		exit(1);
	}
	
	for(int i=0; i<4; i++) fcin.getline(buff,Max); 	//Pula 4 primeiras linhas do arquivo de texto

	//		nx		ny		nz	
	fcin >> nx >> ny >> nz;							//Atribui valores às variáveis
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Tipo de Malha
	fcin >> mesh;									//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Tipo de Nucleação
	fcin >> nucleationType;							//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Tipo de Nucleação durante o Crescimento
	fcin >> postNucleation;							//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Nome do Arquivo de Entrada
	fcin >> inputFileName;							//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Método de Crescimento	
	fcin >> growthMethod;							//Atribui valores às variáveis
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Tipo de Vizinhanca	
	fcin >> neighborsType;							//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Número de núcleos iniciais 
	fcin >> nNucleus;								//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Tipo de crescimento do Cone Causal
	fcin >> growthType;								//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Critério de parada	
	fcin >> criterio;								//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Parâmetro de parada
	fcin >> parametro;								//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Tipo de arquivo de saída	
	fcin >> outputType;								//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Nome do arquivo de saída
	fcin >> outputFile;								//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Porcentagem inicial para geração de arquivos de saída
	fcin >> percOut;								//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Intervalo percentual de geração de arquivos de saída
	fcin >> intOut;									//Atribui valor à variável
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Determina se haverá Saída de Dados pós-Nucleação
	fcin >> switchSDPN;
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Determina se haverá Crescimento
	fcin >> switchCresc;							
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Determina se haverá Saída de dados final
	fcin >> switchSDF;
	fcin.getline(buff,Max);							//Pula para próxima linha do arquivo de texto

	//Determina se serão feitos os cálculos de posição dos vértices
	fcin >> switchVert;

	//####Arquivo .DAT lido####

	fcin.out; //Fecha o arquivo
}

