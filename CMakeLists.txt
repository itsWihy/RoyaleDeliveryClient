cmake_minimum_required(VERSION 3.16)

project(RoyaleDeliveryClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(PROJECT_SOURCES
        source/main.cpp
        source/net/remotepi.cpp
        headers/net/remotepi.h
        source/windows/mainwindow.cpp
        headers/windows/mainwindow.h
        headers/windows/signupwindow.h
        source/windows/signupwindow.cpp
        headers/net/commands.h
        source/windows/loginwindow.cpp
        headers/windows/loginwindow.h
        headers/windows/windowutils.h
        source/windows/mainmailwindow.cpp
        headers/windows/mainmailwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RoyaleDeliveryClient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
            headers/net/remotepi.h source/net/remotepi.cpp
            headers/windows/signupwindow.h source/windows/signupwindow.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RoyaleDeliveryClient APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    add_executable(RoyaleDeliveryClient
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(RoyaleDeliveryClient PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(RoyaleDeliveryClient PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RoyaleDeliveryClient)
endif()
set_target_properties(RoyaleDeliveryClient PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS RoyaleDeliveryClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RoyaleDeliveryClient)
endif()
