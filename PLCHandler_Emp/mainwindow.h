#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimerEvent>
#include "PLCHdlApi.h"
#include "dialogaboutqt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



public slots:
    void on_pushButton_ConnectPLc_clicked();

private:
    Ui::MainWindow *ui;

    int m_freshtimer = 0;

    DialogAboutQt *dlgaboutqt;

    CPLCHandleApi * m_plchdl_api = nullptr;

    void CheckPlcCntSta();

    void InitForm();

    void RefreshPlcVar();

    void AddPlcVar(CPLCVar & var);

    QString Var2String(T_PLC_VAL var, quint32 size);

protected:
    virtual void timerEvent(QTimerEvent *event);
private slots:
    void on_action_triggered();
};

#endif // MAINWINDOW_H
