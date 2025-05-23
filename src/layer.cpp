#include "layer.hpp"

const unsigned int layer::FIRST_LAYER_INPUTS_NUMBER=1;

layer::layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral, unsigned int p_inputs_number,rand_dist &distribution,rand_gen &generator):
neurons_number(p_neuron_number),
inputs_number(p_inputs_number)
{
	this->neurons = new neuron*[p_neuron_number];
	this->inputs  = new double[p_inputs_number];
	
	for (unsigned int i = 0; i < this->neurons_number; ++i)
	{
		this->neurons[i] = new neuron(p_function_input[i],
			p_function_activation[i],
			p_function_output[i],
			this->inputs,
			this->inputs_number,
			p_umbral[i],
			distribution,
			generator
		);
	}
}
layer::layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral, unsigned int p_inputs_number, double** p_weights):
neurons_number(p_neuron_number),
inputs_number(p_inputs_number)
{
	this->neurons = new neuron*[this->neurons_number];
	this->inputs  = new double[this->inputs_number];

	for (unsigned int i = 0; i < p_neuron_number; ++i)
	{
		this->neurons[i] = new neuron(p_function_input[i],
			p_function_activation[i],
			p_function_output[i],
			this->inputs,
			this->inputs_number,
			p_umbral[i],
			p_weights[i]
		);
	}
}
layer::layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral):
neurons_number(p_neuron_number),
inputs_number(p_neuron_number)
{
	this->neurons = new neuron*[this->neurons_number];
	this->inputs  = new double[this->inputs_number];
	
	double weights[this->inputs_number]{};
	for (unsigned int i = 0; i < this->inputs_number; ++i) 
		weights[i] = 1.0;

	for (unsigned int i = 0; i < this->neurons_number; ++i)
	{
		this->neurons[i] = new neuron(p_function_input[i],
			p_function_activation[i],
			p_function_output[i],
			this->inputs+i,
			FIRST_LAYER_INPUTS_NUMBER,
			p_umbral[i],
			weights
			);
	}
}

layer* layer::build_first_layer(unsigned int p_neuron_number,function** p_function_input, function** p_function_activation, function** p_function_output, double* p_umbral)
{return new layer(p_neuron_number,p_function_input,p_function_activation,p_function_output,p_umbral);}

unsigned int layer::get_layer_size()
{
	return this->neurons_number;
}
neuron* layer::get_neuron(const unsigned int position)
{
	if(position>=this->neurons_number) return nullptr;
	return this->neurons[position];
}
double* layer::get_inputs()
{
	return this->inputs;
}
void layer::set_inputs(double* p_inputs)
{
	std::memcpy(this->inputs,p_inputs,sizeof(double)*(this->inputs_number));
}
void layer::put_outputs(double* outputs)
{
	const int neurons_number=this->neurons_number;
	std::thread threads[neurons_number];
	for (unsigned int i = 0; i < neurons_number; ++i)
		threads[i] = std::thread([outputs,i,this](){outputs[i] = this->neurons[i]->Output();});
	for (auto& t:threads)
		t.join();
}

layer::~layer()
{
	for (int i = 0; i < this->neurons_number; ++i)
		delete this->neurons[i];
	delete[] neurons;
	delete[] inputs;
}