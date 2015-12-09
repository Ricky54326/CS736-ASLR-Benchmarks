jpeg("performance-aslr.jpg");
vec <- scan("http://www.upl.cs.wisc.edu/~riccardo/performance.txt")

barplot(vec, 
		ylim=c(0,120000),
		xlab="Performance Test",
		ylab="Execution Time (ns)", 
    	main="HardenedBSD Performance (ASLR On)",
    	names.arg=c("1",  
		    "2", 
	 	    "3",
		    "4"),
    	col=c("#0000FF", 
	      	  "#0000FF", 
	      	  "#0000FF", 
              "#0000FF"))

dev.off()
