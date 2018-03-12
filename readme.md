# Huffman encoder/decoder

The program is split into a few files :

```
main.cpp    - The program itself
content.hpp - Contains hard-coded 'default' values
tree.hpp    - Tree data structure for a Huffman tree
huffman.hpp - Encoding/Decoding functions
```

Just run './build.sh' to compile the program then './build/{release|debug}/prog [FILENAME]' to encode a given file. The result will be stored in [FILENAME].{encoded|decoded}.
