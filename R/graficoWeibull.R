#Função para criação de gráfico da Distribuição de Weibull
#
#
#Autor: Jonathas Luis Groetares Ferreira  24/02/2017
#
#

grafWeibull <- function(enderecoarquivo)
{
  library(EnvStats)
  
  #Preparação do vetor que contém os dados
  dados <- read.table(enderecoarquivo)
  dados <- data.matrix(dados)
  dados <- dados[1:length(dados),]

  #Determinação da abscissa
  x <- strtoi(names(table(dados)))
  
  #Estimação dos parâmetros de Weibull
  estpar <- eweibull(dados, method = "mle")
  
  parametros <- estpar[["parameters"]]
  
  K <- parametros[["shape"]]
  L <- parametros[["scale"]] 

  #Determinação da densidade com uso dos parâmetros estimados
  dw <- dweibull(x, shape = K, scale = L)
  
  #Construção do gráfico
  hist(dados, freq = F)
  lines(x, dw)

  #Determinação da frequencia e densidade
  f <- table(dados)
  d <- f/sum(f)
  
  #Calculo de R²
  rquad = cor(d, dw)^2
  
  return(rquad)
}



arquivo <- "TG.dat"
grafWeibull(arquivo)
