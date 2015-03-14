#include "stdafx.h"
#include <tpl/BinRegExp.h>

using namespace tpl;

// -------------------------------------------------------------------------
// word_text

struct FIB_HEADER;

class SelectTableStream
{
private:
	const FIB_HEADER& m_fib;

public:
	SelectTableStream(const FIB_HEADER& fib)
		: m_fib(fib) {}

	typedef ComPtr<IStream*> result_type;

	template <class ContextT>
	bool TPL_CALL match(IStorage* stg, ContextT& context, result_type& strm) const {
		WCHAR Table[] = { '0', 'T', 'a', 'b', 'l', 'e', '\0' };
		Table[0] += m_fib.fWhichTblStm;
		return S_OK == stg->OpenStream(Table, NULL, TPL_STGM_READONLY, 0, &strm);
	}
};

__forceinline
StgRule<SelectTableStream> TPL_CALL selectTableStream(const FIB_HEADER& fib) {
	return StgRule<SelectTableStream>(fib);
}

void word_text()
{
/*	typedef StorageImplementation impl;

	FIB_HEADER fib;
	
	UINT16 cWordArray;
	UINT16 cLongArray;
	UINT16 cRestPart;

	c_type<UINT16>()/assign(cWordArray);
	c_type<UINT16>(cWordArray);

	stream(L"WordDocument")[ c_struct<FIB_HEADER>()/assign(fib) ];
	selectTableStream(fib);

	impl::match_file(L"test.doc", );
*/
}

// -------------------------------------------------------------------------
