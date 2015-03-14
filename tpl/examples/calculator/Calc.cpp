#define TPL_USE_AUTO_ALLOC
#include <cmath>		// sin, cos, pow
#include <deque>		// std::deque
#include <iostream> 	// std::cout
#include <functional>	// std::plus, std::minus, etc
#include <algorithm>	// std::max_element
#include <tpl/ext/Calculator.h>

using namespace tpl;

double max_value(const double x[], int count)
{
	return *std::max_element(x, x+count);
}

void calculator(const char* szExpr)
{
	typedef SimpleImplementation impl;

	// ---- define rules ----

	std::deque<double> stk;

	impl::Grammar::Var rFactor;

	impl::Grammar rTerm =
		rFactor + *(
			'*' + rFactor/calc<std::multiplies>(stk) | 
			'/' + rFactor/calc<std::divides>(stk) );

	impl::Grammar rExpr =
		rTerm + *(
			'+' + rTerm/calc<std::plus>(stk) |
			'-' + rTerm/calc<std::minus>(stk) );

	int arity;
	impl::Rule rFun =
		"sin"/calc(stk, sin, arity) | "cos"/calc(stk, cos, arity) |
		"pow"/calc(stk, pow, arity) | "max"/calc(stk, max_value, arity);

	rFactor =
		real()/append(stk) |
		'-' + rFactor/calc<std::negate>(stk) |
		'(' + rExpr + ')' |
		(gr(c_symbol()) + '(' + rExpr % ','/assign(arity) + ')')/(gr(rFun) + '(') |
		'+' + rFactor;

	// ---- do match ----

	try {
		if ( szExpr != skipws_[rExpr] )
			std::cerr << ">>> ERROR: invalid expression!\n";
		else
			std::cout << stk.back() << '\n';
	}
	catch (const std::logic_error& e) {
		std::cerr << ">>> ERROR: " << e.what() << '\n';
	}
}

int main(int argc, const char* argv[])
{
	if (argc == 2) {
		calculator(argv[1]);
		return 0;
	}
	else {
		for (;;)
		{
			std::string strExp;
			std::cout << "input an expression (q to quit): ";
			if (!std::getline(std::cin, strExp) || strExp == "q") {
				std::cout << '\n';
				return 0;
			}
			calculator(strExp.c_str());
		}
	}
}

