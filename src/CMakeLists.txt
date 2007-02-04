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

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(lua-5.1.1/src)

IF(WINDOWS)
    ADD_DEFINITIONS(-DPLATFORM_WINDOWS=1)
ENDIF(WINDOWS)

IF(MACOSX)
    ADD_DEFINITIONS(-DPLATFORM_MACOSX=1)
    ADD_DEFINITIONS(-DLUA_USE_DYLD=1)
    IF(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
        ADD_DEFINITIONS(-DMAC_OS_X_VERSION_MIN_REQUIRED=1020)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} "-mmacosx-version-min=10.2")
    ENDIF(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
    SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} "-framework Carbon")
ENDIF(MACOSX)

IF(BEOS)
    ADD_DEFINITIONS(-DPLATFORM_BEOS=1)
    # !!! FIXME: Workaround for lua issue...fix this better.
    ADD_DEFINITIONS(-D_setjmp=setjmp)
    ADD_DEFINITIONS(-D_longjmp=longjmp)
ENDIF(BEOS)

IF(UNIX)
    ADD_DEFINITIONS(-DPLATFORM_UNIX=1)
    ADD_DEFINITIONS(-DLUA_USE_POSIX=1)
    IF(NOT MACOSX AND NOT BEOS)
        ADD_DEFINITIONS(-DLUA_USE_DLOPEN=1)
    ENDIF(NOT MACOSX AND NOT BEOS)
ENDIF(UNIX)

IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS(-pipe -Wall -Werror -fsigned-char)
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# We need wxWidgets for the GUI stuff right now...
SET(wxWidgets_USE_LIBS base core adv)
SET(wxWidgets_INCLUDE_DIRS_NO_SYSTEM 1)
FIND_PACKAGE(wxWidgets)
IF(wxWidgets_FOUND)
    INCLUDE(${wxWidgets_USE_FILE})
    SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} ${wxWidgets_LIBRARIES})
    ADD_DEFINITIONS(${wxWidgets_CXX_FLAGS})
ELSE(wxWidgets_FOUND)
    # !!! FIXME: does this actually kill the config process?
    MESSAGE(FATAL_ERROR "Toby requires wxWidgets: http://www.wxwidgets.org/")
ENDIF(wxWidgets_FOUND)

SET(TOBY_SRCS
    buildver.c
    toby_app.c
    toby_wxwidgets.cpp
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
    CHECK_LIBRARY_EXISTS("dl" "dlopen" "" HAVE_LIBDL)
    IF(HAVE_LIBDL)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} dl)
    ENDIF(HAVE_LIBDL)
    CHECK_LIBRARY_EXISTS("m" "sin" "" HAVE_LIBM)
    IF(HAVE_LIBM)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} m)
    ENDIF(HAVE_LIBM)
ENDIF(UNIX AND NOT MACOSX)

ADD_EXECUTABLE(toby ${TOBY_SRCS} ${OPTIONAL_SRCS})
TARGET_LINK_LIBRARIES(toby ${OPTIONAL_LIBS})

# end of CMakeLists.txt ...

