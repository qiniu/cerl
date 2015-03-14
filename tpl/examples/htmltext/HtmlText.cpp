#include <vector>		// std::vector
#include <iostream> 	// std::cout
#include <tpl/RegExp.h>
#include <tpl/html/Lex.h>

using namespace tpl;

// -------------------------------------------------------------------------
// htmltext

int main()
{
	int nPre = 0;
	std::vector<std::string> text;
	
	const char html[] = "<html><body> \tHello,\t\t<PRE> world!\t\t!</pre></body><b> abc !!</b></html>";
	
	html >> *(
		'<' +
		(
			html_symbol() 					// $tag-start$
			[
				case_(eqICase("pre"))/inc(nPre),
				true
			] + find<true>('>') |
			(
				'/' + 
				html_symbol()				// $tag-end$
				[
					case_(eqICase("pre"))/dec(nPre),
					true
				] +
				'>'
			) |
			done() // invalid grammar!
		) |
		switch_(nPre) 						// $text$
		[
			case_(0) // nPre == 0
			[
				html_skipws() +
				find('<' | html_space())/append(text) +
				!( html_ws()/append(text, " ") )
			],
			default_() // nPre != 0
			[
				find('<')/append(text)
			]
		] );

	for (std::vector<std::string>::iterator it = text.begin(); it != text.end(); ++it) {
		std::cout << *it;
	}
	std::cout << '\n';
	return 0;
}

// -------------------------------------------------------------------------

