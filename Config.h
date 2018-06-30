#ifndef CONFIG_H
#define CONFIG_H

// Macros are default config values
// Config structs are population specific

//#define MUTATION_RATE_ADD_NODE 0.03
//#define MUTATION_RATE_ADD_CONNECTION 0.20
//#define MUTATION_RATE_PERTURB_WEIGHT_GENOME 0.80
#define MUTATION_RATE_CORE 0.05
#define MUTATION_RATE_SWAP_WEIGHT_CONNECTION 0.10
#define MUTATION_RATE_PERTURB_WEIGHT_CONNECTION 0.90
#define MUTATION_RATE_TOGGLE_NODE 0.00
#define MUTATION_RATE_TOGGLE_CONNECTION 0.00
#define MUTATION_RATE_REMOVE_NODE 0.00
#define MUTATION_RATE_REMOVE_CONNECTION 0.00
//#define MUTATION_RATE_ADD_RBF_NODE 0.025
#define MUTATION_RATE_PERTURB_RBF_NODE_GENOME 0.75
#define MUTATION_RATE_PERTURB_RBF_NODE_CENTER 0.80
#define MUTATION_RATE_PERTURB_RBF_NODE_RADIUS 0.80
//#define MUTATION_RATE_ADD_CASCADE_NODE 0.025


#define CONNECTION_GENE_INITIAL_WEIGHT 1.0							//
#define CONNECTION_GENE_MAX_WEIGHT_PERTURBATION 2.0
#define CONNECTION_GENE_ABS_WEIGHT_CAP 20.0							//

#define RBF_NODE_INITIAL_CENTER 0.0
#define RBF_NODE_INITIAL_RADIUS 1.0
#define RBF_NODE_MAX_CENTER_PERTURBATION 1.0 
#define RBF_NODE_MAX_RADIUS_PERTURBATION 1.0
#define RBF_NODE_ABS_CENTER_CAP 10.0
#define RBF_NODE_MIN_RADIUS 0.0001
#define RBF_NODE_MAX_RADIUS 10.0

#define OPERATOR_VALUE_LEARNING_RATE 0.05
#define OPERATOR_PROB_LEARNING_RATE 0.05
#define INITIALIZATION_PERIOD 10000
#define MIN_OP_PROB 0.10

#define GENOME_NUM_INPUT_NODES 2
#define GENOME_NUM_OUTPUT_NODES 1


#define POPULATION_SIZE 1000
#define POPULATION_MAX_GENERATION 200								//
//#define POPULATION_MAX_GENOME_FITNESS 99.99
#define POPULATION_MAX_GENOME_FITNESS 0.999998
#define POPULATION_SPECIES_CULL_RATE 0.50							///
#define POPULATION_SPECIES_MAX_STAGNATION 15						//
#define POPULATION_TARGET_SPECIES_NUMBER 15							///
#define POPULATION_SPECIES_SIZE_FOR_ELITISM 5						//
#define POPULATION_CROSSOVER_RATE 0.75								///
#define POPULATION_INITIALIZE_GENOMES_CONNECTED true

#define VERBOSE_LOG false											//
#define RESET_AT_MAX_GENERATION false								//

#define GENOME_COMPATIBILITY_COEFFICIENT_ONE 1.0					//
#define GENOME_COMPATIBILITY_COEFFICIENT_TWO 1.0					//
#define GENOME_COMPATIBILITY_COEFFICIENT_THREE 1.0					//
#define GENOME_INITIAL_COMPATIBILITY_THRESHOLD 4.0					//

//#define GENOME_COMPATIBILITY_THRESHOLD_PERTURBATION_AMOUNT 0.05		//
#define GENOME_COMPATIBILITY_THRESHOLD_PERTURBATION_AMOUNT 0.10

struct Config{
	//double mutationRateAddNode;
	//double mutationRateAddConnection;
	//double mutationRatePerturbWeightGenome;
	double mutationRateCore;
	double mutationRateSwapWeightConnection;
	double mutationRatePerturbWeightConnection;
	double mutationRateToggleNode;
	double mutationRateToggleConnection;
	double mutationRateRemoveNode;
	double mutationRateRemoveConnection;
	//double mutationRateAddRbfNode;
	double mutationRatePerturbRbfNodeGenome;
	double mutationRatePerturbRbfNodeCenter;
	double mutationRatePerturbRbfNodeRadius;
	//double mutationRateAddCascadeNode;

	double connectionGeneInitialWeight;							//
	double connectionGeneMaxWeightPerturbation;
	double connectionGeneAbsWeightCap;							//

	double rbfNodeInitialCenter;
	double rbfNodeInitialRadius;
	double rbfNodeMaxCenterPerturbation;
	double rbfNodeMaxRadiusPerturbation;
	double rbfNodeAbsCenterCap;
	double rbfNodeMinRadius;
	double rbfNodeMaxRadius;

	double operatorValueLearningRate;
	double operatorProbLearningRate;
	int initializationPeriod;
	double minOpProb;
	
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
