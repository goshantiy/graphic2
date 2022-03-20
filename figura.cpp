#include <math.h>
#include "figura.h"

Matrix::Matrix(std::initializer_list<row> matrix) : std::vector<row>(matrix)
{
    rowCount = matrix.size();
    columnCount = matrix.begin()->size();
}

Matrix::Matrix(int rows, int columns) : std::vector<row>(rows, row(columns))
{
    rowCount = rows;
    columnCount = columns;
}

Matrix Matrix::operator*(const Matrix &obj)
{
    Matrix res(rowCount, obj.columnCount);

    for (int j = 0; j < rowCount; j++)
    {
        for (int i = 0; i < res.columnCount; i++)
        {
            for (int k = 0; k < columnCount; k++)
            {
                res[j][i] += (*this)[j][k] * obj[k][i];
            }
        }
    }

    return res;
}


void Figura::computeCoordinates(double obsCoord[3])
{

    Matrix T = {{1,0,0,0},
                {0,1,0,0},
                {0,0,1,0},
                {-obsCoord[0],-obsCoord[1],-obsCoord[2],1}};
    Matrix S = {{-1,0,0,0},
                {0,1,0,0},
                {0,0,1,0},
                {0,0,0,1}};
    Matrix Rx90 = {{1,0,0,0},
                   {0,0,-1,0},
                   {0,1,0,0},
                   {0,0,0,1}};

    double d = std::sqrt(std::pow(obsCoord[0],2) + std::pow(obsCoord[1],2));
    double sinU;
    double cosU;
    if (d == 0)
    {
        sinU = 0;
        cosU = 1;
    } else {
        sinU = obsCoord[0] / d;
        cosU = obsCoord[1] / d;
    }

    Matrix Ry = {{cosU,0,sinU,0},
                 {0,1,0,0},
                 {-sinU,0,cosU,0},
                 {0,0,0,1}};

    double s = std::sqrt(std::pow(obsCoord[0],2) + std::pow(obsCoord[1],2) + std::pow(obsCoord[2],2));
    double sinW;
    double cosW;
    if(s == 0)
    {
        sinW = 0;
        cosW = 1;
    } else {
        sinW = obsCoord[2] / s;
        cosW = d / s;
    }

    Matrix Rx = {{1,0,0,0},
                 {0,cosW,-sinW,0},
                 {0,sinW,cosW,0},
                 {0,0,0,1}};


    Matrix V = T*S*Rx90*Ry*Rx;

    Matrix VerNa(NV,3);

    for(int i = 0; i < NV; i++)
    {
        Matrix buf = {{Ver[i][0],Ver[i][1],Ver[i][2],1}};
        VerNa[i] = (buf * V)[0];
    }

    Matrix VerKa(NV,2);

    for(int i = 0; i < NV; i++)
    {
        if (VerNa[i][2] != 0)
        {
            VerKa[i][0] = VerNa[i][0] / VerNa[i][2] * s;
            VerKa[i][1] = VerNa[i][1] / VerNa[i][2] * s;
        }
        else
            VerNa[i][0] = VerNa[i][1] = -1;
    }


    double Pk = 3;
//    double Xc = 500;
//    double Yc = 300;
//    double Xe = 300;
//    double Ye = 200;
    double Xc = 500;
    double Yc = 600;
    double Xe = 200;
    double Ye = 200;

    for(int i = 0 ; i < NV; i++)
    {
        VerEk[i][0] = (VerKa[i][0] * Xe / Pk) + Xc;
        VerEk[i][1] = (VerKa[i][1] * Ye / Pk + Yc) * (-1) + 2*Yc;
    }

}

void Figura::draw(QPainter *painter)
{
    for(auto reb : Reb)
    {
        painter->drawLine(VerEk[reb[0]][0],VerEk[reb[0]][1],VerEk[reb[1]][0],VerEk[reb[1]][1]);
    }
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(4);

    painter->setPen(pen);
    painter->drawPoint(VerEk[markedNode][0],VerEk[markedNode][1]);
    painter->drawLine(VerEk[Reb[markedReb][0]][0],VerEk[Reb[markedReb][0]][1],VerEk[Reb[markedReb][1]][0],VerEk[Reb[markedReb][1]][1]);
}

