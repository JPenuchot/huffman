#include <iostream>
#include <fstream>
#include <array>
#include <tuple>
#include <map>

#include "huffman.hpp"
#include "content.hpp"
#include "tree.hpp"

using namespace std;

int main(int argc, char** argv)
{
  //  On lit le texte dans le fichier en argument, sinon le texte par défaut
  string text;
  string outname = "default";

  if(argc < 2)
    text = default_text;
  else
  {
    ifstream text_ifs(argv[1]);
    outname = argv[1];
    string str( (istreambuf_iterator<char>(text_ifs)),
                 istreambuf_iterator<char>() );
    text = str;
  }

  //  Les sorties encodées et décodées seront écrites dans deux fichiers séparés
  ofstream out_encoded(outname + ".encoded");
  ofstream out_decoded(outname + ".decoded");

  //  On zip deux tableaux dans une liste de tuples
  //  pour trier les lettres plus facilement
  array<tuple<char, float>, arrsize> tuplelist;
  for(int i = 0; i < arrsize; i++) tuplelist[i] = { charlist[i], problist[i] };

  //  On trie la liste de tuples en fonction du nombre
  //  d'occurrence de chaque lettre du tuple
  sort( tuplelist.begin(), tuplelist.end()
      , [](auto const& a, auto const& b) { return get<1>(a) < get<1>(b); } );

  //  Création de l'arbre
  auto huffman_tree = generate_huffman_tree(tuplelist);

  //  Création de la map
  map<char, string> char_to_code;
  generate_huffman_map(huffman_tree, char_to_code);

  //  Encodage du texte
  string encoded_text;
  for(auto& c : text) encoded_text += char_to_code[c];
  out_encoded << encoded_text << '\n';
  
  //  Décodage du texte
  string decoded_text;
  huffman_decode(huffman_tree, encoded_text, decoded_text);
  out_decoded << decoded_text << '\n';
}
