#ifndef FUNCTION_HPP
#define FUNCTION_HPP

/*
*	Is a template of a function
*/
class function
{
public:
	function();
	~function();
	/*
	*	Evaluates the function in x
	*	@param x The point in which the function will be evaluated
	*	@return The value of "y" with the function evaluated in x
	*/
	virtual double evaluate(double x) = 0;
	/*
	*	Creates a function which is the derivative of the function
	*	(the third derivative has very high error)
	*	@return A function pointer to an aproximation to the derivative of the function
	*/
	function* derivative();
	/*
	*	Finds the slope in the function at a given point x
	*	@param x The point in which the slope will be calculated
	*	@return An aproximation of the slope of the function
	*/
	double derivative(double x);
	/*
	*	Evaluates the defined Integral (aproximation) of the function from a to b
	*	(is very slow)
	*	@param a The starting point in the defined integral
	*	@param b The end point in the defined integral 
	*/
	double integral(double a, double b);
	/*
	*	Overloads the operator () to evaluate the function in x
	*	@param x The point in which the function will be evaluated
	*	@return The value of "y" with the function evaluated in x
	*/
	double operator()(double x);
protected:
	//A very small number (limit t->0)
	static const double h;
private:
};
/*
*	Is a template of a derivative
*/
class derivative_function : public function
{
	public:
		/*
		*	Creates an object which is the derivative(aproximation)
		*	of another function (is possible to get the derivatives derivative)
		*	@param original_function Is the original function
		*	@return A function being the derivative of the original function
		*/
		derivative_function(function* original_function);
		/*
		*	Gets the slope of the original function at a given point x
		*	@param x The point in which the slope will be calculated
		*	@return An aproximation of the slope of the function
		*/
		virtual double evaluate(double x);
	private:
		//The original function
		function* original_function;
};

#endif // FUNCTION_HPP