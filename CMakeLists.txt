cmake_minimum_required(VERSION 3.5)

project(COURSE_WORK VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Sql)


set(PROJECT_SOURCES
        TestGuru.cpp
        testpassage.cpp
        testpassage.h
        TestGuru.h
        TestGuru.ui
        main.cpp
        TestButton.cpp
        TestButton.h
        question.cpp
        question.h
        answer.cpp
        answer.h
        user.cpp
        user.h
)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/development.sqlite3
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(COURSE_WORK
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(COURSE_WORK
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(COURSE_WORK PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Sql)

set_target_properties(COURSE_WORK PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
install(TARGETS
            COURSE_WORK
        DESTINATION
            .
)

install(FILES $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Core> DESTINATION lib)

install(FILES $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Gui> DESTINATION lib)

install(FILES $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Widgets> DESTINATION lib)

install(FILES $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Sql> DESTINATION lib)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(COURSE_WORK)
endif()
