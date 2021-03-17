#ifndef BACKPROPAGATION_HPP
#define BACKPROPAGATION_HPP 1

#include "web_file_handle.hpp"

#include <cmath>
#include <thread>

class backpropagation
{
public:
	static void train(web &myweb, double** inputs, double** expected_outputs, unsigned int n_patrons, double permisible_error);
	static void force_end_training();
private:

	static const double LEARNING_COEFFICIENT;
	static const double MOMENTUM_COEFFICIENT;

	static bool end_training;
	backpropagation();
	~backpropagation();

	static void neuron_update(neuron* toupdate,double expected_output,double* previous_delta, double &delta);
	static void neuron_update(neuron* toupdate,double* previous_delta, double &delta, double* next_layer_delta, layer* next_layer, int toupdate_position);
};
#endif