#pragma once

#include "../utils/iterator_traits.hpp"
#include "../utils/enable_if.hpp"
#include "../utils/is_integral.hpp"
#include "../utils/utils.hpp"
#include "../vector/vector.hpp"
namespace ft{
    template <class T, class Container = ft::vector<T> >
    class stack
    {
        public:
            typedef T value_t;
            typedef Container container_t;
            typedef size_t size_type;

            explicit stack(const container_t &cnr = container_t()){c = cnr;}

            bool empty() const{return c.empty();}

            size_type size() const{return c.size();}

            value_t &top() {return c.back();}

            const value_t &top() const{return c.back_back();}

            void push(const value_t &val){c.push_back(val);}

            void pop(){c.pop_back();}

            template <class FT, class fContainer>
            friend bool operator==(const ft::stack <FT, fContainer>&lhs, const ft::stack<FT, fContainer>&rhs);

            template <class FT, class fContainer>
            friend bool operator!=(const ft::stack <FT, fContainer>&lhs, const ft::stack<FT, fContainer>&rhs);

            template <class FT, class fContainer>
            friend bool operator<(const ft::stack <FT, fContainer>&lhs, const ft::stack<FT, fContainer>&rhs);

            template <class FT, class fContainer>
            friend bool operator<=(const ft::stack <FT, fContainer>&lhs, const ft::stack<FT, fContainer>&rhs);

            template <class FT, class fContainer>
            friend bool operator>(const ft::stack <FT, fContainer>&lhs, const ft::stack<FT, fContainer>&rhs);

            template <class FT, class fContainer>
            friend bool operator>=(const ft::stack <FT, fContainer>&lhs, const ft::stack<FT, fContainer>&rhs);

        protected:
            container_t c;
    };
    template <class T, class Container>
    bool operator==(const ft::stack<T, Container>&lhs, const ft::stack<T, Container> &rhs){return lhs.c == rhs.c;}

    template <class T, class Container>
    bool operator!=(const ft::stack<T, Container>&lhs, const ft::stack<T, Container> &rhs){return lhs.c != rhs.c;}

    template <class T, class Container>
    bool operator<(const ft::stack<T, Container>&lhs, const ft::stack<T, Container> &rhs){return lhs.c < rhs.c;}

    template <class T, class Container>
    bool operator>(const ft::stack<T, Container>&lhs, const ft::stack<T, Container> &rhs){return lhs.c > rhs.c;}

    template <class T, class Container>
    bool operator<=(const ft::stack<T, Container>&lhs, const ft::stack<T, Container> &rhs){return lhs.c <= rhs.c;}

    template <class T, class Container>
    bool operator>=(const ft::stack<T, Container>&lhs, const ft::stack<T, Container> &rhs){return lhs.c >= rhs.c;}

}