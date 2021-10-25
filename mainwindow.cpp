#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <string>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
#include <unistd.h>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
#include <chrono>
#include <thread>
#include <windows.h>
#include <unistd.h>
//#define qRgb(255,255,0) BRESENHAMLINECOLOR

using namespace std;
using namespace std::chrono;
QImage img=QImage(700,700,QImage::Format_RGB888);

void delay (int SLEEP_ms = 300) {
    QTime dieTime= QTime::currentTime().addMSecs(SLEEP_ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
//////not used
void MainWindow::on_X_clicked(){}
void MainWindow::on_duumydda_clicked(){}
void MainWindow::on_pushButton_2_clicked(){}
void MainWindow::on_pushButton_3_clicked(){}
void MainWindow::on_circle_midpoint_pushbutton_2_clicked(){}
//////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->x_axis->hide();
    ui->y_axis->hide();
    p1.setX(-1);
    p1.setY(-1);

    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
    connect(ui->r_horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(setColor()));
    connect(ui->g_horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(setColor()));
    connect(ui->b_horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(setColor()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setColor()
{
    R = ui->r_horizontalSlider->value();
    G = ui->g_horizontalSlider->value();
    B = ui->b_horizontalSlider->value();
    QImage pic = QImage(120,100,QImage::Format_RGB888);
    pic.fill(QColor(qRgb(R,G,B)));
    ui->label_color->setPixmap(QPixmap::fromImage(pic));
    currentcolor = qRgb(R,G,B);//qRgb(R,G,B)
}



void MainWindow::point(int x,int y,int r,int g,int b)
{
    int k = ui->gridsize->value();//GridSize
    if(k>1)
    {
        int startX=x/k*k;
        int startY=y/k*k;

        //Drawing the pixels
        for(int i=startX+1;i<(startX+k);i++)
            for(int j=startY+1;j<(startY+k);j++)
                img.setPixel(i,j,qRgb(r,g,b));
    }
    else//k==1
        img.setPixel(x,y,qRgb(r,g,b));
    ui->frame->setPixmap(QPixmap::fromImage(img));
}
void MainWindow::point(int x,int y, QRgb cc)//cc=currentcolor
{
    int k = ui->gridsize->value();//GridSize
    if(k>1)
    {
        int startX=x/k*k;
        int startY=y/k*k;

        //Drawing the pixels
        for(int i=startX+1;i<(startX+k);i++)
            for(int j=startY+1;j<(startY+k);j++)
                img.setPixel(i,j,cc);
    }
    else//k==1
        img.setPixel(x,y,cc);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}


/*
int MainWindow::changeX(int x)
{
    int k = ui->gridsize->value();
    x=x-img.width()/2;
    x/=k;
    return x;
}

int MainWindow::changeY(int y)
{
    int k = ui->gridsize->value();
    y=img.height()/2-y;
    y/=k;
    return y;
}*/

int MainWindow::scale(int X) {
    int k = ui->gridsize->value();
    if (k == 0) return X;
    if (X % k) return X/k;
    return (X/k) - 1;
}

int MainWindow::changeX(int X) {
    X = scale(X);
    return (X-scale(img.width()/2));
}

int MainWindow::changeY(int Y) {
    Y = scale(Y);
    return -(Y-scale(img.height()/2));
}

void MainWindow::showMousePosition(QPoint &pos)
{
    ui->mouse_movement->setText(" X : "+QString::number(changeX(pos.x()))+", Y : "+QString::number(changeY(pos.y())));
    /* int k = ui->gridsize->value();
    int x = (pos.x() - ui->frame->width()/2)/k;
        int y = (ui->frame->height()/2 - pos.y())/k;
        ui->mouse_movement->setText(" X : "+QString::number(x)+", Y : "+QString::number(y));*/
}

void MainWindow::BoundaryFill4(int x, int y, QRgb newcolor, QRgb edgecolor)
{
    int k = ui->gridsize->value();
   if(x < 0 || y < 0 || x >= img.width() || y >= img.height())
       return;
   if(img.pixel(x,y) == edgecolor || img.pixel(x,y) == newcolor)
       return;
   point(x,y,R,G,B);//qRgb(R,G,B)=newcolor
   BoundaryFill4(x, y - k, newcolor,edgecolor);
   BoundaryFill4(x - k, y, newcolor,edgecolor);
   BoundaryFill4(x, y + k, newcolor,edgecolor);
   BoundaryFill4(x + k, y, newcolor,edgecolor);


}

void MainWindow::BoundaryFill8(int x, int y, QRgb newcolor, QRgb edgecolor)
{
   int k = ui->gridsize->value();


   if(x < 0 || y < 0 || x >= img.width() || y >= img.height())
       return;
   if(img.pixel(x,y) == edgecolor || img.pixel(x,y) == newcolor)
       return;
   point(x,y,R,G,B);//qRgb(R,G,B)=newcolor
   BoundaryFill8(x, y - k, newcolor,edgecolor);
   BoundaryFill8(x - k, y, newcolor,edgecolor);
   BoundaryFill8(x, y + k, newcolor,edgecolor);
   BoundaryFill8(x + k, y, newcolor,edgecolor);
   BoundaryFill8(x + k, y + k, newcolor,edgecolor);
   BoundaryFill8(x + k, y - k, newcolor,edgecolor);
   BoundaryFill8(x - k, y - k, newcolor,edgecolor);
   BoundaryFill8(x - k, y + k, newcolor,edgecolor);

}

void MainWindow::FloodFill4 (int x, int y, QRgb newcolor,QRgb oldcolor) {
 //   oldcolor=qRgb(0,0,0);
    int k = ui->gridsize->value();
    if(x < 0 || y < 0 || x >= img.width() || y >= img.height()) return;
    if(img.pixel(x,y) != oldcolor) return;
    point(x,y,R,G,B);
    FloodFill4(x - k, y, newcolor, oldcolor);
    FloodFill4(x + k, y, newcolor, oldcolor);
    FloodFill4(x, y - k, newcolor, oldcolor);
    FloodFill4(x, y + k, newcolor, oldcolor);

}

void MainWindow::FloodFill8 (int x, int y, QRgb newcolor,QRgb oldcolor)
{
    int k = ui->gridsize->value();
    if(x < 0 || y < 0 || x >= img.width() || y >= img.height()) return;
    if(img.pixel(x,y) != oldcolor) return;
    point(x,y,R,G,B);
    FloodFill8(x - k, y, newcolor, oldcolor);
    FloodFill8(x + k, y, newcolor, oldcolor);
    FloodFill8(x, y - k, newcolor, oldcolor);
    FloodFill8(x, y + k, newcolor, oldcolor);
    FloodFill8(x - k, y - k, newcolor, oldcolor);
    FloodFill8(x + k, y + k, newcolor, oldcolor);
    FloodFill8(x + k, y - k, newcolor, oldcolor);
    FloodFill8(x - k, y + k, newcolor, oldcolor);
}


void MainWindow::Mouse_Pressed()
{
    int k = ui->gridsize->value();
    QRgb pressedpointcolor = img.pixel(ui->frame->x,ui->frame->y);
    ui->mouse_pressed->setText(" X : "+QString::number(changeX(ui->frame->x))+", Y : "+QString::number(changeY(ui->frame->y)));
    //point(ui->frame->x,ui->frame->y);
    ui->x_axis->move(0,ui->frame->y);
    ui->y_axis->move(ui->frame->x,0);
    point(ui->frame->x,ui->frame->y,255,255,255);
     /*int k = ui->gridsize->value();
       int x = (ui->frame->x - ui->frame->width()/2)/k;
       int y = (ui->frame->height()/2 - ui->frame->y)/k;
       ui->mouse_pressed->setText(" X : "+QString::number(x)+", Y : "+QString::number(y));
       point(ui->frame->x,ui->frame->y);*/
    if(ui->draw_polygon->isChecked() && p1.rx()!=-1)
    {

        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);
        draw_bresenhamLine(p1,p2);
        p1=p2;
    }
    else if(ui->boundaryfill4button->isChecked())

    {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

       BoundaryFill4((p1.rx()/k)*k+k/2,(p1.ry()/k)*k+k/2,qRgb(R,G,B),qRgb(255,255,0));
    }
    else if(ui->boundaryfill8button->isChecked())
    {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

       BoundaryFill8((p1.rx()/k)*k+k/2,(p1.ry()/k)*k+k/2,qRgb(R,G,B),qRgb(255,255,0));
    }
    else if(ui->floodfill4button->isChecked())
    {

                p1.setX(ui->frame->x);
                p1.setY(ui->frame->y);
                point(p1.rx(),p1.ry(),pressedpointcolor);
                FloodFill4((p1.rx()/k)*k+k/2,(p1.ry()/k)*k+k/2,qRgb(R,G,B),pressedpointcolor);
    }
    else if(ui->floodfill8button->isChecked())
    {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        point(p1.rx(),p1.ry(),pressedpointcolor);
        FloodFill8((p1.rx()/k)*k+k/2,(p1.ry()/k)*k+k/2,qRgb(R,G,B),pressedpointcolor);
    }
    else if(ui->draw_polygon->isChecked() && p1.rx()==-1)
    {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
    }
}

void MainWindow::on_show_axes_clicked()
{
        int gridsize = ui->gridsize->value();

        int frame_width = ui->frame->width();

        int frame_height = ui->frame->height();

        if(ui->show_axes->isChecked())
        {
            for(int j=0;j<frame_width;j+=gridsize)
            {
                point(img.width()/2,j,237,171,230);
            }
            for(int i=0;i<= frame_height;i+=gridsize)
            {
                point(i,img.height()/2,237,171,230);
            }
        }
        else{
            on_pushButton_clicked();
        }


}
void MainWindow::on_set_point1_clicked()
{
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
}

void MainWindow::on_set_point2_clicked()
{
        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);

}

void MainWindow::on_Draw_clicked()
{
/*
    QPainter painter(&img);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::red);

    if(ui->draw_line->isChecked()){
        painter.setPen(Qt::yellow);
        painter.drawLine(p1,p2);
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));*/
}

void MainWindow::on_pushButton_clicked()//resetbutton
{
    int k = ui->gridsize->value();
    for(int j=0;j<img.height();j++)
    {
        for(int i=0;i<img.width();i++)
        {
            img.setPixel(i,j,qRgb(0,0,0));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
    for(int i=0;i<img.height();i+=k)
        for(int j=0;j<img.width();j++)
            img.setPixel(i,j,qRgb(232,88,0));

    for(int i=0;i<img.width();i+=k)
        for(int j=0;j<img.height();j++)
            img.setPixel(j,i,qRgb(232,88,0));

    ui->frame->setPixmap(QPixmap::fromImage(img));
    p1.setX(-1);
    p1.setY(-1);
}

void MainWindow::on_showgrid_clicked()
{
    int k = ui->gridsize->value();

    for(int j=0;j<img.height();j++)
    {
        for(int i=0;i<img.width();i++)
        {
            img.setPixel(i,j,qRgb(0,0,0));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));

    /*for(int i=0;i<img.height();i+=k)
        for(int j=0;j<img.width();j++)
            img.setPixel(i,j,qRgb(0,255,0));

    for(int i=0;i<img.width();i+=k)
        for(int j=0;j<img.height();j++)
            img.setPixel(j,i,qRgb(0,255,0));*/

    if(k>=1)
       {
           for(int i=0;i<img.width();i+=k)
           {
               for(int j=0;j<img.height();j++)
               {
                   img.setPixel(i,j,qRgb(232,88,0));
                   img.setPixel(j,i,qRgb(232,88,0));
               }
           }
        }

    ui->frame->setPixmap(QPixmap::fromImage(img));
    p1.setX(-1);
    p1.setY(-1);
}

void MainWindow::on_dda_clicked()
{
     drawDDALine();

}
void MainWindow::drawDDALine (){
       // (x0, y0) -> point1 ; (xn, yn) -> point 2
       int gridsize = ui->gridsize->value();
       double x0 = p1.x() / gridsize;
       double xn = p2.x() / gridsize;
       double y0 = p1.y() / gridsize;
       double yn = p2.y() / gridsize;


       //required for evaluating the step count in the algorithm
       double dx = fabs(xn - x0);
       double dy = fabs(yn - y0);

       double Dx, Dy;

       if (dx > dy ) { //y coordinate will increase backward or forward by the slope value
           Dx = 1;
           Dy = dy / dx;
       }

       else { // x coordinate will increase backward or forward by the inverse slope value
           Dx = dx / dy;
           Dy = 1;
       }
       //change the direction of movement accordingly
       if (x0 > xn) Dx *= -1;
       if (y0 > yn) Dy *= -1;

       double x = x0*gridsize + gridsize / 2; //adjusting the initial x coordinate according to the grid size
       double y = y0*gridsize + gridsize / 2; //adjusting the initial y coordinate according to the grid size
       long excludetime = 0;
       auto start = high_resolution_clock::now(); //start the timer

       for (int steps =0; steps <= (dx > dy ? dx : dy); steps++) {

           auto s = high_resolution_clock::now();
           point (x, y, 0, 255, 0);
           auto e = high_resolution_clock::now();
           auto d = duration_cast<microseconds>(e-s);
           excludetime = excludetime+d.count();
           x += Dx * gridsize;
           y += Dy * gridsize;
       }
       auto end = high_resolution_clock::now(); //end the timer for final time evaluation

       auto duration = duration_cast<microseconds>(end - start);
       long executionTime = duration.count();
       QTextStream out(stdout);
       out << "\nDDA\nTotal executionTime:- " << executionTime << "\n";
       out << "Excluded time:- " << excludetime << "\n";
       //out << "Execution time for DDA Algorithm executionTime:- " << executionTime-excludetime << "\n";
       ui->ddalabel->setText(QString::number((executionTime-excludetime)));
}

//string s=to_string(changeX(x1))+" "+to_string(changeY(y1))+" "+to_string(changeX(x2))+" "+to_string(changeY(y2))+" "+to_string(p0);
//ui->debugger->setText(s.c_str());





void MainWindow::draw_bresenhamLine(QPoint point1,QPoint point2)
{
        int gridsize = ui->gridsize->value();
        int x0 = point1.x()/gridsize;
        int y0 = point1.y()/gridsize;
        int xn = point2.x()/gridsize;
        int yn = point2.y()/gridsize;

        //required for evaluating the step count of the algorithm
        int dx = fabs(xn - x0);
        int dy = fabs(yn - y0);

        int Dx = (x0 > xn ? -1 : 1);
        int Dy = (y0 > yn ? -1 : 1);
        bool flag = 1;
        int x = x0*gridsize + gridsize/2; //adjusting the initial x coordinate according to the grid size
        int y = y0*gridsize + gridsize/2; //adjusting the initial y coordinate according to the grid size
        if(dy > dx) { // if slope > 1, then swap the elements
            swap(dx,dy);
            swap(x,y);
            swap(Dx,Dy);
            flag = 0;
        }


        int decision = 2*dy - dx;


        for(int steps=0;steps<=dx;steps++) {

            if(flag) {
                point(x,y,255,255,0);
            }

            else point(y,x,255,255,0);


            if(decision < 0) {
                x += Dx*gridsize;
                decision += 2*dy;
            } else {
                x += Dx*gridsize;
                y += Dy*gridsize;
                decision += 2*dy - 2*dx;
            }
        }
}




void MainWindow::on_bresenhamLine_clicked()
{
        int gridsize = ui->gridsize->value();
        int x0 = p1.x()/gridsize;
        int y0 = p1.y()/gridsize;
        int xn = p2.x()/gridsize;
        int yn = p2.y()/gridsize;

        //required for evaluating the step count of the algorithm
        int dx = fabs(xn - x0);
        int dy = fabs(yn - y0);

        int Dx = (x0 > xn ? -1 : 1);
        int Dy = (y0 > yn ? -1 : 1);
        bool flag = 1;
        int x = x0*gridsize + gridsize/2; //adjusting the initial x coordinate according to the grid size
        int y = y0*gridsize + gridsize/2; //adjusting the initial y coordinate according to the grid size
        if(dy > dx) { // if slope > 1, then swap the elements
            swap(dx,dy);
            swap(x,y);
            swap(Dx,Dy);
            flag = 0;
        }

        //evauating the decision for pixel illumination
        int decision = 2*dy - dx;
        long excludetime = 0;
        auto start = high_resolution_clock::now();
        //evaluating the algorithm

        for(int steps=0;steps<=dx;steps++) {
            auto s = high_resolution_clock::now();
            if(flag) {
                point(x,y,255,255,0);
            }

            else point(y,x,255,255,0);
            auto e = high_resolution_clock::now();
            auto d = duration_cast<microseconds>(e - s);
            excludetime = excludetime+d.count();

            if(decision < 0) {
                x += Dx*gridsize;
                decision += 2*dy;
            } else {
                x += Dx*gridsize;
                y += Dy*gridsize;
                decision += 2*dy - 2*dx;
            }
        }
        auto end = high_resolution_clock::now();
        long executionTime = duration_cast<microseconds>(end - start).count();
        QTextStream out(stdout);
        out << "\nBresenham\nTotal executionTime:- " << executionTime << "\n";
        out << "Excluded time:- " << excludetime << "\n";
        //out << "Execution time for DDA Algorithm executionTime:- " << executionTime-excludetime << "\n";
        ui->bresenhamlabel->setText(QString::number((executionTime-excludetime)));
}
///////////////////

void MainWindow::on_dummydda_clicked()
{
           // (x0, y0) -> point1 ; (xn, yn) -> point 2
           int gridsize = ui->gridsize->value();
          /* double x0 = p1.x() / gridsize;
           double xn = p2.x() / gridsize;
           double y0 = p1.y() / gridsize;
           double yn = p2.y() / gridsize;*/
           double x0 = -10000 / gridsize;
           double xn = 10000 / gridsize;
           double y0 = 10000 / gridsize;
           double yn = -10000 / gridsize;

           //required for evaluating the step count in the algorithm
           double dx = fabs(xn - x0);
           double dy = fabs(yn - y0);

           double Dx, Dy;

           if (dx > dy ) { //y coordinate will increase backward or forward by the slope value
               Dx = 1;
               Dy = dy / dx;
           }

           else { // x coordinate will increase backward or forward by the inverse slope value
               Dx = dx / dy;
               Dy = 1;
           }
           //change the direction of movement accordingly
           if (x0 > xn) Dx *= -1;
           if (y0 > yn) Dy *= -1;

           double x = x0*gridsize + gridsize / 2; //adjusting the initial x coordinate according to the grid size
           double y = y0*gridsize + gridsize / 2; //adjusting the initial y coordinate according to the grid size
           long excludetime = 0;
           auto start = high_resolution_clock::now(); //start the timer

           for (int steps =0; steps <= (dx > dy ? dx : dy); steps++) {

               auto s = high_resolution_clock::now();
               //point (x, y, 0, 255, 0);
               auto e = high_resolution_clock::now();
               auto d = duration_cast<microseconds>(e-s);
               excludetime = excludetime+d.count();
               x += Dx * gridsize;
               y += Dy * gridsize;
           }
           auto end = high_resolution_clock::now(); //end the timer for final time evaluation

           auto duration = duration_cast<microseconds>(end - start);
           long executionTime = duration.count();
           QTextStream out(stdout);
           out << "\nFull execution time:- " << executionTime << "\n";
           out << "Time excluded:- " << excludetime << "\n";
           out << "Execution time for DDA Algorithm executionTime:- " << executionTime-excludetime << "\n";

}



void MainWindow::on_brsenhamdummy_clicked()
{
    int gridsize = ui->gridsize->value();
    /*int x0 = p1.x()/gridsize;
    int y0 = p1.y()/gridsize;
    int xn = p2.x()/gridsize;
    int yn = p2.y()/gridsize;*/
    double x0 = -10000 / gridsize;
    double xn = 10000 / gridsize;
    double y0 = 10000 / gridsize;
    double yn = -10000 / gridsize;

    //required for evaluating the step count of the algorithm
    int dx = fabs(xn - x0);
    int dy = fabs(yn - y0);

    int Dx = (x0 > xn ? -1 : 1);
    int Dy = (y0 > yn ? -1 : 1);
    bool flag = 1;
    int x = x0*gridsize + gridsize/2; //adjusting the initial x coordinate according to the grid size
    int y = y0*gridsize + gridsize/2; //adjusting the initial y coordinate according to the grid size
    if(dy > dx) { // if slope > 1, then swap the elements
        swap(dx,dy);
        swap(x,y);
        swap(Dx,Dy);
        flag = 0;
    }

    //evauating the decision for pixel illumination
    int decision = 2*dy - dx;
    long excludetime = 0;
    auto start = high_resolution_clock::now();
    //evaluating the algorithm

    for(int steps=0;steps<=dx;steps++) {
        auto s = high_resolution_clock::now();
      /*  if(flag) {
            point(x,y,255,255,0);
        }

        else point(y,x,255,255,0);*/
        auto e = high_resolution_clock::now();
        auto d = duration_cast<microseconds>(e - s);
        excludetime = excludetime+d.count();

        if(decision < 0) {
            x += Dx*gridsize;
            decision += 2*dy;
        } else {
            x += Dx*gridsize;
            y += Dy*gridsize;
            decision += 2*dy - 2*dx;
        }
    }
    auto end = high_resolution_clock::now();
    long executionTime = duration_cast<microseconds>(end - start).count();
    QTextStream out(stdout);
    out << "\nTotal executionTime:- " << executionTime << "\n";
    out << "Excluded time:- " << excludetime << "\n";
    out << "Execution time for Bresenham's Algorithm executionTime:- " << executionTime-excludetime << "\n";
}

void MainWindow::on_circle_midpoint_pushbutton_clicked()
{
    int radius = ui->radius->value();
       int gridsize = ui->gridsize->value();
       p1.setX(ui->frame->x);
       p1.setY(ui->frame->y);

       int x0 = p1.x() / gridsize;
       int y0 = p1.y() / gridsize;

       x0 = x0 * gridsize + gridsize / 2;
       y0 = y0 * gridsize + gridsize / 2;

       int x = 0;
       int y = radius * gridsize;

      // int p = (1 - radius) * gridsize;
       int p;
       long excludetime = 0;
       auto start = high_resolution_clock::now();
       while(y>=x) {
               auto s = high_resolution_clock::now();

               point(x0 - x, y0 - y, 255, 255, 0);
               point(x0 + x, y0 - y, 255, 255, 0);
               point(x0 - x, y0 + y, 255, 255, 0);
               point(x0 + x, y0 + y, 255, 255, 0);
               point(x0 - y, y0 - x, 255, 255, 0);
               point(x0 + y, y0 - x, 255, 255, 0);
               point(x0 - y, y0 + x, 255, 255, 0);
               point(x0 + y, y0 + x, 255, 255, 0);

               auto e = high_resolution_clock::now();
               auto d = duration_cast<microseconds>(e - s);
               excludetime = excludetime+d.count();

               x += gridsize;
               p = x*x+(y-0.5)*(y-0.5)-(radius * gridsize)*(radius * gridsize);
               if(p < 0) {
                  //same y
               } else {
                   y-=gridsize;
               }
       }
       auto end = high_resolution_clock::now();
       long executionTime = duration_cast<microseconds>(end - start).count();
       QTextStream out(stdout);
       out << "\nTotal executionTime:- " << executionTime << "\n";
       out << "Excluded time:- " << excludetime << "\n";
       ui->circlemidpointlabel->setText(QString::number((executionTime-excludetime)));
}

void MainWindow::on_circle_bresenham_pushbutton_clicked()
{
        int radius = ui->radius->value();
        int k = ui->gridsize->value();
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

        int x0 = p1.x()/k;
        int y0 = p1.y()/k;
        x0 = x0*k + k/2;
        y0 = y0*k + k/2;

        int x = 0;
        int y = radius*k;
       // int p = (1-radius);
        int p = (1 - radius*k);
        long excludetime = 0;
        auto start = high_resolution_clock::now();
        while(y >= x) {
            auto s = high_resolution_clock::now();
           /* point(x0 - k*x, y0 - k*y, 255, 0, 0);
            point(x0 + k*x, y0 - k*y, 255, 0, 0);
            point(x0 - k*x, y0 + k*y, 255, 0, 0);
            point(x0 + k*x, y0 + k*y, 255, 0, 0);
            point(x0 - k*y, y0 - k*x, 255, 0, 0);
            point(x0 + k*y, y0 - k*x, 255, 0, 0);
            point(x0 - k*y, y0 + k*x, 255, 0, 0);
            point(x0 + k*y, y0 + k*x, 255, 0, 0);*/
                point(x0 - x, y0 - y, 255, 255, 0);
                point(x0 + x, y0 - y, 255, 255, 0);
                point(x0 - x, y0 + y, 255, 255, 0);
                point(x0 + x, y0 + y, 255, 255, 0);
                point(x0 - y, y0 - x, 255, 255, 0);
                point(x0 + y, y0 - x, 255, 255, 0);
                point(x0 - y, y0 + x, 255, 255, 0);
                point(x0 + y, y0 + x, 255, 255, 0);


            auto e = high_resolution_clock::now();
            auto d = duration_cast<microseconds>(e - s);
            excludetime = excludetime+d.count();


            x += k;
            if(p < 0) {
                p = p + 2*(x+1) + 1;
            } else {
                p = p + 2*(x+1)+1-2*(y-1);
                y -= k;
            }
        }

        auto end = high_resolution_clock::now();
        int executionTime = duration_cast<microseconds>(end - start).count();



        QTextStream out(stdout);
        out << "\nTotal executionTime:- " << executionTime << "\n";
        out << "Excluded time:- " << excludetime << "\n";
        ui->circlebesenhamlabel->setText(QString::number((executionTime-excludetime)));
}

void MainWindow::on_circle_polar_pushbutton_clicked()
{
      p1.setX(ui->frame->x);
      p1.setY(ui->frame->y);
      int gridsize = ui->gridsize->value();
      int x0 = p1.x()/gridsize;
      int y0 = p1.y()/gridsize;
      x0 = x0*gridsize + gridsize/2;
      y0 = y0*gridsize + gridsize/2;

      int r = ui->radius->value();//radius

      long excludetime = 0;
      auto start = high_resolution_clock::now();

      for (int theta_degrees = 0; theta_degrees <= 45; theta_degrees++) {
          double theta_radians = M_PI * theta_degrees / 180;
          int x =  r * cos (theta_radians) * gridsize;
          int y =  r * sin (theta_radians) * gridsize;
          auto s = high_resolution_clock::now();
          point(x0 - x, y0 - y, 255, 255, 0);
          point(x0 + x, y0 - y, 255, 255, 0);
          point(x0 - x, y0 + y, 255, 255, 0);
          point(x0 + x, y0 + y, 255, 255, 0);
          point(x0 - y, y0 - x, 255, 255, 0);
          point(x0 + y, y0 - x, 255, 255, 0);
          point(x0 - y, y0 + x, 255, 255, 0);
          point(x0 + y, y0 + x, 255, 255, 0);

          auto e = high_resolution_clock::now();
          auto d = duration_cast<microseconds>(e - s);
          excludetime = excludetime+d.count();

      }
      auto end = high_resolution_clock::now();
      int executionTime = duration_cast<microseconds>(end - start).count();
      QTextStream out(stdout);
      out << "\nTotal executionTime:- " << executionTime << "\n";
      out << "Excluded time:- " << excludetime << "\n";
      ui->circlepolarlabel->setText(QString::number((executionTime-excludetime)));
}
/////////////////////////not used
void MainWindow::on_midpointanimationbutton_clicked()
{
        int radius = ui->radius->value();
       int gridsize = ui->gridsize->value();
       p1.setX(ui->frame->x);
       p1.setY(ui->frame->y);

       int x0 = p1.x() / gridsize;
       int y0 = p1.y() / gridsize;

       x0 = x0 * gridsize + gridsize / 2;
       y0 = y0 * gridsize + gridsize / 2;

       int x = 0;
       int y = radius * gridsize;

      // int p = (1 - radius) * gridsize;
       int p;


       while(y>=x) {


           point(x0 - x, y0 - y, 255, 255, 0);
           point(x0 + x, y0 - y, 200, 255, 0);
           point(x0 - x, y0 + y, 255, 35, 5);
           point(x0 + x, y0 + y, 55, 255, 33);
           point(x0 - y, y0 - x, 255, 25, 100);
           point(x0 + y, y0 - x, 255, 23, 0);
           point(x0 - y, y0 + x, 255, 255, 200);
           point(x0 + y, y0 + x, 255, 255, 0);
           delay();


               x += gridsize;
               p = x*x+(y-0.5)*(y-0.5)-(radius * gridsize)*(radius * gridsize);
               if(p < 0) {
                  //same y
               } else {
                   y-=gridsize;
               }
       }
}

//////////////////////


void MainWindow::on_bresnhamanimationbutton_clicked()
{
    int radius = ui->radius->value();
    int k = ui->gridsize->value();
    p1.setX(ui->frame->x);
    p1.setY(ui->frame->y);

    int x0 = p1.x()/k;
    int y0 = p1.y()/k;
    x0 = x0*k + k/2;
    y0 = y0*k + k/2;

    int x = 0;
    int y = radius*k;
   // int p = (1-radius);
    int p = (1 - radius*k);

    while(y >= x) {


        point(x0 - x, y0 - y, 255, 255, 0);
        point(x0 + x, y0 - y, 200, 255, 0);
        point(x0 - x, y0 + y, 255, 35, 5);
        point(x0 + x, y0 + y, 55, 255, 33);
        point(x0 - y, y0 - x, 255, 25, 100);
        point(x0 + y, y0 - x, 255, 23, 0);
        point(x0 - y, y0 + x, 255, 255, 200);
        point(x0 + y, y0 + x, 255, 255, 0);

        delay();

        x += k;
        if(p < 0) {
            p = p + 2*(x+1) + 1;
        } else {
            p = p + 2*(x+1)+1-2*(y-1);
            y -= k;
        }
    }



}





void MainWindow::on_polaranimationbutton_clicked()
{
    p1.setX(ui->frame->x);
    p1.setY(ui->frame->y);
    int gridsize = ui->gridsize->value();
    int x0 = p1.x()/gridsize;
    int y0 = p1.y()/gridsize;
    x0 = x0*gridsize + gridsize/2;
    y0 = y0*gridsize + gridsize/2;

    int r = ui->radius->value();//radius

    for (int theta_degrees = 0; theta_degrees <= 45; theta_degrees++) {
        double theta_radians = M_PI * theta_degrees / 180;
        double x =  r * cos (theta_radians) * gridsize;
        double y =  r * sin (theta_radians) * gridsize;

        point(x0 - x, y0 - y, 255, 255, 0);
        point(x0 + x, y0 - y, 200, 255, 0);
        point(x0 - x, y0 + y, 255, 35, 5);
        point(x0 + x, y0 + y, 55, 255, 33);
        point(x0 - y, y0 - x, 255, 25, 100);
        point(x0 + y, y0 - x, 255, 23, 0);
        point(x0 - y, y0 + x, 255, 255, 200);
        point(x0 + y, y0 + x, 255, 255, 0);

        delay();

    }

}

void MainWindow::on_ellipse_polar_pushbutton_clicked()
{
       p1.setX(ui->frame->x);
       p1.setY(ui->frame->y);
       int gridsize = ui->gridsize->value();
       int x_center = p1.x()/gridsize;
       int y_center = p1.y()/gridsize;
       x_center = x_center*gridsize + gridsize/2;
       y_center = y_center*gridsize + gridsize/2;

       int a = ui->radius_x->value();
       int b = ui->radius_y->value();

       float theta = 0.0;

       long excludetime = 0;
       auto start = high_resolution_clock::now();

       while (theta < 90.0) {
           int xk = a * cos(theta*M_PI/180) * gridsize;
           int yk = b * sin (theta*M_PI/180) * gridsize;
           //4-point Symmetry
           auto s = high_resolution_clock::now();
           point (x_center + xk, y_center + yk, 255, 0, 0);
           point (x_center - xk, y_center + yk, 255, 0, 0);
           point (x_center - xk, y_center - yk, 255, 0, 0);
           point (x_center + xk, y_center - yk, 255, 0, 0);
           auto e = high_resolution_clock::now();
           auto d = duration_cast<microseconds>(e - s);
           theta++;
           excludetime = excludetime+d.count();

       }
       auto end = high_resolution_clock::now();
       int executionTime = duration_cast<microseconds>(end - start).count();
       QTextStream out(stdout);
       out << "\nTotal executionTime:- " << executionTime << "\n";
       out << "Excluded time:- " << excludetime << "\n";
       ui->ellipsepolarlabel->setText(QString::number((executionTime-excludetime)));
}

void MainWindow::on_polaraellipsenimationbutton_clicked()
{
    p1.setX(ui->frame->x);
    p1.setY(ui->frame->y);
    int gridsize = ui->gridsize->value();
    int x_center = p1.x()/gridsize;
    int y_center = p1.y()/gridsize;
    x_center = x_center*gridsize + gridsize/2;
    y_center = y_center*gridsize + gridsize/2;

    int a = ui->radius_x->value();
    int b = ui->radius_y->value();

    float theta = 0.0;

    while (theta < 90.0) {
        int xk = a * cos(theta*M_PI/180) * gridsize;
        int yk = b * sin (theta*M_PI/180) * gridsize;
        //4-point Symmetry

        point (x_center + xk, y_center + yk, 255, 50, 0);
        point (x_center - xk, y_center + yk, 200, 23, 0);
        point (x_center - xk, y_center - yk, 22, 100, 0);
        point (x_center + xk, y_center - yk, 250, 211, 0);

        theta++;

        delay(100);

    }

    }


void MainWindow::on_ellipse_bresenham_pushbutton_clicked()
{
           int gridsize = ui->gridsize->value();
           p1.setX(ui->frame->x);
           p1.setY(ui->frame->y);
           int x_radius = ui->radius_x->value();
           int y_radius = ui->radius_y->value();
           int x_centre=p1.x();
           int y_centre=p1.y();

           //scaling up the center of the ellipse according to the gridsize
           x_centre=(x_centre/gridsize)*gridsize+gridsize/2;
           y_centre=(y_centre/gridsize)*gridsize+gridsize/2;

           int x=0;
           int y=y_radius;
           int px=0.0;
           int py=2 * (x_radius * x_radius)*y;
           //For first region
           int p1= (y_radius * y_radius) - (x_radius * x_radius * y_radius) +(0.25)* (x_radius * x_radius); //Initial value of decision paramemter

           long excludetime = 0;
           auto start = high_resolution_clock::now();
           while(px<py)
           {
               auto s = high_resolution_clock::now();
               point(x_centre+x*gridsize,y_centre+y*gridsize, 255, 255, 0);
               point(x_centre-x*gridsize,y_centre+y*gridsize, 255, 255, 0);
               point(x_centre-x*gridsize,y_centre-y*gridsize, 255, 255, 0);
               point(x_centre+x*gridsize,y_centre-y*gridsize, 255, 255, 0);
               auto e = high_resolution_clock::now();
               auto d = duration_cast<microseconds>(e - s);
               excludetime = excludetime+d.count();

               x++;
               px+=(2 * y_radius * y_radius);
               if(p1>=0) {
                   y--;
                   py-=(2 * x_radius * x_radius);
                   p1=p1+(y_radius * y_radius)+px-py;

               }  else{
                   p1=p1+(y_radius * y_radius)+px;
               }


           }

           //For second region
           p1=(y_radius * y_radius) *((double)x+0.5)*((double)x+0.5)+(x_radius * x_radius)*(y-1)*(y-1)-(x_radius * x_radius)*(y_radius * y_radius); //Initial value of decision paramemter


           while(y>=0)
           {
               auto s = high_resolution_clock::now();
               point(x_centre+x*gridsize,y_centre+y*gridsize, 255, 255, 0);
               point(x_centre-x*gridsize,y_centre+y*gridsize, 255, 255, 0);
               point(x_centre-x*gridsize,y_centre-y*gridsize, 255, 255, 0);
               point(x_centre+x*gridsize,y_centre-y*gridsize, 255, 255, 0);
               auto e = high_resolution_clock::now();
               auto d = duration_cast<microseconds>(e - s);
               excludetime = excludetime+d.count();

               y--;
               py-=(x_radius * x_radius);
               if(p1<=0){
                   x++;
                   px+=(y_radius * y_radius);
                   p1=p1+(x_radius * x_radius)-py+px;

               }  else  {
                   p1=p1+(x_radius * x_radius)-py;
               }


           }
           auto end = high_resolution_clock::now();
           int executionTime = duration_cast<microseconds>(end - start).count();
           QTextStream out(stdout);
           out << "\nTotal executionTime:- " << executionTime << "\n";
           out << "Excluded time:- " << excludetime << "\n";
           ui->ellipsebresenhamlabel->setText(QString::number((executionTime-excludetime)));
}

void MainWindow::on_bresenhamellipseanimationbutton_clicked()
{
    int gridsize = ui->gridsize->value();
    p1.setX(ui->frame->x);
    p1.setY(ui->frame->y);
    int x_radius = ui->radius_x->value();
    int y_radius = ui->radius_y->value();
    int x_centre=p1.x();
    int y_centre=p1.y();

    //scaling up the center of the ellipse according to the gridsize
    x_centre=(x_centre/gridsize)*gridsize+gridsize/2;
    y_centre=(y_centre/gridsize)*gridsize+gridsize/2;

    int x=0;
    int y=y_radius;
    int px=0.0;
    int py=2 * (x_radius * x_radius)*y;
    //For first region
    int p1= (y_radius * y_radius) - (x_radius * x_radius * y_radius) +(0.25)* (x_radius * x_radius); //Initial value of decision paramemter

    while(px<py)
    {
        point(x_centre+x*gridsize,y_centre+y*gridsize, 255, 255, 0);
        point(x_centre-x*gridsize,y_centre+y*gridsize, 255, 255, 0);
        point(x_centre-x*gridsize,y_centre-y*gridsize, 255, 255, 0);
        point(x_centre+x*gridsize,y_centre-y*gridsize, 255, 255, 0);
        delay(600);

        x++;
        px+=(2 * y_radius * y_radius);
        if(p1>=0) {
            y--;
            py-=(2 * x_radius * x_radius);
            p1=p1+(y_radius * y_radius)+px-py;

        }  else{
            p1=p1+(y_radius * y_radius)+px;
        }

    }

    //For second region
    p1=(y_radius * y_radius) *((double)x+0.5)*((double)x+0.5)+(x_radius * x_radius)*(y-1)*(y-1)-(x_radius * x_radius)*(y_radius * y_radius); //Initial value of decision paramemter


    while(y>=0)
    {
        point(x_centre+x*gridsize,y_centre+y*gridsize, 200, 54, 0);
        point(x_centre-x*gridsize,y_centre+y*gridsize, 160, 22, 22);
        point(x_centre-x*gridsize,y_centre-y*gridsize, 195, 230, 225);
        point(x_centre+x*gridsize,y_centre-y*gridsize, 255, 100, 0);
        delay(600);

        y--;
        py-=(x_radius * x_radius);
        if(p1<=0){
            x++;
            px+=(y_radius * y_radius);
            p1=p1+(x_radius * x_radius)-py+px;

        }  else  {
            p1=p1+(x_radius * x_radius)-py;
        }

    }
}

void MainWindow::on_circle_cartesian_pushbutton_clicked()
{
    p1.setX(ui->frame->x);
    p1.setY(ui->frame->y);
    int gridsize = ui->gridsize->value();
    int x0 = p1.x()/gridsize;
    int y0 = p1.y()/gridsize;
    x0 = x0*gridsize + gridsize/2;
    y0 = y0*gridsize + gridsize/2;

    int r = ui->radius->value();//radius

    long excludetime = 0;
    auto start = high_resolution_clock::now();
    int y,x=0;

    while (y>=x) {


        y =  pow(((r*gridsize)*(r*gridsize)-(x*x)),0.5)+0.5;
        auto s = high_resolution_clock::now();
        point(x0 - x, y0 - y, 255, 255, 0);
        point(x0 + x, y0 - y, 255, 255, 0);
        point(x0 - x, y0 + y, 255, 255, 0);
        point(x0 + x, y0 + y, 255, 255, 0);
        point(x0 - y, y0 - x, 255, 255, 0);
        point(x0 + y, y0 - x, 255, 255, 0);
        point(x0 - y, y0 + x, 255, 255, 0);
        point(x0 + y, y0 + x, 255, 255, 0);

        auto e = high_resolution_clock::now();
        auto d = duration_cast<microseconds>(e - s);
        excludetime = excludetime+d.count();

         x += gridsize;

    }
    auto end = high_resolution_clock::now();
    int executionTime = duration_cast<microseconds>(end - start).count();
    QTextStream out(stdout);
    out << "\nTotal executionTime:- " << executionTime << "\n";
    out << "Excluded time:- " << excludetime << "\n";
    ui->circlecartesianlabel->setText(QString::number((executionTime-excludetime)));
}

void MainWindow::on_cartesiananimationbutton_clicked()
{
    p1.setX(ui->frame->x);
    p1.setY(ui->frame->y);
    int gridsize = ui->gridsize->value();
    int x0 = p1.x()/gridsize;
    int y0 = p1.y()/gridsize;
    x0 = x0*gridsize + gridsize/2;
    y0 = y0*gridsize + gridsize/2;

    int r = ui->radius->value();//radius

    int y,x=0;

    while (y>=x) {


        y =  pow(((r*gridsize)*(r*gridsize)-(x*x)),0.5)+0.5;

        point(x0 - x, y0 - y, 255, 255, 0);
        point(x0 + x, y0 - y, 200, 255, 0);
        point(x0 - x, y0 + y, 255, 35, 5);
        point(x0 + x, y0 + y, 55, 255, 33);
        point(x0 - y, y0 - x, 255, 25, 100);
        point(x0 + y, y0 - x, 255, 23, 0);
        point(x0 - y, y0 + x, 255, 255, 200);
        point(x0 + y, y0 + x, 255, 255, 0);

        delay(600);

         x += gridsize;

    }

}

void MainWindow::on_gridsize_valueChanged()
{
    on_showgrid_clicked();
}

void MainWindow::on_draw_polygon_clicked()
{
    p1.setX(-1);
    p1.setY(-1);
}


void MainWindow::on_boundaryfill4button_clicked()
{
   // p1.setX(-1);
    //p1.setY(-1);
}
