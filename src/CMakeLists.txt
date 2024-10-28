add_executable(
  huffman-c

  decoder.c
  dict.c
  encoder.c
  main.c
  tiny-string.c
  tree.c
)

target_compile_options(huffman-c PRIVATE -Wall -Wextra -Wpedantic -Werror)

set_property(TARGET huffman-c PROPERTY C_STANDARD 23)
target_link_libraries(
  huffman-c
)
