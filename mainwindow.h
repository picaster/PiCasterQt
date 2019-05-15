#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

#include "playlistitemmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addStopPushButton_clicked();

    void on_addMusicPushButton_clicked();

    void on_exitButton_clicked();

    void on_jackButton_toggled(bool checked);

    void on_playButton_clicked();

    void on_stopButton_clicked();

    void on_removeItemButton_clicked();

private:
    Ui::MainWindow *ui;
    QString settingsFileName;
    QString musicDir;
    PlaylistItemModel* model;
    //QStringListModel* model;

    void loadSettings();
    void saveSettings();
};

#endif // MAINWINDOW_H
