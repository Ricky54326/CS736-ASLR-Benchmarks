jpeg("entropybits.jpg");
vec <- scan("http://www.upl.cs.wisc.edu/~riccardo/entropybits.txt")

barplot(vec, 
		ylim=c(0,30),
		xlab="Region of Memory",
		ylab="Entropy (in bits)", 
    	main="Debian Bits of Entropy",
    	names.arg=c("Stack", "Heap", "Code"),
    	col=c("#0000FF", "#0000FF", "#0000FF"))

dev.off()
