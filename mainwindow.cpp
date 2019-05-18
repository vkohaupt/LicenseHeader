#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDirIterator>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    editHeader();
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::haveHeader( QString filePath )
{

    QFile file_original( filePath );
    file_original.open( QIODevice::ReadOnly );
    QTextStream stream( &file_original);
    QString line = stream.readAll();

    return line.contains( flagEndLicense );
}


void MainWindow::addHeader( QString filePath )
{
    QStringList list;
    list << "/* vokoscreenNG - A desktop recorder\n";
    list << " * Copyright (C) 2017-2019 Volker Kohaupt\n";
    list << " * \n";
    list << " * Author:\n";
    list << " *      Volker Kohaupt <vkohaupt@freenet.de>\n";
    list << " *\n";
    list << " * This file is free software; you can redistribute it and/or modify\n";
    list << " * it under the terms of version 2 of the GNU General Public License\n";
    list << " * as published by the Free Software Foundation.\n";
    list << " *\n";
    list << " * This program is distributed in the hope that it will be useful,\n";
    list << " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
    list << " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
    list << " * GNU General Public License for more details.\n";
    list << " *\n";
    list << " * You should have received a copy of the GNU General Public License\n";
    list << " * along with this program; if not, write to the Free Software Foundation,\n";
    list << " * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.\n";
    list << " *\n";
    list << " * " + flagEndLicense + "\n";
    list << " */\n";
    list << "\n";

    QString string = list.join("");
    // write license header in new file with suffix "new"
    QFile file_new( filePath + ".new" ) ;
    file_new.open( QFile::WriteOnly | QFile::Truncate );
    QTextStream out( &file_new );
    for ( int i = 0; i < list.count(); i++ )
    {
        out << list.at(i);
    }

    // read original file and append to new file
    QFile file_original( filePath );
    file_original.open( QIODevice::ReadOnly );
    QTextStream stream( &file_original);
    QString line = stream.readAll();
    out << line;

    // delete original file
    file_original.close();
    file_original.remove();

    // rename new file to original file
    file_new.close();
    file_new.rename( filePath );
}


void MainWindow::removeHeader( QString filePath )
{
    // Ermitteln ab welcher Zeile das flagEndLicense steht
    QFile file_original( filePath );
    file_original.open( QFile::ReadOnly );
    QTextStream stream( &file_original);

    int lineCount = 0;
    while ( !stream.atEnd() )
    {
        lineCount++;
        if ( stream.readLine().contains( flagEndLicense ) == true )
        {
            lineCount += 2;
            break;
        }
    }
    file_original.close();

    // Original file open and read count lines from licenes
    file_original.open( QFile::ReadWrite | QFile::Text );
    stream.setDevice( &file_original);
    for ( int i = 0; i < lineCount; i++ )
    {
        stream.readLine();
    }

    // Read the rest, resize the file to zero and write the content into the file
    QString content = stream.readAll();
    file_original.resize(0);
    stream << content;
    file_original.close();
}


void MainWindow::editHeader()
{
    // read *.cpp and *.h
    QDirIterator iterator( "/home/vk/Programmieren/vokoscreenNG/src/", QDir::Files, QDirIterator::Subdirectories );
    while ( iterator.hasNext() )
    {
        iterator.next();
        if ( ( iterator.fileInfo().suffix() == "cpp" ) or ( iterator.fileInfo().suffix() == "h" ) )
        {
            if ( haveHeader( iterator.filePath() ) == false )
            {
                addHeader( iterator.filePath() );
            }
            else
            {
                removeHeader( iterator.filePath() );
                addHeader( iterator.filePath() );
            }
        }
    }
}
