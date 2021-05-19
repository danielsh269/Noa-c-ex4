cmake_minimum_required(VERSION 3.15)
project(Noa_ex4_c C)

set(CMAKE_C_STANDARD 99)

add_executable(Noa_ex4_c main.c pair.c pair.h vector.h hash_funcs.h hashmap.h pair_char_int.h test_suite.h vector.c hashmap.c test_suite.c test_pairs.h)