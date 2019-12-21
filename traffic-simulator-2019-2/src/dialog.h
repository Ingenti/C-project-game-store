#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPainter>
#include <QStyle>
#include <QMessageBox>
#include <QScreen>
#include <iostream>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(std::vector<int> data, int h, QWidget *parent = nullptr);
    ~Dialog();
    virtual void paintEvent(QPaintEvent *event);
    void ask_save();
    void save_screen();
    void closeEvent(QCloseEvent* e);

private:
    Ui::Dialog *ui;
    std::vector<int> data_;
    int h_;
};

#endif // DIALOG_H
