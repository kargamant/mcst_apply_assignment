add_library(ray Ray.cpp Ray.h)
target_include_directories(ray PUBLIC .)
target_link_libraries(ray xyr shape)