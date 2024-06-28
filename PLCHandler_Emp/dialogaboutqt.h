#ifndef DIALOGABOUTQT_H
#define DIALOGABOUTQT_H

#include <QDialog>

namespace Ui {
class DialogAboutQt;
}

class DialogAboutQt : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAboutQt(QWidget *parent = nullptr);
    ~DialogAboutQt();

private slots:
    void on_pushButton_Comfirm_clicked();

private:
    Ui::DialogAboutQt *ui;
};

#endif // DIALOGABOUTQT_H
