chi2IsUniform<-function(dataset,significance=0.05){
  chi2IsUniform=(chisq.test(dataset)$p.value>significance)
}


vec_freebsd_entropy1 <- read.table("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/results/freebsd/entropy1-freebsd.txt")
vec_freebsd_entropy2 <- read.table("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/results/freebsd/entropy2-freebsd.txt")
vec_freebsd_entropy3 <- read.table("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/results/freebsd/entropy3-freebsd.txt")

vec_debian_entropy1 <- read.table("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/results/debian/15-11-08__14-44-entropy1.p-unpickled.txt")
vec_debian_entropy2 <- read.table("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/results/debian/15-11-08__14-44-entropy2.p-unpickled.txt")
vec_debian_entropy3 <- read.table("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/results/debian/15-11-08__14-44-entropy3.p-unpickled.txt")

# vec_hbsd_entropy1 <- read.table("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/results/hbsd/entropy1-hbsd.txt")
# vec_hbsd_entropy2 <- read.table("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/results/hbsd/entropy2-hbsd.txt")
# vec_hbsd_entropy3 <- read.table("/home/r/riccardo/Development/CS736-ASLR-Benchmarks/results/hbsd/entropy3-hbsd.txt")


print("FreeBSD entropy test uniformity:")
for (ds in list(vec_freebsd_entropy1, vec_freebsd_entropy2, vec_freebsd_entropy3)){
  #print(c("Data set:",ds))
  #print(chisq.test(ds))
  print(paste("uniform?",chi2IsUniform(ds)))
}

print("Debian entropy test uniformity:")
for (ds in list(vec_debian_entropy1, vec_debian_entropy2, vec_debian_entropy3)){
  #print(c("Data set:",ds))
  #print(chisq.test(ds))
  print(paste("uniform?",chi2IsUniform(ds)))
}

# print("HBSD entropy test uniformity:")
# for (ds in list(vec_hbsd_entropy1, vec_hbsd_entropy2, vec_hbsd_entropy3)){
#   #print(c("Data set:",ds))
#   #print(chisq.test(ds))
#   print(paste("uniform?",chi2IsUniform(ds)))
# }