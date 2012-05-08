/*

    ZHSC
    
    bmdialog.cpp
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

#include <qlayout.h>

#include "bmdialog.h"
#include "codec.h"

BMDialog::BMDialog( QWidget* parent, const char* name )
    : QDialog( parent, name, TRUE )
{
  bml.clear();
  dbml.clear();

  setCaption( Unicode( "中华诗词 Qt版" ) );

  QVBoxLayout *lmain = new QVBoxLayout( this, 10 );

  table = new QTable( 0, 1, this );
  table->setSelectionMode( QTable::NoSelection );
  table->setLeftMargin( 0 );
  table->setTopMargin( 0 );
  table->setColumnWidth( 0, 300 );

  lmain->addWidget( table );

  QHBoxLayout *ldown = new QHBoxLayout( lmain, 3 );

  pbDelete = new QPushButton( Unicode( "删除(&D)" ), this );
  pbCancel = new QPushButton( Unicode( "取消(&C)" ), this );

  ldown->addSpacing( 35 );
  ldown->addWidget( pbDelete, 10 );
  ldown->addSpacing( 30 );
  ldown->addWidget( pbCancel, 10 );
  ldown->addSpacing( 35 );

  //lmain->addLayout( ldown );

  connect( pbDelete, SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( pbCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

  s = size();
}

BMDialog::~BMDialog()
{
}

void BMDialog::setBML( BookmarkList b )
{
  int i;
  QCheckTableItem *c = NULL;

  bml.clear();
  cl.clear();

  for ( i = 0; i < table->numRows(); ++i )
  {
    table->removeRow( i );
  }

  BookmarkList::Iterator it;

  for ( it = b.begin(), i = 0; it != b.end(); ++it, ++i )
  {
    bml.append( (*it) );

    table->insertRows( i );

    c = new QCheckTableItem( table, "" );
    cl.append( c );

    table->setItem( i, 0, c );
    table->setText( i, 0, (*it).content );
  }
}

BookmarkList BMDialog::deletedBML() const
{
  return dbml;
}

void BMDialog::accept()
{
  dbml.clear();

  QCheckTableItem *c;
  BookmarkList::Iterator it;

  for ( c = cl.first(), it = bml.begin(); c; c = cl.next(), ++it )
  {
    if ( c->isChecked() )
    {
      dbml.append( (*it) );
    }
  }

  QDialog::accept();
}
