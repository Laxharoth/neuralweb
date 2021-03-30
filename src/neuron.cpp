#include "neuron.hpp"

neuron::neuron(function* p_input_function, function* p_activation_function, function* p_output_function, double* p_inputs, unsigned int p_inputs_number, double p_umbral,rand_dist &distribution,rand_gen &generator): 
input_function(p_input_function),
activation_function(p_activation_function),
output_function(p_output_function),
inputs(p_inputs),
inputs_number(p_inputs_number),
umbral(p_umbral),
last_activation(0.0)
{
	const int v_prev_layer_size = this->inputs_number;
	this->weights = new double[v_prev_layer_size];
	for (unsigned int i = 0; i < (v_prev_layer_size); ++i)
		weights[i] = distribution(generator);
}
neuron::neuron(function* p_input_function, function* p_activation_function, function* p_output_function, double* p_inputs, unsigned int p_inputs_number, double p_umbral,double* p_weights): 
input_function(p_input_function),
activation_function(p_activation_function),
output_function(p_output_function),
inputs(p_inputs),
inputs_number(p_inputs_number),
umbral(p_umbral),
last_activation(0.0)
{
	this->weights = new double[(this->inputs_number)];
	std::memcpy(this->weights,p_weights,sizeof(double)*(this->inputs_number));
}
neuron::~neuron()
{
	delete[] weights;
}

double neuron::Input()
{
	const int v_prev_layer_size = this->inputs_number;
	double sum = 0.0;
	double* inputs = this->inputs;
	for (unsigned int i = 0; i < v_prev_layer_size; ++i)
		sum += this->input_function->evaluate(inputs[i]*weights[i]);
	return sum+this->umbral;
}
double neuron::Activation()
{
	this->last_activation = this->activation_function->evaluate(this->Input());
	return this->last_activation;
}
double neuron::Output()
{
	return this->output_function->evaluate(this->Activation());
}
unsigned int neuron::get_inputs_number()
{
	return this->inputs_number;
}
double neuron::get_umbral()
{
	return this->umbral;
}
double neuron::get_weight(unsigned int position)
{
	if(position>=this->inputs_number) return 0;
	return this->weights[position];
}
void neuron::set_weight(unsigned int position, double new_weight)
{
	if(position>=this->inputs_number) return;
	this->weights[position] = new_weight;
}
double neuron::get_input(unsigned int position)
{
	return this->inputs[position];
}
double neuron::get_last_activation()
{
	return this->last_activation;
}