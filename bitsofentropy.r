jpeg("entropybits.jpg");
vec <- scan("http://www.upl.cs.wisc.edu/~riccardo/entropybits.txt")

barplot(vec, 
		xlab="OS",
		ylab="Entropy (in bits)", 
    	main="Debian Bits of Entropy",
    	names.arg=c("Stack", "Heap", "Code"),
    	col=c("#316EFF", "#316EFF", "#316EFF"))

dev.off()
