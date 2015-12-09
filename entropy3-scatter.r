jpeg("entropy3-freebsd.jpg");
vec <- read.csv("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/entropy3-freebsd.txt-clean.csv")
#vec <- read.csv("http://www.upl.cs.wisc.edu/~riccardo/entropy1-debian-clean.csv")
y <- vec$high
x <- vec$low


plot(x, y, main="FreeBSD - Code", xaxt = "n", yaxt = "n",
     xlab="Low 8 bits ", ylab="High 8 bits ", pch=19, cex=0.1)



startX <- min(x) # 0
endX <-  max(x) # 60000
stepX <- max(x) / 5 # 10000

startY <- min(y) # 0
endY   <- max(y) # 60000
stepY  <- max(y) / 10 # 1000

axis(1,seq(startX,endX,stepX), paste0("0x",as.hexmode(seq(startX,endX,stepX))))
axis(2,seq(startY,endY,stepY), paste0("0x",as.hexmode(seq(startY,endY,stepY))))
dev.off()