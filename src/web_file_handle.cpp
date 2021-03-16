#include "web_file_handle.hpp"

void save(web &myweb)
{
	const int filename_buffer_size  = 19;
	char filename_buffer[filename_buffer_size];
	const char* format = "%Y%m%d%H%M%S.dat\0";
	auto current_time_chrono = std::chrono::system_clock::now();
	std::time_t current_time = std::chrono::system_clock::to_time_t(current_time_chrono);
	std::strftime(filename_buffer,filename_buffer_size,format, std::localtime(&current_time));

	unsigned int n_layers = myweb.get_num_layers();
	unsigned int input_size{};
	unsigned int neuron_size{};
	double current_weight{};
	double current_umbral{};
	layer* current_layer{};
	neuron* current_neuron{};

	//Open file
	std::ofstream output_file(filename_buffer,std::ios::binary|std::ios::out);
	//If is not open abort
	if(!output_file.is_open())return;
	//Save the number of layers
	output_file.write(CAST2CHARPTR(n_layers),sizeof(int));
	//Write every layer
	for (unsigned int i = 0; i < n_layers; ++i)
	{
		//Get layer number of neurons in layer
		current_layer = myweb.get_layer(i);
		neuron_size = current_layer->get_layer_size();
		output_file.write(CAST2CHARPTR(neuron_size),sizeof(int));
		//Write every neuron
		for (unsigned int j = 0; j < neuron_size; ++j)
		{
			//Get number of inputs(weights) in neuron
			current_neuron = current_layer->get_neuron(j);
			input_size = current_neuron->get_inputs_number();
			output_file.write(CAST2CHARPTR(input_size),sizeof(int));
			//Write neuron umbral;
			current_umbral = current_neuron->get_umbral();
			output_file.write(CAST2CHARPTR(current_umbral),sizeof(double));
			//Write every weight
			for (int k = 0; k < input_size; ++k)
			{
				current_weight = current_neuron->get_weight(k);
				output_file.write(CAST2CHARPTR(current_weight),sizeof(double));
			}
		}
	}
	//Close file
	output_file.close();
}

web load(const char* filename, function*** p_function_input, function*** p_function_activation, function*** p_function_output)
{
	unsigned int n_layers{};
	unsigned int input_size{};
	unsigned int neuron_size{};
	double current_weight{};
	double current_umbral{};

	double*** p_weights{};
	double** p_umbral{};
	unsigned int* p_layers_size;

	//Open file
	std::ifstream input_file(filename,std::ios::binary|std::ios::in);
	//Save the number of layers
	input_file.read(CAST2CHARPTR(n_layers),sizeof(int));
	p_weights = new double**[n_layers];
	p_umbral = new double*[n_layers];
	p_layers_size = new unsigned int[n_layers];
	//Read every layer
	for (unsigned int i = 0; i < n_layers; ++i)
	{
		//Get layer number of neurons in layer
		input_file.read(CAST2CHARPTR(neuron_size),sizeof(int));
		p_weights[i] = new double*[neuron_size];
		p_umbral[i] = new double[neuron_size];
		p_layers_size[i]= neuron_size;
		//Read every neuron
		for (unsigned int j = 0; j < neuron_size; ++j)
		{
			//Get number of inputs(weights) in neuron
			input_file.read(CAST2CHARPTR(input_size),sizeof(int));
			//Read neuron umbral;
			input_file.read(CAST2CHARPTR(current_umbral),sizeof(double));
			p_umbral[i][j] = current_umbral;

			//Read every weight
			p_weights[i][j] = new double[input_size];
			for (int k = 0; k < input_size; ++k)
			{
				input_file.read(CAST2CHARPTR(current_weight),sizeof(double));
				p_weights[i][j][k] = current_weight;
			}
		}
	}
	input_file.close();
	return web(n_layers,p_layers_size, p_function_input, p_function_activation, p_function_output,p_umbral, p_weights);
}