/*

    ZHSC
    
    zhscwidget.h
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

#ifndef ZHSCWIDGET_H
#define ZHSCWIDGET_H

#include <qwidget.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qmap.h>
#include <qstring.h>
#include <qfont.h>
#include <qdatetime.h>
#include <qvaluelist.h>

#include "bookmark.h"
#include "bmdialog.h"

class ZhscWidget : public QWidget
{
  Q_OBJECT
public:
  ZhscWidget( QWidget* parent, const char* name );
  ~ZhscWidget();

  void init();

public slots:
  void dynastyActivated( int index );
  void poetActivated( int index );
  void poemActivated( int index );

  void changeFont( QFont font );
  
  void educe();

  void loadHistory();
  void addHistory();
  void history();
  void historyPrev();
  void historyNext();
  void clearHistory();

  void record();
  void recordFirst();
  void recordPrev();
  void recordNext();
  void recordLast();
  void recordRandom();
  void recordNumber();

  void bookmark();
  void addBookmark();
  void modifyBookmark();
  void clearBookmark();
  void selectBookmark( QString bm );

signals:
  void poemCount( int count );
  void poetCount( QString poet, int count );
  void poemIndex( int index );

  void historyPrevEnabled( bool enable );
  void historyNextEnabled( bool enable );

  void recordFirstEnabled( bool enable );
  void recordPrevEnabled( bool enable );
  void recordNextEnabled( bool enable );
  void recordLastEnabled( bool enable );

  void bookmarkChanged( BookmarkList bml );

private:
  void setPoem();

  void changeDynasty( int index );
  void changePoet( int index );
  void changePoem( int index );

  void buildCBDynasty();
  void buildCBPoet();
  void buildCBPoem();
  void buildTEPoet();
  void buildTEPoem();

  void calPoemCount();
  void calPoetCount();
  void calPoemIndex();

  int dynastyNoKey();
  int poetNoKey();
  int poemNoKey();

private:
  QGridLayout *lmain;

  QSplitter *spPoem;
  QSplitter *spPoet;
  
  QComboBox *cbDynasty;
  QComboBox *cbPoet;
  QComboBox *cbPoem;
  QTextEdit *tePoem;
  QTextEdit *teMemo;
  QTextEdit *tePoet;

  QString sDynastyNo;
  QString sPoetNo;
  QString sPoemNo;

  bool bConStatus;

  StringMap smDynasty;
  StringMap smPoet;
  StringMap smPoem;

  int nDynasty;
  int nPoet;
  int nPoem;

  int nPoemCount;
  int nPoetCount;
  int nPoemIndex;
  int nPoemMin;
  int nPoemMax;

  int nHistoryCount;
  int nHistoryIndex;
  int nHistoryMin;
  int nHistoryMax;
  QDateTime dtHistoryDT;

  bool bManualSign;

  BookmarkList bml;
};

#endif //ZHSCWIDGET_H

