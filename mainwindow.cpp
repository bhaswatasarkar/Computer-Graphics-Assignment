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
#include <utility>
#include <bits/stdc++.h>
#include <sstream>
//#define qRgb(255,255,0) BRESENHAMLINECOLOR


using namespace std;
using namespace std::chrono;

std::vector<std::pair<int, int> > vertex_list;
//for taking multiple lines as input
vector<pair<pair<int, int>, pair<int, int>>> lines;



namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


void delay (int SLEEP_ms = 300) {
    QTime dieTime= QTime::currentTime().addMSecs(SLEEP_ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QImage img=QImage(700,700,QImage::Format_RGB888);
void MainWindow::on_gridsize_valueChanged()
{
    on_showgrid_clicked();
}

void MainWindow::on_draw_polygon_clicked()
{
    p1.setX(-1);
    p1.setY(-1);
    vertex_list.clear();
    initEdgeTable();
}

//////not used
void MainWindow::on_boundaryfill4button_clicked()
{
   // p1.setX(-1);
    //p1.setY(-1);
   // ui->boundaryfill4button->toggle();
}
void MainWindow::on_X_clicked(){}
void MainWindow::on_duumydda_clicked(){}
void MainWindow::on_pushButton_2_clicked(){}
void MainWindow::on_pushButton_3_clicked(){}
void MainWindow::on_circle_midpoint_pushbutton_2_clicked(){}
void MainWindow::on_reflectionButton_clicked(){}
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
   if(ui->checkBox_adddelay->isChecked())
       delay(20);
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
   if(ui->checkBox_adddelay->isChecked())
       delay(20);
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
    if(ui->checkBox_adddelay->isChecked())
        delay(20);
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
    if(ui->checkBox_adddelay->isChecked())
        delay(20);
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
    int gridsize = ui->gridsize->value();
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
    if(ui->draw_polygon->isChecked() && p1.rx()!=-1 && !ui->reflectorline->isChecked())
    {
        int gridsize = ui->gridsize->value();
        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);
        //////////////////////
        vertex_list.push_back(std::make_pair((ui->frame->x/gridsize)*gridsize+gridsize/2,(ui->frame->y/gridsize)*gridsize+gridsize/2));
        int i=vertex_list.size();
        if(i>=2)
        {
            storeEdgeInTable(vertex_list[i-2].first, vertex_list[i-2].second, vertex_list[i-1].first, vertex_list[i-1].second);//storage of edges in edge table.
          }
        //////////////////
        draw_bresenhamLine(p1,p2);
        p1=p2;
        polygoncolor=qRgb(R,G,B);
    }
    else if(ui->boundaryfill4button->isChecked())

    {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

       BoundaryFill4((p1.rx()/k)*k+k/2,(p1.ry()/k)*k+k/2,qRgb(R,G,B),polygoncolor);
    }
    else if(ui->boundaryfill8button->isChecked())
    {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);

       BoundaryFill8((p1.rx()/k)*k+k/2,(p1.ry()/k)*k+k/2,qRgb(R,G,B),polygoncolor);
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
    else if(ui->draw_polygon->isChecked() && p1.rx()==-1 && !ui->reflectorline->isChecked())
    {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        ////////////////////
        vertex_list.push_back(std::make_pair((ui->frame->x/gridsize)*gridsize+gridsize/2,(ui->frame->y/gridsize)*gridsize+gridsize/2));
        ///////////////////
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
            on_resetButton_clicked();
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

void MainWindow::on_resetButton_clicked()//resetbutton
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
    //////////////////////
    if(ui->show_axes->isChecked())
    {
        for(int j=0;j<img.width();j+=k)
        {
            point(img.width()/2,j,237,171,230);
        }
        for(int i=0;i<= img.height();i+=k)
        {
            point(i,img.height()/2,237,171,230);
        }
    }
/////////////////////
    ui->frame->setPixmap(QPixmap::fromImage(img));
    p1.setX(-1);
    p1.setY(-1);
    vertex_list.clear();
    lines.clear();
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
                point(x,y, R, G, B);
            }

            else point(y,x, R, G, B);


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

void MainWindow::draw_bresenhamLine(QPoint point1,QPoint point2, int r, int g, int b)
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
                point(x, y, r, g, b);
            }

            else point(y, x, r, g, b);


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



//////////////////////////////

// Scanline Filling
typedef struct edgebucket
{
    int ymax;   //max y-coordinate of edge
    float xofymin;  //x-coordinate of lowest edge point updated only in aet
    float slopeinverse;
}EdgeBucket;

typedef struct edgetabletup
{
    // the array will give the scanline number
    // The edge table (ET) with edges entries sorted
    // in increasing y and x of the lower end

    int countEdgeBucket;    //no. of edgebuckets
    EdgeBucket buckets[maxVer];
}EdgeTableTuple;

EdgeTableTuple EdgeTable[maxHt], ActiveEdgeTuple;

void MainWindow::initEdgeTable()
{
    int i;
    for (i=0; i<maxHt; i++)
    {
        EdgeTable[i].countEdgeBucket = 0;
    }

    ActiveEdgeTuple.countEdgeBucket = 0;
}

void insertionSort(EdgeTableTuple *ett)
{
    int i,j;
    EdgeBucket temp;

    for (i = 1; i < ett->countEdgeBucket; i++)
    {
        temp.ymax = ett->buckets[i].ymax;
        temp.xofymin = ett->buckets[i].xofymin;
        temp.slopeinverse = ett->buckets[i].slopeinverse;
        j = i - 1;

    while ((temp.xofymin < ett->buckets[j].xofymin) && (j >= 0))
    {
        ett->buckets[j + 1].ymax = ett->buckets[j].ymax;
        ett->buckets[j + 1].xofymin = ett->buckets[j].xofymin;
        ett->buckets[j + 1].slopeinverse = ett->buckets[j].slopeinverse;
        j = j - 1;
    }
        ett->buckets[j + 1].ymax = temp.ymax;
        ett->buckets[j + 1].xofymin = temp.xofymin;
        ett->buckets[j + 1].slopeinverse = temp.slopeinverse;
    }
}

void storeEdgeInTuple (EdgeTableTuple *receiver,int ym,int xm,float slopInv)
{
    (receiver->buckets[(receiver)->countEdgeBucket]).ymax = ym;
    (receiver->buckets[(receiver)->countEdgeBucket]).xofymin = (float)xm;
    (receiver->buckets[(receiver)->countEdgeBucket]).slopeinverse = slopInv;

    insertionSort(receiver);
    (receiver->countEdgeBucket)++;
}

void MainWindow::storeEdgeInTable (int x1,int y1, int x2, int y2)
{
    float m,minv;
    int ymaxTS,xwithyminTS, scanline;

    if (x2==x1)
    {
        minv=0.000000;
    }
    else
    {
    m = ((float)(y2-y1))/((float)(x2-x1));

    if (y2==y1)
        return;

    minv = (float)1.0/m;
    }

    if (y1>y2)
    {
        scanline=y2;
        ymaxTS=y1;
        xwithyminTS=x2;
    }
    else
    {
        scanline=y1;
        ymaxTS=y2;
        xwithyminTS=x1;
    }
    storeEdgeInTuple(&EdgeTable[scanline],ymaxTS,xwithyminTS,minv);
}

void removeEdgeByYmax(EdgeTableTuple *Tup,int yy)
{
    int i,j;
    for (i=0; i< Tup->countEdgeBucket; i++)
    {
        if (Tup->buckets[i].ymax == yy)
        {
            for ( j = i ; j < Tup->countEdgeBucket -1 ; j++ )
                {
                Tup->buckets[j].ymax =Tup->buckets[j+1].ymax;
                Tup->buckets[j].xofymin =Tup->buckets[j+1].xofymin;
                Tup->buckets[j].slopeinverse = Tup->buckets[j+1].slopeinverse;
                }
                Tup->countEdgeBucket--;
            i--;
        }
    }
}

void updatexbyslopeinv(EdgeTableTuple *Tup)
{
    int i;

    for (i=0; i<Tup->countEdgeBucket; i++)
    {
        (Tup->buckets[i]).xofymin =(Tup->buckets[i]).xofymin + (Tup->buckets[i]).slopeinverse;
    }
}


void MainWindow::on_scanlinefill_clicked()
{
    int k, i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;
    k=ui->gridsize->value();

    for (i=0; i<maxHt; i++)
    {
        for (j=0; j<EdgeTable[i].countEdgeBucket; j++)
        {
            storeEdgeInTuple(&ActiveEdgeTuple,EdgeTable[i].buckets[j].
                     ymax,EdgeTable[i].buckets[j].xofymin,
                    EdgeTable[i].buckets[j].slopeinverse);
        }

        removeEdgeByYmax(&ActiveEdgeTuple, i);

        insertionSort(&ActiveEdgeTuple);

        j = 0;
        FillFlag = 0;
        coordCount = 0;
        x1 = 0;
        x2 = 0;
        ymax1 = 0;
        ymax2 = 0;
        while (j<ActiveEdgeTuple.countEdgeBucket)
        {
            if (coordCount%2==0)
            {
                x1 = (int)(ActiveEdgeTuple.buckets[j].xofymin);
                ymax1 = ActiveEdgeTuple.buckets[j].ymax;
                if (x1==x2)
                {
                    if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
                    {
                        x2 = x1;
                        ymax2 = ymax1;
                    }

                    else
                    {
                        coordCount++;
                    }
                }

                else
                {
                        coordCount++;
                }
            }
            else
            {
                x2 = (int)ActiveEdgeTuple.buckets[j].xofymin;
                ymax2 = ActiveEdgeTuple.buckets[j].ymax;

                FillFlag = 0;
                if (x1==x2)
                {
                    if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
                    {
                        x1 = x2;
                        ymax1 = ymax2;
                    }
                    else
                    {
                        coordCount++;
                        FillFlag = 1;
                    }
                }
                else
                {
                    coordCount++;
                    FillFlag = 1;
                }

            if(FillFlag)
            {
                    p1.setX(x1-k);p1.setY(i);
                    p2.setX(x2+k);p2.setY(i);
                   if(ui->checkBox_adddelay->isChecked())
                        delay(50);
                    draw_bresenhamLine(p1,p2);

            }

        }

        j++;
    }
    updatexbyslopeinv(&ActiveEdgeTuple);
}

    vertex_list.clear();
    initEdgeTable();
}

///////////transformations
int* MainWindow::matMul3x3(double mat[3][3],int coord[3])
{
    int i,k,res[3];
    for (i = 0; i < 3; i++)
    {
            res[i]= 0;
            for (k = 0; k < 3; k++)
            {

                res[i] += mat[i][k]*coord[k];
                std::string s=patch::to_string(mat[i][k])+"||"+patch::to_string(coord[k])+"="+patch::to_string(res[i]);
                qDebug("%s",s.c_str());
            }

    }
    std::string s=patch::to_string(res[0])+"$$"+patch::to_string(res[1])+"$$"+patch::to_string(res[2]);
    qDebug("%s",s.c_str());
    return res;
}

void MainWindow::drawPoly(std::vector<std::pair<int,int> > vlist,int r,int g,int b)
{
    for(int i=0;i<vlist.size()-1;i++)
    {
        p1.setX(vlist[i].first);p1.setY(vlist[i].second);
        p2.setX(vlist[i+1].first);p2.setY(vlist[i+1].second);
        draw_bresenhamLine(p1,p2,r,g,b);
    }
}

void MainWindow::on_translationButton_clicked()
{

    std::vector<std::pair<int,int> > old_vertex;
    for(int i=0;i<vertex_list.size();i++)
    {
        old_vertex.push_back(vertex_list[i]);
    }

    int k=ui->gridsize->value();
    int tx=ui->_x->value();
    int ty=ui->_y->value();
    tx*=k;
    ty*=k;
    ty=-ty;
    int i,len=vertex_list.size();

    // matrix for translation
    double mat[3][3]={{1,0,tx},{0,1,ty},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=vertex_list[i].first;
        coord[1]=vertex_list[i].second;
        coord[2]=1;
        coord=matMul3x3(mat,coord);
        vertex_list[i].first=coord[0]/coord[2];
        vertex_list[i].second=coord[1]/coord[2];
    }

    drawPoly(old_vertex,0,0,0);
    on_show_axes_clicked();
    drawPoly(vertex_list,R,G,B);

}

void MainWindow::on_rotateButton_clicked()
{
    int gridsize = ui->gridsize->value();
    std::vector<std::pair<int,int> > old_vertex;
    for(int i=0;i<vertex_list.size();i++)
    {
        old_vertex.push_back(vertex_list[i]);
    }

    int angle=ui->rotate_angle->value();
    double dang=(double)angle*M_PI/180.0;
    double sinang=sin(dang);
    double cosang=cos(dang);

    //Point about which to be rotated
    //int piv_x=(ui->frame->x/gridsize)*gridsize+gridsize/2;
    //int piv_y=(ui->frame->y/gridsize)*gridsize+gridsize/2;
    int piv_x=((img.width()/2)/gridsize)*gridsize+(ui->origin_x->value())*gridsize+gridsize/2;
    int piv_y=((img.height()/2)/gridsize)*gridsize-(ui->origin_y->value())*gridsize+gridsize/2;

    int i,len=vertex_list.size();

    // matrix for rotation
    double mat[3][3]={{cosang,-sinang,0},{sinang,cosang,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=vertex_list[i].first-piv_x;
        coord[1]=piv_y-vertex_list[i].second;
        coord[2]=1;
        coord=matMul3x3(mat,coord);

        vertex_list[i].first=coord[0]/coord[2]+piv_x;
        vertex_list[i].second=piv_y-coord[1]/coord[2];
    }

    drawPoly(old_vertex,0,0,0);
    on_show_axes_clicked();
    drawPoly(vertex_list,255,255,0);

}

void MainWindow::on_shearButton_clicked()
{
    int gridsize = ui->gridsize->value();
    std::vector<std::pair<int,int> > old_vertex;
    for(int i=0;i<vertex_list.size();i++)
    {
        old_vertex.push_back(vertex_list[i]);
    }

    double shx=ui->_x->value();
    double shy=ui->_y->value();
    //Point about which to be scaled
    //int piv_x=(ui->frame->x/gridsize)*gridsize+gridsize/2;
    //int piv_y=(ui->frame->y/gridsize)*gridsize+gridsize/2;
    int piv_x=((img.width()/2)/gridsize)*gridsize+(ui->origin_x->value())*gridsize+gridsize/2;
    int piv_y=((img.height()/2)/gridsize)*gridsize-(ui->origin_y->value())*gridsize+gridsize/2;

    int i,len=vertex_list.size();

    // matrix for shearing
    double mat[3][3]={{1,shx,0},{shy,1,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=vertex_list[i].first-piv_x;
        coord[1]=piv_y-vertex_list[i].second;
        coord[2]=1;
        coord=matMul3x3(mat,coord);
        vertex_list[i].first=coord[0]/coord[2]+piv_x;
        vertex_list[i].second=piv_y-coord[1]/coord[2];
    }
    drawPoly(old_vertex,0,0,0);
    on_show_axes_clicked();
    drawPoly(vertex_list,255,255,0);

}

void MainWindow::on_reflectionXButton_clicked()
{
    int gridsize = ui->gridsize->value();
    std::vector<std::pair<int,int> > old_vertex;
    for(int i=0;i<vertex_list.size();i++)
    {
        old_vertex.push_back(vertex_list[i]);
    }

    //double sx=ui->_x->value();
    //double sy=ui->_y->value();
    //Point about which to be scaled
    int piv_x=((img.width()/2)/gridsize)*gridsize+gridsize/2;
    int piv_y=((img.height()/2)/gridsize)*gridsize+gridsize/2;
    //int piv_x=(ui->frame->x/gridsize)*gridsize+gridsize/2;
    //int piv_y=(ui->frame->y/gridsize)*gridsize+gridsize/2;

    int i,len=vertex_list.size();

    // matrix for scaling
    double mat[3][3]={{1,0,0},{0,-1,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=vertex_list[i].first-piv_x;
        coord[1]=piv_y-vertex_list[i].second;
        coord[2]=1;
        coord=matMul3x3(mat,coord);
        vertex_list[i].first=coord[0]/coord[2]+piv_x;
        vertex_list[i].second=piv_y-coord[1]/coord[2];

    }

    //drawPoly(old_vertex,0,0,0);
    on_show_axes_clicked();
    drawPoly(vertex_list,255,255,0);

}


void MainWindow::on_reflectionYButton_clicked()
{
    int gridsize = ui->gridsize->value();
    std::vector<std::pair<int,int> > old_vertex;
    for(int i=0;i<vertex_list.size();i++)
    {
        old_vertex.push_back(vertex_list[i]);
    }

    //double sx=ui->_x->value();
    //double sy=ui->_y->value();
    //Point about which to be scaled
    int piv_x=((img.width()/2)/gridsize)*gridsize+gridsize/2;
    int piv_y=((img.height()/2)/gridsize)*gridsize+gridsize/2;
    //int piv_x=(ui->frame->x/gridsize)*gridsize+gridsize/2;
    //int piv_y=(ui->frame->y/gridsize)*gridsize+gridsize/2;

    int i,len=vertex_list.size();

    // matrix for scaling
    double mat[3][3]={{-1,0,0},{0,1,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=vertex_list[i].first-piv_x;
        coord[1]=piv_y-vertex_list[i].second;
        coord[2]=1;
        coord=matMul3x3(mat,coord);
        vertex_list[i].first=coord[0]/coord[2]+piv_x;
        vertex_list[i].second=piv_y-coord[1]/coord[2];

    }

    //drawPoly(old_vertex,0,0,0);
    on_show_axes_clicked();
    drawPoly(vertex_list,255,255,0);

}





void MainWindow::on_scaleButton_clicked()
{
    int gridsize = ui->gridsize->value();
    std::vector<std::pair<int,int> > old_vertex;
    for(int i=0;i<vertex_list.size();i++)
    {
        old_vertex.push_back(vertex_list[i]);
    }

    double sx=ui->_x->value();
    double sy=ui->_y->value();
    //Point about which to be scaled
    int piv_x=((img.width()/2)/gridsize)*gridsize+(ui->origin_x->value())*gridsize+gridsize/2;
    int piv_y=((img.height()/2)/gridsize)*gridsize-(ui->origin_y->value())*gridsize+gridsize/2;
    //int piv_x=(ui->frame->x/gridsize)*gridsize+gridsize/2;
    //int piv_y=(ui->frame->y/gridsize)*gridsize+gridsize/2;
     // QTextStream out(stdout);
     // out<<piv_x;

    int i,len=vertex_list.size();

    // matrix for scaling
    double mat[3][3]={{sx,0,0},{0,sy,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=vertex_list[i].first-piv_x;
        coord[1]=piv_y-vertex_list[i].second;
        coord[2]=1;
        coord=matMul3x3(mat,coord);
        vertex_list[i].first=coord[0]/coord[2]+piv_x;
        vertex_list[i].second=piv_y-coord[1]/coord[2];

    }

    drawPoly(old_vertex,0,0,0);
    on_show_axes_clicked();
    drawPoly(vertex_list,255,255,0);


}

void MainWindow::on_reflectionOriginButton_clicked()
{
    int gridsize = ui->gridsize->value();
    std::vector<std::pair<int,int> > old_vertex;
    for(int i=0;i<vertex_list.size();i++)
    {
        old_vertex.push_back(vertex_list[i]);
    }


    int piv_x=((img.width()/2)/gridsize)*gridsize+gridsize/2;
    int piv_y=((img.height()/2)/gridsize)*gridsize+gridsize/2;
    //int piv_x=(ui->frame->x/gridsize)*gridsize+gridsize/2;
    //int piv_y=(ui->frame->y/gridsize)*gridsize+gridsize/2;

    int i,len=vertex_list.size();

    // matrix for scaling
    double mat[3][3]={{-1,0,0},{0,-1,0},{0,0,1}};

    for(i=0;i<len;i++)
    {
        int* coord=(int*)malloc(3*sizeof(int));
        coord[0]=vertex_list[i].first-piv_x;
        coord[1]=piv_y-vertex_list[i].second;
        coord[2]=1;
        coord=matMul3x3(mat,coord);
        vertex_list[i].first=coord[0]/coord[2]+piv_x;
        vertex_list[i].second=piv_y-coord[1]/coord[2];

    }

    //drawPoly(old_vertex,0,0,0);
    on_show_axes_clicked();
    drawPoly(vertex_list,255,255,0);
}


void MainWindow::on_reflectwrtarbitraryline_clicked()
{
     int gridsize = ui->gridsize->value();
    std::vector<std::pair<int,int> > old_vertex;
    for(int i=0;i<vertex_list.size();i++)
    {
        old_vertex.push_back(vertex_list[i]);
    }


    int i,len=vertex_list.size();

      double dx=p1.x()-p2.x();
      double dy=p1.y()-p2.y();

      double a=-dy;
      double b=dx;
      double c=p1.x()*dy-p1.y()*dx;

      for(i=0;i<len;i++)
      {
          int x1=vertex_list[i].first;
          int y1=vertex_list[i].second;

          vertex_list[i].first=(int)((double)x1-(double)(2*a*(a*x1+b*y1+c))/(double)((a*a+b*b)));
          vertex_list[i].second=(int)((double)y1-(double)(2*b*(a*x1+b*y1+c))/(double)((a*a+b*b)));
      }


   // drawPoly(old_vertex,0,0,0);
    on_show_axes_clicked();
    drawPoly(vertex_list,255,255,0);

}

// =================== CLIPPING =====================================================

int clipper_points[4][2];

void MainWindow::on_setCP1Button_clicked()
{
    int gridsize = ui->gridsize->value();
    cp1.setX((ui->frame->x/gridsize)*gridsize+gridsize/2);
    cp1.setY((ui->frame->y/gridsize)*gridsize+gridsize/2);
}

void MainWindow::on_setCP2Button_clicked()
{
       int gridsize = ui->gridsize->value();
       cp2.setX((ui->frame->x/gridsize)*gridsize+gridsize/2);
       cp2.setY((ui->frame->y/gridsize)*gridsize+gridsize/2);

       clipper_points[0][0]=cp1.x();
       clipper_points[0][1]=cp1.y();
       clipper_points[1][0]=cp1.x();
       clipper_points[1][1]=cp2.y();
       clipper_points[2][0]=cp2.x();
       clipper_points[2][1]=cp2.y();
       clipper_points[3][0]=cp2.x();
       clipper_points[3][1]=cp1.y();

       draw_Window();
}

void MainWindow::draw_Window()
{
    p1.setX(clipper_points[0][0]);
    p1.setY(clipper_points[0][1]);
    p2.setX(clipper_points[1][0]);
    p2.setY(clipper_points[1][1]);
    draw_bresenhamLine(p1,p2,54,101,232);

    p1.setX(clipper_points[1][0]);
    p1.setY(clipper_points[1][1]);
    p2.setX(clipper_points[2][0]);
    p2.setY(clipper_points[2][1]);
    draw_bresenhamLine(p1,p2,54,101,232);

    p1.setX(clipper_points[2][0]);
    p1.setY(clipper_points[2][1]);
    p2.setX(clipper_points[3][0]);
    p2.setY(clipper_points[3][1]);
    draw_bresenhamLine(p1,p2,54,101,232);

    p1.setX(clipper_points[3][0]);
    p1.setY(clipper_points[3][1]);
    p2.setX(clipper_points[0][0]);
    p2.setY(clipper_points[0][1]);
    draw_bresenhamLine(p1,p2,54,101,232);
}
// ************************** LINE CLIPPING ********************************
// Defining region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000


// Function to compute region code for a point(x, y)
int MainWindow::computeCode(int xa, int ya)
{
    int x_min=cp1.x(),x_max=cp2.x(),y_min=cp1.y(),y_max=cp2.y();

    // initialized as being inside
        int code = INSIDE;
        if (xa < x_min)       // to the left of rectangle
            code |= LEFT;
        else if (xa > x_max)  // to the right of rectangle
            code |= RIGHT;
        if (ya < y_min)       // below the rectangle
            code |= BOTTOM;
        else if (ya > y_max)  // above the rectangle
            code |= TOP;

        return code;
}


void MainWindow::cohenSutherlandClip(int x1, int y1,int x2, int y2)
{
    int x_min=clipper_points[0][0],x_max=clipper_points[2][0],y_min=clipper_points[0][1],y_max=clipper_points[2][1];
    // Compute region codes for P1, P2
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    // Initialize line as outside the rectangular window
    bool accept = false;

    while (true)
    {

        if ((code1 == 0) && (code2 == 0))
        {
            // If both endpoints lie within rectangle
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // If both endpoints are outside rectangle,
            // in same region
            break;
        }
        else
        {
            // Some segment of line lies within the
            // rectangle
            int code_out;
            int x, y;

            // At least one endpoint is outside the
            // rectangle, pick it.
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & TOP)
            {
                // point is above the clip rectangle
                x = x1 + (int)((double)(x2 - x1) *(double)(y_max - y1) /(double)(y2 - y1));
                y = y_max;
            }
            else if (code_out & BOTTOM)
            {
                // point is below the rectangle
                x = x1 + (int)((double)(x2 - x1) * (double)(y_min - y1) / (double)(y2 - y1));
                y = y_min;
            }
            else if (code_out & RIGHT)
            {
                // point is to the right of rectangle
                y = y1 + (int)((double)(y2 - y1) * (double)(x_max - x1) / (double)(x2 - x1));
                x = x_max;
            }
            else if (code_out & LEFT)
            {
                // point is to the left of rectangle
                y = y1 + (int)((double)(y2 - y1) * (double)(x_min - x1) / (double)(x2 - x1));
                x = x_min;
            }

            // Now intersection point x,y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1)
            {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    if (accept)
    {
        //If accepted
        //Just reset and draw the boundary and the line
        //Reset the screen and draw the grid

        p1.setX(x1);
        p1.setY(y1);

        p2.setX(x2);
        p2.setY(y2);

        draw_bresenhamLine(p1,p2,255,255,255);
        draw_Window();
    }
    else
    {
        //If not accepted
        //Just reset and draw the boundary
        //Reset the screen and draw the grid
        draw_Window();
    }
    }


void MainWindow::on_lineClippingButton_clicked()
{

    for (pair<pair<int, int>, pair<int, int>> p : lines) {
        p1.setX(p.first.first);
        p1.setY(p.first.second);
        p2.setX(p.second.first);
        p2.setY(p.second.second);

        draw_bresenhamLine(p1,p2,0,0,0);
        cohenSutherlandClip(p1.x(),p1.y(),p2.x(),p2.y());
        //cohenSutherlandClip(p.first.first, p.first.second, p.second.first, p.second.second);
        }
        lines.clear();
}

void MainWindow::on_drawlineforclipping_clicked()
{       int gridsize = ui->gridsize->value();
    // (x0, y0) -> point1 ; (xn, yn) -> point 2
        double x0 = p1.x() / gridsize;
        double xn = p2.x() / gridsize;
        double y0 = p1.y() / gridsize;
        double yn = p2.y() / gridsize;

        lines.push_back({{p1.x(), p1.y()}, {p2.x(), p2.y()}});

        /*//required for evaluating the step count in the algorithm
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

        for (int steps =0; steps <= (dx > dy ? dx : dy); steps++) {
            point (x, y, R, G, B);
            x += Dx * gridsize;
            y += Dy * gridsize;
        }*/
        draw_bresenhamLine(p1,p2,R,G,B);

}






//**************************************************************************
//**************************************************************************
//**************polygon clipping********************************************

const int MAX_POINTS = 20;

// Returns x-value of point of intersection of two
// lines
int MainWindow:: x_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4)
{
    y1=ui->frame->height()-y1+1;
    y2=ui->frame->height()-y2+1;
    y3=ui->frame->height()-y3+1;
    y4=ui->frame->height()-y4+1;
    int num = (x1*y2 - y1*x2) * (x3-x4) -
              (x1-x2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    int retx=num/den;
    return retx;
}

// Returns y-value of point of intersection of
// two lines
int MainWindow:: y_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4)
{
    y1=ui->frame->height()-y1+1;
    y2=ui->frame->height()-y2+1;
    y3=ui->frame->height()-y3+1;
    y4=ui->frame->height()-y4+1;
    int num = (x1*y2 - y1*x2) * (y3-y4) -
              (y1-y2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    int rety= (ui->frame->height()-num/den+1);
    return rety;
}

// This functions clips all the edges w.r.t one clip
// edge of clipping area
void MainWindow:: clip(int x1, int y1, int x2, int y2)
{
    int poly_size=vertex_list.size()-1;
    int new_poly_size = 0;

    std::vector<std::pair<int,int> > new_points;

    // (ix,iy),(kx,ky) are the co-ordinate values of
    // the points
    for (int i = 0; i < poly_size; i++)
    {
        // i and k form a line in polygon
        int k = (i+1) % poly_size;
        int ix = vertex_list[i].first, iy = vertex_list[i].second;
        int kx = vertex_list[k].first, ky = vertex_list[k].second;

        // Calculating position of first and second point

        int i_pos,k_pos;
        if(x2==x1 && ix>x1) i_pos=1;
        else if(x2==x1 && ix<x1) i_pos=-1;
        else if(y2==y1 && iy<y1) i_pos=1;
        else i_pos=-1;

        if(x2==x1 && kx>x1) k_pos=1;
        else if(x2==x1 && kx<x1) k_pos=-1;
        else if(y2==y1 && ky<y1) k_pos=1;
        else k_pos=-1;

        if(y1>y2||x1>x2)
        {
            i_pos=(-1)*i_pos;
            k_pos=(-1)*k_pos;
        }

        // Case 1 : When both points are inside
        if (i_pos >= 0  && k_pos >= 0)
        {
            //Only second point is added
            new_points.push_back(std::make_pair(kx,ky));
            new_poly_size++;
        }

        // Case 2: When only first point is outside
        else if (i_pos < 0  && k_pos >= 0)
        {
            // Point of intersection with edge
            // and the second point is added

            new_points.push_back(std::make_pair(x_intersect(x1,y1, x2, y2, ix, iy, kx, ky),y_intersect(x1,y1, x2, y2, ix, iy, kx, ky)));
            new_poly_size++;

            new_points.push_back(std::make_pair(kx,ky));
            new_poly_size++;
        }

        // Case 3: When only second point is outside
        else if (i_pos >= 0  && k_pos < 0)
        {
            //Only point of intersection with edge is added

            new_points.push_back(std::make_pair(x_intersect(x1,y1, x2, y2, ix, iy, kx, ky),y_intersect(x1,y1, x2, y2, ix, iy, kx, ky)));
            new_poly_size++;
        }

        // Case 4: When both points are outside
        else
        {
            //No points are added
        }
    }

    // Copying new points into original array
    // and changing the no. of vertices
    poly_size = new_poly_size;
    vertex_list.clear();
    for (int i = 0; i < new_points.size(); i++)
    {
        vertex_list.push_back(new_points[i]);
    }
    vertex_list.push_back(new_points[0]);
}

void MainWindow::on_polygonClippingButton_clicked()
{
    std::vector<std::pair<int,int> > old_vertex;
    for(int i=0;i<vertex_list.size();i++)
    {
        old_vertex.push_back(vertex_list[i]);
    }
    int clipper_size=4;
    //i and k are two consecutive indexes
    for (int i=0; i<clipper_size; i++)
    {
        int k = (i+1) % clipper_size;

        // We pass the current array of vertices, it's size
        // and the end points of the selected clipper line
        clip(clipper_points[i][0],
             clipper_points[i][1], clipper_points[k][0],
             clipper_points[k][1]);
    }

    drawPoly(old_vertex,0,0,0);
    draw_Window();
    drawPoly(vertex_list,255,255,255);

}
