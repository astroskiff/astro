link_directories(${CPPUTEST_LIBRARIES})

find_package(libutil REQUIRED)

include_directories(
  ${CPPUTEST_INCLUDE_DIRS}
  ${LIBUTIL_INCLUDE_DIRS}
  ../
)

add_executable(astro_unit_tests
        ${ASTRO_SOURCES}
        example.cpp
        lexer.cpp
        main.cpp)


target_link_libraries(astro_unit_tests
        ${CPPUTEST_LDFLAGS}
        libutil)

add_custom_command(TARGET astro_unit_tests COMMAND ./astro_unit_tests POST_BUILD)