#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <qdebug.h>
#include <QMessageBox>

#define LABEL_STA_GREEN tr("background-color: rgb(0, 255, 0);border-radius: 10px; ")
#define LABEL_STA_RED   tr("background-color: rgb(255, 0, 0);border-radius: 10px; ")

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitForm();


    dlgaboutqt = new DialogAboutQt(this);
    dlgaboutqt->hide();


    QString str = nullptr;
    if (QFile::exists(QString("PLCHandler.ini")))
    {
        //m_plchdl_api = new CPLCHandleApi(QString("PLCHandler.ini"));
        m_plchdl_api = (CPLCHandleApi *)GetPLCHandleApiCls();
    }
    else
    {

        str = "PLCHandler.ini Not Exsit";
        qDebug()<<str;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitForm()
{
    QStringList headstr;
    CPLCVar var;

    headstr << QString("序号") << QString("参数名") << QString("长度") << QString("值");
    ui->tableWidgetPlcVarsList->setHorizontalHeaderLabels(headstr);
    ui->tableWidgetPlcVarsList->setColumnWidth(0,40);
    ui->tableWidgetPlcVarsList->setColumnWidth(1,280);
    ui->tableWidgetPlcVarsList->setColumnWidth(2,40);
    ui->tableWidgetPlcVarsList->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::on_pushButton_ConnectPLc_clicked()
{
    if (ui->pushButton_ConnectPLc->text() == QString("PLC连接"))
    {

        //plc连接
        if ( m_plchdl_api->StartProc())
        {
            quint32 num = m_plchdl_api->GetAllVariantSymbols();
            QMessageBox::information(this, QString("提示"), QString("PLC 符号表数量：%1").arg(num));
            RefreshPlcVar();
            ui->label_PlcConnectState->setStyleSheet(LABEL_STA_GREEN);
            ui->pushButton_ConnectPLc->setText(QString("PLC断开"));

            m_freshtimer = startTimer(1000);
        }
        else
        {
            ui->pushButton_ConnectPLc->setText(QString("PLC断开"));
        }


    }
    else
    {
        m_plchdl_api->StopProc();

        ui->pushButton_ConnectPLc->setText(QString("PLC连接"));
        ui->label_PlcConnectState->setStyleSheet(LABEL_STA_RED);

        if(m_freshtimer)
        {
            killTimer(m_freshtimer);
            m_freshtimer = 0;
        }

    }
}

//开启定时器
void MainWindow::timerEvent(QTimerEvent *event)
{
    int nId = event->timerId();

    if( nId == m_freshtimer)
    {
        CheckPlcCntSta();

        RefreshPlcVar();
    }
}



//检测PLC连接状态
void MainWindow::CheckPlcCntSta()
{
    int ret = m_plchdl_api->GetPlcHandleState();// GetPlcHandleState

    if(ret == 4)
    {
        ui->label_PlcConnectState->setStyleSheet(LABEL_STA_GREEN);
    }
    else
    {
        ui->label_PlcConnectState->setStyleSheet(LABEL_STA_RED);
    }
}
void MainWindow::RefreshPlcVar()
{
    if (m_plchdl_api)
    {
        CPLCVar cvar;
        qint32  num;

        ui->tableWidgetPlcVarsList->clearContents();
        for (qint32 i = ui->tableWidgetPlcVarsList->rowCount(); i >= 0; i--)
        {
            ui->tableWidgetPlcVarsList->removeRow(i);
        }

        //读取所有共享变量
        num = m_plchdl_api->TrigReadAll();

        for (qint32 i = 0; i < num; i++)
        {
            if (m_plchdl_api->GetVarInfo(i, &cvar))
            {
                AddPlcVar(cvar);
            }
        }
    }
    return;
}

void MainWindow::AddPlcVar(CPLCVar & var)
{
    qint32 row = ui->tableWidgetPlcVarsList->rowCount();
    QTableWidgetItem * pItem = nullptr;

    ui->tableWidgetPlcVarsList->insertRow(row);

    pItem = new QTableWidgetItem();
    pItem->setText(QString("%1").arg(row+1));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
    ui->tableWidgetPlcVarsList->setItem(row,0,pItem);

    pItem = new QTableWidgetItem();
    pItem->setText(QString(var.name.c_str()));
    pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
    ui->tableWidgetPlcVarsList->setItem(row,1,pItem);

    pItem = new QTableWidgetItem();
    pItem->setText(QString("%1").arg(var.size));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
    ui->tableWidgetPlcVarsList->setItem(row,2,pItem);

    pItem = new QTableWidgetItem();
    //pItem->setText(QString("%1").arg(var.val.value<>.toInt()));
    pItem->setText(Var2String(var.val, var.size));
    pItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidgetPlcVarsList->setItem(row,3,pItem);

    row += 1;
}

QString MainWindow::Var2String(T_PLC_VAL var, quint32 size)
{
    QString qstr;
    int base = 10;
    int wide = 0;
#if 0
    if (ui->checkBoxDispHex->isChecked())
    {
        base = 16;
        wide = size*2;
    }
#endif
    qstr.clear();
    switch(size)
    {
    case 1:
        qstr = QString("%1").arg(var.u8v, wide, base, QChar('0'));
        break;
    case 2:
        qstr = QString("%1").arg(var.u16v, wide, base, QChar('0'));
        break;
    case 4:
        qstr = QString("%1").arg(var.u32v, wide, base, QChar('0'));
        break;
    case 8:
        qstr = QString("%1").arg(var.u64v, wide, base, QChar('0'));
        break;
    default:
        qstr = QString("%1").arg(var.u32v, wide, base, QChar('0'));
        break;
    }

    return qstr;
}

void MainWindow::on_action_triggered()
{
    dlgaboutqt->show();
}
