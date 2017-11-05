#include "Activation.h"

double sigmoid(double x){
	return (1.0 / (1.0 + exp(-x)));
}

double steepSigmoid(double x){
	return (1.0 / (1.0 + exp(-5.0 * x)));
}