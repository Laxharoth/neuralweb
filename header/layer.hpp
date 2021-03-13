#ifndef LAYER_HPP
#define LAYER_HPP 1

#include <cstring>
#include <thread>

#include "neuron.hpp"
#include "function.hpp"

class layer
{
public:
	static const bool IS_FIRST_LAYER;


	layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral, unsigned int previous_layer_neurons_number,rand_dist &distribution,rand_gen &generator);
	layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral, unsigned int previous_layer_neurons_number, double** p_weights);
	layer();
	~layer();

	neuron* get_neuron(unsigned int position);
	void set_inputs(double* p_inputs);
	void put_outputs(double* outputs);
	double* get_inputs();

	static layer build_first_layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral);
private:
	static const unsigned int FIRST_LAYER_PREVIOUS_NEURON_NUMBER;
	
	neuron* neurons;
	double* inputs;
	unsigned int neurons_number;
	unsigned int previous_layer_neurons_number;
	
	layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral);
	
	void evaluate_neuron_into_output(unsigned int position, double*& output);
};

#endif