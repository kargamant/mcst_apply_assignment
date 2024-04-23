add_library(parcing Parcing.cpp Parcing.h)
target_include_directories(parcing PUBLIC .)
target_link_libraries(parcing layer shape xyr ray mlayer)