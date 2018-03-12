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
  float res = 0.f;
  for(auto& elmt : tuple_list)
    res += get<1>(elmt) * encoding[get<0>(elmt)].size();
  return res;
}

void generate_tuple_list(string& text, vector<tuple<char, float>>& dest)
{
  //  On mémorise l'ensemble des caractères ET leurs nombres d'occurrences
  // (std::map ne stocke pas la liste des clés)
  map<char, unsigned int> occurrence_map;
  set<char> char_set;
  
  //  Le nombre de caractères
  unsigned int char_num = 0;

  for(char c : text)
  {
    char_num++;

    if(occurrence_map.count(c))
      occurrence_map[c]++;
    else
    {
      occurrence_map[c] = 1;
      char_set.insert(c);
    }
  }

  //  Ajout des caractères à la liste de tuples
  for(auto c : char_set)
    dest.push_back(make_tuple(c, (float)occurrence_map[c] / (float)char_num));

  //  Tri des tuples
  sort( dest.begin(), dest.end()
      , [](auto const& a, auto const& b) { return get<1>(a) < get<1>(b); } );
}

shared_ptr<tree<char>> generate_huffman_tree
        ( vector<tuple<char, float>>& couplelist )
{
  //  On vérifie la taille de la liste de tuples
  //  (encodage inutile en-dessous ou égal à 2 caractères)
  if(couplelist.size() <= 2)
    throw runtime_error("rly");

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