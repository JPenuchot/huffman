#pragma once

#include <map>
#include <vector>
#include <set>

#include "tree.hpp"

using namespace std;

float compute_entropy ( vector<tuple<char, float>>& tuple_list
                      , map<char, string>& encoding
                      )
{
  //  TODO : Correction
  float res = 0.f;
  for(auto& elmt : tuple_list)
    res += get<1>(elmt) * encoding[get<0>(elmt)].size();
  return res;
}

void generate_tuple_list(string& text, vector<tuple<char, float>>& dest)
{
  // We store both a character set *and* their occurrence counts in different
  // structures (std::map doesn't keep a track of the indexes)
  map<char, unsigned int> occurrence_map;
  set<char> char_set;
  
  //  Character count
  unsigned int char_count = 0;

  for(char c : text)
  {
    char_count++;

    if(occurrence_map.count(c))
      occurrence_map[c]++;
    else
    {
      occurrence_map[c] = 1;
      char_set.insert(c);
    }
  }

  //  Adding characters with their occurrence rates to the tuple list
  for(auto c : char_set)
    dest.push_back(make_tuple(c, (float)occurrence_map[c] / (float)char_count));

  //  Sorting tuples using occurrence rates
  sort( dest.begin(), dest.end()
      , [](auto const& a, auto const& b) { return get<1>(a) < get<1>(b); } );
}

template<typename T>
shared_ptr<tree<T>> generate_huffman_tree
        ( vector<tuple<T, float>>& couplelist )
{
  //  Checking the size of the tuple list
  //  (encoding is useless when you have less than 2 symbols)
  if(couplelist.size() <= 2)
    throw runtime_error("rly");

  //  Initializing iterators
  auto current = couplelist.begin();
  auto end = couplelist.end();

  //  At the end this will be the root of the tree
  shared_ptr<tree<T>> root = make_unique<tree<T>>();
  
  //  Creating the bottom node first
  root->r = get<0>(*current++);
  root->l = get<0>(*current++);

  while(current != end)  //  Iterating on the tuple<char, float> array
  {
    //  Storing the previous root
    auto prev_root(move(root));
    //  Generating a new root
    root = make_shared<tree<T>>();
    
    //  Left value becomes the current character
    root->l = get<0>(*current);
    //  Right value is a pointer to the previous root
    root->r = move(prev_root);

    //  Incrementing iterator
    current++;
  }

  return root;
}

template<typename T>
void generate_huffman_map ( shared_ptr<tree<T>> top
                          , map<T, string>& dest
                          )
{
  string current = "";

  //  Iterating on the tree using the given iterator
  iter<T>( top
    //  Case of a leaf
    , [&](T e, side s) { current.push_back(s); dest[e] = current; }
    //  Case of a node
    , [&](side s) { current.push_back(s); }
    //  Unstack (backtrack)
    , [&]() { current.pop_back(); }
  );
}

template<typename T>
void huffman_decode(shared_ptr<tree<T>> root, string& src, string& dest)
{
  //  We start from the root (obvious)
  auto current = root;

  for(T& c : src)
  {
    if(c == side::right)
    {
      //  Checking right child's type (leaf or node)
      //    See doc : http://en.cppreference.com/w/cpp/utility/variant/index
      if(current->r.index() == 0)
      //  In case if a leaf we add its content to the result then start over
      //  from the root.
      {
        dest.push_back(get<T>(current->r));
        current = root;
      }
      else
        //  Or else, we pursue.
        current = get<shared_ptr<tree<T>>>(current->r);
    }
    else if (c == side::left)
    {
      //  Same as before.
      if(current->l.index() == 0)
      {
        dest.push_back(get<T>(current->l));
        current = root;
      }
      else
        current = get<shared_ptr<tree<T>>>(current->l);
    }
    else
      throw runtime_error("Not an encoded string.");
  }
}