#include "Mutation.h"
#include "Population.h"

void Mutation::mutate(Genome* genome){
	double r;
	for(int i = 0; i < MutationType::End; i++){
		r = ((double)rand() / (RAND_MAX));

		switch(i){
		case(MutationType::AddNode):
			if(r < MUTATION_RATE_ADD_NODE){
				Mutation::addNode(genome);
			}
			break;
		case(MutationType::AddConnection):
			if(r < MUTATION_RATE_ADD_CONNECTION){
				Mutation::addConnection(genome);
			}
			break;
		case(MutationType::PerturbBias):
			if(r < MUTATION_RATE_PERTURB_BIAS){
				Mutation::perturbBias(genome);
			}
			break;
		case(MutationType::PerturbWeight):
			if(r < MUTATION_RATE_PERTURB_WEIGHT){
				Mutation::perturbWeight(genome);
			}
			break;
		case(MutationType::ToggleNode):
			if(r < MUTATION_RATE_TOGGLE_NODE){
				Mutation::toggleNode(genome);
			}
			break;
		case(MutationType::ToggleConnection):
			if(r < MUTATION_RATE_TOGGLE_CONNECTION){
				Mutation::toggleConnection(genome);
			}
			break;
		}
	}
}

void Mutation::addNode(Genome* genome){
	// Get random connection key
	// Add new node
	// Add two new connections where previous connection was
	// Disable connection

	int index = rand() % genome->getConnectionKeys()->size();
	ConnectionGene* currentConnectionGene = genome->getConnectionGenes[index];

	int nodeGeneInnovation = Population::getInnovationNumber();
	Population::incrementInnovationNumber();
	NodeGene* newNodeGene = new NodeGene();
	newNodeGene->innovation = nodeGeneInnovation;
	newNodeGene->bias = NODE_GENE_INITIAL_BIAS;
	newNodeGene->type = NodeType::Hidden;
	newNodeGene->enabled = true;

	genome->getHiddenNodeKeys()->push_back(nodeGeneInnovation);
	genome->getNodeKeys()->push_back(nodeGeneInnovation);
	genome->getNodeGenes()->insert(std::pair<int, NodeGene*>(nodeGeneInnovation, newNodeGene));

	ConnectionGene* inputConnectionGene = new ConnectionGene();
	inputConnectionGene->innovation = Population::getInnovationNumber();
	inputConnectionGene->inputId = currentConnectionGene->inputId;
	inputConnectionGene->outputId = nodeGeneInnovation;
	inputConnectionGene->weight = 1.0;
	inputConnectionGene->enabled = true;
	Population::incrementInnovationNumber();

	ConnectionGene* outputConnectionGene = new ConnectionGene();
	outputConnectionGene->innovation = Population::getInnovationNumber();
	outputConnectionGene->inputId = nodeGeneInnovation;
	outputConnectionGene->outputId = currentConnectionGene->outputId;
	outputConnectionGene->weight = currentConnectionGene->weight;
	outputConnectionGene->enabled = true;
	Population::incrementInnovationNumber();

	currentConnectionGene->enabled = false;
}

void Mutation::addConnection(Genome* genome){
	// Get random two random nodes (output cannot be of type input)
	// Add new connection between them
	
	int inputKey = rand() % genome->getNodeKeys()->size();
	NodeGene* inputNodeGene = genome->getNodeGenes()->operator[][inputKey];
	
	std::vector<int>* currentOutputKeys = new std::vector<int>();
	for(int i = 0; i < genome->getConnectionKeys()->size(); i++){
		if(genome->getConnectionGenes()->operator[][genome->getConnectionKeys()->operator[][i]]->inputId == inputNodeGene->innovation){
			currentOutputKeys->push_back(genome->getConnectionGenes()->operator[][genome->getConnectionKeys()->operator[][i]]->outputId);
		}
	}

	int outputKey;
	NodeGene* outputNodeGene;
	//	TODO: Fix infinite loop if no outputs are available
	do{
		outputKey = rand() % genome->getNodeKeys()->size();
	} while((genome->getNodeGenes()->operator[][outputKey]->type != NodeType::Input) && (std::find(currentOutputKeys->begin(), currentOutputKeys->end(), outputKey) != currentOutputKeys->end()));

	outputNodeGene = genome->getNodeGenes()->operator[][outputKey];

	ConnectionGene* newConnectionGene = new ConnectionGene();
	newConnectionGene->innovation = Population::getInnovationNumber();
	newConnectionGene->inputId = inputKey;
	newConnectionGene->outputId = outputKey;
	newConnectionGene->weight = CONNECTION_GENE_INITIAL_WEIGHT;
	newConnectionGene->enabled = true;
	Population::incrementInnovationNumber();

	delete currentOutputKeys;
}

void Mutation::perturbBias(Genome* genome){
	// Get random node
	// Perturb bias
	int index = rand() % genome->getNodeKeys()->size();
	double perturbationPercent = (2.0 * ((double)rand() / (RAND_MAX))) - 1.0;
	genome->getNodeGenes()->operator[][index]->bias += (NODE_GENE_MAX_BIAS_PERTURBATION * perturbationPercent);
}

void Mutation::perturbWeight(Genome* genome){
	// Get random connection
	// Perturb weight
	int index = rand() % genome->getConnectionKeys()->size();
	double perturbationPercent = (2.0 * ((double)rand() / (RAND_MAX))) - 1.0;
	genome->getConnectionGenes()->operator[][index]->weight += (CONNECTION_GENE_MAX_WEIGHT_PERTURBATION * perturbationPercent);
}

void Mutation::toggleNode(Genome* genome){
	// Get random hidden node
	// Toggle enabled state
	int index = rand() % genome->getHiddenNodeKeys()->size();
	bool state = genome->getNodeGenes[genome->getHiddenNodeKeys()->operator[][index]].enabled;
	genome->getNodeGenes[genome->getHiddenNodeKeys()->operator[][index]].enabled = !state;
}

void Mutation::toggleConnection(Genome* genome){
	// Get random connection
	// Toggle enabled state
	int index = rand() % genome->getConnectionKeys()->size();
	bool state = genome->getConnectionGenes[genome->getConnectionKeys()->operator[][index]]->enabled;
	genome->getConnectionGenes[genome->getConnectionKeys()->operator[][index]]->enabled = !state; 
}