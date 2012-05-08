/*

    ZHSC
    
    bookmark.h
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

#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <qmap.h>
#include <qstring.h>
#include <qtable.h>
#include <qvaluelist.h>
#include <qptrlist.h>

typedef QMap<int, QString> StringMap;

typedef struct
{
  QString dynastyno;
  QString poetno;
  QString poemno;
  QString content;
}Bookmark;

typedef QValueList<Bookmark> BookmarkList;

typedef QPtrList<QCheckTableItem> CTIPList;

#endif //BOOKMARK_H

