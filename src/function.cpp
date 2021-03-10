#include "function.hpp"

//A very small number (limit t->0)
const double function::h = 0.000001f;

function::function(){};
function::~function(){};

/*
*	Creates a function which is the derivative of the function
*	(the third derivative has very high error)
*	@return A function pointer to an aproximation to the derivative of the function
*/
function* function::derivative()
{
	return new derivative_function(this);
}

/*
*	Finds the slope in the function at a given point x
*	@param x The point in which the slope will be calculated
*	@return An aproximation of the slope of the function
*/
double function::derivative(double x){
	return ((this->evaluate(x+h)-this->evaluate(x))/h);
}

/*
*	Evaluates the defined Integral (aproximation) of the function from a to b
*	(is very slow)
*	@param a The starting point in the defined integral
*	@param b The end point in the defined integral 
*/
double function::integral(double a, double b){
	double sum = 0;
	double first = (a<b)? a:b;
	double second=(a<b)? b:a;
	for (double i =first; first <= second; first+=h)
		sum += this->evaluate(i)*h;
	if(a>b)sum*=-1;
	return sum;
}

/*
*	Overloads the operator () to evaluate the function in x
*	@param x The point in which the function will be evaluated
*	@return The value of "y" with the function evaluated in x
*/
double function::operator()(double x){
	return this->evaluate(x);
}

/*
*	Creates an object which is the derivative(aproximation)
*	of another function (is possible to get the derivatives derivative)
*	@param original_function Is the original function
*	@return A function being the derivative of the original function
*/
derivative_function::derivative_function(function* original_function):
original_function(original_function){};

/*
*	Gets the slope of the original function at a given point x
*	@param x The point in which the slope will be calculated
*	@return An aproximation of the slope of the function
*/
double derivative_function::evaluate(double x)
{
	return ((this->original_function->evaluate(x+h)-this->original_function->evaluate(x))/h);
}
