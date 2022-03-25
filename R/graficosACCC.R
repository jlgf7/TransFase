#Script para geração de gráficos de Vv, Sv e Caminho Microestrutural
#Autor: Jonathas Luis Groetares Ferreira  12/12/2016
#
#
#
#Observação: Verificar se os arquivos para leitura estão na pasta do sistema
#
#

#Limpeza do Ambiente
rm(list = ls())

# AUTÔMATO CELULAR

#Leitura da Fração Volumétrica
frac_volAC <- read.table("VvAC.dat")

#Leitura da Svnr e Svr
frac_areNRAC <- read.table("SvnrAC.dat")
frac_areRAC <- read.table("SvrAC.dat")

#Leitura da Na
NaAC <- read.table("NaAC.dat")

#Transformação em Matriz
MVVAC <- data.matrix(frac_volAC)
MSVNRAC <- data.matrix(frac_areNRAC)
MSVRAC <- data.matrix(frac_areRAC)
NaAC <- data.matrix(NaAC) 

#Atribuição dos valores para vetores
tempoAC <- MVVAC[,1]   # O tempo do AC é maior que o do CC
VVAC <- MVVAC[,2]
SVNRAC <- MSVNRAC[,2]
SVRAC <- MSVRAC[,2]
NaAC <- NaAC[,2]

#Velocidade dR/dt
GGAC = 1  #1 célula por unidade de tempo

#cálculo da constante N (Número de núcleos por unidade de volume)
NAC <- VVAC[2]

#Cálculo da Vv
Vv_calcAC <- 1 - exp(-(4/3)*NAC*(GGAC^3)*((tempoAC)^3)) #O tempo do CC é t-1

#Cálculo da Sv
Sv_calcAC <- 12*NAC*(GGAC^3)*((tempoAC)^2)*exp(-(4/3)*NAC*(GGAC^3)*((tempoAC)^3)) #O tempo do CC é t-1

#Cálculo de Caminho Microestrutural
SVVAC <- (3*GGAC*(36*NAC)^(1/3))*(1-Vv_calcAC)*((log(1/(1-Vv_calcAC)))^(2/3))

#Cálculo de velocidade de interface G
tamanhoAC <- length(VVAC) - 1
GAC <- 0
for (i in 1:tamanhoAC )
{
  GAC[i] <- 2*(-VVAC[i] + VVAC[i+1])/(SVNRAC[i]+SVNRAC[i+1])
}
GAC[tamanhoAC+1] <- GAC[tamanhoAC]

#G teórico
GteoAC <- rep(1/3,tamanhoAC+1)

#Cálculo de Hg com Svnr
#Hg1 = 2.0 * pi * Na / SVNR

#Cálculo de Hg com Svr
#Hg2 = 2.0 * pi * Na / SVR

#################################################################
#Remoção de pontos excessivos

intervaloAC <- seq(1,length(tempoAC),by = length(tempoAC)%/%20) #Quinta parte (100/20)
temporedAC <- tempoAC[intervaloAC]
VVredAC <- VVAC[intervaloAC]
SVNRredAC <- SVNRAC[intervaloAC]
intervaloAC[length(intervaloAC)] = length(GAC)-2 #este ponto é mais representativo
GredAC <- GAC[intervaloAC]
SVRredAC <- SVRAC[intervaloAC]

# CONE CAUSAL

#Leitura da Fração Volumétrica
frac_volCC <- read.table("VvCC.dat")

#Leitura da Svnr e Svr
frac_areNRCC <- read.table("SvnrCC.dat")
frac_areRCC <- read.table("SvrCC.dat")

#Leitura da Na
NaCC <- read.table("NaCC.dat")

#Transformação em Matriz
MVVCC <- data.matrix(frac_volCC)
MSVNRCC <- data.matrix(frac_areNRCC)
MSVRCC <- data.matrix(frac_areRCC)
NaCC <- data.matrix(NaCC) 

#Atribuição dos valores para vetores
tempoCC <- MVVCC[,1]
VVCC <- MVVCC[,2]
SVNRCC <- MSVNRCC[,2]
SVRCC <- MSVRCC[,2]
NaCC <- NaCC[,2]

#Velocidade dR/dt
GGCC = 1  #1 célula por unidade de tempo

#cálculo da constante N (Número de núcleos por unidade de volume)
NCC <- VVCC[2]

#Cálculo da Vv
Vv_calcCC <- 1 - exp(-(4/3)*pi*(GGCC^3)*NCC*((tempoCC-1)^3)) #ajuste tempo - 1

#Cálculo da Sv
Sv_calcCC <- 4*pi*1.5*NCC*(GGCC^3)*((tempoCC-1)^2)*exp(-(4/3)*NCC*(GGCC^3)*pi*((tempoCC-1)^3)) #ajuste (tempo - 1)

#Cálculo de Caminho Microestrutural
SVVCC <- 1.5*GGCC*((3*12*pi*NCC)^(1/3))*(1-Vv_calcCC)*((log(1/(1-Vv_calcCC)))^(2/3))


#Cálculo de velocidade de interface G
tamanhoCC <- length(VVCC) - 1
GCC <- 0
for (i in 1:tamanhoCC )
{
  GCC[i] <- 2*(-VVCC[i] + VVCC[i+1])/(SVNRCC[i]+SVNRCC[i+1])
}
GCC[tamanhoCC+1] <- GCC[tamanhoCC]

#G teórico
GteoCC <- rep(1/3 * 2,tamanhoCC+1) #ajuste (* 2)

#Cálculo de Hg com Svnr
#Hg1 = 2.0 * pi * Na / SVNR

#Cálculo de Hg com Svr
#Hg2 = 2.0 * pi * Na / SVR

#################################################################
#Remoção de pontos excessivos

intervaloCC <- seq(1,length(tempoCC),by = length(tempoCC)%/%20) #Quinta parte (100/20)
temporedCC <- tempoCC[intervaloCC]
VVredCC <- VVCC[intervaloCC]
SVNRredCC <- SVNRCC[intervaloCC]
SVRredCC <- SVRCC[intervaloCC]
intervaloCC[length(intervaloCC)] = length(GCC)-2 #este ponto é mais representativo
GredCC <- GCC[intervaloCC]
SVRredCC <- SVRCC[intervaloCC]

#################################################################
#                           GRÁFICOS
#################################################################

# O Gráfico do AC é maior que o do CC

#Gráfico de Vv

#png(file = "grafVV.png", bg = "white")
pdf(file = "grafVV.pdf", bg = "white")

par( mar = c(5,5,4,2))

# AC Calculada
grafVv <- plot(tempoAC,Vv_calcAC, type = "l", xlab = "Time", ylab = expression(bold(V[V])), col="black", cex.lab= 1.4, font.lab = 2, lty = 1)

# AC Simulada
grafVv <- points(temporedAC,VVredAC, col="black", cex = 1.5, pch = 0)

# CC Calculada
grafVv <- lines(tempoCC,Vv_calcCC, col = "black", lty = 5) #longdash

# CC Simulada
grafVv <- points(temporedCC,VVredCC, col="black", cex = 1.5, pch = 1)

legend(x = 105, y = 0.2, legend=c("Simulated CA", "Simulated CC", "Calculated CA", 
                                  "Calculated CC"), lty=c(NA, NA, 1, 5), lwd=1:1, bty="n", 
                                  pch=c(0,1,NA,NA),  col=c("black","black", "black","black"),
                                  pt.cex = 1.5)


dev.off()

#################

#Gráfico de Svnr

#png(file = "grafSVnr.png", bg = "white")
pdf(file = "grafSVnr.pdf", bg = "white")

par( mar = c(5,5,4,2))

# AC Calculada
grafVv <- plot(tempoAC,Sv_calcAC, type = "l", xlab = "Time", ylab = expression(bold(S[Vnr])), col="black", cex.lab= 1.4, font.lab = 2)

# AC Simulada
grafVv <- points(temporedAC, SVNRredAC, col="black", cex = 1.5, pch = 0)

# CC Calculada
grafVv <- lines(tempoCC,Sv_calcCC, col = "black", lty = 5) #longdash

# CC Simulada
grafVv <- points(temporedCC,SVNRredCC, col="black", cex = 1.5, pch = 1)

legend(x = 105, y = 0.049, legend=c("Simulated CA", "Simulated CC", "Calculated CA", 
                                    "Calculated CC"), lty=c(NA, NA, 1, 5), lwd=1:1, bty="n", 
       pch=c(0,1,NA,NA),  col=c("black","black", "black","black"),
       pt.cex = 1.5)


dev.off()

##################

#Gráfico de Svr

#png(file = "grafSVr.png", bg = "white")
pdf(file = "grafSVr.pdf", bg = "white")

par( mar = c(5,5,4,2))

grafVv <- plot(temporedAC,SVRredAC, type = "p", xlab = "Time", ylab = expression(bold(S[Vr])), col="black", cex.lab= 1.4, font.lab = 2,cex = 1.5, pch = 0)
#grafVv <- points(tempo,SVNR)
#legend(x = 25.5, y = 0.18, legend=c("Valores simulados", "Valores calculados"), lty=c(NA,1), col=c(1,1), lwd=1:1, bty="o", pch=c(1,NA))

# CC Simulada
grafVv <- points(temporedCC,SVRredCC, col="black", cex = 1.5, pch = 1)

legend(x = 105, y = 0.01, legend=c("Simulated CA", "Simulated CC"), bty="n", 
       pch=c(0,1),  col=c("black","black"), pt.cex = 1.5)

dev.off()

########################################

#Gráfico de Caminho Microestrutural

#png(file = "caminho.png", bg = "white")
pdf(file = "caminho.pdf", bg = "white")

par( mar = c(5,5,4,2))

# AC Calculada
grafVv <- plot(Vv_calcAC, SVVAC, type = "l", xlab = expression(bold(V[V])), ylab = expression(bold(S[Vnr])), col="black", cex.lab= 1.4, font.lab = 2)

# AC Simulada
grafVv <- points(VVredAC, SVNRredAC, col = "black", cex = 1.5, pch = 0)

# CC Calculada
grafVv <- lines(Vv_calcCC,SVVCC, col = "black", lty = 5) #longdash

# CC Simulada
grafVv <- points(VVredCC,SVNRredCC, col="black", cex = 1.5, pch = 1)

legend(x = 0.37, y = 0.01, legend=c("Simulated CA", "Simulated CC", "Calculated CA", 
                                    "Calculated CC"), lty=c(NA, NA, 1, 5), lwd=1:1, bty="n", 
       pch=c(0,1,NA,NA),  col=c("black","black", "black","black"),
       pt.cex = 1.5)


dev.off()

#########################################

#Gráfico de velocidade de interface G

#Neste caso,  gráfico principal deverá ser feito com o CC

#png(file = "velocidade.png", bg = "white")
pdf(file = "velocidade.pdf", bg = "white")

par( mar = c(5,5,4,2))

# CC Calculada
grafVv <- plot(tempoCC, GteoCC, type = "l", lty = 5, xlab = "Time", ylab = "G", col = "black",  cex.lab= 1.4, font.lab = 2, ylim = c(0.2, 0.8), xlim = c(0, 150))

# CC Calculada
grafVv <- points(temporedCC, GredCC, col = "black", cex = 1.5)

# AC Calculada
grafVv <- lines(tempoAC, GteoAC, col = "black", lty = 1) #longdash

# AC Simulada
grafVv <- points(temporedAC, GredAC, col="black", cex = 1.5, pch = 0)

legend(x = 105, y = 0.3, legend=c("Simulated CA", "Simulated CC", "Calculated CA", 
                                    "Calculated CC"), lty=c(NA, NA, 1, 5), lwd=1:1, bty="n", 
       pch=c(0,1,NA,NA),  col=c("black","black", "black","black"),
       pt.cex = 1.5)


dev.off()

# Mean square error

# Vv
# AC

mseVvAC <- mse(VVAC,Vv_calcAC)

print("MSE VV AC: ")
print(mseVvAC)

# CC

mseVvCC <- mse(VVCC,Vv_calcCC)

print("MSE VV CC: ")
print(mseVvCC)

# #Gráfico de Na
# png(file = "grafNa.png", bg = "white")
# grafVv <- plot(Na, type = "l", xlab = "Tempo", ylab = "Na")
# #grafVv <- points(tempo,SV)
# #legend(x = 25.5, y = 0.18, legend=c("Valores simulados", "Valores calculados"), lty=c(NA,1), col=c(1,1), lwd=1:1, bty="o", pch=c(1,NA))
# dev.off()
# 
# #Gráfico de Hg com Svnr
# png(file = "grafHg1.png", bg = "white")
# grafVv <- plot(Hg1, type = "l", xlab = "Tempo", ylab = "Hg")
# #grafVv <- points(tempo,SV)
# #legend(x = 25.5, y = 0.18, legend=c("Valores simulados", "Valores calculados"), lty=c(NA,1), col=c(1,1), lwd=1:1, bty="o", pch=c(1,NA))
# dev.off()
# 
# #Gráfico de Hg com Svr
# png(file = "grafHg2.png", bg = "white")
# grafVv <- plot(Hg2, type = "l", xlab = "Tempo", ylab = "Hg")
# #grafVv <- points(tempo,SV)
# #legend(x = 25.5, y = 0.18, legend=c("Valores simulados", "Valores calculados"), lty=c(NA,1), col=c(1,1), lwd=1:1, bty="o", pch=c(1,NA))
# dev.off()
