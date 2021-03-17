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
	function* input_function;
	function* activation_function;
	function* output_function;

	neuron(function* p_input_function, function* p_activation_function, function* p_output_function, double* p_inputs, unsigned int p_previous_layer_neurons_number, double p_umbral,rand_dist &distribution,rand_gen &generator);
	neuron(function* p_input_function, function* p_activation_function, function* p_output_function, double* p_inputs, unsigned int p_previous_layer_neurons_number, double p_umbral, double* p_weights);
	~neuron();
	
	double Input();
	double Activation();
	double Output();
	unsigned int get_inputs_number();
	double get_umbral();
	double get_weight(unsigned int position);
	void set_weight(unsigned int position, double new_weight);
	double get_input(unsigned int position);
	double get_last_activation();

private:
	unsigned int previous_layer_neurons_number;
	double* weights;
	double* inputs;
	double umbral;
	double last_activation;

};
#endif // NEURON