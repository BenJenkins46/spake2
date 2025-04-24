cmake_minimum_required(VERSION 3.14)

set(TEST_NAME spake2_tests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SPAKE_2_SRC 
    ../source/EllipticCurve.hpp                      ../source/EllipticCurve.cpp
    ../source/HashFunctions.hpp                      ../source/HashFunctions.cpp
    ../source/KeyDerivationFunctions.hpp             ../source/KeyDerivationFunctions.cpp 
    ../source/MessageAuthenticationCodeFunctions.hpp ../source/MessageAuthenticationCodeFunctions.cpp
    ../source/Spake2.hpp                             ../source/Spake2.cpp
    ../source/Spake2CipherSuite.hpp                  ../source/Spake2CipherSuite.cpp)

    
set(TEST_SOURCES 
    EllipticCurveTests.cpp
    Spake2Tests.hpp Spake2Tests.cpp
    StringHelpersTests.cpp)

include(FetchContent)
cmake_policy(SET CMP0135 NEW)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(${TEST_NAME} ${TEST_SOURCES})

# Add required include directories
target_include_directories(${TEST_NAME} PUBLIC ../source)
add_dependencies(${TEST_NAME} gtest)

# Link against required libraries.
target_link_libraries(${TEST_NAME} GTest::gtest_main spake2_core)

set(GCOV_LCOV_FOUND FALSE)
find_program(GCOV_PATH gcov)
find_program(LCOV_PATH NAMES lcov lcov.bat lcov.exe lcov.perl)

if ( LCOV_PATH AND TEST_COVERAGE )
  message("Adding coverage flags to ${TEST_NAME}")
  
  target_compile_options(${TEST_NAME} PRIVATE -fprofile-arcs -ftest-coverage -O0 -g)
  target_link_options   (${TEST_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
  
  set(GCOV_LCOV_FOUND TRUE)
  include(${EXTERN_DIR}/CodeCoverage.cmake)
  set(COVERAGE_EXCLUDES build/* /Library/Developer/CommandLineTools/SDKs/MacOSX11.3.sdk/usr/include/* )
  
  SETUP_TARGET_FOR_COVERAGE_LCOV(NAME spake2_tests_coverage
    EXECUTABLE      ${TEST_NAME}
    EXECUTABLE_ARGS ${LCOV_ARGS}
    DEPENDENCIES    ${TEST_NAME})
endif()
