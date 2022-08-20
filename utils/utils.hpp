#pragma once
#include "is_integral.hpp"
#include "enable_if.hpp"
#include <iostream>
namespace ft{
    /*-------------------------lexicographical compare------------*/
    template <class InputIt1, class InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2){
        while (first1 != last1){
            if(first2 == last2 || *first2 < *first1)
                return false;
            else if (*first1 < *first2)
                return true;
            ++first1;
            ++first2;
        }
        return (first2 != last2);
    }

    template <class InputIt1, class InputIt2, class Compare>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp){
        while (first1 != first2){
            if (first2 == last2 || comp(*first2 < *first1))
                return false;
            else if (*first1 < *first2)
                return true;
            ++first1;
            ++first2;
        }
        return (first2 != last2);
    }
    /*-------------------------equal---------------------------*/
    template <class InputIt1, class InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2){
		for (; first1 != last1; first1++, first2++)
			if (*first1 != *first2)
				return false;
		return true;
	}

    template <class InputIt1, class InputIt2, class BinaryPredicate>
    bool equal (InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate predicate){
        while (first1 != last1){
            if (!predicate(*first1, *first2))
                return false;
            ++first1;
            ++first2;
        }
        return true;
    }

    /* -------------------------------- enable if ---------------------------------*/
//    template<bool B, class T = void>
//    struct enable_if {};
//
//    template<class T>
//    struct enable_if<true, T> {
//        typedef T type;
//    };
//
//    template <class T>
//    void swap(T& x, T& y){
//        T tmp(x);
//        x = y;
//        y = tmp;
//    }
}