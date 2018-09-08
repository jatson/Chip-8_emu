#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Qt libraries */
#include <QMainWindow>
#include <QGraphicsView>
#include <QFileDialog>
#include <QDebug>

/* External libraries */
#include "chip_8.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_start_pushButton_clicked();

    void on_browse_pushButton_clicked();

    void emulationCycle();

signals:
    void startEmulationTrigger();

private:
    Ui::MainWindow *ui;
    chip_8 m_chip8;
    QString m_fileName;
    bool m_fileOpened;

    void m_initGraphics();
    void m_initInput();
    void m_updateScreen();
};

#endif // MAINWINDOW_H
