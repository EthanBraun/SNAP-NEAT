#include "Config.h" 
void initializeDefaultConfig(Config* config){
	//config->mutationRateAddNode = MUTATION_RATE_ADD_NODE;
	//config->mutationRateAddConnection = MUTATION_RATE_ADD_CONNECTION;
	//config->mutationRatePerturbWeightGenome = MUTATION_RATE_PERTURB_WEIGHT_GENOME;
	config->mutationRateCore = MUTATION_RATE_CORE;
	config->mutationRateSwapWeightConnection = MUTATION_RATE_SWAP_WEIGHT_CONNECTION;
	config->mutationRatePerturbWeightConnection = MUTATION_RATE_PERTURB_WEIGHT_CONNECTION;
	config->mutationRateToggleNode = MUTATION_RATE_TOGGLE_NODE;
	config->mutationRateToggleConnection = MUTATION_RATE_TOGGLE_CONNECTION;
	config->mutationRateRemoveNode = MUTATION_RATE_REMOVE_NODE;
	config->mutationRateRemoveConnection = MUTATION_RATE_REMOVE_CONNECTION;
	//config->mutationRateAddRbfNode = MUTATION_RATE_ADD_RBF_NODE;
	config->mutationRatePerturbRbfNodeGenome = MUTATION_RATE_PERTURB_RBF_NODE_GENOME;
	config->mutationRatePerturbRbfNodeCenter = MUTATION_RATE_PERTURB_RBF_NODE_CENTER;
	config->mutationRatePerturbRbfNodeRadius = MUTATION_RATE_PERTURB_RBF_NODE_RADIUS;
	//config->mutationRateAddCascadeNode = MUTATION_RATE_ADD_CASCADE_NODE;

	config->connectionGeneInitialWeight = CONNECTION_GENE_INITIAL_WEIGHT;
	config->connectionGeneMaxWeightPerturbation = CONNECTION_GENE_MAX_WEIGHT_PERTURBATION;
	config->connectionGeneAbsWeightCap = CONNECTION_GENE_ABS_WEIGHT_CAP;

	config->rbfNodeInitialCenter = RBF_NODE_INITIAL_CENTER;
	config->rbfNodeInitialRadius = RBF_NODE_INITIAL_RADIUS;
	config->rbfNodeMaxCenterPerturbation = RBF_NODE_MAX_CENTER_PERTURBATION;
	config->rbfNodeMaxRadiusPerturbation = RBF_NODE_MAX_RADIUS_PERTURBATION;
	config->rbfNodeAbsCenterCap = RBF_NODE_ABS_CENTER_CAP;
	config->rbfNodeMinRadius = RBF_NODE_MIN_RADIUS;
	config->rbfNodeMaxRadius = RBF_NODE_MAX_RADIUS;

	config->operatorValueLearningRate = OPERATOR_VALUE_LEARNING_RATE;
	config->operatorProbLearningRate = OPERATOR_PROB_LEARNING_RATE;
	config->initializationPeriod = INITIALIZATION_PERIOD;
	config->minOpProb = MIN_OP_PROB;

	config->genomeNumInputNodes = GENOME_NUM_INPUT_NODES;
	config->genomeNumOutputNodes = GENOME_NUM_OUTPUT_NODES;

	config->populationSize = POPULATION_SIZE;
	config->populationMaxGeneration = POPULATION_MAX_GENERATION;
	config->populationMaxGenomeFitness = POPULATION_MAX_GENOME_FITNESS;
	config->populationSpeciesCullRate = POPULATION_SPECIES_CULL_RATE;
	config->populationSpeciesMaxStagnation = POPULATION_SPECIES_MAX_STAGNATION;
	config->populationTargetSpeciesNumber = POPULATION_TARGET_SPECIES_NUMBER;
	config->populationSpeciesSizeForElitism = POPULATION_SPECIES_SIZE_FOR_ELITISM;
	config->populationCrossoverRate = POPULATION_CROSSOVER_RATE;
	config->populationInitializeGenomesConnected = POPULATION_INITIALIZE_GENOMES_CONNECTED;

	config->verboseLog = VERBOSE_LOG;
	config->resetAtMaxGeneration = RESET_AT_MAX_GENERATION;									

	config->genomeCompatibilityCoefficientOne = GENOME_COMPATIBILITY_COEFFICIENT_ONE;
	config->genomeCompatibilityCoefficientTwo = GENOME_COMPATIBILITY_COEFFICIENT_TWO;
	config->genomeCompatibilityCoefficientThree = GENOME_COMPATIBILITY_COEFFICIENT_THREE;					
	config->genomeInitialCompatibilityThreshold = GENOME_INITIAL_COMPATIBILITY_THRESHOLD;
	config->adjustedGenomeCompatibilityThreshold = GENOME_INITIAL_COMPATIBILITY_THRESHOLD;				

	config->genomeCompatibilityThresholdPerturbationAmount = GENOME_COMPATIBILITY_THRESHOLD_PERTURBATION_AMOUNT;		
}
