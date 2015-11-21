vec <- table(scan("http://www.upl.cs.wisc.edu/~riccardo/entropy1.p"))

# hist(vec, xlab="Address", breaks=len)

barplot(vec, xlab="OS", ylab="# Collisions", main="ASLR Entropy")

