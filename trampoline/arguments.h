#ifndef _ARGUMENTS_H
#define _ARGUMENTS_H

template <typename ... Args>
struct arguments;

template <>
struct arguments<>
{
    static const int INTEGER = 0;
    static const int SSE = 0;
};

template <typename ... Args>
struct arguments<float, Args ...>
{
    static const int INTEGER = arguments<Args ...>::INTEGER;
    static const int SSE = arguments<Args ...>::SSE + 1;
};

template <typename ... Args>
struct arguments<double, Args ...>
{
    static const int INTEGER = arguments<Args ...>::INTEGER;
    static const int SSE = arguments<Args ...>::SSE + 1;
};

template <typename First, typename ... Args>
struct arguments<First, Args ...>
{
    static const int INTEGER = arguments<Args ...>::INTEGER + 1;
    static const int SSE = arguments<Args ...>::SSE;
};

#endif
