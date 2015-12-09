jpeg("entropy3-debian.jpg");
vec <- read.csv("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/entropy3-debian.txt-clean.csv")
#vec <- read.csv("http://www.upl.cs.wisc.edu/~riccardo/entropy1-debian-clean.csv")
y <- vec$high
x <- vec$low


plot(x, y, main="Debian - Code", xaxt = "n", yaxt = "n",
     xlab="Low 16-bits ", ylab="High 16-bits ", pch=19, cex=0.1)


start <- 0
end <- 60000
step <- 10000

axis(1,seq(start,end,step), paste0("0x",as.hexmode(seq(start,end,step))))
axis(2,seq(start,end,step), paste0("0x",as.hexmode(seq(start,end,step))))
dev.off()
