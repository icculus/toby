# Toby -- A programming language for learning.
# Copyright (C) 2007  Ryan C. Gordon.
#
# Please refer to LICENSE.txt in the root directory of the source
#  distribution for licensing details.

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(Toby)
SET(TOBY_VERSION 0.1)

SET(LUA_DIR lua-5.1.3)

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
INCLUDE_DIRECTORIES(${LUA_DIR}/src)

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
    SET(USES_WINMAIN WIN32)
ENDIF(WINDOWS)

IF(BEOS)
    ADD_DEFINITIONS(-DPLATFORM_BEOS=1)
    # !!! FIXME: Workaround for lua issue on some platforms...fix this better.
    ADD_DEFINITIONS(-D_setjmp=setjmp)
    ADD_DEFINITIONS(-D_longjmp=longjmp)
ENDIF(BEOS)

IF(UNIX)
    ADD_DEFINITIONS(-DPLATFORM_UNIX=1)
ENDIF(UNIX)

IF(CMAKE_COMPILER_IS_GNUCC)
    # Always build with debug symbols...you can strip it later.
    ADD_DEFINITIONS(-g -pipe -Wall -Werror -fsigned-char)
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(MSVC)
    # VS.NET 8.0 got really really anal about strcpy, etc.
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS=1)
ENDIF(MSVC)

SET(TOBY_HAVE_GUI FALSE)

OPTION(TOBY_GUI_WXWIDGETS "Build wxWidgets GUI(s)" TRUE)
IF(TOBY_GUI_WXWIDGETS)
    SET(wxWidgets_USE_LIBS base core adv)
    SET(wxWidgets_INCLUDE_DIRS_NO_SYSTEM 1)
    FIND_PACKAGE(wxWidgets)
    IF(wxWidgets_FOUND)
        SET(TOBY_HAVE_WX_GUI FALSE)

        OPTION(TOBY_WX_STANDALONE "Build wxWidgets standalone GUI" TRUE)
        IF(TOBY_WX_STANDALONE)
            SET(TOBY_HAVE_WX_GUI TRUE)
            ADD_DEFINITIONS(-DTOBY_WX_BUILD_STANDALONE=1)
        ENDIF(TOBY_WX_STANDALONE)

        OPTION(TOBY_WX_IDE "Build wxWidgets IDE GUI" TRUE)
        IF(TOBY_WX_IDE)
            SET(TOBY_HAVE_WX_GUI TRUE)
            ADD_DEFINITIONS(-DTOBY_WX_BUILD_IDE=1)
        ENDIF(TOBY_WX_IDE)

        IF(NOT TOBY_HAVE_WX_GUI)
            MESSAGE(STATUS "No wxWidgets GUIs enabled. Disabling wx app.")
            SET(TOBY_GUI_WXWIDGETS FALSE)
        ELSE(NOT TOBY_HAVE_WX_GUI)
            SET(TOBY_HAVE_GUI TRUE)
            INCLUDE(${wxWidgets_USE_FILE})
            ADD_DEFINITIONS(${wxWidgets_CXX_FLAGS})
        ENDIF(NOT TOBY_HAVE_WX_GUI)
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
    ${LUA_DIR}/src/lapi.c
    ${LUA_DIR}/src/ldebug.c
    ${LUA_DIR}/src/ldo.c
    ${LUA_DIR}/src/ldump.c
    ${LUA_DIR}/src/lfunc.c
    ${LUA_DIR}/src/lgc.c
    ${LUA_DIR}/src/lmem.c
    ${LUA_DIR}/src/lobject.c
    ${LUA_DIR}/src/lopcodes.c
    ${LUA_DIR}/src/lstate.c
    ${LUA_DIR}/src/lstring.c
    ${LUA_DIR}/src/ltable.c
    ${LUA_DIR}/src/ltm.c
    ${LUA_DIR}/src/lundump.c
    ${LUA_DIR}/src/lvm.c
    ${LUA_DIR}/src/lzio.c
    ${LUA_DIR}/src/loadlib.c
    ${LUA_DIR}/src/lparser.c
    ${LUA_DIR}/src/llex.c
    ${LUA_DIR}/src/lcode.c
    ${LUA_DIR}/src/lauxlib.c
)

IF(UNIX AND NOT MACOSX)
    CHECK_LIBRARY_EXISTS("m" "sin" "" HAVE_LIBM)
    IF(HAVE_LIBM)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} m)
    ENDIF(HAVE_LIBM)
ENDIF(UNIX AND NOT MACOSX)

ADD_LIBRARY(tobybackend STATIC ${TOBY_SRCS} ${OPTIONAL_SRCS})

IF(TOBY_GUI_WXWIDGETS)
    ADD_EXECUTABLE(toby ${USES_WINMAIN} toby_wxwidgets.cpp)
    TARGET_LINK_LIBRARIES(toby tobybackend ${OPTIONAL_LIBS} ${wxWidgets_LIBRARIES})
ENDIF(TOBY_GUI_WXWIDGETS)

IF(TOBY_GUI_SDL)
    ADD_EXECUTABLE(toby-sdl ${USES_WINMAIN} toby_sdl.c)
    TARGET_LINK_LIBRARIES(toby-sdl tobybackend ${OPTIONAL_LIBS} ${SDL_LIBRARY})
ENDIF(TOBY_GUI_SDL)

MACRO(MESSAGE_BOOL_OPTION _NAME _VALUE)
    IF(${_VALUE})
        MESSAGE(STATUS "  ${_NAME}: enabled")
    ELSE(${_VALUE})
        MESSAGE(STATUS "  ${_NAME}: disabled")
    ENDIF(${_VALUE})
ENDMACRO(MESSAGE_BOOL_OPTION)

MESSAGE(STATUS "Toby will build with the following options:")
MESSAGE_BOOL_OPTION("wxWidgets-based IDE application" TOBY_GUI_WXWIDGETS)
IF(TOBY_GUI_WXWIDGETS)
    MESSAGE_BOOL_OPTION("  wxWidgets standalone support" TOBY_WX_STANDALONE)
    MESSAGE_BOOL_OPTION("  wxWidgets IDE support" TOBY_WX_IDE)
ENDIF(TOBY_GUI_WXWIDGETS)
MESSAGE_BOOL_OPTION("SDL-based interpreter application" TOBY_GUI_SDL)

# end of CMakeLists.txt ...

