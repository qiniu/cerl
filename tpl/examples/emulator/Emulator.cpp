#define TPL_USE_AUTO_ALLOC
#include <cmath>		// sin, cos, pow
#include <iostream> 	// std::cout
#include <algorithm>	// std::max_element
#include <tpl/Emulator.h>

double max_value(const double x[], int count)
{
	return *std::max_element(x, x+count);
}

class TestEmulator
{
public:
	typedef tpl::emu::CPU<double, true> cpu;

	void simplest()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		
		// 2.0 * 3.0
		code <<
			cpu::push(2.0),
			cpu::push(3.0),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(0, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
	}
	
	void local_var()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		cpu::var_type x, y;
		
		// x = 2.0;
		// ++x;
		// y = 3.0;
		// x * y;
		code <<
			cpu::var(x, y),
			
			cpu::lea(x),
			cpu::push(2.0),
			cpu::assign(),
			cpu::pop(),
			
			cpu::lea(x),
			cpu::inc(),
			cpu::pop(),
			
			cpu::lea(y),
			cpu::push(3.0),
			cpu::assign(),
			cpu::pop(),

			cpu::push(x),
			cpu::push(y),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(0, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
	}
	
	void local_var_optimization()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		cpu::var_type x, y;
		
		// x = 2.0;
		// ++x;
		// y = 3.0;
		// x * y;
		code <<			
			cpu::push(2.0),
			cpu::var_init(x),
			
			cpu::lea(x),
			cpu::inc(),
			cpu::pop(),
			
			cpu::push(3.0),
			cpu::var_init(y),
			
			cpu::push(x),
			cpu::push(y),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(0, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
	}
	
	void vargs()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);

		// max(2.0, 3.0, sin(4.0))
		code <<
			cpu::push(2.0),
			cpu::push(3.0),
			cpu::push(4.0),
			cpu::func(sin),
			cpu::arity(3),
			cpu::func(max_value);
		
		cpu::stack_type stk;
		code.exec(0, code.size(), stk);

		std::cout << stk.top() << "\n\n";
	}
	
	void call_proc()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);

		cpu::proc_type<> my_div;
		cpu::label_type<> my_label;
		cpu::var_type x, y;

		code <<
			cpu::jmp(my_label),

			cpu::proc(my_div),
			cpu::arg(x),
			cpu::arg(y),
			cpu::end_arglist(),
			cpu::push(x),
			cpu::push(y),
			cpu::div(),
			cpu::ret(2),

			cpu::label(my_label),
			cpu::push(2.0),
			cpu::push(3.0),
			cpu::call(my_div);

		cpu::stack_type stk;
		code.exec(0, code.size(), stk);

		std::cout << stk.top() << "\n\n";
	}

	void call_proc2()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);

		cpu::proc_type<0> my_div;
		cpu::label_type<0> my_label;
		cpu::var_type x, y;

		code <<
			cpu::proc(my_div, x, y),
			cpu::push(x),
			cpu::push(y),
			cpu::div(),
			cpu::ret(2),

			cpu::label(my_label),
			cpu::push(2.0),
			cpu::push(3.0),
			cpu::call(my_div);

		cpu::stack_type stk;
		code.exec(my_label, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
	}
	
	void factal()
	{
		typedef tpl::emu::CPU<size_t, true> cpu;

		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		
		cpu::proc_type<0> my_factal;
		cpu::label_type<0> my_label;
		cpu::label_type<1> gt_1;
		cpu::var_type n;
		
		code <<
			// --> proc my_factal(n)
			cpu::proc(my_factal, n),
			
			// if (n <= 1)
			//	return 1;
			cpu::push(n),
			cpu::push(1),
			cpu::le(), // n <= 1?
			cpu::jz(gt_1), // jmp if false
			cpu::push(1),
			cpu::ret(1),
			
			// else
			//	return my_factal(n-1)*n;
			cpu::label(gt_1),
			cpu::push(n),
			cpu::sub(1),
			cpu::call(my_factal),
			cpu::push(n),
			cpu::mul(),
			cpu::ret(1),
			// --> end proc

			cpu::label(my_label),
			cpu::push(4),
			cpu::call(my_factal); // call my_factal(4)

		cpu::stack_type stk;
		code.exec(my_label, code.size(), stk);
	
		std::cout << "4! = " << stk.top() << "\n\n";
	}
};

int main()
{
	TestEmulator test;
	test.simplest();
	test.local_var();
	test.local_var_optimization();
	test.vargs();
	test.call_proc();
	test.call_proc2();
	test.factal();
}

// -------------------------------------------------------------------------

