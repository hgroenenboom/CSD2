import numpy

x = []

for i in range(100):
	x.append(numpy.random.normal(0, 1))

x = numpy.multiply(x, 10)
x = numpy.add(x, 150)	
	
#print(numpy.mean(x))
print(x)