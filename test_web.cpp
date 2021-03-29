#include "function.hpp"
#include "web.hpp"
#include "web_file_handle.hpp"
#include "backpropagation.hpp"
#include "bytestoarray.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <random>
#include <chrono>
#include <cstring>
#include <thread>

#define e 2.718281828459045235360

using std::cout;
using std::endl;
using namespace std::chrono;

class simoigde : public function
{
	public:
	virtual double evaluate(double x)
	{
		return 1/(  1+( 1/std::pow(e,x) )  );
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

int main(int argc, char **argv)
{
	#ifdef LOAD_FILE
	if(argc<2)
	{
		std::cout<<"Ingresar archivo de red";
		return 0;
	}
	char* filename = argv[1];

	#endif //LOAD_FILE


	std::uniform_real_distribution<double> web_distribution(0.0,0.3);
	std::mt19937_64 web_generator(std::chrono::system_clock::now().time_since_epoch().count());

	int input_size_f{};
	int output_size_f{};
	int n_patrons{};
	double** inputs_patron = bytes2array::file2array("input/Bitcoin.bin",n_patrons,input_size_f);
	double** output_patron = bytes2array::file2array("output/Bitcoin.bin",n_patrons,output_size_f);

	cout<<input_size_f<<endl;
	cout<<output_size_f<<endl;

	unsigned int num_layer = 5;

	unsigned int* layers_size = new unsigned int[num_layer];
	if (num_layer == 3)
		{layers_size[0] = 2;layers_size[1] = 4;layers_size[2] = 2;}
	else
		{layers_size[0] = input_size_f;layers_size[1] = 30;layers_size[2] = 30;layers_size[3] = 30;layers_size[4] = output_size_f;}

	function* identity_f = new identity();
	function* simoigde_f = new simoigde();
	function*** p_input =  new function**[num_layer];
	function*** p_activ =  new function**[num_layer];
	function*** p_outpu =  new function**[num_layer];
	#ifndef LOAD_FILE
	double** umbral = new double*[num_layer];
	#endif //LOAD_FILE
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
			umbral[i][j]  = 0.0;
			#endif //LOAD_FILE
		}
	}
#ifndef LOAD_FILE
	web myweb(num_layer,layers_size,p_input,p_activ,p_outpu,umbral,web_distribution,web_generator);
#else //LOAD_FILE
	web myweb = load(filename, p_input, p_activ, p_outpu);
#endif //LOAD_FILE
	delete[] layers_size;
	/*
	double inputs[] = {0.2,0.5};
	double** inputs_patron;
	inputs_patron = new double*[4];
		inputs_patron[0]= new double[2]{0.0,0.0};
		inputs_patron[1]= new double[2]{1.0,0.0};
		inputs_patron[2]= new double[2]{0.0,1.0};
		inputs_patron[3]= new double[2]{1.0,1.0};
	double** outputs_patron;
	output_patron = new double*[4];
		output_patron[0]= new double[2]{0.0,0.0};
		output_patron[1]= new double[2]{0.0,1.0};
		output_patron[2]= new double[2]{0.0,1.0};
		output_patron[3]= new double[2]{1.0,1.0};
	*/


	std::thread mythread([&myweb,&inputs_patron,&output_patron,n_patrons](){
		std::cout<<backpropagation::train(
			myweb,
			inputs_patron,
			output_patron,
			n_patrons,
			0.001
			)<<std::endl;
	});

	cout<<"press enter:"<<endl;
	std::cin.get();
	backpropagation::force_end_training();

	mythread.join();
	/*
	delete[] inputs_patron[0];
	delete[] inputs_patron[1];
	delete[] inputs_patron[2];
	delete[] inputs_patron[3];
	delete[] outputs_patron[0];
	delete[] outputs_patron[1];
	delete[] outputs_patron[2];
	delete[] outputs_patron[3];
	delete[] inputs_patron;
	delete[] outputs_patron;
	auto start = high_resolution_clock::now(); 	
	double *outputs = myweb.Evaluate(inputs);
	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<microseconds>(stop - start); 

	unsigned int output_size = myweb.get_output_size();

	for (unsigned int i = 0; i < output_size; ++i)
		cout<<"Output "<<i<<":"<<outputs[i]<<endl;
	
	cout <<"Duration:"<< duration.count() << endl; 
	*/
	
	for(unsigned int i = 0; i < num_layer; ++i)
	{
		#ifndef LOAD_FILE
		delete[] umbral[i];
		#endif //LOAD_FILE
		delete[] p_input[i];
		delete[] p_activ[i];
		delete[] p_outpu[i];
	}
	#ifndef LOAD_FILE
	delete[] umbral;
	#endif //LOAD_FILE
	delete[] p_input;
	delete[] p_activ;
	delete[] p_outpu;
	delete identity_f;
	delete simoigde_f;

	bytes2array::freearray(inputs_patron,n_patrons);
	bytes2array::freearray(output_patron,n_patrons);

#ifndef LOAD_FILE
	save(myweb);
#endif //LOAD_FILE

	//print_web(myweb);
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

