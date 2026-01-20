cmake_minimum_required(VERSION 3.16)
project(StudyFlowConsole VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(studyflow
    src/main.cpp
    src/login.cpp
    include/login.h

    src/signup.cpp
    include/signup.h

    src/dashboard.cpp
    include/dashboard.h

    src/subject.cpp
    include/subject.h

    src/profile.cpp
    include/profile.h

    src/tasks.cpp
    include/tasks.h

)

target_include_directories(studyflow PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

target_include_directories(studyflow PRIVATE ${SQLITE3_INCLUDE_DIRS})
target_link_libraries(studyflow PRIVATE ${SQLITE3_LIBRARIES})

