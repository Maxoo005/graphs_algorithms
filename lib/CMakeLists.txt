# Uzywamy prekompilowanej biblioteki libparameters.so
add_library(parameters SHARED IMPORTED GLOBAL)

set_target_properties(parameters PROPERTIES
    IMPORTED_LOCATION          ${CMAKE_SOURCE_DIR}/lib/libparameters.so
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/lib
)