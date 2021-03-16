#include "function.hpp"
#include "web.hpp"

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

class mylogfunction : public function
{
public:
	virtual double evaluate(double x)
	{
		return std::log(x);
	}
};
class myexpfunction : public function
{
public:
	virtual double evaluate(double x)
	{
		return std::pow(e,x);
	}
};
class mytowerfunction : public function
{
public:
	virtual double evaluate(double x)
	{
		return std::pow(x,x);
	}
};
class myfibonaccifunction : public function
{
public:
	myfibonaccifunction(){}
	virtual double evaluate(double x)
	{
		int x_floor = std::floor(x);
		if(x_floor <= Fibonacci_Numbers.size() - 1 )
			return Fibonacci_Numbers[x_floor];
		else
		{
			double Fibonacci_Number = this->evaluate(x_floor-1)+this->evaluate(x_floor-2);
			Fibonacci_Numbers.push_back(Fibonacci_Number);
			return Fibonacci_Number;
		}
	}
private:
	static std::vector<double> Fibonacci_Numbers;
};
std::vector<double> myfibonaccifunction::Fibonacci_Numbers(2,1);

class myfibonaccifunction_slow : public function
{
public:
	virtual double evaluate(double x)
	{
		double x_floor = std::floor(x);
		if(x_floor == 0 || x_floor == 1)
			return 1;
		else
			return this->evaluate(x_floor-2)+this->evaluate(x_floor-1);
	}
private:
};

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

void print_web(web *this_web)
{
	unsigned int n_layers = this_web->get_num_layers();
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
		current_layer = this_web->get_layer(i);
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
	//Close file
}

int main()
{
#ifdef TEST_FUNCTION
	int choose{};
	bool run{true};
	function* f = nullptr;
	std::vector<double> results;
	
	while(run)
	{
		cout<<"Select the type of function:"<<endl;
		cout<<"\t1: Natural log"<<endl;
		cout<<"\t2: Expotent e^x"<<endl;
		cout<<"\t3: Tower function (x^x)"<<endl;
		cout<<"\t4: Fibonacci"<<endl;
		cout<<"\t5: Fibonacci slow"<<endl;
		std::cin>>choose;

		switch(choose)
		{
			case 1: 
			f = new mylogfunction();
			run = false;
			break;
			case 2:
			f = new myexpfunction();
			run = false;
			break;
			case 3:
			f = new mytowerfunction();
			run = false;
			break;
			case 4:
			f = new myfibonaccifunction();
			run = false;
			break;
			case 5:
			f = new myfibonaccifunction_slow();
			run = false;
			break;
			default: 
			std::system("CLS");
			cout<<"Invalid value"<<endl;
		}
	}
	
	function* p_df{};
	function* p_ddf{};
	function* p_dddf{};

	p_df =  f->derivative();
	p_ddf= p_df->derivative();
	p_dddf= p_ddf->derivative();
	auto start = high_resolution_clock::now(); 
	for (int i = 0; i < 30; ++i)
	{
		results.push_back(f->evaluate(i));
		results.push_back(p_df->evaluate(i));
		results.push_back(p_ddf->evaluate(i));
		results.push_back(p_dddf->evaluate(i));
	}
	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<nanoseconds>(stop - start); 
	for (int i = 0; i < results.size()/4; ++i)
	{
		cout<<"f("<<i<<")   ="<<results[i*4]<<endl;
		cout<<"f'("<<i<<")  ="<<results[i*4+1]<<endl;
		cout<<"f''("<<i<<") ="<<results[i*4+2]<<endl;
		cout<<"f'''("<<i<<")="<<results[i*4+3]<<endl<<endl;
	}

	cout <<"Duration:"<< duration.count() << endl; 
#endif // TEST_FUNCTION
#ifdef TEST_RANDOM
	int t = 5;
	const int n = t;
	const int *p_n = &n;
	//double arr1[] = {1,2,3,4,5};
	//double* arr2 =  new double[*(p_n)];
	//std::memcpy(arr2,arr1,sizeof(double)*(*(p_n)));
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	std::mt19937_64 generator(std::chrono::system_clock::now().time_since_epoch().count());
	for (int i = 0; i < (*(p_n)); ++i)
	{
		//cout<<arr2[i]<<"hola"<<endl;
		cout<< distribution(generator)<<endl;
	}
#endif // TEST_RANDOM
{
	#ifndef LOAD_FILE
	std::uniform_real_distribution<double> web_distribution(0.0,1.0);
	std::mt19937_64 web_generator(std::chrono::system_clock::now().time_since_epoch().count());

	const unsigned int num_layer = 3;
	#endif //LOAD_FILE

	unsigned int* layers_size = new unsigned int[3];
	layers_size[0] = 2;layers_size[1] = 3;layers_size[2] = 2;

	function* identity_f = new identity();
	function* simoigde_f = new simoigde();
	function*** p_input =  new function**[3];
	function*** p_activ =  new function**[3];
	function*** p_outpu =  new function**[3];
	#ifndef LOAD_FILE
	double** umbral = new double*[3];
	#endif //LOAD_FILE
	for(unsigned int i = 0; i < 3; ++i)
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
	cout<<"hi"<<endl;
	web myweb(num_layer,layers_size,p_input,p_activ,p_outpu,umbral,web_distribution,web_generator);
	#else //LOAD_FILE
	web myweb = web::load("20210314134139.dat", p_input, p_activ, p_outpu);
	#endif //LOAD_FILE
	double inputs[] = {0.2,0.5};
	double *outputs = myweb.Evaluate(inputs);

	unsigned int output_size = myweb.get_output_size();
	for (unsigned int i = 0; i < output_size; ++i)
		cout<<"Output "<<i<<":"<<outputs[i]<<endl;
	
	#ifndef LOAD_FILE
	myweb.save();
	#endif //LOAD_FILE
}
	cout<<"end"<<endl;


	return 0;
}