default:
	g++ -Ofast Activation.cpp Config.cpp Genome.cpp main.cpp Mutation.cpp Network.cpp Neuron.cpp Population.cpp -o ../neatEngine.exe
debug: 
	g++ -g Activation.cpp Config.cpp Genome.cpp main.cpp Mutation.cpp Network.cpp Neuron.cpp Population.cpp -o ../neatEngine.exe
clean:
	rm -f ../neatEngine.exe
re:
	rm -f ../neatEngine.exe
	g++ -Ofast Activation.cpp Config.cpp Genome.cpp main.cpp Mutation.cpp Network.cpp Neuron.cpp Population.cpp -o ../neatEngine.exe
