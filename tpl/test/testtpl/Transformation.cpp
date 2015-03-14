#include "stdafx.h"

// -------------------------------------------------------------------------
// html_process

// What we use:

void html_process()
{
	using namespace tpl;

	typedef SimpleImplementation impl;

	impl::Allocator alloc;

	// ---- define rules ----

	char delim;
	std::string strSourceTag;
	std::string strTransformedTag;
	std::string strProp;
	std::string strValue;
	std::string strText;

	impl::Rule rTagName( alloc, (ucase() >> c_symbol()/assign(strTransformedTag))/assign(strSourceTag) );
	impl::Rule rPropVal( alloc, ch('\"', '\'')/assign(delim) + find_ref(delim)/assign(strValue) + ref(delim) );
	impl::Rule rProp( alloc, (ucase() >> c_symbol()/assign(strProp)) + skipws() + '=' + skipws() + rPropVal );
	impl::Rule rTagStart( alloc, '<' + rTagName + !(ws() + rProp % ws()) + skipws() + '>' );
	impl::Rule rText( alloc, find('<')/assign(strText) );
	impl::Rule rTagEnd( alloc, "</" + (ucase() >> c_symbol()/ref(strTransformedTag)) + '>' );
	impl::Rule rHtmlStart(alloc, ucase() >> '<' + c_symbol()/"HTML" + '>' );
	impl::Rule rHtmlEnd(alloc, ucase() >> "</" + c_symbol()/"HTML" +'>' );
	impl::Rule rDoc( alloc, rHtmlStart + rTagStart + rText + rTagEnd +  rHtmlEnd );

	// ---- do match ----

	if (!impl::match("<html><a href='http://xushiwei.com/'>XUSHIWEI.COM</a></html>", rDoc)) {
		std::cout << "match failed\n";
		return;
	}

	// ---- print info ----

	std::cout << "SourceTag: " << strSourceTag << "\n";
	std::cout << "TransformedTag: " << strTransformedTag << "\n";
	std::cout << "Prop: " << strProp << "\n";
	std::cout << "Value: " << strValue << "\n";
	std::cout << "Text: " << strText << "\n";
}
