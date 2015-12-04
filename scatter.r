#jpeg("entropy1-debian.jpg");
vec <- read.csv("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/entropy1-debian-clean.csv")
#vec <- read.csv("http://www.upl.cs.wisc.edu/~riccardo/entropy1-debian-clean.csv")
y <- vec$high
x <- vec$low


plot(x, y, main="Memory Accesses", xaxt = "n", yaxt = "n",
     xlab="Low 32-bits ", ylab="High 32-bits ", pch=19, cex=0.1)


axis(1,seq(0,60000,10000), paste0("0x",as.hexmode(seq(0,60000,10000))))
axis(2,seq(0,60000,10000), paste0("0x",as.hexmode(seq(0,60000,10000))))
#dev.off()
