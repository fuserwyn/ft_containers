#pragma once

#include <memory>
#include "../utils/map_iterator.hpp"
#include "../utils/pair.hpp"
#include "../utils/utils.hpp"
#include "../utils/switch_const.hpp"
#include "../utils/less.hpp"
#include "../utils/pair.hpp"
#include "../utils/pair_compare.hpp"
#include "node.hpp"

namespace ft
{
	template<class T, class Compare = ft::less<T>, class Alloc = std::allocator<T> >
	class red_black_tree
	{
	public:
		typedef T 									value;
		typedef Compare								key_compare;
		typedef Alloc								allocator_type;
		typedef typename allocator_type::size_type	size_type;
		typedef typename allocator_type::pointer 	p_node;

		p_node create_node(value src){
			p_node new_node = _allocator.allocate(1);
			_allocator.construct(new_node, src);
			return new_node;
		}

		void delete_node(p_node node){
			if (node){
				_allocator.destroy(node);
				_allocator.deallocate(node, 1);
			}
		}

        void rotate_right(p_node x, p_node *root)
        {
            p_node y = x->left;
            y->parent = x->parent;
            if (y->parent && y->parent->left == x)
                y->parent->left = y;
            x->parent = y;
            x->left = y->right;
            if (x->left && x->left->parent)
                x->left->parent = x;
            y->right = x;
            x->isBlack = false;
            y->isBlack = true;
            if (!y->parent)
                *root = y;
        }

        void rotate_left(p_node x, p_node *root){
            p_node y = x->right;
            y->parent = x->parent;

            if (y->parent){
                if (y->parent->right == x)
                    y->parent->right = y;
                else
                    y->parent->left = y;
            }
            x->parent = y;
            x->right = y->left;
            if (x->right && x->right->parent == y)
                x->right->parent = x;
            y->left = x;
            bool color = x->isBlack;
            x->isBlack = y->isBlack;
            y->isBlack = color;
            if (!y->parent)
                *root = y;
        }

		p_node min_node(p_node node) const{
			if (node)
				while (node->left)
                    node = node->left;
			return node;
		}

		p_node max_node(p_node node) const{
			if (node)
				while (node->right)
                    node = node->right;
			return node;
		}

        void swap_color(p_node node){
            bool color = node->isBlack;
            node->isBlack = node->right->isBlack;
            node->right->isBlack = color;
            node->left->isBlack = color;
        }

		p_node find_node(p_node node, value key) const{
			if (node){
				if (_compare(node->value, key))
					return find_node(node->right, key);
				else if (_compare(key, node->value))
					return find_node(node->left, key);
			}
			return node;
		}

		p_node lowest_elem(p_node lowest, value cur_val) const{
			p_node node = 0;
            while (lowest){
				if (_compare(lowest->value, cur_val))
					lowest = lowest->right;
				else{
					if (lowest->left){
                        node = lowest_elem(lowest->left, cur_val);
						if (node)
							lowest = node;
						break;
					}
					else
						break;
				}
			}
			return lowest;
		}

		void clear(p_node *root){
			if (*root == 0)
				return;
			if (*root){
				if ((*root)->left)
					clear(&((*root)->left));
				if ((*root)->right)
					clear(&((*root)->right));
				delete_node((*root));
			}
			*root = 0;
		}

		size_type max_size() const{
            return _allocator.max_size();}

		void balance(p_node *root, p_node node){
			p_node parent;
			p_node grand;
			p_node tmp;

			while (node){
				parent = node->parent;
				grand = 0;
				tmp = parent;
				if (parent){
					grand = parent->parent;
					if (node->isBlack == false){
						if (parent->isBlack == true)
                            node = parent;
						else if (grand){
							if (grand->left && grand->left->isBlack == false && grand->right && grand->right->isBlack == false){
								swap_color(grand);
                                node = grand;
							}
							else if (grand->right == parent){
								if (parent->left == node){
									rotate_right(parent, root);
									parent = node;
								}
								if (parent->right == node)
									rotate_left(grand, root);
                                node = parent->parent;
							}
							else{
								if (parent->right == node){
									rotate_left(parent, root);
									parent = node;
								}
								if (parent->left == node)
									rotate_right(grand, root);
                                node = parent->parent;
							}
						}
					}
					else
                        node = parent;
				}
				else{
                    node->isBlack = true;
                    node = node->parent;
				}
			}
		}

		bool insert(p_node *root, p_node new_node){
			if (*root == 0){
				*root = new_node;
                new_node->isBlack = true;}
			else{
				p_node tmp = *root;
				while (tmp){
					if (!_compare(tmp->value, new_node->value) && !_compare(new_node->value, tmp->value) && tmp != new_node){
							delete_node(new_node);
						return false;}
					else if (_compare(new_node->value, tmp->value)){
						if (tmp->left)
							tmp = tmp->left;
						else{
							tmp->left = new_node;
                            new_node->parent = tmp;
							break;}
						}
					else{
						if (!tmp->right){
                            tmp->right = new_node;
                            new_node->parent = tmp;
                            break;}
						else
                            tmp = tmp->right;
                    }
                }
            }
			balance(root, new_node);
			return true;
		}

		void swap_pointers(p_node remove, p_node replace){
			if (remove->parent){
				if (remove->parent->left == remove)
					remove->parent->left = replace;
				else
					remove->parent->right = replace;
			}
			replace->parent = remove->parent;
			if (remove->left){
				remove->left->parent = replace;
			}
			replace->left = remove->left;
			if (remove->right){
				remove->right->parent = replace;
			}
			replace->right = remove->right;
			replace->isBlack = remove->isBlack;
		}

		bool erase(p_node *root, value key){
			p_node remove = find_node(*root, key);

			if (remove){
				p_node replace = 0;
				if (remove->left)
					replace = erase_left(remove, root);
				else if (remove->right)
					replace = erase_right(remove, root);
				else
					no_child_case(remove, root);
				if (replace)
					swap_pointers(remove, replace);
				if (remove == *root){
					if (replace)
						*root = replace;
					else
						*root = 0;
				}
				delete_node(remove);
				return true;
			}
			return false;
		}

		p_node erase_left(p_node some, p_node *root){
			p_node replace = max_node(some->left);

			if (replace){
				if (replace == some->left){
					some->left = replace->left;
					if (replace->left)
						replace->left->parent = some;
				}
				else{
					replace->parent->right = replace->left;//*
					if (replace->left)
						replace->left->parent = replace->parent;
				}
				if (replace->isBlack == true){
					if (replace->left && replace->left->isBlack == false)
						replace->left->isBlack = true;
					erase_balance(root, replace->parent);
				}
			}
			return replace;
		}

		p_node erase_right(p_node some, p_node *root){//*
			p_node replace = min_node(some->right);

			if (replace){
				if (replace == some->right){
					some->right = replace->right;
					if (replace->right)
						replace->right->parent = some;
				}
				else{
					replace->parent->left = replace->right;
					if (replace->right)
						replace->right->parent = replace->parent;
				}
				if (replace->isBlack == true){
					if (replace->right && replace->right->isBlack == false)
						replace->right->isBlack = true;
					erase_balance(root, replace->parent);
				}
			}
			return replace;
		}

		p_node no_child_case(p_node some, p_node *root)
		{
			p_node replace = some;

			if (replace && replace->parent){
				if (replace->parent->right == replace)
					replace->parent->right = 0;
				else
					replace->parent->left = 0;
				if (replace->isBlack == true){
					if (replace->right && replace->right->isBlack == false)
						replace->right->isBlack = true;
					erase_balance(root, replace->parent);
				}
			}
			return replace;
		}

		void erase_balance(p_node *root, p_node some){
			p_node right;
			p_node left;
			p_node brother;

			while (some != *root && some->isBlack){
				brother = some->right;
				if (brother){
					right = brother->right;
					left = brother->left;
					if ((right || left) && (!right || right->isBlack == true) &&
						(!left || left->isBlack == true)){
						if (brother->isBlack == true){
							brother->isBlack = false;
							some->isBlack = true;
							rotate_left(some, root);
						}
						else
							some = some->parent;
					}
					else if (right && brother->isBlack == true){
						if (left && left->isBlack == false &&
							right->isBlack == true){
							rotate_right(brother, root);
							right = brother;
							brother = left;
						}
						if (right->isBlack == false &&
							(!left || left->isBlack == true || !some->left))
							rotate_left(some, root);
						else
							some = some->parent;
					}
					else if (brother->isBlack == true &&
							   some->isBlack == false){
						some->isBlack = true;
						brother->isBlack = false;
						some = some->parent;
					}
					else
						some = some->parent;
				}
				else
					break;
			}
		}
	private:
		allocator_type 		_allocator;
		key_compare 		_compare;
	};
}