#ifndef WEB_HPP
#define WEB_HPP 1

#include "function.hpp"
#include "layer.hpp"

class web
{
public:
	web(unsigned int p_num_layers,unsigned int* p_layers_size, function*** p_function_input, function*** p_function_activation, function*** p_function_output,double** p_umbral,rand_dist &distribution,rand_gen &generator);
	web(unsigned int p_num_layers,unsigned int* p_layers_size, function*** p_function_input, function*** p_function_activation, function*** p_function_output,double** p_umbral, double*** p_weights);
	
	~web();

	double* Evaluate(double* p_inputs);
	
	unsigned int get_num_layers();
	unsigned int get_input_size();
	unsigned int get_output_size();
	double* get_outputs();
	layer* get_layer(int position);

private:
	layer** layers;
	double** inputs;
	unsigned int input_size;
	unsigned int output_size;
	unsigned int num_layers;
};
#endif // WEB_HPP