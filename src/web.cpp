#include "web.hpp"

web::web(unsigned int p_num_layers,unsigned int* p_layers_size, function*** p_function_input, function*** p_function_activation, function*** p_function_output,double** p_umbral,rand_dist &distribution,rand_gen &generator):
num_layers(p_num_layers),
input_size(p_layers_size[0]),
output_size(p_layers_size[p_num_layers-1])
{
	this->layers = new layer[this->num_layers];
	this->inputs = new double*[this->num_layers+1];
	this->layers[0] = layer::build_first_layer(p_layers_size[0],p_function_input[0],p_function_activation[0],p_function_output[0],p_umbral[0]);
	this->inputs[0] = this->layers[0].get_inputs();
	for (unsigned int i = 1; i < this->num_layers; ++i)
	{
		this->layers[i] = layer(p_layers_size[i],p_function_input[i],p_function_activation[i],p_function_output[i],p_umbral[i],p_layers_size[i-1],distribution,generator);
		this->inputs[i] = this->layers[i].get_inputs();
	}
	this->inputs[this->num_layers] = new double[this->output_size];
}
web::web(unsigned int p_num_layers,unsigned int* p_layers_size, function*** p_function_input, function*** p_function_activation, function*** p_function_output,double** p_umbral,double*** p_weights):
num_layers(p_num_layers),
input_size(p_layers_size[0]),
output_size(p_layers_size[p_num_layers-1])
{
	this->layers = new layer[this->num_layers];
	this->inputs = new double*[this->num_layers+1];
	this->layers[0] = layer::build_first_layer(p_layers_size[0],p_function_input[0],p_function_activation[0],p_function_output[0],p_umbral[0]);
	this->inputs[0] = this->layers[0].get_inputs();
	for (unsigned int i = 1; i < this->num_layers; ++i)
	{
		this->layers[i] = layer(p_layers_size[i],p_function_input[i],p_function_activation[i],p_function_output[i],p_umbral[i],p_layers_size[i-1],p_weights[i]);
		this->inputs[i] = this->layers[i].get_inputs();
	}
	this->inputs[this->num_layers] = new double[this->output_size];
}
web::~web()
{
	delete[] this->layers;
	delete[] this->inputs[this->output_size];
	delete[] this->inputs;
}

double* web::Evaluate(double* p_inputs)
{
	this->layers[0].set_inputs(p_inputs);
	for (unsigned int i = 0; i < this->num_layers; ++i)
		this->layers[i].put_outputs(this->inputs[i+1]);
	return this->inputs[this->num_layers];
}

unsigned int web::get_input_size()
{
	return this->input_size;
}
unsigned int web::get_output_size()
{
	return this->input_size;
}
double* web::get_outputs()
{
	return this->inputs[this->num_layers];
}
layer* web::get_layer(int position)
{
	if(position>=this->num_layers)return nullptr;
	return this->layers+position;
}
