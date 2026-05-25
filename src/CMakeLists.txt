add_executable(project
    main.cpp
)

# Flagi
target_compile_options(project PRIVATE
    -Wall
    -Wextra
    -Werror
)

# Sciezki
target_include_directories(project PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link biblio
target_link_libraries(project PRIVATE parameters)
