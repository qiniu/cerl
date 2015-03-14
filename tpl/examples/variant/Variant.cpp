#define TPL_USE_AUTO_ALLOC
#include <cmath>		// sin, cos, pow
#include <iostream> 	// std::cout
#include <tpl/Emulator.h>
#include "Variant.h"

// class TestEmulator

class TestEmulator
{
public:
	typedef tpl::emu::ExtCPU<Variant, true> cpu;

	void simplest()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		
		// 3 / 2
		code <<
			cpu::push(3),
			cpu::push(2),
			cpu::div();

		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
	}
	
	void local_var()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		cpu::var_type x, y;
		
		// x = 2
		// y = 3.0
		// x * y
		code <<
			cpu::var(x, y),
			
			cpu::lea(x),
			cpu::push(2),
			cpu::assign(),
			cpu::pop(),
			
			cpu::lea(y),
			cpu::push(3.0),
			cpu::assign(),
			cpu::pop(),

			cpu::push(x),
			cpu::push(y),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);

		std::cout << stk.top() << "\n\n";
	}
	
	void local_var_optimization()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		cpu::var_type x, y;
		
		// x = 2.0
		// y = 3.0
		// x * y
		code <<
			cpu::push(2.0),
			cpu::var_init(x),
			cpu::push(3.0),
			cpu::var_init(y),
			
			cpu::push(x),
			cpu::push(y),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
	}
	
	static Variant my_sin(cpu::alloc_type& alloc, const Variant& x)
	{
		return Variant(alloc, sin(to_double(x)));
	}

	static Variant max_value(const Variant x[], int count)
	{
		return *std::max_element(x, x+count);
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
			cpu::func(my_sin),
			cpu::arity(3),
			cpu::func(max_value);
		
		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);

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
			cpu::push(3),
			cpu::push(2),
			cpu::call(my_div);

		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);

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
			cpu::push(3),
			cpu::push(2.0),
			cpu::call(my_div);

		cpu::stack_type stk;
		code.exec(alloc, my_label, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
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
}

// -------------------------------------------------------------------------

