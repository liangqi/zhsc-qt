/*

    ZHSC
    
    main.cpp
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
#include <qtranslator.h>

#include "codec.h"
#include "zhscmw.h"

QTextCodec *g_pCodec;

int main( int argc, char ** argv ) 
{
  g_pCodec=QTextCodec::codecForName("GB18030");
  
  QApplication a( argc, argv );

  QTranslator translator_qt( 0 );
  translator_qt.load("qt_zh_CN.qm",".");
  a.installTranslator( & translator_qt );

  ZhscMainWindow *mw = new ZhscMainWindow( 0, "Zhsc Main Window" );
  a.setMainWidget(mw);

  mw->setCaption( "Zhsc" );
  mw->show();

  a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

  return a.exec();
}
