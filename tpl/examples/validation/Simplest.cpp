#include <iostream> 	// std::cout
#include <tpl/regex/Match.h> // minimization of including tpl

using namespace tpl;

// -------------------------------------------------------------------------

void validation_example_1(const char* str)
{
	if (str == integer())
		std::cout << "  " << str << " is an integer value.\n";
	else if (str == real())
		std::cout << "  " << str << " is a real value.\n";
	else
		std::cout << "  " << str << " isn't a numeric value.\n";
}

// -------------------------------------------------------------------------

void validation_example_2(const char* str)
{
	int val;
	if (str == integer()/assign(val) && val >= 1 && val <= 12)
		std::cout << "  " << str << " is an integer value between 1 to 12.\n";
	else
		std::cout << "  " << str << " is not valid data.\n";
}

// -------------------------------------------------------------------------

#include <boost/spirit/phoenix.hpp>

using namespace phoenix;

void validation_example_3(const char* str)
{
	if (str == integer()/meet(arg1 >= 1 && arg1 <= 12))
		std::cout << "  " << str << " is an integer value between 1 to 12.\n";
	else
		std::cout << "  " << str << " is not valid data.\n";
}

// -------------------------------------------------------------------------

void validation_example_4(const char* str)
{
	if (str == integer()/(ge(1) && le(12)))
		std::cout << "  " << str << " is an integer value between 1 to 12.\n";
	else
		std::cout << "  " << str << " is not valid data.\n";
}

// -------------------------------------------------------------------------

int main()
{
	validation_example_1("-135");
	validation_example_1("+.23e-23");
	validation_example_1("-.1.e23");
	std::cout << "\n";

	validation_example_2("3");
	validation_example_2("13");
	validation_example_2("-135");
	validation_example_2("+.23e-23");
	std::cout << "\n";

	validation_example_3("3");
	validation_example_3("13");
	validation_example_3("-135");
	validation_example_3("+.23e-23");
	std::cout << "\n";

	validation_example_4("3");
	validation_example_4("13");
	validation_example_4("-135");
	validation_example_4("+.23e-23");
	std::cout << "\n";
	return 0;
}

// -------------------------------------------------------------------------

