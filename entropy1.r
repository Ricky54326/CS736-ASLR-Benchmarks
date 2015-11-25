jpeg("entropy1.jpg");
vec <- scan("http://www.upl.cs.wisc.edu/~riccardo/entropy.txt")

barplot(vec, 
		xlab="OS",
		ylab="# Collisions", 
    	main="ASLR Stack Entropy",
    	names.arg=c("Debian", "FreeBSD", "HBSD"),
    	col=c("red", "orange", "blue"))

dev.off()
