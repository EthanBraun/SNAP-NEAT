default:
	g++ -Ofast Activation.cpp Config.cpp Genome.cpp main.cpp Mutation.cpp Network.cpp Neuron.cpp Population.cpp -o ../snapneatTest
debug: 
	g++ -g Activation.cpp Config.cpp Genome.cpp main.cpp Mutation.cpp Network.cpp Neuron.cpp Population.cpp -o ../snapneatTest
clean:
	rm -f ../snapneatTest
re:
	rm -f ../snapneatTest
	g++ -Ofast Activation.cpp Config.cpp Genome.cpp main.cpp Mutation.cpp Network.cpp Neuron.cpp Population.cpp -o ../snapneatTest
