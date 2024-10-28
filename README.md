# huffman-c

Naive implementation of [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding) in C.

## Build

This project uses CMake. To build the project the following commands can be used:
```sh
$ mkdir build
$ cmake -B build
$ cd build
$ cmake --build .
```

The resulting binary can be found in `build/src/huffman-c(.exe)` and can be ran from any directory,
since it passes input file paths directly into `fopen()`.

## Usage
```sh
$ ./path/to/huffman-c <encode|decode> <input_file> <output_file>
```

Make sure that both the input and output file paths exist before-hand, e.g.
```sh
$ touch original.in encoded.out decoded.out
$ echo A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED > original.in # example taken from the Wikipedia page
$ ./build/src/huffman-c encode ./original.in ./encoded.out
$ ./build/src/huffman-c decode ./encoded.out ./decoded.out
$ # ./encoded.out can be inspected with a binary editor
$ cat ./decoded.out # should print out the original string, A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED
```

## Further reading

I have a write up about my thought process, progress, and implementation details on my blog (TBD).

## LICENSE

This project is licensed under the MIT license. You can view the LICENSE file [here](./LICENSE).
