jpeg("entropy1.png");
vec <- scan("http://www.upl.cs.wisc.edu/~riccardo/entropy.csv")

barplot(vec, 
		xlab="OS",
		ylab="# Collisions", 
    	main="ASLR Stack Entropy",
    	names.arg=c("Debian", "FreeBSD", "HBSD"),
    	col=c("red", "orange", "blue"))

dev.off()
