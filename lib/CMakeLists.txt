add_library(parameters STATIC
    for-students/AiZO-P2-graphAlgorithms/Parameters.cpp
)

target_include_directories(parameters PUBLIC
    for-students/AiZO-P2-graphAlgorithms
)

target_compile_options(parameters PRIVATE -w)