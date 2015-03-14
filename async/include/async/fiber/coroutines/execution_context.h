#pragma once

namespace execution_context
{
    namespace aux_
    {
        void * adv(void * p, long diff)
        {
            return (char *)p + diff;
        }

        void push(void ** p, void * value)
        {
            *p = adv(*p, - (long)sizeof value);
            *(void **)*p = value;
        }
    }

    typedef void (*function)(void *);

    struct context
    {
        context()
            : eip(0)  , esp(0)  , ebp(0)
            , ebx(0)  , esi(0)  , edi(0)
            , fs_0((void*)-1)
            , fs_4(0)
            , fs_8(0)
        {}

        context(void * eip, void * esp, void * fs_4, void * fs_8)
            : eip(eip), esp(esp), ebp(0)
            , ebx(0)  , esi(0)  , edi(0)
            , fs_0((void*)-1)
            , fs_4(fs_4)
            , fs_8(fs_8)
        {}

        void * eip;
        void * esp;
        void * ebp;
        void * ebx;
        void * esi;
        void * edi;
        void * fs_0;
        void * fs_4;
        void * fs_8;
    };

    context create_context(function f, void * param, void * stack_base, size_t stack_size)
    {
        using namespace aux_;

        void * stack_top = adv(stack_base, (long)stack_size);
        void * c = stack_top;
        push(&c, param);                            // argument
        push(&c, 0);                                // return address

        return context(f, c, stack_top, stack_base);
    }

#pragma warning (push)
#pragma warning (disable : 4731) // frame pointer register 'ebp' modified by inline assembly code
#pragma warning (disable : 4733) // inline asm assigning to 'FS:0' : handler not registered as safe handler

    void __declspec(naked) __fastcall switch_context(context const * load_from, context * store_to)
    {
        __asm
        {
            // maybe xchg?
            mov eax, [ecx + 4]
            mov [edx +  4], esp
            mov esp, eax

            mov eax, [ecx + 8]
            mov [edx +  8], ebp
            mov ebp, eax

            mov eax, [ecx + 12]
            mov [edx + 12], ebx
            mov ebx, eax

            mov eax, [ecx + 16]
            mov [edx + 16], esi
            mov esi, eax

            mov eax, [ecx + 20]
            mov [edx + 20], edi
            mov edi, eax

            push dword ptr fs:[0]
            mov eax, [ecx + 24]
            pop dword ptr [edx + 24]
            mov fs:[0], eax

            push dword ptr fs:[4]
            mov eax, [ecx + 28]
            pop dword ptr [edx + 28]
            mov fs:[4], eax

            push dword ptr fs:[8]
            mov eax, [ecx + 32]
            pop dword ptr [edx + 32]
            mov fs:[8], eax

            mov eax, [ecx]
            // f*cking ms-assembler, I can't just write "mov dword ptr [edx], cont"
            push cont
            pop dword ptr [edx]
            jmp eax

        cont:
            ret
        }
    }
#pragma warning (pop)
}

