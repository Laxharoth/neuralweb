#include "function.hpp"
#include "web.hpp"
#include "web_file_handle.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <random>
#include <chrono>
#include <cstring>

#define e 2.718281828459045235360

using std::cout;
using std::endl;
using namespace std::chrono;

class simoigde : public function
{
	public:
	virtual double evaluate(double x)
	{
		return 1/(1+std::pow(e,-x));
	}
};
class identity : public function
{
	public:
	virtual double evaluate(double x)
	{
		return x;
	}
};

void print_web(web &this_web);

int main()
{
	std::uniform_real_distribution<double> web_distribution(0.0,0.3);
	std::mt19937_64 web_generator(std::chrono::system_clock::now().time_since_epoch().count());

	unsigned int num_layer = 4;

	unsigned int* layers_size = new unsigned int[num_layer];
	layers_size[0] = 2;layers_size[1] = 80;layers_size[2] = 50;layers_size[3] = 16;

	function* identity_f = new identity();
	function* simoigde_f = new simoigde();
	function*** p_input =  new function**[num_layer];
	function*** p_activ =  new function**[num_layer];
	function*** p_outpu =  new function**[num_layer];

	double** umbral = new double*[num_layer];

	for(unsigned int i = 0; i < num_layer; ++i)
	{
		p_input[i] = new function*[layers_size[i]];
		p_activ[i] = new function*[layers_size[i]];
		p_outpu[i] = new function*[layers_size[i]];
		#ifndef LOAD_FILE
		umbral[i] = new double[layers_size[i]];
		#endif //LOAD_FILE
		for (int j = 0; j < layers_size[i]; ++j)
		{
			p_input[i][j] = identity_f;
			p_activ[i][j] = simoigde_f;
			p_outpu[i][j] = identity_f;
			#ifndef LOAD_FILE
			umbral[i][j]  = 0.5;
			#endif //LOAD_FILE
		}
	}
#ifndef LOAD_FILE
	web myweb(num_layer,layers_size,p_input,p_activ,p_outpu,umbral,web_distribution,web_generator);
#else //LOAD_FILE
	web myweb = load("20210315162310.dat", p_input, p_activ, p_outpu);
#endif //LOAD_FILE
	double inputs[] = {0.2,0.5};

	auto start = high_resolution_clock::now(); 	
	double *outputs = myweb.Evaluate(inputs);
	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<microseconds>(stop - start); 

	unsigned int output_size = myweb.get_output_size();

	for (unsigned int i = 0; i < output_size; ++i)
		cout<<"Output "<<i<<":"<<outputs[i]<<endl;

	cout <<"Duration:"<< duration.count() << endl; 
#ifndef LOAD_FILE
	save(myweb);
#endif //LOAD_FILE

	print_web(myweb);
	cout<<"end"<<endl;

	return 0;
}


void print_web(web &this_web)
{
	unsigned int n_layers = this_web.get_num_layers();
	unsigned int input_size{};
	unsigned int neuron_size{};
	double current_weight{};
	double current_umbral{};
	layer* current_layer{};
	neuron* current_neuron{};

	cout<<"Number of layers:"<<n_layers<<endl;
	for (unsigned int i = 0; i < n_layers; ++i)
	{
		//Get layer number of neurons in layer
		current_layer = this_web.get_layer(i);
		neuron_size = current_layer->get_layer_size();
		cout<<"\tLayer Number:"<<i<<endl;
		cout<<"\t\tNumber of Neurons:"<<neuron_size<<endl;
		for (unsigned int j = 0; j < neuron_size; ++j)
		{
			//Get number of inputs(weights) in neuron
			current_neuron = current_layer->get_neuron(j);
			input_size = current_neuron->get_inputs_number();
			current_umbral = current_neuron->get_umbral();
			cout<<"\t\tNeuron Number"<<j<<endl;
			cout<<"\t\t\tNeuron :"<<input_size<<endl;
			cout<<"\t\t\tNumber of Inputs:"<<input_size<<endl;
			cout<<"\t\t\tUmbral:"<<current_umbral<<endl;
			for (int k = 0; k < input_size; ++k)
			{
				current_weight = current_neuron->get_weight(k);
				cout<<"\t\t\t\tWeight "<<k<<":"<<current_weight<<endl;
			}
		}
	}
}

