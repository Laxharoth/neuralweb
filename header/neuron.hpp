#ifndef NEURON
#define NEURON 1

#include <cstring>
#include <random>

#include "function.hpp"

typedef std::uniform_real_distribution<double> rand_dist;
typedef std::mt19937_64 rand_gen;

class neuron
{
public:
	neuron(function* p_input_function, function* p_activation_function, function* p_output_function, double* p_inputs, unsigned int p_previous_layer_neurons_number, double p_umbral,rand_dist &distribution,rand_gen &generator);
	neuron(function* p_input_function, function* p_activation_function, function* p_output_function, double* p_inputs, unsigned int p_previous_layer_neurons_number, double p_umbral, double* p_weights);
	neuron();
	~neuron();
	
	double Output();
	double get_weight(unsigned int position);
	void set_weight(unsigned int position, double new_weight);
private:
	function* input_function;
	function* activation_function;
	function* output_function;
	unsigned int previous_layer_neurons_number;
	double* weights;
	double* inputs;
	double umbral;
	double last_output;

	double Input();
	double Activation();
};
#endif // NEURON