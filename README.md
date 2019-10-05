# Huffman Coding
An attempt at replicating the [Huffman Compression/Decompression](https://en.wikipedia.org/wiki/Huffman_coding#Basic_technique) algorithms in C++.

## How to use

 1. Download the repo;
 2. Compile it using your favorite compiler;
 3. Add a `text.txt` into your executable's path as an input;
 4. Execute and watch it happen.
 
 The application will then show you the following output and generate a `out.txt` file with the encoded text.

### Output
```
    
 GENERATING THE TREE NOW!

TEXT FOUND IN FILE: BIDOOFUS MAXIMUS
CHARACTERS FOUND IN FILE:
        Char: U Count: 2
        Char: S Count: 2
        Char: O Count: 2
        Char: M Count: 2
        Char: I Count: 2
        Char: X Count: 1
        Char: F Count: 1
        Char: D Count: 1
        Char: B Count: 1
        Char: A Count: 1
        Char:   Count: 1

 ORDERING THE TREE NOW!


 GENERATING CHARACTER MAP NOW!

CHARACTERS BUILT:
        Char: I Code: 010
        Char: O Code: 011
        Char: U Code: 100
        Char: S Code: 101
        Char: M Code: 111
        Char: B Code: 0000
        Char: F Code: 0001
        Char: X Code: 0010
        Char: D Code: 0011
        Char: A Code: 1100
        Char:   Code: 1101
        CURRENT SIZE: 54BITS

 GENERATING OUT FILE!


 DECODING...

DECODED TEXT: BIDOOFUS MAXIMUS```

### The Output File
 The output file consists of 4 bytes declaring the length of the output text + itself (used for decoding the exact ammount of characters), followed by the huffman coding itself.
 There is no serialization of the binary-tree yet.

## TO-DO

 - Drawing the Binary Tree
 - Serializing and including the Binary Tree in the out.txt file
 - Custom file input/output
 - Seperately executing the Compressor and Decompressor
