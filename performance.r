jpeg("performance.jpg");
vec <- scan("http://www.upl.cs.wisc.edu/~riccardo/performance.txt")

barplot(vec, 
		ylim=c(0,151720),
		xlab="Performance Test",
		ylab="Execution Time (ns)", 
    	main="FreeBSD Performance",
    	names.arg=c("1", "1 - No ASLR", 
		    "2", "2 - No ASLR",
	 	    "3", "3 - No ASLR",
		    "4", "4 - No ASLR"),
    	col=c("#0000FF", "#FF0000",
	      "#0000FF", "#FF0000",
	      "#0000FF", "#FF0000",
              "#0000FF", "#FF0000"))

dev.off()
