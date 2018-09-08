#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::m_initGraphics()
{

}

void MainWindow::m_initInput()
{

}

void MainWindow::m_updateScreen()
{

}

void MainWindow::on_start_pushButton_clicked()
{
    bool fileOpened = false;
    QString fileName;

    QString path = QDir::homePath();
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open ROM file"),
                                                    path,
                                                    tr("ROM files (*.ch8);;All files (*.*)"));

    m_initGraphics();
    m_initInput();
    m_chip8.init();

    qDebug() << "trying to open a game. OK? ";
    fileOpened = m_chip8.loadGame("Pong.ch8");
    qDebug() << fileOpened;

    while(fileOpened)
    {
        m_chip8.mainCycle();

        if(m_chip8.drawFlag() == true) m_updateScreen();

        m_chip8.setPressedKey();
    }
}
