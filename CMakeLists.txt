cmake_minimum_required(VERSION 3.16)

  project(MusicMaster VERSION 0.1 LANGUAGES CXX)

  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  find_package(Qt6 REQUIRED COMPONENTS Widgets)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(TAGLIB REQUIRED taglib)

  qt_standard_project_setup()

  qt_add_executable(MusicMaster 
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    library.cpp
    library.h
    library.ui
    albumcover.cpp
    albumcover.h
    albumcover.ui
    musicFinder.hpp
    musicFinder.cpp
    albumlist.cpp
    albumlist.h
    albumlist.ui
    songentry.cpp
    songentry.h
    songentry.ui
  )

  target_include_directories(MusicMaster PRIVATE ${TAGLIB_INCLUDE_DIRS})
  target_link_libraries(MusicMaster PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${TAGLIB_LIBRARIES})

  set_target_properties(MusicMaster PROPERTIES 
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} 
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
  )

  include(GNUInstallDirs)
  #install(TARGETS MusicMaster BUNDLE DESTINATION.LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
