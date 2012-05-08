/*

    ZHSC
    
    codec.h
    v 0.0.1
    2003/06/23

    Copyright (C) 2003 Cavendish
                       cavendish@qiliang.net
                       http://www.qiliang.net/software/zhsc.html

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

*/

#ifndef CODEC_H
#define CODEC_H

#include <qtextcodec.h>

extern QTextCodec *g_pCodec;

#define Unicode(x) g_pCodec->toUnicode(x)

#endif //CODEC_h
