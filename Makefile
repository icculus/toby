#-----------------------------------------------------------------------------#
# TOBY -- An abstract interpreter engine and system for learning.
# Copyright (C) 1999  Ryan C. Gordon.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#-----------------------------------------------------------------------------#


#-----------------------------------------------------------------------------#
# Makefile for building Toby on Unix-like systems. Follow the instructions
#  for editing this file, then run "make" on the command line.
#
#  Written by Ryan C. Gordon (icculus@linuxgames.com)
#-----------------------------------------------------------------------------#


#-----------------------------------------------------------------------------#
# Set to your liking.
#-----------------------------------------------------------------------------#
CC = gcc
LINKER = gcc

#-----------------------------------------------------------------------------#
# If this makefile fails to detect Cygwin correctly, or you want to force
#  the build process's behaviour, set it to "true" or "false" (w/o quotes).
#-----------------------------------------------------------------------------#
#cygwin := true
#cygwin := false
cygwin := autodetect

#-----------------------------------------------------------------------------#
# Set this to true if you want to create a debug build.
#-----------------------------------------------------------------------------#
#debugging := false
debugging := true

#-----------------------------------------------------------------------------#
# Set this to what your platform has.
#   Current option(s): "pthreads", "sdl", "null".
#-----------------------------------------------------------------------------#
platform_threads := pthreads
#platform_threads := sdl
#platform_threads := null

#-----------------------------------------------------------------------------#
# Set this to what your platform has.
#   Current option(s): "sdl", "gtk", "null".
#-----------------------------------------------------------------------------#
platform_video := gtk
#platform_video := sdl
#platform_video := null

#-----------------------------------------------------------------------------#
# Set this to what your platform has.
#   Current option(s): "sdl", "unix", "null".
#-----------------------------------------------------------------------------#
platform_clock := unix
#platform_clock := sdl
#platform_clock := null

#-----------------------------------------------------------------------------#
# Choose a default natural language.
#   Current option(s): "english"
#-----------------------------------------------------------------------------#
platform_i18n := english

#-----------------------------------------------------------------------------#
# Set this to what your platform has.
#   Current option(s): "unix", "null".
#-----------------------------------------------------------------------------#
#platform_loader := null
platform_loader := unix

#-----------------------------------------------------------------------------#
# To use a different platform's ASM or portable C, change this.
#  Currently, this MUST be -DUSE_PORTABLE_C
#-----------------------------------------------------------------------------#
#USE_ASM := -DUSE_I386_ASM
USE_ASM := -DUSE_PORTABLE_C

#-----------------------------------------------------------------------------#
# You only need to set SDL_INC_DIR and SDL_LIB_DIR if you are using CygWin
#  as a compiler and SDL for video.
#
#  SDL_INC_DIR is where SDL.h and associated headers can be found, and
#  SDL_LIB_DIR is where SDL.lib and SDL.dll are located. These may be set as
#  environment variables, if you'd prefer to not hack the Makefile.
#
# Same thing applies for GTK+, if you are using that. Good luck.
#
# examples:
#   SDL_INC_DIR := C:/2/SDL-1.1.8/include
#   SDL_LIB_DIR := C:/2/SDL-1.1.8/lib
#-----------------------------------------------------------------------------#
ifeq ($(strip $(SDL_INC_DIR)),)
  SDL_INC_DIR := please_set_me_cygwin_users
endif

ifeq ($(strip $(SDL_LIB_DIR)),)
  SDL_LIB_DIR := please_set_me_cygwin_users
endif

ifeq ($(strip $(GTK_INC_DIR)),)
  GTK_INC_DIR := please_set_me_cygwin_users
endif

ifeq ($(strip $(GTK_LIB_DIR)),)
  GTK_LIB_DIR := please_set_me_cygwin_users
endif





#-----------------------------------------------------------------------------#
#-----------------------------------------------------------------------------#
#-----------------------------------------------------------------------------#
#-----------------------------------------------------------------------------#
# Everything below this line is probably okay.
#-----------------------------------------------------------------------------#
#-----------------------------------------------------------------------------#
#-----------------------------------------------------------------------------#
#-----------------------------------------------------------------------------#


#-----------------------------------------------------------------------------#
# CygWin autodetect.
#-----------------------------------------------------------------------------#

ifeq ($(strip $(cygwin)),autodetect)
  ifneq ($(strip $(shell gcc -v 2>&1 |grep "cygwin")),)
    cygwin := true
  else
    cygwin := false
  endif
endif


#-----------------------------------------------------------------------------#
# Set up video flags...
#-----------------------------------------------------------------------------#
using_sdl_at_all := false
ifeq ($(strip $(platform_video)),sdl)
  using_sdl_at_all := true
endif
ifeq ($(strip $(platform_threads)),sdl)
  using_sdl_at_all := true
endif
ifeq ($(strip $(platform_clock)),sdl)
  using_sdl_at_all := true
endif

ifeq ($(strip $(using_sdl_at_all)),true)
  ifeq ($(strip $(cygwin)),true)
    ifeq ($(strip $(SDL_INC_DIR)),please_set_me_cygwin_users)
      $(error Cygwin users need to set the SDL_INC_DIR envr var.)
    else
      CFLAGS += -I$(SDL_INC_DIR)
    endif

    ifeq ($(strip $(SDL_LIB_DIR)),please_set_me_cygwin_users)
      $(error Cygwin users need to set the SDL_LIB_DIR envr var.)
    else
      LDFLAGS += -L$(SDL_LIB_DIR) -lSDL
    endif
  else
    CFLAGS += $(shell sdl-config --cflags)
    LDFLAGS += $(shell sdl-config --libs)
  endif
endif

ifeq ($(strip $(platform_video)),gtk)
  ifeq ($(strip $(cygwin)),true)
    ifeq ($(strip $(GTK_INC_DIR)),please_set_me_cygwin_users)
      $(error Cygwin users need to set the GTK_INC_DIR envr var.)
    else
      CFLAGS += -I$(GTK_INC_DIR)
    endif

    ifeq ($(strip $(GTK_LIB_DIR)),please_set_me_cygwin_users)
      $(error Cygwin users need to set the GTK_LIB_DIR envr var.)
    else
      LDFLAGS += -L$(GTK_LIB_DIR) -lgtk -lgdk -lglib
    endif
  else
    CFLAGS += $(shell gtk-config --cflags)
    LDFLAGS += $(shell gtk-config --libs)
  endif
endif

#-----------------------------------------------------------------------------#
# Platform-specific binary stuff.
#-----------------------------------------------------------------------------#

ifeq ($(strip $(cygwin)),true)
  ASM = nasmw
  EXE_EXT = .exe
  ASMOBJFMT = win32
  ASMDEFS = -dC_IDENTIFIERS_UNDERSCORED
  CFLAGS += -DC_IDENTIFIERS_UNDERSCORED
else
  ASM = nasm
  EXE_EXT =
  ASMOBJFMT = elf
endif


#-----------------------------------------------------------------------------#
# General compiler, assembler, and linker flags.
#-----------------------------------------------------------------------------#

BINDIR := bin
SRCDIR := src

CFLAGS += $(USE_ASM) -I$(SRCDIR) -D_REENTRANT -fsigned-char -DPLATFORM_UNIX
CFLAGS += -Wall -Werror -fexceptions -frtti -D_REENTRANT

LDFLAGS += -lm

ifeq ($(strip $(debugging)),true)
  CFLAGS += -DDEBUG -g -fno-omit-frame-pointer
  LDFLAGS += -g -fno-omit-frame-pointer
else
  CFLAGS += -D_NDEBUG -O2 -fomit-frame-pointer
  LDFLAGS += -O2 -fomit-frame-pointer
endif

ifneq ($(strip $(USE_ASM)),-DUSE_PORTABLE_C)
  CFLAGS += -O2 -fasm
endif

ASMFLAGS := -f $(ASMOBJFMT) $(ASMDEFS)


#-----------------------------------------------------------------------------#
# Source and target names.
#-----------------------------------------------------------------------------#

MAINEXE := $(BINDIR)/toby$(strip $(EXE_EXT))
STANDALONEEXE := $(BINDIR)/standalone$(strip $(EXE_EXT))

EXES := $(STANDALONEEXE) #$(MAINEXE)

UTILSRCS := util/TobyCollection.cpp util/TobyStack.cpp util/TobyString.cpp \
            util/TobyLanguage.cpp
PARSERSRCS := parsers/Parser.cpp
TURTLESPACESRCS := turtlespace/Turtle.cpp turtlespace/TurtleSpace.cpp
IOSRCS := io/FileReader.cpp io/StringReader.cpp


#-----------------------------------------------------------------------------#
# Language modules
#-----------------------------------------------------------------------------#
CFLAGS += "-ATOBYI18N_DEFAULT($(strip $(platform_i18n)))"


#-----------------------------------------------------------------------------#
# Thread modules
#-----------------------------------------------------------------------------#
valid_threads_target := false
ifeq ($(strip $(platform_threads)),pthreads)
  THREADSRCS := platform/threads/pthreads/PthreadsThread.cpp \
                platform/threads/pthreads/PthreadsMutex.cpp
  THREADSDIR := pthreads
  LDFLAGS += -lpthread
  valid_threads_target := true
endif

ifeq ($(strip $(platform_threads)),sdl)
  THREADSRCS := platform/threads/sdlthreads/SDLThread.cpp \
                platform/threads/sdlthreads/SDLMutex.cpp
  THREADSDIR := sdlthreads
  valid_threads_target := true
endif

ifeq ($(strip $(platform_threads)),null)
  THREADSRCS := platform/threads/nullthreads/NullThread.cpp \
                platform/threads/nullthreads/NullMutex.cpp
  THREADSDIR := nullthreads
  valid_threads_target := true
endif

ifneq ($(strip $(valid_threads_target)),true)
  $(error platform_threads flag in Makefile is not valid.)
endif


#-----------------------------------------------------------------------------#
# Clock modules
#-----------------------------------------------------------------------------#
valid_clock_target := false
ifeq ($(strip $(platform_clock)),sdl)
    CLOCKSRCS := platform/clocks/sdlclock/SDLClock.cpp
    CLOCKDIR := sdlclock
    valid_clock_target := true
endif

ifeq ($(strip $(platform_clock)),unix)
    CLOCKSRCS := platform/clocks/unixclock/UnixClock.cpp
    CLOCKDIR := unixclock
    valid_clock_target := true
endif

ifeq ($(strip $(platform_clock)),null)
    CLOCKSRCS := platform/clocks/nullclock/NullClock.cpp
    CLOCKDIR := nullclock
    valid_clock_target := true
endif

ifneq ($(strip $(valid_clock_target)),true)
  $(error platform_clock flag in Makefile is not valid.)
endif


#-----------------------------------------------------------------------------#
# Loader modules
#-----------------------------------------------------------------------------#
valid_loader_target := false
ifeq ($(strip $(platform_loader)),unix)
    LOADERSRCS := platform/loaders/unixloader/UnixLoader.cpp
    LOADERDIR := unixloader
    valid_loader_target := true
endif

ifeq ($(strip $(platform_loader)),null)
    LOADERSRCS := platform/loaders/nullloader/NullLoader.cpp
    LOADERDIR := nullloader
    valid_loader_target := true
endif

ifneq ($(strip $(valid_loader_target)),true)
  $(error platform_loader flag in Makefile is not valid.)
endif


#-----------------------------------------------------------------------------#
# Video modules
#-----------------------------------------------------------------------------#
valid_video_target := false
ifeq ($(strip $(platform_video)),sdl)
  VIDEOSRCS := platform/renderers/fbrenderer/FrameBufferTurtleSpaceRenderer.cpp \
               platform/renderers/sdlrenderer/SDLTurtleSpaceRenderer.cpp
  VIDEODIR  := sdlrenderer
  valid_video_target := true
endif

ifeq ($(strip $(platform_video)),gtk)
  VIDEOSRCS := platform/renderers/gtkrenderer/GTKTurtleSpaceRenderer.cpp
  VIDEODIR  := gtkrenderer
  valid_video_target := true
endif

ifeq ($(strip $(platform_video)),null)
  VIDEOSRCS := platform/renderers/nullrenderer/NullTurtleSpaceRenderer.cpp
  VIDEODIR  := nullrenderer
  valid_video_target := true
endif

ifneq ($(strip $(valid_video_target)),true)
  $(error platform_video flag in Makefile is not valid.)
endif


#-----------------------------------------------------------------------------#
# Source and object parsing...
#-----------------------------------------------------------------------------#
COMMONSRCS := $(UTILSRCS) $(TURTLESPACESRCS) $(THREADSRCS) $(VIDEOSRCS) \
              $(PARSERSRCS) $(CLOCKSRCS) $(IOSRCS) $(LOADERSRCS)

# Rule for getting list of objects from source
COMMONOBJS1 := $(COMMONSRCS:.c=.o)
COMMONOBJS2 := $(COMMONOBJS1:.cpp=.o)
COMMONOBJS3 := $(COMMONOBJS2:.asm=.o)

COMMONOBJS := $(foreach f,$(COMMONOBJS3),$(BINDIR)/$(f))
COMMONSRCS := $(foreach f,$(COMMONSRCS),$(SRCDIR)/$(f))

CLEANUP = $(wildcard *.exe) $(wildcard *.obj) \
          $(wildcard $(BINDIR)/*.exe) $(wildcard $(BINDIR)/*.obj) \
          $(wildcard *~) $(wildcard *.err) \
          $(wildcard .\#*) core


#-----------------------------------------------------------------------------#
# Rules.
#-----------------------------------------------------------------------------#

# Rules for turning source files into .o files
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

.PHONY: all clean distclean listobjs showcfg

all: $(BINDIR) $(EXES)

$(MAINEXE) : $(BINDIR) $(COMMONOBJS) $(BINDIR)/toby.o
	$(LINKER) -o $(MAINEXE) $(LDFLAGS) $(COMMONOBJS) $(BINDIR)/toby.o

$(STANDALONEEXE) : $(BINDIR) $(COMMONOBJS) $(BINDIR)/standalone.o
	$(LINKER) -o $(STANDALONEEXE) $(LDFLAGS) $(COMMONOBJS) $(BINDIR)/standalone.o

$(BINDIR):
	mkdir -p $(BINDIR)
	mkdir -p $(BINDIR)/turtlespace
	mkdir -p $(BINDIR)/util
	mkdir -p $(BINDIR)/io
	mkdir -p $(BINDIR)/parsers/toby
	mkdir -p $(BINDIR)/platform/renderers/fbrenderer
	mkdir -p $(BINDIR)/platform/renderers/$(VIDEODIR)
	mkdir -p $(BINDIR)/platform/clocks/$(CLOCKDIR)
	mkdir -p $(BINDIR)/platform/threads/$(THREADSDIR)
	mkdir -p $(BINDIR)/platform/loaders/$(LOADERDIR)

distclean: clean

clean:
	rm -f $(CLEANUP)
	rm -rf $(BINDIR)

listobjs:
	@echo SOURCES:
	@echo $(COMMONSRCS)
	@echo
	@echo OBJECTS:
	@echo $(COMMONOBJS)
	@echo
	@echo EXECUTABLES:
	@echo $(EXES)

showcfg:
	@echo "Using CygWin          : $(cygwin)"
	@echo "Debugging             : $(debugging)"
	@echo "ASM flag              : $(USE_ASM)"
	@echo "Video module          : $(platform_video)"
	@echo "Thread module         : $(platform_threads)"
	@echo "Clock module          : $(platform_clock)"
	@echo "Loader module         : $(platform_loader)"
	@echo "Default i18n language : $(platform_i18n)"


#-----------------------------------------------------------------------------#
# This section is pretty much just for Ryan's use to make distributions.
#  You Probably Should Not Touch.
#-----------------------------------------------------------------------------#

# These are the files needed in a binary distribution, regardless of what
#  platform is being used.
BINSCOMMON  = $(MAINEXE) $(STANDALONEEXE)
BINSCOMMON += LICENSE.TXT

.PHONY: package msbins win32bins nocygwin
package: clean
	cd .. ; zip -9rz ./toby-src-$(shell date +%m%d%Y).zip toby -x "*CVS*" < toby/FILEID.DIZ


ifeq ($(strip $(cygwin)),true)
msbins: win32bins

win32bins: clean all
ifeq ($(strip $(platform_video)),sdl)
	cp $(SDL_LIB_DIR)/SDL.dll .
    EXTRAPACKAGELIBS := SDL.dll
endif
	echo -e "\r\n\r\n\r\nHEY YOU.\r\n\r\n\r\nTake a look at README-win32bins.txt FIRST.\r\n\r\n\r\n--ryan. (icculus@linuxgames.com)\r\n\r\n" |zip -9rz ../toby-win32bins-$(shell date +%m%d%Y).zip $(EXES) $(EXTRAPACKAGELIBS) README-win32bins.txt

else
msbins: nocygwin
win32bins: nocygwin
endif

nocygwin:
	@echo This must be done on a Windows box in the Cygwin environment.

#-----------------------------------------------------------------------------#
# That's all, folks.
#-----------------------------------------------------------------------------#

# end of Makefile ...
