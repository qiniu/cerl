#include <tpl/c/Lex.h>

using namespace tpl;

// -------------------------------------------------------------------------

int main()
{
	const char source[] = "\
		class Foo // Foo comment\n\
		  : public Base1, Base2\n\
		{};\
	";

	source >> cpp_skip_[
		*gr(c_token()/TPL_INFO("TOKEN"))
		];
}

// -------------------------------------------------------------------------
