# vec <- c(150, 150, 500)
vec <- scan("http://www.upl.cs.wisc.edu/~riccardo/entropy1.p")

len = length(vec)

hist(vec, xlab="Address", breaks=len)

