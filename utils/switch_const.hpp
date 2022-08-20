#pragma once

namespace ft
{
    template<class T>
    struct switch_const
    { typedef T type; };

    template<class T>
    struct switch_const<const T>
    { typedef T type; };
}