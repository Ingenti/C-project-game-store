#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(std::vector<int> data, int h, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    data_(data),
    h_(h)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *event){
    setStyleSheet("background-color: white;");

    QPainter painter(this);

    painter.drawLine(50,700,26*50,700);
    painter.drawLine(50,700,50,50);
    painter.drawLine(40,60,50,50);
    painter.drawLine(60,60,50,50);
    painter.drawLine(26*50-10,710,26*50,700);
    painter.drawLine(26*50-10,690,26*50,700);

    std::vector<int> v = data_; //ajomaaravektori, tarvitaan koodista


    for (int i = 0; i < 11; i++) {
        QString s = QString::number(i*(20));
        painter.drawText(23,705-i*60,s);
        }

    int lopetusarvo = h_;                                                       //tarvitaan koodista
    int alotusarvo = lopetusarvo;

    if(v.size()<24){
        alotusarvo=lopetusarvo-int(v.size());
        if (alotusarvo<0){
            alotusarvo=24+alotusarvo;
        }
    }

    std::vector<int> v2 = {};
    int h = 0;
    for(int i = alotusarvo; i < 24; i++){
        v2.push_back(i);
        h++;
    }

    if(h<24){
        for(int g = 0; g<alotusarvo; g++){
            v2.push_back(g);
    }
    }

    int j = 0;
    for (std::vector<int>::iterator it = v2.begin(); it != v2.end(); ++it) {

        QString s = QString::number(*it);
        painter.drawText(70+j*50,720,s);
        j++;
        }

    //painter.drawText(20,720,"KLO");


    painter.drawText(500, 35, "Cars per hour during the last 24 hours");


    j = 1;
    int f = int(v.size());
    for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        if (f>24){
            f--;
        }
        else{
        QRect rect(j*50,700,50,(-3)*(*it));
        QColor color(Qt::green);

        //if((*it)<=50) QColor color(Qt::red);

        QBrush brush(color, Qt::SolidPattern);

        painter.fillRect(rect,brush);
        painter.drawRect(rect);
        j++;
        }
    }
}

void Dialog::closeEvent(QCloseEvent *)
{
    ask_save();
}

void Dialog::ask_save()
{
    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(nullptr, "Save", "Do you want to save this histogram? If so, remember to set this window aside.");
    if(answer == QMessageBox::Yes) {save_screen();}
}

void Dialog::save_screen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pm = screen->grabWindow(this->winId());
    bool b = pm.save("histogram.png");
    QMessageBox mb(this);
    if (b) {mb.setText("Screen successfully saved."); mb.exec();}
    else {mb.critical(nullptr, "Error", "Screen was not saved.");}
}
