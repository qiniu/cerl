/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/fiber/wincoro/coro.c
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2010-2-10 10:41:58
// 
// $Id: coro.c 619 2010-2-10 10:41:58 xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FIBER_WINCORO_CORO_C
#define ASYNC_FIBER_WINCORO_CORO_C

#ifndef ASYNC_FIBER_WINCORO_CORO_H
#include "coro.h"
#endif

#pragma warning(disable: 4731) // frame pointer register 'ebp' modified by inline assembly code
#pragma warning(disable: 4733) // inline asm assigning to 'FS:0' : handler not registered as safe handler

/*
 * __fastcall：
 *	 两个以内不大于 4 字节(DWORD)的参数将自左向右分别保存在 ECX、EDX 寄存器内，由于没有将参数压入堆栈，
 *	 函数返回前无需将参数出栈（这就是它FASTCALL由来）；其它参数仍按自右向左压入堆栈，函数返回时需要把这些参数出栈。
 */
EXTERN_C __declspec(noinline) __declspec(naked)
void __fastcall coro_swapcontext(CoroContext* self, CoroContext* to)
{
	__asm
	{
		mov [ecx], ebx;		// self->CsEbx = ebx;
		mov [ecx+4], edi;	// self->CsEdi = edi;
		mov [ecx+8], esi;	// self->CsEsi = esi;
		mov [ecx+12], ebp;	// self->CsEbp = ebp;

		lea eax, [esp+4];	// adjust esp to account for args + return-address;
		mov ebx, [esp];

		mov [ecx+16], eax;	// self->CsEsp = adjusted esp;
		mov [ecx+20], ebx;	// self->CsEip = return-address;

		mov eax, fs:[0];
		mov ebx, fs:[4];
		mov edi, fs:[8];

		mov [ecx+24], eax;	// self->ExceptionList = fs:[0];
		mov [ecx+28], ebx;	// self->StackTop = fs:[4];
		mov [ecx+32], edi;	// self->StackBottom = fs:[8];

		mov eax, [edx+24];
		mov ebx, [edx+28];
		mov edi, [edx+32];

		mov fs:[0], eax; // fs:[0] = to->ExceptionList;
		mov fs:[4], ebx; // fs:[4] = to->StackTop;
		mov fs:[8], edi; // fs:[8] = to->StackBase;

		mov ebx, [edx];
		mov edi, [edx+4];
		mov esi, [edx+8];
		mov ebp, [edx+12];
		mov esp, [edx+16];
		mov eax, [edx+20];
		jmp eax;
	}
}

#pragma warning(default: 4731)
#pragma warning(default: 4733)

#endif /* ASYNC_FIBER_WINCORO_CORO_C */
