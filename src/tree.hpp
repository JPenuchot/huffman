#pragma once

#include <functional>
#include <variant>
#include <memory>

using namespace std;

/**
 * @brief      Represents a binary tree.
 *
 * @tparam     T     Type of the elements stored in the leaves
 */
template<typename T>
struct tree
{
  // l & r contain either a pointer to a sub-tree or a T element.

  //  std::variant is a lighter alternative to C union types.
  //    See doc here: http://en.cppreference.com/w/cpp/utility/variant
  variant<T, shared_ptr<tree<T>>> l;
  variant<T, shared_ptr<tree<T>>> r;
};

/**
 * @brief      Designates left or right side.
 */
enum side { left = '1', right = '0' };

/**
 * @brief      Binary tree iterator
 *
 * @param[in]  tr       Root
 * @param[in]  leaf     Code to run on a leaf
 * @param[in]  node     Code to run on a node
 * @param[in]  unstack  Code to run at unstack
 *
 * @tparam     T        Type of elements contained by the tree
 */
template<typename T>
inline void iter( shared_ptr<tree<T>>     tr
                , function<void(T, side)> leaf
                , function<void(side)>    node
                , function<void()>        unstack
                )
{
  //  Checking contained typed
  if(tr->l.index() == 0)
    //  Case of a leaf
    leaf(get<T>(tr->l), side::left);
  else
    //  Case of a node
  { node(side::left);
    iter( get<shared_ptr<tree<T>>>(tr->l) , leaf, node , unstack ); }
  
  unstack();

  //  Idem, type checking...
  if (tr->r.index() == 0)
    leaf(get<T>(tr->r), side::right);
  else
  { node(side::right);
    iter( get<shared_ptr<tree<T>>>(tr->r) , leaf , node , unstack ); }

  unstack();
}
