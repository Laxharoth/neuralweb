#include "backpropagation.hpp"
#include "web_file_handle.hpp"
#include <cmath>

const double backpropagation::LEARNING_COEFFICIENT= 0.01;
const double backpropagation::MOMENTUM_COEFFICIENT= 0.50;

bool backpropagation::end_training=false;

double backpropagation::train(web &myweb, double** inputs, double** expected_outputs, unsigned int n_patrons, double permisible_error)
{
	backpropagation::end_training = false;
	layer* current_layer{};
	layer* next_layer{};
	neuron* current_neuron{};
	unsigned int layer_num;
	unsigned int neuron_num;
	unsigned int inputs_num;
	unsigned int biggest_layer{};

	double max_error{};
	double patron_error{};
	double*** previous_delta{};
	double** deltas{};
	double* myoutputs{};

	unsigned long iteration = 0;

	std::thread* threads{};

	layer_num = myweb.get_num_layers();
	previous_delta = new double**[layer_num];
	deltas = new double*[layer_num];
	for (unsigned int i = 0; i < layer_num; ++i)
	{
		current_layer = myweb.get_layer(i);
		neuron_num = current_layer->get_layer_size();
		previous_delta[i] = new double*[neuron_num];
		deltas[i] = new double[neuron_num];
		for (unsigned int j = 0; j < neuron_num; ++j)
		{
			current_neuron = current_layer->get_neuron(j);
			inputs_num = current_neuron->get_inputs_number();
			previous_delta[i][j] = new double[inputs_num];
			biggest_layer= (biggest_layer>inputs_num)? biggest_layer:inputs_num;
			for (int k = 0; k < inputs_num; ++k)
				previous_delta[i][j][k] = 0;
			
		}
	}

	threads = new std::thread[biggest_layer];
	
	while( !backpropagation::end_training )
	{	
		max_error = 0;
		iteration++;
		for (unsigned int i = 0; i < n_patrons; ++i)
		{
			patron_error = 0;
			myoutputs = myweb.Evaluate(inputs[i]);
			for (int j = 0; j < myweb.get_output_size(); ++j)
			{
				//if(std::isnan(myoutputs[j]))std::cout<<iteration<<std::endl;
				patron_error += std::pow(expected_outputs[i][j] - myoutputs[j],2);
			}
			max_error=std::max(patron_error,max_error);

			//para la capa de salida
			current_layer = myweb.get_layer(layer_num-1);
			neuron_num=current_layer->get_layer_size();

			for (unsigned int i = 0; i < neuron_num; ++i)
				threads[i] = std::thread([&current_layer,&expected_outputs,&previous_delta,&deltas,layer_num,i]()
					{
						backpropagation::neuron_update(
						current_layer->get_neuron(i),
						expected_outputs[layer_num-1][i],
						previous_delta[layer_num-1][i],
						deltas[layer_num-1][i]);
					});
			for (int i = 0; i < neuron_num; ++i)
				threads[i].join();


			//para las capas ocultas
			for (unsigned int i = layer_num-2; i > 0; --i)
			{
				next_layer = current_layer;
				current_layer = myweb.get_layer(i);
				neuron_num=current_layer->get_layer_size();

				for (unsigned int j = 0; j < neuron_num; ++j)
					threads[j] = std::thread([&current_layer,&previous_delta,&deltas,i,j,next_layer]()
					{
						backpropagation::neuron_update(
						current_layer->get_neuron(j),
						previous_delta[i][j],
						deltas[i][j],
						deltas[i+1],
						next_layer,
						j);
					});

				for (int j = 0; j < neuron_num; ++j)
					threads[j].join();
			}
		}
		backpropagation::end_training = backpropagation::end_training || (max_error < permisible_error);
		#ifdef 	KEEP_IT_SHORT
		backpropagation::end_training=true;
		#endif //	KEEP_IT_SHORT
	}

	delete[] threads;


	save(myweb);

	for (unsigned int i = 0; i < layer_num; ++i)
	{
		current_layer = myweb.get_layer(i);
		neuron_num = current_layer->get_layer_size();
		for (unsigned int j = 0; j < neuron_num; ++j)
		{
			delete[] previous_delta[i][j];
		}
		delete[] previous_delta[i];
		delete[] deltas[i];
	}
	delete[] previous_delta;
	delete[] deltas;

	return max_error;
}

void backpropagation::neuron_update(neuron* toupdate,double expected_output,double* previous_delta, double &delta)
{
	double weight_change{};
	double this_weight{};
	const unsigned int n_input = toupdate->get_inputs_number();
	const double last_activation = toupdate->get_last_activation();
	const double error = expected_output - toupdate->output_function->evaluate(last_activation);
	delta = error*(toupdate->output_function->derivative(last_activation));
	for (unsigned int i = 0; i < n_input; ++i)
	{
		this_weight = toupdate->get_weight(i);
		weight_change =  previous_delta[i]*MOMENTUM_COEFFICIENT + 
			LEARNING_COEFFICIENT*delta*toupdate->get_input(i);
		toupdate->set_weight(i, weight_change + this_weight);
		previous_delta[i] = weight_change;
	}
}

void backpropagation::neuron_update(neuron* toupdate,double* previous_delta, double &delta, double* next_layer_delta, layer* next_layer, int toupdate_position)
{
	double weight_change{};
	double this_weight{};
	const unsigned int n_input = toupdate->get_inputs_number();
	const double last_activation = toupdate->get_last_activation();
	const int n_neuron = next_layer->get_layer_size();
	double error = 0.0;
	for (unsigned int i = 0; i < n_neuron; ++i)
		error += next_layer_delta[i]*next_layer->get_neuron(i)->get_weight(toupdate_position);
	
	delta = error*(toupdate->output_function->derivative(last_activation));
	for (unsigned int i = 0; i < n_input; ++i)
	{
		this_weight = toupdate->get_weight(i);
		weight_change = previous_delta[i]*MOMENTUM_COEFFICIENT + 
			LEARNING_COEFFICIENT*delta*toupdate->get_input(i);
		toupdate->set_weight(i, weight_change+this_weight);
		previous_delta[i] = weight_change;
	}
}

void backpropagation::force_end_training()
{
	backpropagation::end_training=true;
}