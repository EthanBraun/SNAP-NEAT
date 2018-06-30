#include "Mutation.h"
#include "Population.h"

void Mutation::mutate(Genome* genome, Population* population){
	double r;
	int coreMutation;

	if(genome->getElite()){
		return;
	}

	// Select core mutation
	if(((double)rand() / (double)RAND_MAX) < population->getConfig()->mutationRateCore){
		coreMutation = population->chooseOpWeighted();
		switch(coreMutation){
		case(0):
			// Coin flip for addNode or addConnection
			if(((double)rand() / (double)RAND_MAX) < 0.5){
				addNode(genome, population);
			}
			else{
				addConnection(genome, population);
			}
			break;
		case(1):
			addRbfNode(genome, population);
			break;
		case(2):
			addCascadeNode(genome, population);
			break;
		}
		genome->setRecentCoreOp(coreMutation);
	}
	else{
		perturbWeight(genome, population);	
	}

	// Iterate alt mutations
	for(int i = 0; i <= End; i++){
		r = ((double)rand() / (double)RAND_MAX);

		switch(i){
		case(AmToggleNode):
			if(r < population->getConfig()->mutationRateToggleNode){
				toggleNode(genome);
			}
			break;
		case(AmToggleConnection):
			if(r < population->getConfig()->mutationRateToggleConnection){
				toggleConnection(genome);
			}
			break;
		case(AmRemoveNode):
			if(r < population->getConfig()->mutationRateRemoveNode){
				removeNode(genome);
			}
			break;
		case(AmRemoveConnection):
			if(r < population->getConfig()->mutationRateRemoveConnection){
				removeConnection(genome);
			}
			break;
		case(AmPerturbRbfNode):
			if(r < population->getConfig()->mutationRatePerturbRbfNodeGenome){
				perturbRbfNode(genome, population);
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

	int index;
	ConnectionGene* currentConnectionGene;

	if(genome->hasActiveCascade()){
		std::vector<int> cascadeConnections = genome->getActiveCascadeConnections();
		if(cascadeConnections.size() == 0){
			return;
		}
		index = rand() % cascadeConnections.size(); 
		currentConnectionGene = genome->getConnectionGenes()->operator[](cascadeConnections[index]);
	}
	else{
		index = rand() % genome->getConnectionKeys()->size();
		currentConnectionGene = genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](index));
	}

	int nodeGeneInnovation = population->updateInnovations(AddNode, GeneTypeNode, currentConnectionGene->inputId, currentConnectionGene->outputId);
	NodeGene* newNodeGene = new NodeGene();
	newNodeGene->innovation = nodeGeneInnovation;
	newNodeGene->type = Hidden;
	newNodeGene->enabled = true;
	newNodeGene->rbf = false;
	newNodeGene->center = 0.0;
	newNodeGene->radius = 0.0;
	newNodeGene->cascade = false;

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

	int inputKeyIndex = rand() % genome->getNodeKeys()->size();
	int inputKey = genome->getNodeKeys()->operator[](inputKeyIndex);
	NodeGene* inputNodeGene = genome->getNodeGenes()->operator[](inputKey);

	std::vector<int>* currentOutputKeys = new std::vector<int>();
	for(int i = 0; i < genome->getConnectionKeys()->size(); i++){
		if(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->inputId == inputNodeGene->innovation){
			currentOutputKeys->push_back(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->outputId);
		}
	}

	int outputKeyIndex = rand() % genome->getNodeKeys()->size();
	int outputKey = genome->getNodeKeys()->operator[](outputKeyIndex);
	if((genome->getNodeGenes()->operator[](outputKey)->type == Input) || (std::find(currentOutputKeys->begin(), currentOutputKeys->end(), outputKey) != currentOutputKeys->end())){
		delete currentOutputKeys;
		return;
	}

	NodeGene* outputNodeGene = genome->getNodeGenes()->operator[](outputKey);

	ConnectionGene* newConnectionGene = new ConnectionGene();
	newConnectionGene->innovation = population->updateInnovations(AddConnection, GeneTypeConnection, inputKey, outputKey);
	newConnectionGene->inputId = inputKey;
	newConnectionGene->outputId = outputKey;
	newConnectionGene->weight = population->getConfig()->connectionGeneInitialWeight;
	newConnectionGene->enabled = true;

	genome->getConnectionKeys()->push_back(newConnectionGene->innovation);
	genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(newConnectionGene->innovation, newConnectionGene));

	delete currentOutputKeys;
}

void Mutation::perturbWeight(Genome* genome, Population* population){
	// Perturb weights based on rate
	double perturbationPercent;

	if(genome->hasActiveCascade()){
		std::vector<int> cascadeConnections = genome->getActiveCascadeConnections();
		for(int i = 0; i < cascadeConnections.size(); i++){
			if(((double)rand() / (double)RAND_MAX) < population->getConfig()->mutationRateSwapWeightConnection){
				double swapWeight = 2.0 * (((double)rand() / (double)RAND_MAX) - 0.5) * population->getConfig()->connectionGeneAbsWeightCap;
				genome->getConnectionGenes()->operator[](cascadeConnections[i])->weight = swapWeight;
			}
			if(((double)rand() / (double)RAND_MAX) < population->getConfig()->mutationRatePerturbWeightConnection){
				perturbationPercent = (2.0 * ((double)rand() / (RAND_MAX))) - 1.0;
				genome->getConnectionGenes()->operator[](cascadeConnections[i])->weight += (population->getConfig()->connectionGeneMaxWeightPerturbation * perturbationPercent);
				if(genome->getConnectionGenes()->operator[](cascadeConnections[i])->weight > population->getConfig()->connectionGeneAbsWeightCap){
					genome->getConnectionGenes()->operator[](cascadeConnections[i])->weight = population->getConfig()->connectionGeneAbsWeightCap;
				}
				else if(genome->getConnectionGenes()->operator[](cascadeConnections[i])->weight < -population->getConfig()->connectionGeneAbsWeightCap){
					genome->getConnectionGenes()->operator[](cascadeConnections[i])->weight = -population->getConfig()->connectionGeneAbsWeightCap;
				}
			}
		}
	}
	else{
		for(int i = 0; i < genome->getConnectionKeys()->size(); i++){
			if(((double)rand() / (double)RAND_MAX) < population->getConfig()->mutationRateSwapWeightConnection){
				double swapWeight = 2.0 * (((double)rand() / (double)RAND_MAX) - 0.5) * population->getConfig()->connectionGeneAbsWeightCap;
				genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight = swapWeight;
			}
			if(((double)rand() / (double)RAND_MAX) < population->getConfig()->mutationRatePerturbWeightConnection){
				perturbationPercent = (2.0 * ((double)rand() / (RAND_MAX))) - 1.0;
				genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight += (population->getConfig()->connectionGeneMaxWeightPerturbation * perturbationPercent);
				if(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight > population->getConfig()->connectionGeneAbsWeightCap){
					genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight = population->getConfig()->connectionGeneAbsWeightCap;
				}
				else if(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight < -population->getConfig()->connectionGeneAbsWeightCap){
					genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->weight = -population->getConfig()->connectionGeneAbsWeightCap;
				}
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
	bool isRbf = genome->getNodeGenes()->operator[](id)->rbf; 
	bool isCascade = genome->getNodeGenes()->operator[](id)->cascade;
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
	if(isRbf){
		for(int i = 0; i < genome->getRbfNodeKeys()->size(); i++){
			if(genome->getRbfNodeKeys()->operator[](i) == id){
				genome->getRbfNodeKeys()->erase(genome->getRbfNodeKeys()->begin() + i);
				break;
			}
		}
	}
	if(isCascade){
		for(int i = 0; i < genome->getCascadeNodeKeys()->size(); i++){
			if(genome->getCascadeNodeKeys()->operator[](i) == id){
				genome->getCascadeNodeKeys()->erase(genome->getCascadeNodeKeys()->begin() + i);
				break;
			}
		}
	}
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

void Mutation::addRbfNode(Genome* genome, Population* population){
	// Get random connection key
	// Add new rbf node
	// Add two new connections where previous connection was
	// Disable connection
	if(genome->getConnectionKeys()->size() == 0){
		return;
	}

	int index;
	ConnectionGene* currentConnectionGene;

	if(genome->hasActiveCascade()){
		std::vector<int> cascadeConnections = genome->getActiveCascadeConnections();
		if(cascadeConnections.size() == 0){
			return;
		}
		index = rand() % cascadeConnections.size(); 
		currentConnectionGene = genome->getConnectionGenes()->operator[](cascadeConnections[index]);
	}
	else{
		index = rand() % genome->getConnectionKeys()->size();
		currentConnectionGene = genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](index));
	}

	int nodeGeneInnovation = population->updateInnovations(AddRbfNode, GeneTypeNode, currentConnectionGene->inputId, currentConnectionGene->outputId);
	NodeGene* newNodeGene = new NodeGene();
	newNodeGene->innovation = nodeGeneInnovation;
	newNodeGene->type = Hidden;
	newNodeGene->enabled = true;
	newNodeGene->rbf = true;
	newNodeGene->center = population->getConfig()->rbfNodeInitialCenter;
	newNodeGene->radius = population->getConfig()->rbfNodeInitialRadius;
	newNodeGene->cascade = false;

	_ordInsert(genome->getHiddenNodeKeys(), nodeGeneInnovation);
	_ordInsert(genome->getRbfNodeKeys(), nodeGeneInnovation);
	_ordInsert(genome->getNodeKeys(), nodeGeneInnovation);
	genome->getNodeGenes()->insert(std::pair<int, NodeGene*>(nodeGeneInnovation, newNodeGene));

	ConnectionGene* inputConnectionGene = new ConnectionGene();
	inputConnectionGene->innovation = population->updateInnovations(AddRbfNode, GeneTypeConnection, currentConnectionGene->inputId, nodeGeneInnovation);
	inputConnectionGene->inputId = currentConnectionGene->inputId;
	inputConnectionGene->outputId = nodeGeneInnovation;
	inputConnectionGene->weight = 1.0;
	inputConnectionGene->enabled = true;

	ConnectionGene* outputConnectionGene = new ConnectionGene();
	outputConnectionGene->innovation = population->updateInnovations(AddRbfNode, GeneTypeConnection, nodeGeneInnovation, currentConnectionGene->outputId);
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

void Mutation::perturbRbfNode(Genome* genome, Population* population){
    // Perturb center and/or radius of rbf nodes based on rates
    double perturbationPercent;
    for(int i = 0; i < genome->getRbfNodeKeys()->size(); i++){
        if(((double)rand() / (double)RAND_MAX) < population->getConfig()->mutationRatePerturbRbfNodeCenter){
            perturbationPercent = (2.0 * ((double)rand() / (RAND_MAX))) - 1.0;
            genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->center += (population->getConfig()->rbfNodeMaxCenterPerturbation * perturbationPercent);
            if(genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->center > population->getConfig()->rbfNodeAbsCenterCap){
                genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->center = population->getConfig()->rbfNodeAbsCenterCap;
            }
            else if(genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->center < -population->getConfig()->rbfNodeAbsCenterCap){
                genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->center = -population->getConfig()->rbfNodeAbsCenterCap;
            }
        }
        if(((double)rand() / (double)RAND_MAX) < population->getConfig()->mutationRatePerturbRbfNodeRadius){
            perturbationPercent = (2.0 * ((double)rand() / (RAND_MAX))) - 1.0;
            genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->radius += (population->getConfig()->rbfNodeMaxRadiusPerturbation * perturbationPercent);
            if(genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->radius > population->getConfig()->rbfNodeMaxRadius){
                genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->radius = population->getConfig()->rbfNodeMaxRadius;
            }
            else if(genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->radius < population->getConfig()->rbfNodeMinRadius){
                genome->getNodeGenes()->operator[](genome->getRbfNodeKeys()->operator[](i))->radius = population->getConfig()->rbfNodeMinRadius;
            }
        }
    }
}

void Mutation::addCascadeNode(Genome* genome, Population* population){
    // Add new cascade node
    // Add new connections from all inputs and hidden nodes to new node
	// Add new connections from new node to all outputs
	
	// Innovation tracking might need to be revisited
    int nodeGeneInnovation = population->updateInnovations(AddCascadeNode, GeneTypeNode, genome->getCascadeNodeKeys()->size(), genome->getCascadeNodeKeys()->size());
    NodeGene* newNodeGene = new NodeGene();
    newNodeGene->innovation = nodeGeneInnovation;
    newNodeGene->type = Hidden;
    newNodeGene->enabled = true;
    newNodeGene->rbf = false;
    newNodeGene->center = population->getConfig()->rbfNodeInitialCenter;
    newNodeGene->radius = population->getConfig()->rbfNodeInitialRadius;
	newNodeGene->cascade = true;

    _ordInsert(genome->getHiddenNodeKeys(), nodeGeneInnovation);
    _ordInsert(genome->getCascadeNodeKeys(), nodeGeneInnovation);
    _ordInsert(genome->getNodeKeys(), nodeGeneInnovation);
    genome->getNodeGenes()->insert(std::pair<int, NodeGene*>(nodeGeneInnovation, newNodeGene));
	
	// Add connections from/to each existing node and new cascade node
	for(int i = 0; i < genome->getNodeKeys()->size(); i++){
		int curInnov = genome->getNodeKeys()->operator[](i);
        float connectionWeight = ((2.0 * ((float)rand() / (RAND_MAX))) - 1.0) * population->getConfig()->connectionGeneAbsWeightCap;
		if(genome->getNodeGenes()->operator[](curInnov)->type == Input){
			ConnectionGene* inputConnectionGene = new ConnectionGene();
			inputConnectionGene->innovation = population->updateInnovations(AddCascadeNode, GeneTypeConnection, curInnov, nodeGeneInnovation);
			inputConnectionGene->inputId = curInnov;
			inputConnectionGene->outputId = nodeGeneInnovation;
			inputConnectionGene->weight = connectionWeight;
			inputConnectionGene->enabled = true;
			genome->getConnectionKeys()->push_back(inputConnectionGene->innovation);
			genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(inputConnectionGene->innovation, inputConnectionGene));
		}
		else if((genome->getNodeGenes()->operator[](curInnov)->type == Hidden) && (curInnov != nodeGeneInnovation)){
			ConnectionGene* hiddenConnectionGene = new ConnectionGene();
			hiddenConnectionGene->innovation = population->updateInnovations(AddCascadeNode, GeneTypeConnection, curInnov, nodeGeneInnovation);
			hiddenConnectionGene->inputId = curInnov;
			hiddenConnectionGene->outputId = nodeGeneInnovation;
			hiddenConnectionGene->weight = connectionWeight;
			hiddenConnectionGene->enabled = true;
			genome->getConnectionKeys()->push_back(hiddenConnectionGene->innovation);
			genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(hiddenConnectionGene->innovation, hiddenConnectionGene));
		}
		else if(genome->getNodeGenes()->operator[](curInnov)->type == Output){
			ConnectionGene* outputConnectionGene = new ConnectionGene();
			outputConnectionGene->innovation = population->updateInnovations(AddCascadeNode, GeneTypeConnection, curInnov, nodeGeneInnovation);
			outputConnectionGene->inputId = nodeGeneInnovation;
			outputConnectionGene->outputId = curInnov;
			outputConnectionGene->weight = connectionWeight;
			outputConnectionGene->enabled = true;
			genome->getConnectionKeys()->push_back(outputConnectionGene->innovation);
			genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(outputConnectionGene->innovation, outputConnectionGene));
		}
	}
}

void _ordInsert(std::vector<int>* vec, int x){
	for(int i = 0; i < vec->size(); i++){
		if(x < vec->operator[](i)){
			vec->insert(vec->begin() + i, x);
			return;
		}
	}
	vec->push_back(x);
}
