# Toby -- A programming language for learning.
# Copyright (C) 2007  Ryan C. Gordon.
#
# Please refer to LICENSE.txt in the root directory of the source
#  distribution for licensing details.

PROJECT(Toby)
SET(TOBY_VERSION 0.1)

# I hate that they define "WIN32" ... we're about to move to Win64...I hope!
IF(WIN32 AND NOT WINDOWS)
    SET(WINDOWS TRUE)
ENDIF(WIN32 AND NOT WINDOWS)

# Bleh, let's do it for "APPLE" too.
IF(APPLE AND NOT MACOSX)
    SET(MACOSX TRUE)
ENDIF(APPLE AND NOT MACOSX)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckCSourceCompiles)

ADD_DEFINITIONS(-DAPPID=toby)
ADD_DEFINITIONS(-DAPPREV=${TOBY_VERSION})
ADD_DEFINITIONS(-D_REENTRANT)
ADD_DEFINITIONS(-D_THREAD_SAFE)
ADD_DEFINITIONS(-DLUA_ANSI=1)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(lua-5.1.1/src)

IF(MACOSX)
    ADD_DEFINITIONS(-DPLATFORM_MACOSX=1)
    IF(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
        ADD_DEFINITIONS(-DMAC_OS_X_VERSION_MIN_REQUIRED=1020)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} "-mmacosx-version-min=10.2")
    ENDIF(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
    # Not building a library, so disable PIC for minor optimizations.
    ADD_DEFINITIONS(-mdynamic-no-pic)
ENDIF(MACOSX)

IF(WINDOWS)
    ADD_DEFINITIONS(-DPLATFORM_WINDOWS=1)
ENDIF(WINDOWS)

IF(BEOS)
    ADD_DEFINITIONS(-DPLATFORM_BEOS=1)
ENDIF(BEOS)

IF(UNIX)
    ADD_DEFINITIONS(-DPLATFORM_UNIX=1)
ENDIF(UNIX)

IF(CMAKE_COMPILER_IS_GNUCC)
    # Always build with debug symbols...you can strip it later.
    ADD_DEFINITIONS(-g -pipe -Wall -Werror -fsigned-char)
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# !!! FIXME: Workaround for lua issue on some platforms...fix this better.
ADD_DEFINITIONS(-D_setjmp=setjmp)
ADD_DEFINITIONS(-D_longjmp=longjmp)

SET(TOBY_HAVE_GUI FALSE)

OPTION(TOBY_GUI_WXWIDGETS "Build wxWidgets GUI" TRUE)
IF(TOBY_GUI_WXWIDGETS)
    SET(wxWidgets_USE_LIBS base core adv)
    SET(wxWidgets_INCLUDE_DIRS_NO_SYSTEM 1)
    FIND_PACKAGE(wxWidgets)
    IF(wxWidgets_FOUND)
        SET(TOBY_HAVE_GUI TRUE)
        INCLUDE(${wxWidgets_USE_FILE})
        ADD_DEFINITIONS(${wxWidgets_CXX_FLAGS})
    ELSE(wxWidgets_FOUND)
        MESSAGE(STATUS "wxWidgets not found. Support is disabled.")
        SET(TOBY_GUI_WXWIDGETS FALSE)
    ENDIF(wxWidgets_FOUND)
ENDIF(TOBY_GUI_WXWIDGETS)

OPTION(TOBY_GUI_SDL "Build SDL standalone GUI" TRUE)
IF(TOBY_GUI_SDL)
    FIND_PACKAGE(SDL)
    IF(SDL_FOUND)
        SET(TOBY_HAVE_GUI TRUE)
        INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
    ELSE(SDL_FOUND)
        MESSAGE(STATUS "SDL not found. Support is disabled.")
        SET(TOBY_GUI_SDL FALSE)
    ENDIF(SDL_FOUND)
ENDIF(TOBY_GUI_SDL)

IF(NOT TOBY_HAVE_GUI)
    MESSAGE(FATAL_ERROR "Can't find any GUI libraries we can use!")
ENDIF(NOT TOBY_HAVE_GUI)

SET(TOBY_SRCS
    buildver.c
    toby_app.c
    lua-5.1.1/src/lapi.c
    lua-5.1.1/src/ldebug.c
    lua-5.1.1/src/ldo.c
    lua-5.1.1/src/ldump.c
    lua-5.1.1/src/lfunc.c
    lua-5.1.1/src/lgc.c
    lua-5.1.1/src/lmem.c
    lua-5.1.1/src/lobject.c
    lua-5.1.1/src/lopcodes.c
    lua-5.1.1/src/lstate.c
    lua-5.1.1/src/lstring.c
    lua-5.1.1/src/ltable.c
    lua-5.1.1/src/ltm.c
    lua-5.1.1/src/lundump.c
    lua-5.1.1/src/lvm.c
    lua-5.1.1/src/lzio.c
    lua-5.1.1/src/loadlib.c
    lua-5.1.1/src/lparser.c
    lua-5.1.1/src/llex.c
    lua-5.1.1/src/lcode.c
    lua-5.1.1/src/lauxlib.c
)

IF(UNIX AND NOT MACOSX)
    CHECK_LIBRARY_EXISTS("m" "sin" "" HAVE_LIBM)
    IF(HAVE_LIBM)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} m)
    ENDIF(HAVE_LIBM)
ENDIF(UNIX AND NOT MACOSX)

ADD_LIBRARY(tobybackend STATIC ${TOBY_SRCS} ${OPTIONAL_SRCS})

IF(TOBY_GUI_WXWIDGETS)
    ADD_EXECUTABLE(toby toby_wxwidgets.cpp)
    TARGET_LINK_LIBRARIES(toby tobybackend ${OPTIONAL_LIBS} ${wxWidgets_LIBRARIES})
ENDIF(TOBY_GUI_WXWIDGETS)

IF(TOBY_GUI_SDL)
    ADD_EXECUTABLE(toby-sdl toby_sdl.c)
    TARGET_LINK_LIBRARIES(toby-sdl tobybackend ${OPTIONAL_LIBS} ${SDL_LIBRARY})
ENDIF(TOBY_GUI_SDL)

# end of CMakeLists.txt ...

