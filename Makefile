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
#  Written by Ryan C. Gordon (icculus@clutteredmind.org)
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
#debug := false
debug := true

#-----------------------------------------------------------------------------#
# Set this to true to link with Electric Fence. If you don't
#  know what that is, you better not set it.
#  ElectricFence can be found at ftp://ftp.perens.com/pub/ElectricFence/
#-----------------------------------------------------------------------------#
use_efence := false
#use_efence := true

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
#  Please report incorrect translations to Ryan: icculus@clutteredmind.org.
#  Ryan would also love to hear from you if you are willing to translate this
#  software to other natural languages; it's easy to do, if you are bilingual
#  in the first place.
#
#   Current option(s): "english", "french"
#-----------------------------------------------------------------------------#
#platform_i18n := french
platform_i18n := english

#-----------------------------------------------------------------------------#
# Set this to what your platform has.
#   Current option(s): "unix", "null".
#-----------------------------------------------------------------------------#
#platform_loader := null
platform_loader := unix

#-----------------------------------------------------------------------------#
# Set this accordingly. If you choose the XML lexer, you need to be
#  building for a platform that has a working dynamic loader (you can use
#  the NullLoader to compile, but it renders the parsers useless), and has
#  an actual filesystem. The XML loader gives you more flexibility at the
#  cost of more initial overhead and some code bloat. More importantly, the
#  XML lexer can dynamically generate C++ code that duplicates its
#  functionality. This code can be generated on any platform that supports the
#  XML lexer, and then be compiled in for a more lean (but more ridgid)
#  equivalent lexer that should work on all platforms that can't use
#  the XML lexer for whatever reason. This generated code comprises the
#  "static" lexer. The static lexer can be used on any platform, if the code
#  has already been generated (we keep a revision of that code around, so you
#  can have a choice without having to build one then the other). The Null
#  lexer is just for stubbing out a rather important subsystem while porting
#  to a extremely foreign platform.
#
# If you are really confused, the general rule is:
#  Choose "xml", unless you're building for an unusual platform (PalmOS, a
#  port to a new platform), in which case choose "static".
#
#   Current option(s): "xml", "static", "null"
#-----------------------------------------------------------------------------#
platform_lexer := static
#platform_lexer := null
#platform_lexer := xml

#-----------------------------------------------------------------------------#
# Choose all that apply. These are parsers for languages that you will be
#  linking statically into the program. Each one you link in adds more bulk
#  to the binary, but it's one less thing that can go wrong later. If you
#  want to use a language, it MUST be listed either here or in the
#  "dynamic_parsers" section, below. Systems without a dynamic loader
#  (PalmOS, etc) must list all languages they want to use in this section.
#  Each language should be lowercase, and separated by a space character.
#
# If you are really confused, the defaults are probably okay.
#
#   Current option(s): "Toby"
#-----------------------------------------------------------------------------#
#static_parsers :=
static_parsers := Toby

#-----------------------------------------------------------------------------#
# Choose all that apply. These are parsers for languages that you will be
#  linking dynamically against the program. The usual arguments for shared
#  libraries apply here; modularity, plugins, potential errors, yaddayadda...
#  If you want to use a language, it MUST be listed either here or in the
#  "static_parsers" section, above. Systems without a dynamic loader
#  (PalmOS, etc) must list any languages they want to use IN THE STATIC
#  PARSERS SECTION. Each language should be lowercase, and separated by
#  a space character.
#
# If you are really confused, the defaults are probably okay.
#
#   Current option(s): "toby"
#-----------------------------------------------------------------------------#
#dynamic_parsers := toby
dynamic_parsers :=

#-----------------------------------------------------------------------------#
# To use a different platform's ASM or portable C, change this.
#  Currently, this MUST be -DUSE_PORTABLE_C
#-----------------------------------------------------------------------------#
#USE_ASM := -DUSE_I386_ASM
USE_ASM := -DUSE_PORTABLE_C


#-----------------------------------------------------------------------------#
# Set this accordingly.
#-----------------------------------------------------------------------------#
#strcasecmp := stricmp
strcasecmp := strcasecmp

#-----------------------------------------------------------------------------#
# You only need to set SDL_INC_DIR and SDL_LIB_DIR if you are using CygWin
#  as a compiler and SDL for some subsystem or another.
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
# Basic initialization.
#-----------------------------------------------------------------------------#
need_xml := false


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

CFLAGS += -Dstrcasecmp=$(strcasecmp) $(EXTRAINCLUDES)

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

ifeq ($(strip $(use_efence)),true)
  EFENCELIB := /usr/lib/libefence.a
endif

ifeq ($(strip $(debug)),true)
  CFLAGS += -DDEBUG -g -fno-omit-frame-pointer
  LDFLAGS += -g -fno-omit-frame-pointer
else
  CFLAGS += -DNDEBUG -s -O2 -fomit-frame-pointer
  LDFLAGS += -s -O2 -fomit-frame-pointer
endif

ifneq ($(strip $(USE_ASM)),-DUSE_PORTABLE_C)
  CFLAGS += -O2 -fasm
endif

ASMFLAGS := -f $(ASMOBJFMT) $(ASMDEFS)


#-----------------------------------------------------------------------------#
# Language modules.
#-----------------------------------------------------------------------------#
CFLAGS += "-ATOBYI18N_DEFAULT($(strip $(platform_i18n)))"


#-----------------------------------------------------------------------------#
# Thread modules.
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
# Clock modules.
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
# Loader modules.
#-----------------------------------------------------------------------------#
valid_loader_target := false
ifeq ($(strip $(platform_loader)),unix)
    LOADERSRCS := platform/loaders/unixloader/UnixLoader.cpp
    LOADERDIR := unixloader
    LDFLAGS += -ldl
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
# Video modules.
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
# Lexer modules.
#-----------------------------------------------------------------------------#
valid_lexer_target := false
ifeq ($(strip $(platform_lexer)),xml)
  need_xml := true
  LEXERSRCS := platform/lexers/xmllexer/XMLLexer.cpp \
               platform/lexers/xmllexer/LanguageRulesXML.cpp \
               platform/lexers/xmllexer/TokenizerRulesXML.cpp \
               platform/lexers/xmllexer/ElementRulesXML.cpp \
               platform/lexers/xmllexer/RepeatRulesXML.cpp \
               platform/lexers/xmllexer/PickOneRulesXML.cpp \
               platform/lexers/xmllexer/ReqWordRulesXML.cpp \
               platform/lexers/xmllexer/ReqEOFRulesXML.cpp \
               platform/lexers/xmllexer/ReqNewlineRulesXML.cpp \
               platform/lexers/xmllexer/ReqLiteralStringRulesXML.cpp \
               platform/lexers/xmllexer/ReqWhitespaceRulesXML.cpp \
               platform/lexers/xmllexer/ReqSingleLineCommentRulesXML.cpp \
               platform/lexers/xmllexer/ReqMultiLineCommentRulesXML.cpp \
               platform/lexers/xmllexer/ReqNumberRulesXML.cpp \
               platform/lexers/xmllexer/ReqWordCharsRulesXML.cpp \
               platform/lexers/xmllexer/ReqCharRulesXML.cpp \
               platform/lexers/xmllexer/ReqElementRulesXML.cpp
  LEXERDIR  := xmllexer
  valid_lexer_target := true
endif

ifeq ($(strip $(platform_lexer)),static)
  LEXERSRCS := platform/lexers/staticlexer/StaticLexer.cpp
  LEXERSRCS += $(foreach f,$(static_parsers),platform/lexers/staticlexer/Static$(f)Lexer.cpp)
  LEXERDIR  := staticlexer
  valid_lexer_target := true
endif

ifeq ($(strip $(platform_lexer)),null)
  LEXERSRCS := platform/lexers/nulllexer/NullLexer.cpp
  LEXERDIR  := nulllexer
  valid_lexer_target := true
endif

ifneq ($(strip $(valid_lexer_target)),true)
  $(error platform_lexer flag in Makefile is not valid.)
endif


#-----------------------------------------------------------------------------#
# Parser modules.
#-----------------------------------------------------------------------------#

CFLAGS += $(foreach f,$(static_parsers),-DTOBYPARSER_$(shell echo $(f) |tr a-z A-Z))
CFLAGS += $(foreach f,$(dynamic_parsers),-DTOBYPARSER_$(shell echo $(f) |tr a-z A-Z))
PARSERSRCS := $(foreach f,$(static_parsers),parsers/$(f)Parser.cpp)

# (!!! There's no dynamic loading support in the XML parser yet, either.)
ifneq ($(strip $(dynamic_parsers)),)
  $(error Can't build dynamic parsers, yet.)
endif

#-----------------------------------------------------------------------------#
# Source and target names.
#-----------------------------------------------------------------------------#

MAINEXE := $(BINDIR)/toby$(strip $(EXE_EXT))
STANDALONEEXE := $(BINDIR)/standalone/toby$(strip $(EXE_EXT))
LEXGENEXE := $(BINDIR)/standalone/tobylexgen$(strip $(EXE_EXT))

EXES := $(STANDALONEEXE) #$(MAINEXE)
ifeq ($(strip $(platform_lexer)),xml)
  EXES += $(LEXGENEXE)
endif

TESTTOKENIZEREXE := $(BINDIR)/test/tokenizer$(strip $(EXE_EXT))
TESTXMLEXE := $(BINDIR)/test/xml$(strip $(EXE_EXT))
TESTGRAMMAREXE := $(BINDIR)/test/grammar$(strip $(EXE_EXT))
TESTLEXEREXE := $(BINDIR)/test/lexer$(strip $(EXE_EXT))

TESTEXES := $(TESTTOKENIZEREXE) $(TESTLEXEREXE)
ifeq ($(strip $(need_xml)),true)
  TESTEXES += $(TESTXMLEXE) $(TESTGRAMMAREXE)
  XMLSRCS := xml/XMLTree.cpp xml/XMLNode.cpp
endif

UTILSRCS += util/TobyCollection.cpp util/TobyStack.cpp util/TobyString.cpp \
            util/TobyLanguage.cpp util/TobyClock.cpp

PARSERSRCS += parsers/Parser.cpp

TURTLESPACESRCS += turtlespace/Turtle.cpp turtlespace/TurtleSpace.cpp

IOSRCS += io/TobyReader.cpp io/FileReader.cpp io/StringReader.cpp \
          io/Tokenizer.cpp

LEXERSRCS += lexer/Lexer.cpp lexer/LexerRules.cpp lexer/LanguageRules.cpp \
             lexer/TokenizerRules.cpp lexer/ElementRules.cpp \
             lexer/RepeatRules.cpp lexer/PickOneRules.cpp \
             lexer/ReqEOFRules.cpp lexer/ReqWordRules.cpp \
             lexer/ReqNewlineRules.cpp lexer/ReqLiteralStringRules.cpp \
             lexer/ReqWhitespaceRules.cpp lexer/ReqMultiLineCommentRules.cpp \
             lexer/ReqSingleLineCommentRules.cpp lexer/ReqNumberRules.cpp \
             lexer/ReqWordCharsRules.cpp lexer/ReqCharRules.cpp \
             lexer/ReqElementRules.cpp
#-----------------------------------------------------------------------------#
# Source and object parsing...
#-----------------------------------------------------------------------------#
COMMONSRCS := $(UTILSRCS) $(TURTLESPACESRCS) $(THREADSRCS) $(VIDEOSRCS) \
              $(PARSERSRCS) $(CLOCKSRCS) $(IOSRCS) $(LOADERSRCS) $(XMLSRCS) \
              $(LEXERSRCS)

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

.PHONY: all tests clean distclean listobjs showcfg showflags

all: $(BINDIR) $(EXES)

$(MAINEXE) : $(BINDIR) $(COMMONOBJS) $(BINDIR)/toby.o
	$(LINKER) -o $(MAINEXE) $(LDFLAGS) $(COMMONOBJS) $(BINDIR)/toby.o

$(STANDALONEEXE) : $(BINDIR) $(COMMONOBJS) $(BINDIR)/standalone/toby.o
	$(LINKER) -o $(STANDALONEEXE) $(LDFLAGS) $(COMMONOBJS) $(BINDIR)/standalone/toby.o

$(LEXGENEXE) : $(BINDIR) $(COMMONOBJS) $(BINDIR)/standalone/tobylexgen.o
	$(LINKER) -o $(LEXGENEXE) $(LDFLAGS) $(COMMONOBJS) $(BINDIR)/standalone/tobylexgen.o $(EFENCELIB)

tests: $(BINDIR) $(TESTEXES)

$(TESTTOKENIZEREXE) : $(BINDIR) $(COMMONOBJS) $(BINDIR)/test/tokenizer.o
	$(LINKER) -o $(TESTTOKENIZEREXE) $(LDFLAGS) $(COMMONOBJS) $(BINDIR)/test/tokenizer.o

$(TESTXMLEXE) : $(BINDIR) $(COMMONOBJS) $(BINDIR)/test/xml.o
	$(LINKER) -o $(TESTXMLEXE) $(LDFLAGS) $(COMMONOBJS) $(BINDIR)/test/xml.o

$(TESTGRAMMAREXE) : $(BINDIR) $(COMMONOBJS) $(BINDIR)/test/grammar.o
	$(LINKER) -o $(TESTGRAMMAREXE) $(LDFLAGS) $(COMMONOBJS) $(BINDIR)/test/grammar.o

$(TESTLEXEREXE) : $(BINDIR) $(COMMONOBJS) $(BINDIR)/test/lexer.o
	$(LINKER) -o $(TESTLEXEREXE) $(LDFLAGS) $(COMMONOBJS) $(BINDIR)/test/lexer.o


$(BINDIR):
	mkdir -p $(BINDIR)
	mkdir -p $(BINDIR)/standalone
	mkdir -p $(BINDIR)/test
	mkdir -p $(BINDIR)/turtlespace
	mkdir -p $(BINDIR)/util
	mkdir -p $(BINDIR)/io
	mkdir -p $(BINDIR)/xml
	mkdir -p $(BINDIR)/lexer
	mkdir -p $(BINDIR)/parsers
	mkdir -p $(BINDIR)/platform/renderers/fbrenderer
	mkdir -p $(BINDIR)/platform/renderers/$(VIDEODIR)
	mkdir -p $(BINDIR)/platform/clocks/$(CLOCKDIR)
	mkdir -p $(BINDIR)/platform/threads/$(THREADSDIR)
	mkdir -p $(BINDIR)/platform/loaders/$(LOADERDIR)
	mkdir -p $(BINDIR)/platform/lexers/$(LEXERDIR)

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
	@echo "Compiler              : $(CC)"
	@echo "Using CygWin          : $(cygwin)"
	@echo "Debugging             : $(debug)"
	@echo "ASM flag              : $(USE_ASM)"
	@echo "Using ElectricFence   : $(use_efence)"
	@echo "Video module          : $(platform_video)"
	@echo "Thread module         : $(platform_threads)"
	@echo "Clock module          : $(platform_clock)"
	@echo "Loader module         : $(platform_loader)"
	@echo "Lexer module          : $(platform_lexer)"
	@echo "Default i18n language : $(platform_i18n)"
	@echo "Parser module(s)      : $(static_parsers) $(dynamic_parsers)"

showflags:
	@echo 'CFLAGS  : $(CFLAGS)'
	@echo 'LDFLAGS : $(LDFLAGS)'

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
	echo -e "\r\n\r\n\r\nHEY YOU.\r\n\r\n\r\nTake a look at README-win32bins.txt FIRST.\r\n\r\n\r\n--ryan. (icculus@clutteredmind.org)\r\n\r\n" |zip -9rz ../toby-win32bins-$(shell date +%m%d%Y).zip $(EXES) $(EXTRAPACKAGELIBS) README-win32bins.txt

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
