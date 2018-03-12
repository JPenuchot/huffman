#pragma once

#include <map>

#include "tree.hpp"

using namespace std;

template<size_t N>
shared_ptr<tree<char>> generate_huffman_tree
        ( array<tuple<char, float>, N>& couplelist )
{
  //  On ne génère pas d'arbre de Huffman pour moins de 2 valeurs (stupide)
  static_assert(N >= 2, "Man u serious ?");

  //  Initialisation des itérateurs
  auto current = couplelist.begin();
  auto end = couplelist.end();

  //  À la fin cette valeur sera le dernier noeud de l'arbre filiforme
  shared_ptr<tree<char>> top = make_unique<tree<char>>();
  
  //  On crée le noeud du bas (et on initialise les booléens pour les types)
  top->r = get<0>(*current++);
  top->l = get<0>(*current++);

  while(current != end)  //  Iterating on the tuple<char, float> array
  {
    //  On stocke l'ancienne racine
    auto old_top(move(top));
    //  On génère une nouvelle racine
    top = make_shared<tree<char>>();
    
    //  La valeur de gauche est la lettre que l'on lit
    top->l = get<0>(*current);
    //  La valeur de droite pointe vers l'ancienne racine
    top->r = move(old_top);

    //  On incrémente l'itérateur
    current++;
  }

  return top;
}

void generate_huffman_map ( shared_ptr<tree<char>> top
                          , map<char, string>& dest
                          )
{
  string current = "";

  //  On parcourt l'arbre avec iter
  iter<char>( top
    //  Cas d'une feuille
    , [&](char e, side s)
    {
      current.push_back(s == side::left ? '1' : '0');
      dest[e] = current;
    }

    //  Cas d'un noeud
    , [&](side s) { current.push_back(s == side::left ? '1' : '0'); }
    
    //  Dépilement (backtrack)
    , [&]() { current.pop_back(); }
  );
}

void huffman_decode(shared_ptr<tree<char>> root, string& src, string& dest)
{
  //  On démarre de la racine pour le parcours (évident)
  auto current = root;

  for(char c : src)
  {
    if(c == '0')
    {
      //  On check le type contenu à droite
      //  voir : http://en.cppreference.com/w/cpp/utility/variant/index
      if(current->r.index() == 0)
      //  Si c'est une feuille, on ajoute la lettre qu'elle contient
      //  au résultat et on repart de la racine.
      {
        dest.push_back(get<char>(current->r));
        current = root;
      }
      else
        //  Sinon, on descend.
        current = get<shared_ptr<tree<char>>>(current->r);
    }
    else if (c == '1')
    {
      //  Même chose que pour '0' mais à gauche
      if(current->l.index() == 0)
      {
        dest.push_back(get<char>(current->l));
        current = root;
      }
      else
        current = get<shared_ptr<tree<char>>>(current->l);
    }
  }
}