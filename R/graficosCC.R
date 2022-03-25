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

#Leitura da Fração Volumétrica
frac_vol <- read.table("Vv.dat")

#Leitura da Svnr e Svr
frac_areNR <- read.table("Svnr.dat")
frac_areR <- read.table("Svr.dat")

#Leitura da Na
Na <- read.table("Na.dat")

#Transformação em Matriz
MVV <- data.matrix(frac_vol)
MSVNR <- data.matrix(frac_areNR)
MSVR <- data.matrix(frac_areR)
Na <- data.matrix(Na) 

#Atribuição dos valores para vetores
tempo <- MVV[,1]
VV <- MVV[,2]
SVNR <- MSVNR[,2]
SVR <- MSVR[,2]
Na <- Na[,2]

#cálculo da constante N (Número de núcleos por unidade de volume)
N <- VV[2]

#Cálculo da Vv
Vv_calc <- 1 - exp(-(4/3)*pi*N*((tempo-1)^3)) #ajuste tempo - 1

#Cálculo da Sv
Sv_calc <- 4*pi*1.5*N*((tempo-1)^2)*exp(-(4/3)*N*pi*((tempo-1)^3)) #ajuste (tempo - 1)

#Cálculo de Caminho Microestrutural
SVV <- 1.5*((3*12*pi*N)^(1/3))*(1-Vv_calc)*((log(1/(1-Vv_calc)))^(2/3))


#Cálculo de velocidade de interface G
tamanho <- length(VV) - 1
G <- 0
for (i in 1:tamanho )
{
  G[i] <- 2*(-VV[i] + VV[i+1])/(SVNR[i]+SVNR[i+1])
}
G[tamanho+1] <- G[tamanho]

#G teórico
Gteo <- rep(1/3 * 2,tamanho+1) #ajuste (* 2)

#Cálculo de Hg com Svnr
Hg1 = 2.0 * pi * Na / SVNR

#Cálculo de Hg com Svr
Hg2 = 2.0 * pi * Na / SVR

#################################################################
#Remoção de pontos excessivos

intervalo <- seq(1,length(tempo),by = length(tempo)%/%20) #Quinta parte (100/20)
tempored <- tempo[intervalo]
VVred <- VV[intervalo]
SVNRred <- SVNR[intervalo]
SVRred <- SVR[intervalo]
intervalo[length(intervalo)] = length(G)-2 #este ponto é mais representativo
Gred <- G[intervalo]

#################################################################
#                           GRÁFICOS
#################################################################

#Gráfico de Vv
#png(file = "grafVV.png", bg = "white")
pdf(file = "grafVV.pdf", bg = "white")
par( mar = c(5,5,4,2))
grafVv <- plot(tempo,Vv_calc, type = "l", xlab = "Time", ylab = expression(bold(V[V])), col="blue", cex.lab= 1.4, font.lab = 2)
grafVv <- points(tempored,VVred, col="red", cex = 1.5)
legend(x = 75, y = 0.1, legend=c("Simulated", "Calculated"), lty=c(NA,1), lwd=1:1, bty="n", pch=c(1,NA),  col=c("red","blue"), 
       pt.cex = 1.5)
dev.off()

#Gráfico de Svnr
#png(file = "grafSVnr.png", bg = "white")
pdf(file = "grafSVnr.pdf", bg = "white")
par( mar = c(5,5,4,2))
grafVv <- plot(tempo,Sv_calc, type = "l", xlab = "Time", ylab = expression(bold(S[Vnr])), col="blue", cex.lab= 1.4, font.lab = 2)
grafVv <- points(tempored, SVNRred, col="red", cex = 1.5)
legend(x = 37, y = 0.0047, legend=c("Simulated", "Calculated"), lty=c(NA,1), col=c("red","blue"), lwd=1:1, bty="n", pch=c(1,NA), pt.cex = 1.5)
dev.off()

#Gráfico de Svr
#png(file = "grafSVr.png", bg = "white")
pdf(file = "grafSVr.pdf", bg = "white")
par( mar = c(5,5,4,2))
grafVv <- plot(tempored,SVRred, type = "p", xlab = "Time", ylab = expression(bold(S[Vr])), col="red", cex.lab= 1.4, font.lab = 2, cex = 1.5)
#grafVv <- points(tempo,SVNR)
#legend(x = 25.5, y = 0.18, legend=c("Valores simulados", "Valores calculados"), lty=c(NA,1), col=c(1,1), lwd=1:1, bty="o", pch=c(1,NA))
dev.off()

#Gráfico de Caminho Microestrutural
#png(file = "caminho.png", bg = "white")
pdf(file = "caminho.pdf", bg = "white")
par( mar = c(5,5,4,2))
grafVv <- plot(Vv_calc, SVV, type = "l", xlab = expression(bold(V[V])), ylab = expression(bold(S[Vnr])), col="blue", cex.lab= 1.4, font.lab = 2)
grafVv <- points(VVred, SVNRred, col = "red", cex = 1.5)
legend(x = 0.37, y = 0.0047, legend=c("Simulated", "Calculated"), lty=c(NA,1), col=c("red","blue"), lwd=1:1, bty="n", pch=c(1,NA), pt.cex = 1.5)
dev.off()

#Gráfico de velocidade de interface G
#png(file = "velocidade.png", bg = "white")
pdf(file = "velocidade.pdf", bg = "white")
par( mar = c(5,5,4,2))
grafVv <- plot(tempo, Gteo, type = "l", xlab = "Time", ylab = "G", col = "blue",  ylim = c(0.3, 0.8), cex.lab= 1.4, font.lab = 2)
grafVv <- points(tempored, Gred, col = "red", cex = 1.5)
legend(x = 75, y = 0.35, legend=c("Simulated", "Calculated"), lty=c(NA,1), col=c("red","blue"), lwd=1:1, bty="n", pch=c(1,NA), pt.cex = 1.5)
dev.off()

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

############
# dat<- data.frame(t=seq(1, 58, by=1) )
# xhrt <- function(t) Vv_calc
# yhrt <- function(t) Sv_calc
# dat$y=yhrt(dat$t)
# dat$x=xhrt(dat$t)
# with(dat, plot(x,y, type="l"))
# grafVv <- points(VV,SVNR)