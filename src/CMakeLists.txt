cmake_minimum_required(VERSION 3.20)

# "bench.c" is including the libaries as header-only components
# so we don't actually "build" the sorters themselves.

add_executable(bench bench.c)
target_compile_options(bench PRIVATE -O3 -mcpu=native -DNDEBUG)
# vi:ai et sw=4 ts=4:
