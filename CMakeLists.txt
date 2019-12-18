add_contract(tracelytics tracelytics
   ${CMAKE_CURRENT_SOURCE_DIR}/src/tracelytics.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/productInstances.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/users.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/batches.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/deliveries.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/machines.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/sites.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/products.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
)

target_include_directories(tracelytics
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(tracelytics
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/tracelytics.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/tracelytics.contracts.md @ONLY )

target_compile_options( tracelytics PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )