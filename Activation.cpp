#include "Activation.h"

double sigmoid(double x){
	return (1.0 / (1.0 + exp(-x)));
}

double steepSigmoid(double x){
	return (1.0 / (1.0 + exp(-5.0 * x)));
}

double radialBasisFunction(double x, double c, double r){
	return exp(-pow((x - c), 2) / pow(r, 2));
}
