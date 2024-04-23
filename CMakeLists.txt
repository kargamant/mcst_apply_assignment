cmake_minimum_required(VERSION 3.5)

project(mcst_test_assingment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mcst_test_assingment main.cpp
    Exp1/Layout.hkp Exp1/test_Poly Exp2/Layout2.hkp Exp2/test_Poly1 test_PolyFin.txt
    MLayer.h
    Layer.h
    "traversing.h"
    "traversing.cpp"
    MLayer.cpp
    Shape.h
    Shape.cpp
    Layer.cpp
    Xyr.h
    Xyr.cpp
    LayoutFin.hkp
    Ray.h
    Ray.cpp "Parcing.h" "Parcing.cpp")

file(COPY Exp1 DESTINATION .)
file(COPY Exp2 DESTINATION .)
file(COPY test_PolyFin.txt DESTINATION .)
file(COPY layout_prefix.hkp DESTINATION .)
file(COPY layout_postfix.hkp DESTINATION .)



include(GNUInstallDirs)
install(TARGETS mcst_test_assingment
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
