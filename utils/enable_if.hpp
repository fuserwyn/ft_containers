#pragma once

namespace ft{
    /* -------------------------------- enable if ---------------------------------*/
    template<bool B, class T = void>
    struct enable_if {};

    template<class T>
    struct enable_if<true, T> {
	    typedef T type;
    };

    template <class T>
	void swap(T& x, T& y){
		T tmp(x);
		x = y;
		y = tmp;
	}
}