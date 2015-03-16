
dom::NodeMark tagSentences("sentences", true);
dom::NodeMark tagVars("vars", true);
dom::NodeMark tagArgs("args", true);
dom::NodeMark tagItems("items", true);
dom::NodeMark tagCodedef("codedef");
dom::NodeMark tagTypedef("typedef");
dom::NodeMark tagServer("server");
dom::NodeMark tagFunction("function");
dom::NodeMark tagConstructor("ctor");
dom::NodeMark tagType("type");
dom::NodeMark tagNamedType("named_type");
dom::NodeMark tagCodedType("coded_type");
dom::NodeMark tagStruct("struct");
dom::NodeMark tagArray("array");

dom::Mark tagModule("module");
dom::Mark tagName("name");
dom::Mark tagValue("value");
dom::Mark tagSize("size");
dom::Mark tagId("id");
dom::Mark tagAsync("async");
dom::Mark tagCode("code");

impl::Allocator alloc;
impl::MarkedGrammar rType(alloc, sdl_type);
NS_STDEXT::String serverName;

dom::Document doc(alloc);
