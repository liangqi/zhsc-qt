/*

    ZHSC
    
    zhscmw.h
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

#ifndef ZHSCMAINWINDOW_H
#define ZHSCMAINWINDOW_H

#include <qmainwindow.h>
#include <qaction.h>
#include <qlabel.h>

#include "zhscwidget.h"

class ZhscMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  ZhscMainWindow( QWidget* parent, const char* name );
  ~ZhscMainWindow();

public slots:
  void bookmarkChanged( BookmarkList bml );

signals:
  void selectBookmark( QString bm );

private slots:
  void bookmark( int index );

  void poemCount( int count );
  void poetCount( QString poet, int count );
  void poemIndex( int index );

  void about();
  void aboutQt();

private:
  ZhscWidget *zw;

  QAction *actExport;

  QAction *actHistoryPrev;
  QAction *actHistoryNext;
  QAction *actClearHistory;

  QAction *actRecordFirst;
  QAction *actRecordPrev;
  QAction *actRecordNext;
  QAction *actRecordLast;
  QAction *actRecordRandom;
  QAction *actRecordNumber;

  QAction *actAddBookmark;
  QAction *actModifyBookmark;
  QAction *actClearBookmark;

  QPopupMenu *pmBookmark;

  QLabel *lPoemCount;
  QLabel *lPoetCount;
  QLabel *lPoemIndex;
};

#endif //ZHSCMAINWINDOW_H

