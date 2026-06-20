add_executable(project
    main.cpp
    graph/GraphLoader.cpp
    algorithms/mst/Prim.cpp
    algorithms/sp/Dijkstra.cpp
    generator/GraphGenerator.cpp
)

# Flagi
target_compile_options(project PRIVATE
    -Wall
    -Wextra
    -Werror
)

# Sciezki naglowkow
target_include_directories(project PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# RPATH – binarny znajdzie libparameters.so obok projektu
set_target_properties(project PROPERTIES
    BUILD_RPATH "${CMAKE_SOURCE_DIR}/lib"
)

# Link biblio
target_link_libraries(project PRIVATE parameters)
