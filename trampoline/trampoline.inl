#ifndef TRAMPOLINE_CPP
#define TRAMPOLINE_CPP

#include "trampoline.h"
#include <sys/mman.h>
#include <iostream>
#include "arguments.h"
#include "my_allocator.h"


template <typename T, typename ... Args>
trampoline<T (Args ...)>::~trampoline() {
    if (func_obj) deleter(func_obj);
    free_ptr(code);
}

template <typename T, typename ... Args>
template <typename F>
trampoline<T (Args ...)>::trampoline(F func) 
{
    code = get_next();
    char *pcode = (char *)code;
    
    auto push_bytecom = [&pcode](std::string const& command) 
    {
        for (const char *i = command.c_str(); i < command.c_str() + command.size(); i++) *(pcode++) = *i;
    };

    auto displ_regs = [push_bytecom](int cnt) 
    {
        static const char* mov[6] = { "\x48\x89\xfe" /*mov rsi rdi*/, "\x48\x89\xf2" /*mov rdx rsi*/,
                "\x48\x89\xd1" /*mov rcx rdx*/, "\x49\x89\xc8" /*mov r8 rcx;*/,
                "\x4d\x89\xc1" /*mov r9 r8;*/, "\x41\x51"     /*push %%r9;*/
        };
        for (int i = cnt; i >= 0; i--) push_bytecom(mov[i]);
    };


    func_obj = new F(std::move(func));
    deleter = do_delete<F>;

    int stack_size = 8 * (arguments<Args ...>::INTEGER - 5 + std::max(arguments<Args ...>::SSE - 8, 0));

    if (arguments<Args ...>::INTEGER < 6) 
    {
        displ_regs(arguments<Args ...>::INTEGER - 1);
        //mov  rdi, ptr_to_func_obj
        push_bytecom("\x48\xbf");

        *(void **)pcode = func_obj;
        pcode += 8;
        
        //mov  rax, adress_of_do_call
        push_bytecom("\x48\xb8");
        *(void **)pcode = (void *)&do_call<F>;
        pcode += 8;
        
        //jmp  rax
        push_bytecom("\xff\xe0");
    
    } else {
	    //mov  r11 [rsp]
        push_bytecom("\x4c\x8b\x1c\x24");
        displ_regs(5);
        
        //mov  rax rsp 
        //push_bytecom  rax stack_size
        push_bytecom("\x48\x89\xe0\x48\x05");
        *(int32_t *)pcode = stack_size;
        pcode += 4;
        
        //push_bytecom rsp 8
        push_bytecom("\x48\x81\xc4");
        *(int32_t *)pcode = 8;
        pcode += 4;
        char *loop1 = pcode;
        
        //cmp rax rsp
        //je
        push_bytecom("\x48\x39\xe0\x74");
        char *loop2 = pcode;
        pcode++;
        
        //push_bytecom rsp 8
        push_bytecom("\x48\x81\xc4\x08");
        pcode += 3;
        
        //mov rdi [rsp] 
        //mov [rsp-0x8] rdi 
        //jmp
        push_bytecom("\x48\x8b\x3c\x24\x48\x89\x7c\x24\xf8\xeb");
        *pcode = loop1 - pcode - 1;
        pcode++;
        
        *loop2 = pcode - loop2 - 1;

        //mov [rsp] r11 
        //sub rsp stack_size
        push_bytecom("\x4c\x89\x1c\x24\x48\x81\xec");
        *(int32_t *)pcode = stack_size;
        pcode += 4;
        
        //mov rdi imm
        push_bytecom("\x48\xbf");
        *(void **)pcode = func_obj;
        pcode += 8;
        
        //mov rax imm
        push_bytecom("\x48\xb8");
        *(void **)pcode = (void *)&do_call<F>;
        pcode += 8;
        
        //call rax 
        //pop r9 
        //mov r11 [rsp + stack_size]
        push_bytecom("\xff\xd0\x41\x59\x4c\x8b\x9c\x24");
        *(int32_t *)pcode = stack_size - 8;
        pcode += 4;

        //mov [rsp] r11
        push_bytecom("\x4c\x89\x1c\x24\xc3");
    }
}

template <typename T, typename ... Args>
trampoline<T (Args ...)>::trampoline(trampoline&& other) 
{
    func_obj = other.func_obj;
    code = other.code;
    deleter = other.deleter;
    other.func_obj = nullptr;
}

#endif