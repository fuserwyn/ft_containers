#pragma once

#include <type_traits>

namespace ft{//creates integer constant from type and value
    template <class T, bool val>
    struct integral_constant{    
        static const bool value = val; //refers not to an instance but to the class itself, don't need to create an instance to refer to the parameter, const is at compile time and cannot be changed
        typedef T                   value_type;
        typedef integral_constant   type;
        operator value_type()const{
            return value;
        }

    };
        /*-------------------------is_integral---------------------------*/
    
//	An instance of a type predicate has the value true if type T is a form of one of the integer types, otherwise it has the value false.
    template<class T>
    struct is_integral: public ft::integral_constant<T, false>{};
    template<>
    struct is_integral<bool>: public ft::integral_constant<bool, true>{};
    template<>
    struct is_integral<char>: public ft::integral_constant<bool, true>{};
    template<>
    struct is_integral<signed char>: public ft::integral_constant<bool, true>{};
    template<>
    struct is_integral<unsigned char>: public ft::integral_constant<bool, true>{};
    template<>
    struct is_integral<wchar_t>: public ft::integral_constant<bool, true>{};
    template<>
    struct is_integral<char16_t>: public ft::integral_constant<bool, true>{};
    template <>
	struct is_integral<short> : public ft::integral_constant<bool, true> {};
	template <>
	struct is_integral<unsigned short> : public ft::integral_constant<bool, true> {};
	template <>
	struct is_integral<int> : public ft::integral_constant<bool, true> {};
	template <>
	struct is_integral<unsigned int> : public ft::integral_constant<bool, true> {};
	template <>
	struct is_integral<long> : public ft::integral_constant<bool, true> {};
	template <>
	struct is_integral<unsigned long> : public ft::integral_constant<bool, true> {};
	template <>
	struct is_integral<long long> : public ft::integral_constant<bool, true> {};
	template <>
	struct is_integral<unsigned long long> : public ft::integral_constant<bool, true> {};
}