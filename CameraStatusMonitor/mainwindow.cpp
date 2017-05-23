#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pCamController = NULL;

    m_pTableModel = new CustomTableModel();
    ui->StatusTableView->setModel(m_pTableModel);

//    QStringList list1;
//    list1<<"1"<<"2"<<"3";
//    m_pTableModel->InsertRowFromQStringList(list1);
    m_pCamController = new CamConnectContrl();
    m_pCamController->SetDataModel(m_pTableModel);
    //m_pCamController->addCameraByIPaddress("172.18.2.152");
    //m_pCamController->addCameraByIPaddress("172.18.80.166");
    m_pCamController->addCameraByIPaddress("172.18.80.65");
    m_pCamController->StarUpLoad();

    m_pStatusEventCheck = new StatustCheck();
    m_pStatusEventCheck->SetDataModel(m_pTableModel);
    m_pStatusEventCheck->StartEvent();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_pCamController)
    {
        delete m_pCamController;
        m_pCamController = NULL;
    }
    if(m_pStatusEventCheck)
    {
        delete m_pStatusEventCheck;
        m_pStatusEventCheck = NULL;
    }
    if(m_pTableModel)
    {
        delete m_pTableModel;
        m_pTableModel = NULL;
    }
}
