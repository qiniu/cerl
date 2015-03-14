#include "stdafx.h"

// -------------------------------------------------------------------------
// xml_process

// What we use:
//	* Rules: Rule, +, /, %, xml_symbol(), c_symbol(), integer(), ws(), skipws()
//	* Tags: LeafMark, NodeMark
//	* Matching: Rule::match()
//	* Result Processing: Document::all(), []

void xml_process()
{
	using namespace tpl;

	impl::Allocator alloc;

	// ---- define source ----

	impl::Source source("<tag prop-1=1 prop-2 = -2>text</tag>");

	// ---- define rules ----

	impl::LeafMark tagProp;
	impl::LeafMark tagValue;
	impl::LeafMark tagText;
	impl::NodeMark tagProps;

	impl::MarkedRule rProp( alloc, xml_symbol()/tagProp + skipws() + '=' + skipws() + integer()/tagValue );
	impl::MarkedRule rProps( alloc, rProp % ws() / tagProps );
	impl::MarkedRule rTagStart( alloc, '<' + xml_symbol()/"tag" + ws() + rProps + '>' );
	impl::MarkedRule rDoc( alloc, rTagStart + c_symbol()/tagText + "</tag>" );

	// ---- do match ----

	impl::Document doc;
	impl::Context context(alloc, doc);

	if (!rDoc.match(source, context)) {
		std::cout << "match failed\n";
		return;
	}

	// ---- print text ----

	std::cout << "\nText: " << doc[tagText].stl_str() << "\n";

	// ---- print properties ----

	impl::Document::node_data vProps = doc[tagProps];
	for (impl::Document::cons it = vProps.all(); it; it = it.tl())
	{
		impl::Document::value_type item = it.hd();
		std::cout << (item.key() == tagProp ? "Prop: " : "Value: ");

		impl::Document::leaf_data vData = item.leaf();
		std::cout << vData.stl_str() << "\n";
	}
}
