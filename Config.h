#ifndef CONFIG_H
#define CONFIG_H

// Macros are default config values
// Config structs are population specific

#define MUTATION_RATE_ADD_NODE 0.005
#define MUTATION_RATE_ADD_CONNECTION 0.05
#define MUTATION_RATE_PERTURB_WEIGHT_GENOME 0.90
#define MUTATION_RATE_SWAP_WEIGHT_CONNECTION 0.01
#define MUTATION_RATE_PERTURB_WEIGHT_CONNECTION 0.85
#define MUTATION_RATE_TOGGLE_NODE 0.001
#define MUTATION_RATE_TOGGLE_CONNECTION 0.001
#define MUTATION_RATE_REMOVE_NODE 0.005
#define MUTATION_RATE_REMOVE_CONNECTION 0.025

#define CONNECTION_GENE_INITIAL_WEIGHT 0.0							//
#define CONNECTION_GENE_MAX_WEIGHT_PERTURBATION 1.5
#define CONNECTION_GENE_ABS_WEIGHT_CAP 6.0							//

//#define GENOME_NUM_INPUT_NODES 16									//
//#define GENOME_NUM_OUTPUT_NODES 4									//
#define GENOME_NUM_INPUT_NODES 2
#define GENOME_NUM_OUTPUT_NODES 1

#define POPULATION_SIZE 500
#define POPULATION_MAX_GENERATION 3000								//
//#define POPULATION_MAX_GENOME_FITNESS 150.0						//
#define POPULATION_MAX_GENOME_FITNESS 0.999999
#define POPULATION_SPECIES_CULL_RATE 0.50							///
#define POPULATION_SPECIES_MAX_STAGNATION 20						//
#define POPULATION_TARGET_SPECIES_NUMBER 5							///
#define POPULATION_SPECIES_SIZE_FOR_ELITISM 5						//
#define POPULATION_CROSSOVER_RATE 0.75								///
#define POPULATION_INITIALIZE_GENOMES_CONNECTED false

#define VERBOSE_LOG false											//
#define RESET_AT_MAX_GENERATION false								//

#define GENOME_COMPATIBILITY_COEFFICIENT_ONE 1.0					//
#define GENOME_COMPATIBILITY_COEFFICIENT_TWO 1.0					//
#define GENOME_COMPATIBILITY_COEFFICIENT_THREE 1.0					//
#define GENOME_INITIAL_COMPATIBILITY_THRESHOLD 4.0					//

#define GENOME_COMPATIBILITY_THRESHOLD_PERTURBATION_AMOUNT 0.05		//


struct Config{
	double mutationRateAddNode;
	double mutationRateAddConnection;
	double mutationRatePerturbWeightGenome;
	double mutationRateSwapWeightConnection;
	double mutationRatePerturbWeightConnection;
	double mutationRateToggleNode;
	double mutationRateToggleConnection;
	double mutationRateRemoveNode;
	double mutationRateRemoveConnection;

	double connectionGeneInitialWeight;							//
	double connectionGeneMaxWeightPerturbation;
	double connectionGeneAbsWeightCap;							//
	
	int genomeNumInputNodes;									//
	int genomeNumOutputNodes;									//

	int populationSize;											//
	int populationMaxGeneration;								//
	double populationMaxGenomeFitness;							//
	double populationSpeciesCullRate;							//
	int populationSpeciesMaxStagnation;							//
	int populationTargetSpeciesNumber;							//
	int populationSpeciesSizeForElitism;						//
	double populationCrossoverRate;								///
	bool populationInitializeGenomesConnected;					//

	bool verboseLog;											//
	bool resetAtMaxGeneration;									//

	double genomeCompatibilityCoefficientOne;					//
	double genomeCompatibilityCoefficientTwo;					//
	double genomeCompatibilityCoefficientThree;					//
	double genomeInitialCompatibilityThreshold;					//
	double adjustedGenomeCompatibilityThreshold;				//

	double genomeCompatibilityThresholdPerturbationAmount;		//
};

void initializeDefaultConfig(Config*);

#endif
