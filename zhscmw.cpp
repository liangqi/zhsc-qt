/*

    ZHSC
    
    zhscmw.cpp
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

#include <qapplication.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qiconset.h>

#include "zhscmw.h"
#include "codec.h"

ZhscMainWindow::ZhscMainWindow( QWidget* parent, const char* name )
    : QMainWindow( parent, name )
{
  pmBookmark = NULL;
  
  setIcon( QPixmap( "img/poems.xpm" ) );
  setCaption( Unicode( "中华诗词 Qt版" ) );

  zw = new ZhscWidget( this, "Zhsc Widget" );
  
  QToolBar *tb = new QToolBar( this, "Tool Bar" );
  tb->setLabel( Unicode( "工具条" ) );

  QPopupMenu *pmSystem = new QPopupMenu( this );
  menuBar()->insertItem( Unicode( "系统(&S)" ), pmSystem );

  actExport = new QAction( Unicode( "导出到文件" ), Unicode( "导出(&E)" ), 0, this, 0 );
  connect( actExport, SIGNAL( activated() ), zw, SLOT( educe() ) );
  actExport->addTo( pmSystem );

  pmSystem->insertSeparator();

  pmSystem->insertItem( Unicode( "退出(&Q)" ), qApp, SLOT( closeAllWindows() ), CTRL+Key_Q );

  QPopupMenu *pmRecord = new QPopupMenu( this );
  menuBar()->insertItem( Unicode( "记录(&R)" ), pmRecord );

  actRecordFirst = new QAction( Unicode( "第一首记录" ), Unicode( "第一首(&F)" ), 0, this, 0 );
  connect( actRecordFirst, SIGNAL( activated() ), zw, SLOT( recordFirst() ) );
  connect( zw, SIGNAL( recordFirstEnabled( bool ) ), actRecordFirst, SLOT( setEnabled( bool ) ) );
  actRecordFirst->setIconSet( QPixmap( "img/first.xpm") );
  actRecordFirst->addTo( pmRecord );
  actRecordFirst->addTo( tb );  

  actRecordPrev = new QAction( Unicode( "上一首记录" ), Unicode( "上一首(&P)" ), 0, this, 0 );
  connect( actRecordPrev, SIGNAL( activated() ), zw, SLOT( recordPrev() ) );
  connect( zw, SIGNAL( recordPrevEnabled( bool ) ), actRecordPrev, SLOT( setEnabled( bool ) ) );
  actRecordPrev->setIconSet( QPixmap( "img/prev.xpm") );
  actRecordPrev->addTo( pmRecord );
  actRecordPrev->addTo( tb );

  actRecordNext = new QAction( Unicode( "下一首记录" ), Unicode( "下一首(&N)" ), 0, this, 0 );
  connect( actRecordNext, SIGNAL( activated() ), zw, SLOT( recordNext() ) );
  connect( zw, SIGNAL( recordNextEnabled( bool ) ), actRecordNext, SLOT( setEnabled( bool ) ) );
  actRecordNext->setIconSet( QPixmap( "img/next.xpm") );
  actRecordNext->addTo( pmRecord );
  actRecordNext->addTo( tb );

  actRecordLast = new QAction( Unicode( "最后一首记录" ), Unicode( "最后一首条(&L)" ), 0, this, 0 );
  connect( actRecordLast, SIGNAL( activated() ), zw, SLOT( recordLast() ) );
  connect( zw, SIGNAL( recordLastEnabled( bool ) ), actRecordLast, SLOT( setEnabled( bool ) ) );
  actRecordLast->setIconSet( QPixmap( "img/last.xpm") );
  actRecordLast->addTo( pmRecord );
  actRecordLast->addTo( tb );

  pmRecord->insertSeparator();
  tb->addSeparator();

  actRecordRandom = new QAction( Unicode( "乱翻书" ), Unicode( "乱翻书" ), 0, this, 0 );
  connect( actRecordRandom, SIGNAL( activated() ), zw, SLOT( recordRandom() ) );
  actRecordRandom->addTo( pmRecord );

  actRecordNumber = new QAction( Unicode( "第...首" ), Unicode( "第...首" ), 0, this, 0 );
  connect( actRecordNumber, SIGNAL( activated() ), zw, SLOT( recordNumber() ) );
  actRecordNumber->addTo( pmRecord );

  QPopupMenu *pmHistory = new QPopupMenu( this );
  menuBar()->insertItem( Unicode( "历史(&I)" ), pmHistory );

  actHistoryPrev = new QAction( Unicode( "上一首历史记录" ), Unicode( "上一首(&P)" ), 0, this, 0 );
  connect( actHistoryPrev, SIGNAL( activated() ), zw, SLOT( historyPrev() ) );
  connect( zw, SIGNAL( historyPrevEnabled( bool ) ), actHistoryPrev, SLOT( setEnabled( bool ) ) );
  actHistoryPrev->setIconSet( QPixmap( "img/hprev.xpm") );
  actHistoryPrev->addTo( pmHistory );
  actHistoryPrev->addTo( tb );

  actHistoryNext = new QAction( Unicode( "下一首历史记录" ), Unicode( "下一首(&N)" ), 0, this, 0 );
  connect( actHistoryNext, SIGNAL( activated() ), zw, SLOT( historyNext() ) );
  connect( zw, SIGNAL( historyNextEnabled( bool ) ), actHistoryNext, SLOT( setEnabled( bool ) ) );
  actHistoryNext->setIconSet( QPixmap( "img/hnext.xpm") );
  actHistoryNext->addTo( pmHistory );
  actHistoryNext->addTo( tb );

  pmHistory->insertSeparator();

  actClearHistory = new QAction( Unicode( "清空历史记录" ), Unicode( "清空历史记录(&C)" ), 0, this, 0 );
  connect( actClearHistory, SIGNAL( activated() ), zw, SLOT( clearHistory() ) );
  actClearHistory->addTo( pmHistory );

  pmBookmark = new QPopupMenu( this );
  menuBar()->insertItem( Unicode( "书签(&B)" ), pmBookmark );

  actAddBookmark = new QAction( Unicode( "添加书签" ), Unicode( "添加书签(&A)" ), 0, this, 0 );
  connect( actAddBookmark, SIGNAL( activated() ), zw, SLOT( addBookmark() ) );
  actAddBookmark->addTo( pmBookmark );

  actModifyBookmark = new QAction( Unicode( "修改书签" ), Unicode( "修改书签(&M)" ), 0, this, 0 );
  connect( actModifyBookmark, SIGNAL( activated() ), zw, SLOT( modifyBookmark() ) );
  actModifyBookmark->addTo( pmBookmark );

  actClearBookmark = new QAction( Unicode( "清空书签" ), Unicode( "清空书签(&C)" ), 0, this, 0 );
  connect( actClearBookmark, SIGNAL( activated() ), zw, SLOT( clearBookmark() ) );
  actClearBookmark->addTo( pmBookmark );

  pmBookmark->insertSeparator();

  connect( pmBookmark, SIGNAL( activated(int) ), this, SLOT( bookmark(int) ) );
  connect( this, SIGNAL( selectBookmark( QString ) ), zw, SLOT( selectBookmark( QString ) ) );

  QPopupMenu *pmHelp = new QPopupMenu( this );
  menuBar()->insertItem( Unicode ( "帮助(&H)" ), pmHelp );

  pmHelp->insertItem( Unicode( "关于(&A)" ), this, SLOT( about() ), Key_F1 );
  pmHelp->insertItem( Unicode( "关于Qt(&Q)" ), this, SLOT( aboutQt() ) );

  lPoemCount = new QLabel( this );
  statusBar()->addWidget( lPoemCount, 0, TRUE );

  lPoemIndex = new QLabel( this );
  statusBar()->addWidget( lPoemIndex, 0, TRUE );

  lPoetCount = new QLabel( this );
  statusBar()->addWidget( lPoetCount, 0, TRUE );

  connect( zw, SIGNAL( poemCount( int ) ), this, SLOT( poemCount( int ) ) );
  connect( zw, SIGNAL( poetCount( QString, int ) ), this, SLOT( poetCount( QString, int ) ) );
  connect( zw, SIGNAL( poemIndex( int ) ), this, SLOT( poemIndex( int ) ) );

  connect( zw, SIGNAL( bookmarkChanged( BookmarkList ) ), this, SLOT( bookmarkChanged( BookmarkList ) ) );

  zw->init();

  setFocusProxy( zw );
  setCentralWidget( zw );
  showMaximized();
}

ZhscMainWindow::~ZhscMainWindow()
{
}

void ZhscMainWindow::bookmark( int index )
{
  QString bm;

  bm = pmBookmark->text( index );

  emit selectBookmark( bm );
}

void ZhscMainWindow::bookmarkChanged( BookmarkList bml )
{
  if ( pmBookmark != NULL )
  {
    pmBookmark->clear();

    actAddBookmark = new QAction( Unicode( "添加书签" ), Unicode( "添加书签(&A)" ), 0, this, 0 );
    connect( actAddBookmark, SIGNAL( activated() ), zw, SLOT( addBookmark() ) );
    actAddBookmark->addTo( pmBookmark );

    actModifyBookmark = new QAction( Unicode( "修改书签" ), Unicode( "修改书签(&M)" ), 0, this, 0 );
    connect( actModifyBookmark, SIGNAL( activated() ), zw, SLOT( modifyBookmark() ) );
    actModifyBookmark->addTo( pmBookmark );

    actClearBookmark = new QAction( Unicode( "清空书签" ), Unicode( "清空书签(&C)" ), 0, this, 0 );
    connect( actClearBookmark, SIGNAL( activated() ), zw, SLOT( clearBookmark() ) );
    actClearBookmark->addTo( pmBookmark );

    pmBookmark->insertSeparator();

    BookmarkList::Iterator it;

    for ( it = bml.begin(); it != bml.end(); ++it )
    {
      pmBookmark->insertItem( (*it).content );
    }
  }
}

void ZhscMainWindow::about()
{
  QMessageBox::about( this, Unicode( "中华诗词 Qt版" ),
    Unicode( "中华诗词 Qt版\n0.0.1版\n\n改编自 方旻的中华诗词6000\n中华诗词主页：http://www.zhsc.com\n方旻的Email：roadcom@zhsc.com\n\n作者：Cavendish\n个人主页：http://www.qiliang.net\nEmail：cavendish@qiliang.net\n2003年6月23日" ) );
}

void ZhscMainWindow::aboutQt()
{
  QMessageBox::aboutQt( this, Unicode( "关于Qt" ) );
}

void ZhscMainWindow::poemCount( int count )
{
  if ( lPoemCount != NULL )
  {
    lPoemCount->setText( Unicode( "共%1首作品" ).arg( count ) );
  }
}

void ZhscMainWindow::poetCount( QString poet, int count )
{
  if ( lPoetCount != NULL )
  {
    lPoetCount->setText( Unicode( "%1的作品共%2首" ).arg( poet ).arg( count ) );
  }
}

void ZhscMainWindow::poemIndex( int index )
{
  if ( lPoemIndex != NULL )
  {
    lPoemIndex->setText( Unicode( "当前为总第%1首" ).arg( index ) );
  }
}

