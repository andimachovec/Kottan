## Haiku Generic Makefile v2.6 ## 


NAME = Kottan
TYPE = APP
APP_MIME_SIG = application/x-vnd.BlueSky-Kottan

#%{
# @src->@ 
SRCS = \
	 src/app.cpp  \
	 src/mainwindow.cpp  \

RDEFS = \
	Kottan.rdef

RSRC = \

# @<-src@ 
#%}

LIBS = $(STDCPPLIBS) be root localestub tracker
LIBPATHS = 
SYSTEM_INCLUDE_PATHS = /boot/system/develop/headers/private/interface
LOCAL_INCLUDE_PATHS =
OPTIMIZE := 
LOCALES = en de
DEFINES=
WARNINGS=
SYMBOLS :=
DEBUGGER :=
COMPILER_FLAGS = -std=c++11
LINKER_FLAGS =
APP_VERSION :=

DEVEL_DIRECTORY := \
	$(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine

