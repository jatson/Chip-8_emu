#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(startEmulationTrigger()), this, SLOT(emulationCycle()));

    m_fileOpened = false;
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
    if(m_fileOpened)
    {
#ifdef DEBUG
        qDebug() << "init stuff for emulation...";
#endif
        m_initGraphics();
        m_initInput();
        m_chip8.init();

        emit startEmulationTrigger();
    }
}

void MainWindow::on_browse_pushButton_clicked()
{
    QString path = QDir::homePath();
    m_fileName = QFileDialog::getOpenFileName(this,
                                              tr("Open ROM file"),
                                              path,
                                              tr("ROM files (*.ch8);;All files (*.*)"));

#ifdef DEBUG
    qDebug() << "trying to open a game. OK? ";
#endif
    m_fileOpened = m_chip8.loadGame(m_fileName.toStdString());
#ifdef DEBUG
    qDebug() << m_fileOpened;
#endif

    ui->start_pushButton->setEnabled(m_fileOpened);
}

void MainWindow::emulationCycle()
{

#ifdef DEBUG
        qDebug() << "Entering emulation loop...";
#endif

    while(true)
    {


#ifdef DEBUG
        qDebug() << "inside infinite lopp...";
#endif

        m_chip8.mainCycle();

        if(m_chip8.drawFlag() == true) m_updateScreen();

        m_chip8.setPressedKey();
    }
}
