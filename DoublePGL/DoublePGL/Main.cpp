#include <stdio.h>
#include "freeglut.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>


const double m1 = 1; // масса 1-го стержня
const double m2 = 1; // масса 2-го стержня
const double l1 = 1; // длина 1-го стержня
const double l2 = 1; // длина 2-го стержня
const double g = 9.8;
const double initialfi1 = M_PI / 3; // начальное значение угла отклонения 1-го стержня
const double initialfi2 = M_PI / 6; // начальное значение угла отклонения 2-го стержня
const double initialp1 = 0; // начальное значение обобщенного импульса 1-го стержня
const double initialp2 = 0; // начальное значение обобщенного импульса 2-го стержня

const int h = 800; // высота окна
const double k = h / 3; // коэффициент перевода

const double a = (m1 / 3 + m2) * pow(l1, 2);
const double b = m2 * pow(l2, 2) / 3;
const double c = m2 * l1 * l2 / 2;
const double y = (m1 / 2 + m2) * g * l1;
const double e = m2 * g * l2 / 2;
//const int n = 100;


double Fi1(double t, double fi1, double fi2, double p1, double p2) {
    return((b * p1 - c * p2 * cos(fi1 - fi2)) / (a * b - pow(c * cos(fi1 - fi2), 2)));
}
double Fi2(double t, double fi1, double fi2, double p1, double p2) {
    return((a * p2 - c * p1 * cos(fi1 - fi2)) / (a * b - pow(c * cos(fi1 - fi2), 2)));
}
double P1(double t, double fi1, double fi2, double p1, double p2) {
    return(-c * Fi1(t, fi1, fi2, p1, p2) * Fi2(t, fi1, fi2, p1, p2) * sin(fi1 - fi2) - y * sin(fi1));
}
double P2(double t, double fi1, double fi2, double p1, double p2) {
    return(c * Fi1(t, fi1, fi2, p1, p2) * Fi2(t, fi1, fi2, p1, p2) * sin(fi1 - fi2) - e * sin(fi2));
}

struct Size
{
    float w;
    float h;
};

Size size = { 1200, 800 };

struct Coord
{
    double x1;
    double x2;
    double y1;
    double y2;
};

Coord pos = { 0, 0, 0, 0 };

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(size.w / 2, size.h);
    glVertex2f(pos.x1, pos.y1);
    glVertex2f(pos.x2, pos.y2);
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(size.w / 2, size.h);
    glVertex2f(pos.x1, pos.y1);
    glVertex2f(pos.x2, pos.y2);
    glEnd();
    
    glutSwapBuffers();
}

double k1, k2, k3, k4, r1, r2, r3, r4, h1, h2, h3, h4, g1, g2, g3, g4, dt;
double t = 0;
double end = 10;
double fi1 = initialfi1;
double fi2 = initialfi2;
double p1 = initialp1;
double p2 = initialp2;

void time(int=0)
{
    
    // Изменяться будут координаты
    
    dt = 0.01;
    
    k1 = dt * Fi1(t, fi1, fi2, p1, p2);
    r1 = dt * Fi2(t, fi1, fi2, p1, p2);
    h1 = dt * P1(t, fi1, fi2, p1, p2);
    g1 = dt * P2(t, fi1, fi2, p1, p2);

    k2 = dt * Fi1(t + dt / 2, fi1 + k1 / 2, fi2 + r1 / 2, p1 + h1 / 2, p2 + g1 / 2);
    r2 = dt * Fi2(t + dt / 2, fi1 + k1 / 2, fi2 + r1 / 2, p1 + h1 / 2, p2 + g1 / 2);
    h2 = dt * P1(t + dt / 2, fi1 + k1 / 2, fi2 + r1 / 2, p1 + h1 / 2, p2 + g1 / 2);
    g2 = dt * P2(t + dt / 2, fi1 + k1 / 2, fi2 + r1 / 2, p1 + h1 / 2, p2 + g1 / 2);

    k3 = dt * Fi1(t + dt / 2, fi1 + k2 / 2, fi2 + r2 / 2, p1 + h2 / 2, p2 + g2 / 2);
    r3 = dt * Fi2(t + dt / 2, fi1 + k2 / 2, fi2 + r2 / 2, p1 + h2 / 2, p2 + g2 / 2);
    h3 = dt * P1(t + dt / 2, fi1 + k2 / 2, fi2 + r2 / 2, p1 + h2 / 2, p2 + g2 / 2);
    g3 = dt * P2(t + dt / 2, fi1 + k2 / 2, fi2 + r2 / 2, p1 + h2 / 2, p2 + g2 / 2);

    k4 = dt * Fi1(t + dt, fi1 + k3, fi2 + r3, p1 + h3, p2 + g3);
    r4 = dt * Fi2(t + dt, fi1 + k3, fi2 + r3, p1 + h3, p2 + g3);
    h4 = dt * P1(t + dt, fi1 + k3, fi2 + r3, p1 + h3, p2 + g3);
    g4 = dt * P2(t + dt, fi1 + k3, fi2 + r3, p1 + h3, p2 + g3);

    fi1 += (k1 + k2 / 2 + k3 / 2 + k4) / 6;
    fi2 += (r1 + r2 / 2 + r3 / 2 + r4) / 6;
    p1 += (h1 + h2 / 2 + h3 / 2 + h4) / 6;
    p2 += (g1 + g2 / 2 + g3 / 2 + g4) / 6;

    t += dt;

    pos.x1 = k * l1 * sin(fi1) + size.w / 2;
    pos.y1 = -k * l1 * cos(fi1) + size.h;
    pos.x2 = k * (l1 * sin(fi1) + l2 * sin(fi2)) + size.w / 2;
    pos.y2 = k * (-l1 * cos(fi1) - l2 * cos(fi2)) + size.h;
    
    display();
    glutTimerFunc(10, time, 0);

}

int main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(size.w, size.h);
    glutCreateWindow("DoubleP");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    time();
    glutMainLoop();

    return 0;
}