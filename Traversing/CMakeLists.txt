add_library(traversing traversing.cpp traversing.h)
target_include_directories(traversing PUBLIC .)
target_link_libraries(traversing layer shape xyr ray mlayer)