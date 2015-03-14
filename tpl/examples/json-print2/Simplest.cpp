#define TPL_USE_AUTO_ALLOC
#include <tpl/c/Lex.h>
#include <tpl/regex/DOM.h>

using namespace tpl;

// -------------------------------------------------------------------------
// json print

// A json print example.

int main()
{
	typedef DOM<> dom;

	const char source[] = "\
		class Foo // Foo comment\n\
		  : public Base1, Base2\n\
		{};\
	";

	dom::Mark tagName("name");
	dom::NodeMark tagBase("base", true);
		dom::Mark tagAccess("access");
		//dom::Mark tagName("name");
	
	dom::Allocator alloc;
	dom::Document doc(alloc);

	source >> cpp_skip_
		[
			gr(c_symbol()/eq("class")) + c_symbol()/tagName +
			!(':' +
				(
					!gr(c_symbol()/eq("public")/tagAccess) +
					c_symbol()/tagName
				)/tagBase % ','
			) +
			'{' + '}' + ';'
		]/doc;

	std::OutputLog log;
	json_print(alloc, log, doc);
	return 0;
}

// -------------------------------------------------------------------------

