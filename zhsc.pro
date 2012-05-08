#
#
#   ZHSC
#
#   zhsc.pro
#   v 0.0.1
#   2003/06/23
#
#   Copyright (C) 2003 Cavendish
#                      cavendish@qiliang.net
#                      http://www.qiliang.net/software/zhsc.html
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#
SOURCES	+= main.cpp \
	connection.cpp \
	zhscmw.cpp \
	zhscwidget.cpp \
	bmdialog.cpp 
HEADERS	+= connection.h \
	zhscmw.h \
	zhscwidget.h \
	bmdialog.h \
	bookmark.h \
	codec.h
TRANSLATIONS =  qt_zh_CN.ts
TEMPLATE	=app
CONFIG	+= qt warn_on release
LANGUAGE	= C++
