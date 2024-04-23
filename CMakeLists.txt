cmake_minimum_required(VERSION 3.5)

project(mcst_test_assingment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Layer)
add_subdirectory(MLayer)
add_subdirectory(Parcing)
add_subdirectory(Ray)
add_subdirectory(Shape)
add_subdirectory(Traversing)
add_subdirectory(Xyr)

file(COPY Exp1 DESTINATION .)
file(COPY Exp2 DESTINATION .)
file(COPY test_PolyFin.txt DESTINATION .)
file(COPY layout_templates DESTINATION .)

add_executable(main main.cpp)

target_link_libraries(main layer mlayer ray shape parcing traversing xyr)

