## Packages
find_package(Catch2 CONFIG REQUIRED)
include(Catch)

## Build
set(CLARA_TESTS_SOURCES
	"src/ParserHelper.h"
	"src/main.cpp"
	"src/AssemblyTest.cpp"
	"src/CompilerTest.cpp"
	"src/ParserTest.cpp"
	"src/SourceTest.cpp"
)
add_executable(clara_tests)
target_sources(clara_tests PRIVATE ${CLARA_TESTS_SOURCES})
target_link_libraries(clara_tests ${CLARA_TARGET_NAME} Catch2::Catch2 perfvect::perfvect)
target_compile_definitions(clara_tests PUBLIC _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING)

if(MSVC)
	target_compile_options(clara_tests PRIVATE /W4 /WX /JMC)
else()
	target_compile_options(clara_tests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

include(CTest)
catch_discover_tests(clara_tests)