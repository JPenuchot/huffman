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
  //  Reading the given file or falling back to the hard-coded text
  string text;
  string outname = "default";

  if(argc < 2)
  {
    cout  << "Usage : " << argv[0] << " [FILE_NAME]" << '\n'
          << "Not specifying a file will make the program fall back to the \
              default text." << '\n';
    text = default_text;
  }
  else
  {
    ifstream text_ifs(argv[1]);
    outname = argv[1];
    string str( (istreambuf_iterator<char>(text_ifs)),
                 istreambuf_iterator<char>() );
    text = str;
  }

  //  encoded/decoded outputs are stored in separate files.
  ofstream out_encoded(outname + ".encoded");
  ofstream out_decoded(outname + ".decoded");

  //  Generating a <char, float> tuple list sorted by the probability of
  //  a symbol's occurence rate
  vector<tuple<char, float>> tuple_list;
  generate_tuple_list(text, tuple_list);

  //  Generating tree
  auto huffman_tree = generate_huffman_tree(tuple_list);

  //  Generating char -> code map
  map<char, string> char_to_code;
  generate_huffman_map(huffman_tree, char_to_code);

  //  Encoding text
  string encoded_text;
  for(auto& c : text) encoded_text += char_to_code[c];
  out_encoded << encoded_text << '\n';
  
  //  Decoding text
  string decoded_text;
  huffman_decode(huffman_tree, encoded_text, decoded_text);
  out_decoded << decoded_text << '\n';

  //  Displaying stats
  cout  << "Original size : "
          << text.size() * 8 << " bits" << '\n'
        
        << "Compressed size : "
          << encoded_text.size() << " bits" << '\n'
        
        << "Compression ratio : "
          << (double)encoded_text.size() / (double)(text.size() * 8) << '\n'
        
        << "Symbol count : "
          << tuple_list.size() << '\n'
        
        << "Entropy : "
          << compute_entropy(tuple_list, char_to_code) << '\n';
}
