cmake_minimum_required(VERSION 3.16)

  project(MusicMaster VERSION 0.1 LANGUAGES CXX)

  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(TAGLIB REQUIRED taglib)

  qt_standard_project_setup()

  qt_add_executable(MusicMaster 
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.hpp
    src/mainwindow.ui

    src/albumlist/albumlist.cpp
    src/albumlist/albumlist.hpp
    src/albumlist/albumlist.ui

    src/library/library/library.cpp
    src/library/library/library.hpp
    src/library/library/library.ui

    src/library/albumcover/albumcover.cpp
    src/library/albumcover/albumcover.hpp
    src/library/albumcover/albumcover.ui

    src/musicUtils/musicFinder/musicFinder.hpp
    src/musicUtils/musicFinder/musicFinder.cpp

    src/musicUtils/musicPlayer/musicPlayer.hpp
    src/musicUtils/musicPlayer/musicPlayer.cpp
    
    src/songentry/songentry.cpp
    src/songentry/songentry.hpp
    src/songentry/songentry.ui
  )

  target_include_directories(MusicMaster PRIVATE ${TAGLIB_INCLUDE_DIRS})
  target_link_libraries(MusicMaster PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia ${TAGLIB_LIBRARIES})

  set_target_properties(MusicMaster PROPERTIES 
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} 
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
  )

  include(GNUInstallDirs)
  #install(TARGETS MusicMaster BUNDLE DESTINATION.LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
