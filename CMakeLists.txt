cmake_minimum_required(VERSION 3.29)
project(IntelliSerra)

set(CMAKE_CXX_STANDARD 20)

# Aggiunta della directory degli header
include_directories(${CMAKE_SOURCE_DIR}/Include)

# Definizione dell'eseguibile con i file sorgente
add_executable(IntelliSerra
        main.cpp
        Src/Impianto.cpp
        Include/Orario.h
        Src/Orario.cpp
        Include/Serra.h
        Src/Serra.cpp
        Include/ImpiantoTropicale.h  # Rinominato
        Src/ImpiantoTropicale.cpp    # Rinominato
        Include/Mediterraneo.h
)
