#include "Mutation.h"
#include "Population.h"

void Mutation::mutate(Genome* genome, Population* population){
	double r;
	for(int i = 0; i <= End; i++){
		r = ((double)rand() / (double)RAND_MAX);

		switch(i){
		case(AddNode):
			if(r < MUTATION_RATE_ADD_NODE){
				addNode(genome, population);
			}
			break;
		case(AddConnection):
			if(r < MUTATION_RATE_ADD_CONNECTION){
				addConnection(genome, population);
			}
			break;
		case(PerturbWeight):
			if(r < MUTATION_RATE_PERTURB_WEIGHT_GENOME){
				perturbWeight(genome);
			}
			break;
		case(ToggleNode):
			if(r < MUTATION_RATE_TOGGLE_NODE){
				toggleNode(genome);
			}
			break;
		case(ToggleConnection):
			if(r < MUTATION_RATE_TOGGLE_CONNECTION){
				toggleConnection(genome);
			}
			break;
		case(RemoveNode):
			if(r < MUTATION_RATE_REMOVE_NODE){
				removeNode(genome);
			}
			break;
		case(RemoveConnection):
			if(r < MUTATION_RATE_REMOVE_CONNECTION){
				removeConnection(genome);
			}
			break;
		}
	}
}

void Mutation::addNode(Genome* genome, Population* population){
	// Get random connection key
	// Add new node
	// Add two new connections where previous connection was
	// Disable connection
	if(genome->getConnectionKeys()->size() == 0){
		return;
	}

	int index = rand() % genome->getConnectionKeys()->size();
	ConnectionGene* currentConnectionGene = genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](index));

	int nodeGeneInnovation = population->updateInnovations(AddNode, GeneTypeNode, currentConnectionGene->inputId, currentConnectionGene->outputId);
	NodeGene* newNodeGene = new NodeGene();
	newNodeGene->innovation = nodeGeneInnovation;
	newNodeGene->type = Hidden;
	newNodeGene->enabled = true;

	genome->getHiddenNodeKeys()->push_back(nodeGeneInnovation);
	genome->getNodeKeys()->push_back(nodeGeneInnovation);
	genome->getNodeGenes()->insert(std::pair<int, NodeGene*>(nodeGeneInnovation, newNodeGene));

	ConnectionGene* inputConnectionGene = new ConnectionGene();
	inputConnectionGene->innovation = population->updateInnovations(AddNode, GeneTypeConnection, currentConnectionGene->inputId, nodeGeneInnovation);
	inputConnectionGene->inputId = currentConnectionGene->inputId;
	inputConnectionGene->outputId = nodeGeneInnovation;
	inputConnectionGene->weight = 1.0;
	inputConnectionGene->enabled = true;

	ConnectionGene* outputConnectionGene = new ConnectionGene();
	outputConnectionGene->innovation = population->updateInnovations(AddNode, GeneTypeConnection, nodeGeneInnovation, currentConnectionGene->outputId);
	outputConnectionGene->inputId = nodeGeneInnovation;
	outputConnectionGene->outputId = currentConnectionGene->outputId;
	outputConnectionGene->weight = currentConnectionGene->weight;
	outputConnectionGene->enabled = true;

	genome->getConnectionKeys()->push_back(inputConnectionGene->innovation);
	genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(inputConnectionGene->innovation, inputConnectionGene));
	genome->getConnectionKeys()->push_back(outputConnectionGene->innovation);
	genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(outputConnectionGene->innovation, outputConnectionGene));

	currentConnectionGene->enabled = false;
}

void Mutation::addConnection(Genome* genome, Population* population){
	// Get random two random nodes (output cannot be of type input)
	// Add new connection between them


	int inputKey = rand() % genome->getNodeKeys()->size();
	NodeGene* inputNodeGene = genome->getNodeGenes()->operator[](genome->getNodeKeys()->operator[](inputKey));

	std::vector<int>* currentOutputKeys = new std::vector<int>();
	for(int i = 0; i < genome->getConnectionKeys()->size(); i++){
		if(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->inputId == inputNodeGene->innovation){
			currentOutputKeys->push_back(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->outputId);
		}
	}

	int outputKey = rand() % genome->getNodeKeys()->size();
	if((genome->getNodeGenes()->operator[](genome->getNodeKeys()->operator[](outputKey))->type == Input) || (std::find(currentOutputKeys->begin(), currentOutputKeys->end(), outputKey) != currentOutputKeys->end())){
		delete currentOutputKeys;
		return;
	}


	NodeGene* outputNodeGene = genome->getNodeGenes()->operator[](genome->getNodeKeys()->operator[](outputKey));

	ConnectionGene* newConnectionGene = new ConnectionGene();
	newConnectionGene->innovation = population->updateInnovations(AddConnection, GeneTypeConnection, inputKey, outputKey);
	newConnectionGene->inputId = inputKey;
	newConnectionGene->outputId = outputKey;
	newConnectionGene->weight = CONNECTION_GENE_INITIAL_WEIGHT;
	newConnectionGene->enabled = true;

	genome->getConnectionKeys()->push_back(newConnectionGene->innovation);
	genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(newConnectionGene->innovation, newConnectionGene));

	delete currentOutputKeys;
}

void Mutation::perturbWeight(Genome* genome){
	// Perturb weights based on rate
	double perturbationPercent;
	for(int i = 0; i < genome->getConnectionKeys()->size(); i++){
		if(((double)rand() / (double)RAND_MAX) < MUTATION_RATE_SWAP_WEIGHT_CONNECTION){
			double swapWeight = 2.0 * (((double)rand() / (double)RAND_MAX) - 0.5) * CONNECTION_GENE_ABS_WEIGHT_CAP;
			printf("\t\t\t\tWeight swapped from %f to %f\n", genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight, swapWeight);
			genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight = swapWeight;
		}
		if(((double)rand() / (double)RAND_MAX) < MUTATION_RATE_PERTURB_WEIGHT_CONNECTION){
			perturbationPercent = (2.0 * ((double)rand() / (RAND_MAX))) - 1.0;
			genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight += (CONNECTION_GENE_MAX_WEIGHT_PERTURBATION * perturbationPercent);
			if(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight > CONNECTION_GENE_ABS_WEIGHT_CAP){
				genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight = CONNECTION_GENE_ABS_WEIGHT_CAP;
			}
			else if(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight < -CONNECTION_GENE_ABS_WEIGHT_CAP){
				genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight = -CONNECTION_GENE_ABS_WEIGHT_CAP;
			}
		}
	}
}

void Mutation::toggleNode(Genome* genome){
	// Get random hidden node
	// Toggle enabled state
	if(genome->getHiddenNodeKeys()->size() == 0){
		return;
	}

	int index = rand() % genome->getHiddenNodeKeys()->size();
	NodeGene* node = genome->getNodeGenes()->operator[](genome->getHiddenNodeKeys()->operator[](index));
	node->enabled = !node->enabled;
}

void Mutation::toggleConnection(Genome* genome){
	// Get random connection
	// Toggle enabled state
	if(genome->getConnectionKeys()->size() == 0){
		return;
	}

	int index = rand() % genome->getConnectionKeys()->size();
	bool state = genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](index))->enabled;
	genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](index))->enabled = !state;
}

void Mutation::removeNode(Genome* genome){
	// Get random hidden node
	// Remove all connection genes to and from node
	// Remove node gene from genome
	if(genome->getHiddenNodeKeys()->size() == 0){
		return;
	}

	int hiddenIndex = rand() % genome->getHiddenNodeKeys()->size();
	int id = genome->getHiddenNodeKeys()->operator[](hiddenIndex);
	int index;
	for(int i = 0; i < genome->getNodeKeys()->size(); i++){
		if(genome->getNodeKeys()->operator[](i) == id){
			index = i;
			break;
		}
	}
	
	for(int i = 0; i < genome->getConnectionKeys()->size(); i++){
		if(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->inputId == id){
			delete genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i));
			genome->getConnectionGenes()->erase(genome->getConnectionKeys()->operator[](i));
			genome->getConnectionKeys()->erase(genome->getConnectionKeys()->begin() + i);
			i--;
		}
		else if(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->outputId == id){
			delete genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i));
			genome->getConnectionGenes()->erase(genome->getConnectionKeys()->operator[](i));
			genome->getConnectionKeys()->erase(genome->getConnectionKeys()->begin() + i);
			i--;
		}
	}

	delete genome->getNodeGenes()->operator[](id);
	genome->getNodeGenes()->erase(id);
	genome->getNodeKeys()->erase(genome->getNodeKeys()->begin() + index);
	genome->getHiddenNodeKeys()->erase(genome->getHiddenNodeKeys()->begin() + hiddenIndex);
}

void Mutation::removeConnection(Genome* genome){
	// Get random connection
	// Remove connection gene from genome
	if(genome->getConnectionKeys()->size() == 0){
		return;
	}

	int index = rand() % genome->getConnectionKeys()->size();
	int id = genome->getConnectionKeys()->operator[](index);
	
	delete genome->getConnectionGenes()->operator[](id);
	genome->getConnectionGenes()->erase(id);
	genome->getConnectionKeys()->erase(genome->getConnectionKeys()->begin() + index);
}