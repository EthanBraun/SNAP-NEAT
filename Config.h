#ifndef CONFIG_H
#define CONFIG_H

// TODO -- change to static variables instead of macros

#define MUTATION_RATE_ADD_NODE 0.05
#define MUTATION_RATE_ADD_CONNECTION 0.05
#define MUTATION_RATE_PERTURB_BIAS 0.05
#define MUTATION_RATE_PERTURB_WEIGHT 0.05
#define MUTATION_RATE_TOGGLE_NODE 0.01
#define MUTATION_RATE_TOGGLE_CONNECTION 0.01

#define NODE_GENE_MAX_BIAS_PERTURBATION 0.5

#define CONNECTION_GENE_INITIAL_WEIGHT 1.0
#define CONNECTION_GENE_MAX_WEIGHT_PERTURBATION 0.5

#define GENOME_NUM_INPUT_NODES 2
#define GENOME_NUM_OUTPUT_NODES 1

#define POPULATION_SIZE 300
#define POPULATION_PURGE_COUNT 50
#define POPULATION_MAX_GENERATION 1000

#define GENOME_COMPATIBILITY_COEFFICIENT_ONE 1.0
#define GENOME_COMPATIBILITY_COEFFICIENT_TWO 1.0
#define GENOME_COMPATIBILITY_COEFFICIENT_THREE 1.0
#define GENOME_COMPATIBILITY_THRESHOLD 3.0

#endif