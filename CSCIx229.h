#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
//#include <QOpenGLFunctions>
//#include <QString>
//#include <QFont>
//#include <QFontMetricsF>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim, int firstP, int mode);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);
//For Text
//void initfont(QFont & f, int thickness);
//void print(QString text);
//void buildglyph(GLuint b, int c);
//QFont* font;
//QFontMetricsF* fm;
//float glyphthickness;
//GLuint base;

#ifdef __cplusplus
}
#endif

#endif
