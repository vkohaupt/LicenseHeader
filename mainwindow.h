#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString flagEndLicense = "--End_License--";
    void editHeader();
    void addHeader(QString filePath);
    void removeHeader(QString filePath);
    bool haveHeader(QString filePath);


};

#endif // MAINWINDOW_H
