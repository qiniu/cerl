#define TPL_USE_AUTO_ALLOC
#include <tpl/regex/DOM.h>
#include <stdext/FileBuf.h>

using namespace tpl;

// -------------------------------------------------------------------------

#define PRJ_PATH	token<'\r', '\n', '['>()
#define PRJ_ITEM	token<';', '\r', '\n'>()

int main(int argc, const char* argv[])
{
	const bool array = true;

	NS_STDEXT::ErrorLog err;
	if (argc < 2) {
		err.print("Usage: prj2json <project.prj>\n");
		return -1;
	}

	NS_STDEXT::FileBuf file(argv[1]);
	if (!file.good()) {
		err.print(">>> ERROR: open input file failed!\n");
		return -2;
	}

	typedef DOM<> dom;
	
	dom::Mark tagInclude("include", array);
	dom::NodeMark tagCfg("config", array);
		dom::Mark tagName("name");
		dom::NodeMark tagProduct("product");
			dom::Mark tagType("type");
			dom::Mark tagPath("path");
		dom::Mark tagOption("option", array);
		dom::Mark tagDefine("define", array);
		dom::Mark tagObjDir("objdir");
		dom::Mark tagLibDir("libdir", array);
		dom::Mark tagLib("lib", array);
	dom::Mark tagSrc("src", array);

	dom::Allocator alloc;
	dom::Document doc(alloc);

	dom::NodeMark tagPlatform("platform");
		dom::Mark tagCPU("cpu");
		dom::Mark tagByteSwap("byteswap");
		dom::Mark tagBits("bits");
		dom::Mark tagOS("os");

	dom::Node* plat = doc.insertNode(alloc, tagPlatform);
	
	#define PRJ_INSERT_LEAF(tag, data)	\
		plat->insertLeaf(alloc, tag, NS_STDEXT::TempString<char>(data))
	
	#if defined(__INTEL__)
		PRJ_INSERT_LEAF(tagCPU, "intel");
	#elif defined(__SPARC__)
		PRJ_INSERT_LEAF(tagCPU, "sparc");
	#endif
	
	#if defined(__BYTESWAP__) || defined(BYTESWAP)
		PRJ_INSERT_LEAF(tagByteSwap, "true");
	#else
		PRJ_INSERT_LEAF(tagByteSwap, "false");
	#endif

	#if defined(__32BIT__) || defined(_WIN32)
		PRJ_INSERT_LEAF(tagBits, "32");
	#elif defined(__64BIT__) || defined(_WIN64)
		PRJ_INSERT_LEAF(tagBits, "64");
	#endif

	#if defined(__LINUX__) || defined(LINUX)
		PRJ_INSERT_LEAF(tagOS, "linux");
	#elif defined(_WIN32) || defined(_WIN64) || defined(X_OS_WINDOWS)
		PRJ_INSERT_LEAF(tagOS, "windows");
	#endif
	
	if (file >> (skipws() % ("//" + find_eol<true>()))
		[
			!(
				gr(icase["[include]"]) + *gr(PRJ_PATH/tagInclude)
			) +
			+(
				(
					gr(icase["[config"] + ws() + find(']')/tagName + ']') +
					+gr(
						c_symbol()
						[
							case_(eqICase("product"))
							[
								skip_non_eol_ws_
								[
									gr(c_symbol()/tagType) + gr(PRJ_PATH/tagPath)
								]/tagProduct
							],
							case_(eqICase("options"))
							[
								!(
									skip_non_eol_ws_
									[
										gr(PRJ_ITEM/tagOption) % ';'
									]
								)
							],
							case_(eqICase("defines"))
							[
								!(
									skip_non_eol_ws_
									[
										gr(PRJ_ITEM/tagDefine) % ';'
									]
								)
							],
							case_(eqICase("objdir"))
							[
								!(
									skip_non_eol_ws_
									[
										gr(PRJ_PATH/tagObjDir)
									]
								)
							],
							case_(eqICase("libdir"))
							[
								!(
									skip_non_eol_ws_
									[
										gr(PRJ_ITEM/tagLibDir) % ';'
									]
								)
							],
							case_(eqICase("libraries"))
							[
								!(
									skip_non_eol_ws_
									[
										gr(PRJ_ITEM/tagLib) % ';'
									]
								)
							]
						]
					)
				)/tagCfg
			) +
			(
				gr(icase["[source]"]) + +gr(PRJ_PATH/tagSrc)
			)
		]/doc)
	{
		NS_STDEXT::OutputLog log;
		json_print(alloc, log, doc);
		return 0;
	}
	
	err.print("Invalid project format!!!\n");
	return -3;
}

// -------------------------------------------------------------------------

