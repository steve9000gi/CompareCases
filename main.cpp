//
////////////////////////////////////////////////////////////////////////////////
//
// main.cpp: contains the main function for CompareCases.  Just instantiates a
// QApplication object and the application main window using Qt.
//
// Author:    Steve Chall, RENCI
//
// Primary collaborators: 
//            Joseph Lo, Shiva Das, Vorakarn Chanyavanich, Duke Medical Center
//
// Copyright: The Renaissance Computing Institute (RENCI)
//
// License:   Licensed under the RENCI Open Source Software License v. 1.0
//
//            See http://www.renci.org/resources/open-source-software-license
//            for details.
//
////////////////////////////////////////////////////////////////////////////////

#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}
