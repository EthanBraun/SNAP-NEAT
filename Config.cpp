#include "Config.h"

void initializeDefaultConfig(Config* config){
	config->mutationRateAddNode = MUTATION_RATE_ADD_NODE;
	config->mutationRateAddConnection = MUTATION_RATE_ADD_CONNECTION;
	config->mutationRatePerturbWeightGenome = MUTATION_RATE_PERTURB_WEIGHT_GENOME;
	config->mutationRateSwapWeightConnection = MUTATION_RATE_SWAP_WEIGHT_CONNECTION;
	config->mutationRatePerturbWeightConnection = MUTATION_RATE_PERTURB_WEIGHT_CONNECTION;
	config->mutationRateToggleNode = MUTATION_RATE_TOGGLE_NODE;
	config->mutationRateToggleConnection = MUTATION_RATE_TOGGLE_CONNECTION;
	config->mutationRateRemoveNode = MUTATION_RATE_REMOVE_NODE;
	config->mutationRateRemoveConnection = MUTATION_RATE_REMOVE_CONNECTION;

	config->connectionGeneInitialWeight = CONNECTION_GENE_INITIAL_WEIGHT;
	config->connectionGeneMaxWeightPerturbation = CONNECTION_GENE_MAX_WEIGHT_PERTURBATION;
	config->connectionGeneAbsWeightCap = CONNECTION_GENE_ABS_WEIGHT_CAP;

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