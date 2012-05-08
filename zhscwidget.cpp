/*

    ZHSC
    
    zhscwidget.cpp
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

#include <qstring.h>
#include <qsqlquery.h>
#include <qsqlcursor.h>
#include <qsqlindex.h>
#include <qcstring.h>
#include <qstring.h>
#include <qvariant.h>
#include <qinputdialog.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <qgb18030codec.h>

#include <stdlib.h>

#include "zhscwidget.h"
#include "codec.h"
#include "connection.h"

ZhscWidget::ZhscWidget( QWidget* parent, const char* name )
    : QWidget( parent, name )
{
  bConStatus = createConnections();

  cbDynasty = NULL;
  cbPoet = NULL;
  cbPoem = NULL;
  tePoem = NULL;
  teMemo = NULL;
  tePoet = NULL;

  nDynasty = -1;
  nPoet = -1;
  nPoem = -1;

  nHistoryCount = -1;
  nHistoryIndex = -1;
  nHistoryMin = -1;
  nHistoryMax = -1;

  nPoemCount = 0;
  nPoetCount = 0;
  nPoemIndex = 0;
  nPoemMin = 0;
  nPoemMax = 0;

  sDynastyNo = "";
  sPoetNo = "";
  sPoemNo = "";

  bManualSign = FALSE;

  QFont font = this->font();
  
  font.setPointSize( font.pointSize() * 1.4 );

  this->setFont( font );

  if ( bConStatus ) 
  {
    lmain = new QGridLayout( this, 10, 2, 10, 5 );
    lmain->setRowStretch( 0, 10 );
    lmain->setRowStretch( 1, 20 );

    cbDynasty = new QComboBox( FALSE, this, "dynasty" );

    cbPoet = new QComboBox( FALSE, this, "poet" );

    cbPoem = new QComboBox( FALSE, this, "caption" );

    lmain->addMultiCellWidget( cbDynasty, 0, 0, 0, 1 );
    lmain->addMultiCellWidget( cbPoet, 0, 0, 2, 3 );
    lmain->addMultiCellWidget( cbPoem, 0, 0, 4, 6 );

    spPoem = new QSplitter( this );

    tePoem = new QTextEdit( spPoem, "Poem" );

    spPoet = new QSplitter( spPoem );
    spPoet->setOrientation( Qt::Vertical );

    teMemo = new QTextEdit( spPoet, "Memo" );
    tePoet = new QTextEdit( spPoet, "Poet" );

    lmain->addMultiCellWidget( spPoem, 1, 1, 0, 9 );

    changeFont( font );

    connect( cbDynasty, SIGNAL(activated( int )), this, SLOT(dynastyActivated( int )) );
    connect( cbPoet, SIGNAL(activated( int )), this, SLOT(poetActivated( int )) );
    connect( cbPoem, SIGNAL(activated( int )), this, SLOT(poemActivated( int )) );
  }
}

ZhscWidget::~ZhscWidget()
{
}

void ZhscWidget::init()
{
  if ( bConStatus ) 
  {
    loadHistory();

    buildCBDynasty();
    buildCBPoet();
    buildCBPoem();
    buildTEPoet();
    buildTEPoem();

    setPoem();

    record();

    bookmark();
  }
}

void ZhscWidget::educe()
{
  if ( !bConStatus ) 
  {
    return;
  }

  uint len = 0;
  uint index = 0;
  int n = 0;

  QString tmp;

  QChar c( 0x2028 );

  tePoem->setTextFormat( Qt::PlainText );
  teMemo->setTextFormat( Qt::PlainText );
  tePoet->setTextFormat( Qt::PlainText );

  tmp = tePoem->text() + "\n" + teMemo->text() + "\n" + tePoet->text();

  tePoem->setTextFormat( Qt::AutoText );
  teMemo->setTextFormat( Qt::AutoText );
  tePoet->setTextFormat( Qt::AutoText );

  len = tmp.length();
  
  while ( 1 )
  {
    n = tmp.find( c, index );
    if ( n < 0 )
      break;
    tmp.replace( n, 1, "\n" );
    index = n;
  }

  QString sFileName;

  QFileDialog *fd = new QFileDialog( this );

  fd->setMode( QFileDialog::AnyFile );
  fd->setFilter( Unicode( "所有文件 (*.*)" ) );
  fd->setViewMode( QFileDialog::Detail );
  fd->setCaption( Unicode( "中华诗词 Qt版" ) );

  if ( fd->exec() == QDialog::Accepted )
  {
    sFileName = fd->selectedFile(); 
    if ( QFile::exists( sFileName ) )
    {
      if ( QMessageBox::warning( NULL, Unicode( "中华诗词 Qt版" ),
          Unicode( "这个文件已经存在，覆盖它？" ),
          Unicode( "确定" ),
          Unicode( "取消" ), 0, 0, 1 ) == 1)
      {
        delete fd;
        return;
      }
    }
    delete fd;
  }

  if ( sFileName != "" )
  {
    QFile f( sFileName );
    if ( f.open( IO_WriteOnly | IO_Truncate ) ) 
    {
      QTextCodec *codec = new QGb18030Codec();
      QTextStream ts( &f );
      ts.setCodec( codec );
      ts << tmp;
      f.close();
    }
  }
  
  return;
}

void ZhscWidget::setPoem()
{
  int i;

  i = dynastyNoKey();

  if ( i >= 0 )
  {
    cbDynasty->setCurrentItem( i );
    changeDynasty( i );
  }

  i = poetNoKey();

  if ( i >= 0 )
  {
    cbPoet->setCurrentItem( i );
    changePoet( i );
  }

  i = poemNoKey();

  if ( i >= 0 )
  {
    cbPoem->setCurrentItem( i );
    changePoem( i );
  }

  calPoemCount();
}

void ZhscWidget::dynastyActivated( int index )
{
  bManualSign = TRUE;

  changeDynasty( index );
}

void ZhscWidget::poetActivated( int index )
{
  bManualSign = TRUE;

  changePoet( index );
}

void ZhscWidget::poemActivated( int index )
{
  bManualSign = TRUE;

  changePoem( index );
}

void ZhscWidget::changeDynasty( int index )
{
  nDynasty = index;
  nPoet = -1;
  nPoem = -1;

  buildCBPoet();

  sDynastyNo = smDynasty[index];

  if ( bManualSign )
  {
    bManualSign = FALSE;

    cbPoet->setCurrentItem( cbPoet->currentItem() );
    changePoet( cbPoet->currentItem() );
  }
  else
  {
    cbPoet->setCurrentItem( poetNoKey() );
    changePoet( poetNoKey() );
  }
}

void ZhscWidget::changePoet( int index )
{
  nPoet = index;
  nPoem = -1;

  buildCBPoem();
  buildTEPoet();

  sPoetNo = smPoet[index];

  if ( bManualSign )
  {
    bManualSign = FALSE;

    cbPoem->setCurrentItem( cbPoem->currentItem() );
    changePoem( cbPoem->currentItem() );
  }
  else
  {
    cbPoem->setCurrentItem( poemNoKey() );
    changePoem( poemNoKey() );
  }

  calPoetCount();
}

void ZhscWidget::changePoem( int index )
{
  nPoem = index;

  buildTEPoem();

  sPoemNo = smPoem[index];

  if ( bManualSign )
  {
    bManualSign = FALSE;
    addHistory();
  }

  calPoemIndex();
}

void ZhscWidget::changeFont( QFont font )
{
  if ( cbDynasty != NULL)
  {
    cbDynasty->setFont( font );
  }

  if ( cbPoet != NULL)
  {
    cbPoet->setFont( font );
  }

  if ( cbPoem != NULL)
  {
    cbPoem->setFont( font );
  }

  if ( tePoem != NULL)
  {
    tePoem->setFont( font );
  }

  if ( teMemo != NULL)
  {
    teMemo->setFont( font );
  }

  if ( tePoet != NULL)
  {
    tePoet->setFont( font );
  }
}

void ZhscWidget::buildCBDynasty()
{
  int i = 0;

  if ( bConStatus )
  {
    QString sq = "select distinct dynastyno, dynasty from tsc order by dynastyno";
    QSqlQuery query( sq );

    cbDynasty->clear();
    smDynasty.clear();

    while ( query.next() )
    {
      cbDynasty->insertItem( trUtf8( query.value(1).toByteArray() ) );
      smDynasty[i] = query.value(0).toString();
      i++;
    }
  }
}

void ZhscWidget::buildCBPoet()
{
  int i = 0;

  if ( bConStatus )
  {
    QString sq = "select distinct poetno, poet from tsc where dynastyno = '" + smDynasty[nDynasty] + "' order by dynastyno, poetno";
    QSqlQuery query( sq );
    
    cbPoet->clear();
    smPoet.clear();

    while ( query.next() )
    {
      cbPoet->insertItem( trUtf8( query.value(1).toByteArray() ) );
      smPoet[i] = query.value(0).toString();
      i++;
    }
  }
}

void ZhscWidget::buildCBPoem()
{
  int i = 0;

  if ( bConStatus )
  {
    QString sq = "select distinct poemno, caption from tsc where dynastyno = '" + smDynasty[nDynasty] + "' and poetno = '" + smPoet[nPoet] + "' order by dynastyno, poetno, poemno";
    QSqlQuery query( sq );
    
    cbPoem->clear();
    smPoem.clear();

    while ( query.next() )
    {
      cbPoem->insertItem( trUtf8( query.value(1).toByteArray() ) );
      smPoem[i] = query.value(0).toString();
      i++;
    }
  }
}

void ZhscWidget::buildTEPoet()
{
  if ( bConStatus )
  {
    QString sq1 = "select content from poet where dynastyno = '" + smDynasty[nDynasty] + "' and poetno = '" + smPoet[nPoet] + "'";
    QSqlQuery query1( sq1 );
    
    tePoet->setText( "" );

    if ( query1.size() > 0 )
    {
      if ( query1.next() )
      {
        tePoet->setText( trUtf8( query1.value(0).toByteArray() ) );
      }
    }
    else
    {
      QString sq2 = "select content from poet where dynastyno = 'O' and poetno = '0023'";
      QSqlQuery query2( sq2 );

      if ( query2.size() > 0 )
      {
        if ( query2.next() )
        {
          tePoet->setText( trUtf8( query2.value(0).toByteArray() ).replace( 11, 2, cbPoet->currentText() ) );
        }
      }
    }
  }
}

void ZhscWidget::buildTEPoem()
{
  if ( bConStatus )
  {
    QString sq = "select content, memo from poem where dynastyno = '" + smDynasty[nDynasty] + "' and poetno = '" + smPoet[nPoet] + "' and poemno = '" + smPoem[nPoem] + "'";
    QSqlQuery query( sq );
    
    tePoem->setText( "" );
    teMemo->setText( "" );

    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        tePoem->setText( trUtf8( query.value(0).toByteArray() ) );
        teMemo->setText( trUtf8( query.value(1).toByteArray() ) );
      }
    }
  }
}

void ZhscWidget::calPoemCount()
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    QString sq = "select count(*), min(id), max(id) from poem";
    QSqlQuery query( sq );
    
    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        nPoemCount = query.value(0).toInt( &ok );
        nPoemMin = query.value(1).toInt( &ok );
        nPoemMax = query.value(2).toInt( &ok );
      }
    }
    else
    {
      nPoemCount = 0;
      nPoemMin = 0;
      nPoemMax = 0;
    }

    emit poemCount( nPoemCount );
  }
}

void ZhscWidget::calPoetCount( )
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    QString sq = "select count(*) from poem where dynastyno = '" + smDynasty[nDynasty] + "' and poetno = '" + smPoet[nPoet] + "'";
    QSqlQuery query( sq );
    
    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        nPoetCount = query.value(0).toInt( &ok );
      }
    }
    else
    {
      nPoetCount = 0;
    }

    emit poetCount( cbPoet->currentText(), nPoetCount );
  }
}

void ZhscWidget::calPoemIndex()
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    QString sq = "select id from poem where dynastyno = '" + smDynasty[nDynasty] + "' and poetno = '" + smPoet[nPoet] + "' and poemno = '" + smPoem[nPoem] + "'";
    QSqlQuery query( sq );
    
    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        nPoemIndex = query.value(0).toInt( &ok );
      }
    }
    else
    {
      nPoemIndex = 0;
    }

    emit poemIndex( nPoemIndex );
  }
}

void ZhscWidget::addHistory()
{
  bool bSign = FALSE;

  if ( bConStatus )
  {
    QString sq = "select dynastyno, poetno, poemno from history order by history desc";
    QSqlQuery query( sq );

    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        if ( ( query.value(0).toString() == smDynasty[nDynasty] ) 
          && ( query.value(1).toString() == smPoet[nPoet] )
          && ( query.value(2).toString() == smPoem[nPoem] ) )
        {
          bSign = FALSE;
        }
        else
        {
          bSign = TRUE;
        }
      }
    }
    else
    {
      bSign = TRUE;
    }

    if ( bSign )
    {
      sDynastyNo = smDynasty[nDynasty];
      sPoetNo = smPoet[nPoet];
      sPoemNo = smPoem[nPoem];
      dtHistoryDT = QDateTime::currentDateTime();

      QSqlCursor cur( "history" ); 
      QSqlRecord *buffer = cur.primeInsert();
      buffer->setValue( "dynastyno", sDynastyNo );
      buffer->setValue( "poetno", sPoetNo );
      buffer->setValue( "poemno", sPoemNo );
      buffer->setValue( "history", dtHistoryDT );
      cur.insert();
    }

    history();
  }
}

void ZhscWidget::loadHistory()
{
  if ( bConStatus )
  {
    QString sq = "select dynastyno, poetno, poemno, history from history order by history desc";
    QSqlQuery query( sq );

    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        sDynastyNo = query.value(0).toString();
        sPoetNo = query.value(1).toString();
        sPoemNo = query.value(2).toString();
        dtHistoryDT = query.value(3).toDateTime();
      }
    }
    else
    {
      sDynastyNo = "A";
      sPoetNo = "0001";
      sPoemNo = "0000";
      dtHistoryDT = QDateTime::fromString( "0000-00-00 00:00:00", Qt::TextDate );
    }

    history();
  }
}

void ZhscWidget::history()
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    QString sq1 = "select count(*), min(id), max(id) from history";
    QSqlQuery query1( sq1 );

    if ( query1.size() > 0 )
    {
      if ( query1.next() )
      {
        nHistoryCount = query1.value(0).toInt( &ok );
        nHistoryMin = query1.value(1).toInt( &ok );
        nHistoryMax = query1.value(2).toInt( &ok );

        QString sq2 = "select id from history where history = '" + dtHistoryDT.toString( "yyyy-MM-dd hh-mm-ss" ) + "'";
        QSqlQuery query2( sq2 );

        if ( query2.size() > 0 )
        {
          if ( query2.next() )
          {
            nHistoryIndex = query2.value(0).toInt( &ok );

            if ( nHistoryIndex > nHistoryMin )
            {
              emit historyPrevEnabled( TRUE );
            }
            else
            {
              emit historyPrevEnabled( FALSE );
            }

            if ( nHistoryIndex < nHistoryMax )
            {
              emit historyNextEnabled( TRUE );
            }
            else
            {
              emit historyNextEnabled( FALSE );
            }
          }
        }
        else
        {
          nHistoryIndex = -1;
          dtHistoryDT = QDateTime::fromString( "0000-00-00 00:00:00", Qt::TextDate );

          emit historyPrevEnabled( FALSE );
          emit historyNextEnabled( FALSE );
        }
      }
    }
    else
    {
      nHistoryCount = -1;
      nHistoryMin = -1;
      nHistoryMax = -1;
      nHistoryIndex = -1;
      dtHistoryDT = QDateTime::fromString( "0000-00-00 00:00:00", Qt::TextDate );

      emit historyPrevEnabled( FALSE );
      emit historyNextEnabled( FALSE );
    }
  }
}

void ZhscWidget::historyPrev()
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    if ( ( nHistoryIndex != -1 ) && ( nHistoryMin != -1 ) && ( nHistoryIndex > nHistoryMin ) )
    {
      QString sid1;
      sid1.setNum( nHistoryIndex );

      QString sq1 = "select max(id - " + sid1 + ") from history where id - " + sid1 + " < 0";
      QSqlQuery query1( sq1 );

      if ( query1.size() > 0 )
      {
        if ( query1.next() )
        {
          QString max;
          max.setNum( query1.value(0).toInt( &ok ) );

          QString sq2 = "select id, dynastyno, poetno, poemno, history from history where id - " + sid1 + " = " + max;
          QSqlQuery query2( sq2 );

          if ( query2.size() > 0 )
          {
            if ( query2.next() )
            {
              nHistoryIndex = query2.value(0).toInt( &ok );
              sDynastyNo = query2.value(1).toString();
              sPoetNo = query2.value(2).toString();
              sPoemNo = query2.value(3).toString();
              dtHistoryDT = query2.value(4).toDateTime();

              setPoem();
              history();
            }
          }
        }
      }
    }
  }
}

void ZhscWidget::historyNext()
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    if ( ( nHistoryIndex != -1 ) && ( nHistoryMax != -1 ) && ( nHistoryIndex < nHistoryMax ) )
    {
      QString sid1;
      sid1.setNum( nHistoryIndex );

      QString sq1 = "select min(id - " + sid1 + ") from history where id - " + sid1 + " > 0";
      QSqlQuery query1( sq1 );

      if ( query1.size() > 0 )
      {
        if ( query1.next() )
        {
          QString min;
          min.setNum( query1.value(0).toInt( &ok ) );

          QString sq2 = "select id, dynastyno, poetno, poemno, history from history where id - " + sid1 + " = " + min;
          QSqlQuery query2( sq2 );

          if ( query2.size() > 0 )
          {
            if ( query2.next() )
            {
              nHistoryIndex = query2.value(0).toInt( &ok );
              sDynastyNo = query2.value(1).toString();
              sPoetNo = query2.value(2).toString();
              sPoemNo = query2.value(3).toString();
              dtHistoryDT = query2.value(4).toDateTime();

              setPoem();
              history();
            }
          }
        }
      }
    }
  }
}

void ZhscWidget::clearHistory()
{
  if ( bConStatus )
  {
    QString sq = "delete from history";
    QSqlQuery query( sq );

    nHistoryCount = -1;
    nHistoryIndex = -1;
    nHistoryMin = -1;
    nHistoryMax = -1;
    dtHistoryDT = QDateTime::fromString( "0000-00-00 00:00:00", Qt::TextDate );

    emit historyPrevEnabled( FALSE );
    emit historyNextEnabled( FALSE );
  }
}

void ZhscWidget::record()
{
  if ( nPoemIndex == nPoemMin )
  {
    emit recordFirstEnabled( FALSE );
  }
  else
  {
    emit recordFirstEnabled( TRUE );
  }

  if ( nPoemIndex > nPoemMin )
  {
    emit recordPrevEnabled( TRUE );
  }
  else
  {
    emit recordPrevEnabled( FALSE );
  }

  if ( nPoemIndex == nPoemMax )
  {
    emit recordLastEnabled( FALSE );
  }
  else
  {
    emit recordLastEnabled( TRUE );
  }

  if ( nPoemIndex < nPoemMax )
  {
    emit recordNextEnabled( TRUE );
  }
  else
  {
    emit recordNextEnabled( FALSE );
  }
}

void ZhscWidget::recordFirst()
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    QString min;
    min.setNum( nPoemMin );

    QString sq = "select id, dynastyno, poetno, poemno from tsc where id = " + min;
    QSqlQuery query( sq );

    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        nPoemIndex = query.value(0).toInt( &ok );
        sDynastyNo = query.value(1).toString();
        sPoetNo = query.value(2).toString();
        sPoemNo = query.value(3).toString();

        setPoem();
        record();
      }
    }
  }
}

void ZhscWidget::recordPrev()
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    if ( ( nPoemIndex != 0 ) && ( nPoemMin != 0 ) && ( nPoemIndex > nPoemMin ) )
    {
      QString sid1;
      sid1.setNum( nPoemIndex );

      QString sq1 = "select max(id - " + sid1 + ") from tsc where id - " + sid1 + " < 0";
      QSqlQuery query1( sq1 );

      if ( query1.size() > 0 )
      {
        if ( query1.next() )
        {
          QString max;
          max.setNum( query1.value(0).toInt( &ok ) );

          QString sq2 = "select id, dynastyno, poetno, poemno from tsc where id - " + sid1 + " = " + max;
          QSqlQuery query2( sq2 );

          if ( query2.size() > 0 )
          {
            if ( query2.next() )
            {
              nPoemIndex = query2.value(0).toInt( &ok );
              sDynastyNo = query2.value(1).toString();
              sPoetNo = query2.value(2).toString();
              sPoemNo = query2.value(3).toString();

              setPoem();
              record();
            }
          }
        }
      }
    }
  }
}

void ZhscWidget::recordNext()
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    if ( ( nPoemIndex != 0 ) && ( nPoemMax != 0 ) && ( nPoemIndex < nPoemMax ) )
    {
      QString sid1;
      sid1.setNum( nPoemIndex );

      QString sq1 = "select min(id - " + sid1 + ") from tsc where id - " + sid1 + " > 0";
      QSqlQuery query1( sq1 );

      if ( query1.size() > 0 )
      {
        if ( query1.next() )
        {
          QString min;
          min.setNum( query1.value(0).toInt( &ok ) );

          QString sq2 = "select id, dynastyno, poetno, poemno from tsc where id - " + sid1 + " = " + min;
          QSqlQuery query2( sq2 );

          if ( query2.size() > 0 )
          {
            if ( query2.next() )
            {
              nPoemIndex = query2.value(0).toInt( &ok );
              sDynastyNo = query2.value(1).toString();
              sPoetNo = query2.value(2).toString();
              sPoemNo = query2.value(3).toString();

              setPoem();
              record();
            }
          }
        }
      }
    }
  }
}

void ZhscWidget::recordLast()
{
  bool ok = FALSE;

  if ( bConStatus )
  {
    QString max;
    max.setNum( nPoemMax );

    QString sq = "select id, dynastyno, poetno, poemno from tsc where id = " + max;
    QSqlQuery query( sq );

    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        nPoemIndex = query.value(0).toInt( &ok );
        sDynastyNo = query.value(1).toString();
        sPoetNo = query.value(2).toString();
        sPoemNo = query.value(3).toString();

        setPoem();
        record();
      }
    }
  }
}

void ZhscWidget::recordRandom()
{
  bool ok = FALSE;
  int poemrandom;

  if ( bConStatus )
  {
    poemrandom = rand() % nPoemMax + 1;

    QString pr;
    pr.setNum( poemrandom );

    QString sq = "select id, dynastyno, poetno, poemno from tsc where id = " + pr;
    QSqlQuery query( sq );

    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        nPoemIndex = query.value(0).toInt( &ok );
        sDynastyNo = query.value(1).toString();
        sPoetNo = query.value(2).toString();
        sPoemNo = query.value(3).toString();

        setPoem();
        record();
      }
    }
  }
}

void ZhscWidget::recordNumber()
{
  bool ok = FALSE;

  int res = QInputDialog::getInteger(
                Unicode( "中华诗词 Qt版" ),
                Unicode( "请输入你要看的作品编号：" ), nPoemIndex, nPoemMin, nPoemMax, 1, &ok, this );
  if ( ok )
  {
    ok = FALSE;
    if ( bConStatus )
    {
      QString number;
      number.setNum( res );

      QString sq = "select id, dynastyno, poetno, poemno from tsc where id = " + number;
      QSqlQuery query( sq );

      if ( query.size() > 0 )
      {
        if ( query.next() )
        {
          nPoemIndex = query.value(0).toInt( &ok );
          sDynastyNo = query.value(1).toString();
          sPoetNo = query.value(2).toString();
          sPoemNo = query.value(3).toString();

          setPoem();
          record();
        }
      }
    }

    return;
  }
  else
  {
    return;
  }
}

void ZhscWidget::bookmark()
{
  QString tmp;
  Bookmark bm;
  QString dynasty, poet, poem;

  bml.clear();

  QString sq2 = "select * from tsc";
  QSqlQuery query2( sq2 );

  if ( bConStatus )
  {
    QString sq1 = "select dynastyno, poetno, poemno from bookmark order by id";
    QSqlQuery query1( sq1 );

    if ( query1.size() > 0 )
    {
      while ( query1.next() )
      {
        dynasty = query1.value(0).toString();
        poet = query1.value(1).toString();
        poem = query1.value(2).toString();

        bm.dynastyno = dynasty;
        bm.poetno = poet;
        bm.poemno = poem;

        sq2 = "select dynasty, poet, caption from tsc where dynastyno = '" + dynasty + "' and poetno = '" + poet + "' and poemno = '" + poem + "'";
        query2.exec( sq2 );

        if ( query2.size() > 0 )
        {
          if ( query2.next() )
          {
            tmp = trUtf8( query2.value(0).toByteArray() ) + "-" 
              + trUtf8( query2.value(1).toByteArray() ) + "-"
              + trUtf8( query2.value(2).toByteArray() );

            bm.content = tmp;

            bml.append( bm ); 
          }
        }
      }
    }

    emit bookmarkChanged( bml );
  }
}

void ZhscWidget::addBookmark()
{
  bool bSign = FALSE;

  if ( bConStatus )
  {
    QString sq = "select dynastyno, poetno, poemno from bookmark where dynastyno = '" + smDynasty[nDynasty] + "' and poetno = '" + smPoet[nPoet] + "' and poemno = '" + smPoem[nPoem] + "'";
    QSqlQuery query( sq );

    if ( query.size() > 0 )
    {
      if ( query.next() )
      {
        if ( ( query.value(0).toString() == smDynasty[nDynasty] ) 
          && ( query.value(1).toString() == smPoet[nPoet] )
          && ( query.value(2).toString() == smPoem[nPoem] ) )
        {
          bSign = FALSE;
        }
        else
        {
          bSign = TRUE;
        }
      }
    }
    else
    {
      bSign = TRUE;
    }

    if ( bSign )
    {
      QSqlCursor cur( "bookmark" ); 
      QSqlRecord *buffer = cur.primeInsert();
      buffer->setValue( "dynastyno", smDynasty[nDynasty] );
      buffer->setValue( "poetno", smPoet[nPoet] );
      buffer->setValue( "poemno", smPoem[nPoem] );
      cur.insert();

      bookmark();
    }
  }
}

void ZhscWidget::modifyBookmark()
{
  BookmarkList dbml;
  BookmarkList b;
  dbml.clear();

  Bookmark bm;
  BookmarkList::Iterator it;

  BMDialog *dlg = new BMDialog( this, "bmdialog" );
  dlg->setBML( bml );

  if ( dlg->exec() == QDialog::Accepted ) 
  {
    b = dlg->deletedBML();
    for ( it = b.begin(); it != b.end(); ++it )
    {
      bm.dynastyno = (*it).dynastyno;
      bm.poetno = (*it).poetno;
      bm.poemno = (*it).poemno;
      bm.content = (*it).content;

      dbml.append( bm );
    }

    if ( bConStatus )
    {
      QString sq = "select * from bookmark";
      QSqlQuery query( sq );

      for ( it = dbml.begin(); it != dbml.end(); ++it )
      {
        sq = "delete from bookmark where dynastyno = '" + (*it).dynastyno + "' and poetno = '" + (*it).poetno + "' and poemno = '" + (*it).poemno + "'";
        query.exec( sq );
      }
    }

    bookmark();
  }

  delete dlg;
}

void ZhscWidget::clearBookmark()
{
  if ( bConStatus )
  {
    QString sq = "select * from bookmark";
    QSqlQuery query( sq );

    sq = "delete from bookmark";
    query.exec( sq );

    bookmark();
  }
}

void ZhscWidget::selectBookmark( QString bm )
{
  BookmarkList::Iterator it;

  for ( it = bml.begin(); it != bml.end(); ++it )
  {
    if ( (*it).content == bm )
    {
      sDynastyNo = (*it).dynastyno;
      sPoetNo = (*it).poetno;
      sPoemNo = (*it).poemno;

      setPoem();

      record();

      return;
    }
  }
}

int ZhscWidget::dynastyNoKey()
{
  int i = 0;

  StringMap::Iterator it;
  for ( it = smDynasty.begin(); it != smDynasty.end(); ++it )
  {
    if ( it.data() == sDynastyNo )
    {
      i = it.key();
    }
  }

  return i;
}

int ZhscWidget::poetNoKey()
{
  int i = 0;

  StringMap::Iterator it;
  for ( it = smPoet.begin(); it != smPoet.end(); ++it )
  {
    if ( it.data() == sPoetNo )
    {
      i = it.key();
    }
  }

  return i;
}

int ZhscWidget::poemNoKey()
{
  int i = 0;

  StringMap::Iterator it;
  for ( it = smPoem.begin(); it != smPoem.end(); ++it )
  {
    if ( it.data() == sPoemNo )
    {
      i = it.key();
    }
  }

  return i;
}
