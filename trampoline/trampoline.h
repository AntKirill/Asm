#ifndef TRAMPOLINE_TRAMPOLINE_H
#define TRAMPOLINE_TRAMPOLINE_H

#include <iostream>

template <typename T>
struct trampoline;

template <typename T, typename ... Args>
struct trampoline<T (Args ...)> 
{

    template <typename F>
    trampoline(F func);

    trampoline(trampoline&& other);

    T (*get() const)(Args ... args) {
        return (T(*)(Args ... args))code;
    }

    ~trampoline();

private:
    template <typename F>
    static T do_call(void* obj, Args ...args) 
    {
        return  (*static_cast<F*>(obj))(std::forward<Args>(args)...);
    }
    
    void* func_obj;

    void* code;

    void (*deleter)(void*);

};

#include "trampoline.cpp"

#endif //TRAMPOLINE_TRAMPOLINE_H