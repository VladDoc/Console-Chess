cmake_minimum_required(VERSION 2.8)
project(Chess)

set(FILES_EXE
    src/Bishop.cpp
    src/BoilerPlate.cpp
    src/Field.cpp
    src/Figure.cpp
    src/FigureFactory.cpp
    src/GameObject.cpp
    src/King.cpp
    src/Knight.cpp
    src/main.cpp
    src/Pawn.cpp
    src/Queen.cpp
    src/Rook.cpp
    src/UpdateData.cpp
    src/UtilityBS.cpp
    src/Vector2D.cpp
)

include_directories(include)

add_executable(chess ${FILES_EXE})