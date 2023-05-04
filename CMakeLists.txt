cmake_minimum_required(VERSION 3.23)
project(vocode C)

set(CMAKE_C_STANDARD 11)

add_executable(vocode main.c fft/fft.h fft/fft.c tinywav/tinywav.c tinywav/tinywav.h)
target_link_libraries(vocode m)