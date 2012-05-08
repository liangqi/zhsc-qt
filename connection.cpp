/*

    ZHSC
    
    connection.cpp
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

#include <qsqldatabase.h>
#include "connection.h"

bool createConnections()
{

  QSqlDatabase *defaultDB = QSqlDatabase::addDatabase( DB_ZHSC_DRIVER );
  defaultDB->setDatabaseName( DB_ZHSC_DBNAME );
  defaultDB->setUserName( DB_ZHSC_USER );
  defaultDB->setPassword( DB_ZHSC_PASSWD );
  defaultDB->setHostName( DB_ZHSC_HOST );
  if ( ! defaultDB->open() ) 
  {
    qWarning( "Failed to open ZHSC database: " +
    defaultDB->lastError().driverText() );
    qWarning( defaultDB->lastError().databaseText() );
    return FALSE;
  }
  
  return TRUE;
}
