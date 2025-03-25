#ifndef POINT2D_H
#define POINT2D_H

class Point2D
{
private:
    int x;
    int y;

public:
    Point2D();

    Point2D(int x, int y);

    int getX();

    int getY();
};

#endif // POINT2D_H
