#include "dados.h"
#include <string>


//Construtor Dados()
Dados::Dados()											
{
	//Inicializa vari�veis
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
	//Vari�veis para leitura
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
	fcin >> nx >> ny >> nz;							//Atribui valores �s vari�veis
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Tipo de Malha
	fcin >> mesh;									//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Tipo de Nuclea��o
	fcin >> nucleationType;							//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Tipo de Nuclea��o durante o Crescimento
	fcin >> postNucleation;							//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Nome do Arquivo de Entrada
	fcin >> inputFileName;							//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//M�todo de Crescimento	
	fcin >> growthMethod;							//Atribui valores �s vari�veis
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Tipo de Vizinhanca	
	fcin >> neighborsType;							//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//N�mero de n�cleos iniciais 
	fcin >> nNucleus;								//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Tipo de crescimento do Cone Causal
	fcin >> growthType;								//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Crit�rio de parada	
	fcin >> criterio;								//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Par�metro de parada
	fcin >> parametro;								//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Tipo de arquivo de sa�da	
	fcin >> outputType;								//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Nome do arquivo de sa�da
	fcin >> outputFile;								//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Porcentagem inicial para gera��o de arquivos de sa�da
	fcin >> percOut;								//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Intervalo percentual de gera��o de arquivos de sa�da
	fcin >> intOut;									//Atribui valor � vari�vel
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Determina se haver� Sa�da de Dados p�s-Nuclea��o
	fcin >> switchSDPN;
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Determina se haver� Crescimento
	fcin >> switchCresc;							
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Determina se haver� Sa�da de dados final
	fcin >> switchSDF;
	fcin.getline(buff,Max);							//Pula para pr�xima linha do arquivo de texto

	//Determina se ser�o feitos os c�lculos de posi��o dos v�rtices
	fcin >> switchVert;

	//####Arquivo .DAT lido####

	fcin.out; //Fecha o arquivo
}

