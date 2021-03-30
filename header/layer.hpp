#ifndef LAYER_HPP
#define LAYER_HPP 1

#include <cstring>
#include <thread>

#include "neuron.hpp"
#include "function.hpp"

class layer
{
public:
	layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral, unsigned int inputs_number,rand_dist &distribution,rand_gen &generator);
	layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral, unsigned int inputs_number, double** p_weights);
	
	~layer();

	unsigned int get_layer_size();
	neuron* get_neuron(unsigned int position);
	void set_inputs(double* p_inputs);
	void put_outputs(double* outputs);
	double* get_inputs();

	static layer* build_first_layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral);
private:
	static const unsigned int FIRST_LAYER_INPUTS_NUMBER;
	
	neuron** neurons;
	double* inputs;
	unsigned int neurons_number;
	unsigned int inputs_number;
		
	layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral);
	void evaluate_neuron_into_output(unsigned int position, double*& output);
};

#endif