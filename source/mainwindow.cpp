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

void MainWindow::on_start_pushButton_clicked()
{
    m_initGraphics();
    m_initInput();
    m_chip8.init();

    m_chip8.loadGame("pong");

    while(true) // this will freeze the gui. shall be moved to the signal-slot, but for now ok.
    {
        m_chip8.mainCycle();

        if(m_chip8.drawFlag() == true) m_updateScreen();

        m_chip8.setPressedKey();
    }
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
