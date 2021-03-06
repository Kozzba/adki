#include "draw.h"
#include <QtGui>
#include <fstream>
#include <QWidget>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //Set default draw mode
    draw_mode = true;

    //Draw point outside Canvas
    q.setX(-50);
    q.setY(-50);
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    //Cursor position
    double x = e->pos().x();
    double y = e->pos().y();

    //Mode = polygon
    if (draw_mode)
    {
        //Create new point
        QPoint p(x, y);

        //Add point to the polygon
        polygon.push_back(p);
    }

    //Mode = q
    else
    {
        //Change coordinates of q
        q.setX(x);
        q.setY(y);
    }

    polygons.push_back(polygon);
    repaint();
}


void Draw::paintEvent(QPaintEvent *e)
{
    //Start drawing
    QPainter painter(this);
    painter.begin(this);

    //Create polygon
    QPolygon pol;

    //Copy all points into polygon
    for (int i = 0; i< polygons.size(); i++ )
    {
         pol.append(polygons[i]);
    }

    //Draw polygon
    painter.drawPolygon(pol);

    //Set color of colored polygon
    QBrush brush;
    QPainterPath path;
    QPolygon colored_polygon;
    brush.setColor(Qt::green);
    brush.setStyle(Qt::BDiagPattern);

    //Draw polygon with point inside
    for(int i = 0; i < result.size(); i++)
    {
        if(result[i] == 1 || result[i] == -1)
        {
            colored_polygon << polygons[i];
            path.addPolygon(colored_polygon);
            painter.fillPath(path, brush);
            painter.drawPolygon(colored_polygon);
            colored_polygon.clear();
        }

    }

    //Draw q point
    int r = 5;
    painter.drawEllipse(q.x(), q.y(), 2 * r, 2 *r);

    //End drawing
    painter.end();
}

void Draw::loadFile(std::string &string_path)
{

    polygons.clear();
    repaint();
    int id;
    double x;
    double y;
    QPolygon polygon;

    //Path to polygon file
    std::ifstream polygonFile(string_path);

    if(polygonFile.is_open())
    {
        //Fill the vectors
        while(polygonFile >> id >> x >> y)
        {
            //New polygon
            if (id == 1)
            {
                if (polygon.isEmpty() == FALSE)
                {
                    polygons.push_back(polygon);
                }
                polygon.clear();
                polygon << QPoint(x, y);
            }
            else
                polygon << QPoint(x, y);
        }

        //Add last polygon into vector
        polygons.push_back(polygon);

        //Close polygon file
        polygonFile.close();
    }

   //Repaint screen
   repaint();
}

void Draw::clearC()
{
    //Clear polygon and vector polygons
    polygon.clear();
    polygons.clear();

    //Draw point outside Canvas
    q.setX(-50);
    q.setY(-50);

    //Remove polygons from Canvas
    result.clear();

    //Repaint screen
    repaint();

}
