/*

    ZHSC
    
    bmdialog.h
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

#ifndef BMDIALOG_H
#define BMDIALOG_H

#include <qdialog.h>
#include <qtable.h>
#include <qpushbutton.h>
#include <qsize.h>

#include "bookmark.h"

class BMDialog : public QDialog
{
  Q_OBJECT
public:
  BMDialog( QWidget* parent, const char* name );
  ~BMDialog();

  void setBML( BookmarkList b );
  BookmarkList deletedBML() const;

  //virtual QSize sizeHint() const { return s; }
  //virtual QSize minimumSizeHint() const { return s; }

protected slots:
  virtual void accept();

private:
  QTable *table;

  QPushButton *pbDelete;
  QPushButton *pbCancel;

  BookmarkList bml;

  BookmarkList dbml;

  CTIPList cl;

  QSize s;
};

#endif //BMDIALOG

