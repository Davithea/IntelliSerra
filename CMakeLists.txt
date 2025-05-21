cmake_minimum_required(VERSION 3.29)
project(IntelliSerra)

set(CMAKE_CXX_STANDARD 20)

# Aggiunta della directory degli header
include_directories(${CMAKE_SOURCE_DIR}/Include)

# Definizione dell'eseguibile con i file sorgente
add_executable(IntelliSerra
        main.cpp
        Src/Impianto.cpp
        Include/Impianto.h
        Include/Orario.h
        Src/Orario.cpp
        Include/Serra.h
        Src/Serra.cpp
        Include/Tropicale.h
        Src/Tropicale.cpp
        Include/Mediterraneo.h
        Src/Mediterraneo.cpp
        Src/Mediterraneo.cpp
        Include/UserInterface.h
        Src/UserInterface.cpp
        Include/Desertico.h
        Include/Alpino.h
        Include/Carnivoro.h
        Src/Alpino.cpp
        Src/Carnivoro.cpp
        Src/Desertico.cpp
        Include/Logger.h
        Src/Logger.cpp
)
