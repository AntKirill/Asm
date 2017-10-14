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

    template <typename F>
    static void do_delete(void* func_obj) 
    {
        delete static_cast<F*>(func_obj);
    }

    void* func_obj;

    void* code;

    void (*deleter)(void*);

};

#include "trampoline.inl"

#endif //TRAMPOLINE_TRAMPOLINE_H