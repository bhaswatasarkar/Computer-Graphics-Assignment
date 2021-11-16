#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <vector>
#include <algorithm>
#define maxVer 50
#define maxHt 1000
#define maxWd 800

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void Mouse_Pressed();
    void showMousePosition(QPoint& pos);

private:
    Ui::MainWindow *ui;
    QPoint p1,p2;
    int x_max,x_min,y_max,y_min;
    int R=255,G=255,B=255;
    QRgb currentcolor;
    QPoint cp1,cp2;

   // QRgb pressedpointcolor;

/*
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

    std::vector<std::pair<int,int> > EdgeList;
    std::vector<std::pair<int,int> > BezList;*/

    void point(int x,int y,int r=255,int g=255,int b=0);
    void point(int x,int y, QRgb cc);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_show_axes_clicked();

    void on_Draw_clicked();

    void on_set_point1_clicked();

    void on_set_point2_clicked();

    void on_resetButton_clicked();

    int changeX(int x);

    int changeY(int y);

    void on_showgrid_clicked();

    void drawDDALine ();

    void on_dda_clicked();

    int scale(int X);

    void setColor();


    void on_bresenhamLine_clicked();

    void on_X_clicked();
    void on_duumydda_clicked();
    void on_dummydda_clicked();
    void on_brsenhamdummy_clicked();
    void on_pushButton_2_clicked();
    void on_circle_midpoint_pushbutton_clicked();
    void on_pushButton_3_clicked();
    void on_circle_bresenham_pushbutton_clicked();
    void on_midpointanimationbutton_clicked();
    void on_bresnhamanimationbutton_clicked();
    void on_circle_midpoint_pushbutton_2_clicked();
    void on_circle_polar_pushbutton_clicked();
    void on_polaranimationbutton_clicked();
    void on_ellipse_polar_pushbutton_clicked();
    void on_polaraellipsenimationbutton_clicked();
    void on_ellipse_bresenham_pushbutton_clicked();
    void on_bresenhamellipseanimationbutton_clicked();
    void on_circle_cartesian_pushbutton_clicked();
    void on_cartesiananimationbutton_clicked();
    void on_gridsize_valueChanged();
    void on_draw_polygon_clicked();
    void draw_bresenhamLine(QPoint,QPoint);
    void draw_bresenhamLine(QPoint,QPoint,int,int,int);
    void BoundaryFill4(int x, int y, QRgb newcolor, QRgb edgecolor);
    void on_boundaryfill4button_clicked();
    void BoundaryFill8(int x, int y, QRgb newcolor, QRgb edgecolor);
    void FloodFill4 (int x, int y, QRgb newcolor,QRgb oldcolor);
    void FloodFill8 (int x, int y, QRgb newcolor,QRgb oldcolor);


    ///////////////////////
    void initEdgeTable ();

    void storeEdgeInTable (int, int, int, int);

    void on_scanlinefill_clicked();

    int* matMul3x3(double mat[3][3],int coord[3]);

    void drawPoly(std::vector<std::pair<int,int> > vlist,int R,int G,int B);

    void on_translationButton_clicked();

    void on_scaleButton_clicked();

    void on_rotateButton_clicked();

    void on_shearButton_clicked();

    void on_reflectionXButton_clicked();
    void on_reflectionButton_clicked();
    void on_reflectionYButton_clicked();
    void on_reflectwrtarbitraryline_clicked();


    //////////
    int computeCode(int xa, int ya);
    void cohenSutherlandClip(int x1, int y1,int x2, int y2);
    void on_lineClippingButton_clicked();
    int x_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4);
    int y_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4);
    void draw_Window();
    void on_setCP1Button_clicked();
    void on_setCP2Button_clicked();
    void clip(int x1, int y1, int x2, int y2);
    void on_reflectionOriginButton_clicked();
    void on_drawlineforclipping_clicked();

    void on_polygonClippingButton_clicked();
};

#endif // MAINWINDOW_H
