/*

    ZHSC
    
    connection.h
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

#ifndef CONNECTION_H
#define CONNECTION_H

#define DB_ZHSC_DRIVER	    "QMYSQL3"
#define DB_ZHSC_DBNAME	    "zhsc"
#define DB_ZHSC_USER	    "zhsc"
#define DB_ZHSC_PASSWD	    "zhsc"
#define DB_ZHSC_HOST	    ""

bool createConnections();

#endif //CONNECTION_h
