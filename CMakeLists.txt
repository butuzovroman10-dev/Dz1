cmake_minimum_required(VERSION 3.10)
project(NetworkSecurityAnalysis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройки компилятора
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W3")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

# Поиск библиотек
find_package(Threads REQUIRED)

# Включение директорий
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ml
    ${CMAKE_CURRENT_SOURCE_DIR}/src/crypto
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests
)

# Исходные файлы
set(ML_SOURCES
    src/ml/knn_classifier.cpp
    src/ml/data_processor.cpp
)

set(CRYPTO_SOURCES
    src/crypto/blowfish.cpp
)

set(MAIN_SOURCES
    src/main.cpp
)

# Основной исполняемый файл
add_executable(network_analysis
    ${MAIN_SOURCES}
    ${ML_SOURCES}
    ${CRYPTO_SOURCES}
)

# Настройки связывания
target_link_libraries(network_analysis
    ${CMAKE_THREAD_LIBS_INIT}
)

if(WIN32)
    target_link_libraries(network_analysis ws2_32)
endif()

# Установка выходных директорий
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Информация о сборке
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Binary directory: ${CMAKE_BINARY_DIR}")
