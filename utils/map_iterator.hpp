#pragma once

#include "iterator_traits.hpp"
#include "pair.hpp"
#include "utils.hpp"
#include "switch_const.hpp"
#include "../map/node.hpp"

namespace ft
{
    template<typename T>
    class rbt_iterator{
    public:
        typedef typename std::iterator<std::bidirectional_iterator_tag, T>	rbt_it;
        typedef typename rbt_it::iterator_category 							iterator_category;
        typedef typename rbt_it::value_type 								value_type;
        typedef typename rbt_it::difference_type 							difference_type;
        typedef T 															*pointer;
        typedef T 															&reference;
        typedef ft::node<typename ft::switch_const<T>::type> 		        *p_node;

        rbt_iterator(): root(nullptr), node(nullptr){}

        explicit rbt_iterator(const p_node &root, const p_node &node): root(root), node(node){}

        rbt_iterator(const rbt_iterator &src){
            *this = src;}

        ~rbt_iterator(){}

        rbt_iterator &operator=(const rbt_iterator &src){
            if (this == &src)
                return *this;
            root = src.root;
            node = src.node;
            return *this;
        }

        reference &operator*(){
            return node->value; }

        pointer operator->() const{
            return &(node->value);}

        rbt_iterator &operator++(){
            if (!node)
                return *this;
            else if (node->right)
                node = min_node(node->right);
            else if (node == max_node(root->parent))
                node = 0;
            else{
                while (node->parent->right == node)
                    node = node->parent;
                node = node->parent;
            }
            return *this;
        }

        rbt_iterator operator++(int){
            rbt_iterator temp = *this;
            ++(*this);
            return temp;
        }

        rbt_iterator &operator--(){
            if (!node)
                node = max_node(root->parent);
            else if (node->left)
                node = max_node(node->left);
            else{
                while (node->parent->left == node)
                    node = node->parent;
                node = node->parent;
            }
            return *this;
        }

        rbt_iterator operator--(int){
            rbt_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const rbt_iterator &it) const{
            return (it.node == node);}

        bool operator!=(const rbt_iterator &it) const{
            return (it.node != node);}

        operator rbt_iterator<const T>() const{
            return rbt_iterator<const T>(root, node);}

    private:
        p_node root;
        p_node node;

        p_node min_node(p_node joint) const{
            if (joint)
                while (joint->left)
                    joint = joint->left;
            return joint;
        }

        p_node max_node(p_node joint) const{
            if (joint)
                while (joint->right)
                    joint = joint->right;
            return joint;
        }
    };
}