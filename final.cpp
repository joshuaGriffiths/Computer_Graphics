/*
 *  Final Project
 *
 *  Demonstrates differnt styles of text.
 *
 *  Key bindings:
 *  l          Toggle lighting on/off
 *  t          Change textures
 *  m          Toggles Perspecitve mode modulate/replace
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  p/P        Changes presets
 *  []         Lower/rise light
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "text3d.h"

using namespace std;


// Mode Variables
int mode=0;       //  Texture mode
int ntex=0;       //  Cube faces
int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=10.0;   //  Size of world
int fp=0;         //  First person mode
int fov=45;       //  Field of view (for perspective)
int rot = 0.0;    //rotation var for first person
int p = 0;      //First Preset

/* Eye coords */
double eX = 0;
double eY = 0;
double eZ = 10;

//***** Camera looking coords ******//
double cX = 0;
double cZ = 0;


// Light values
int emission  =   0;  // Emission intensity (%)
float ambient   =  30;  // Ambient intensity (%)
float diffuse   = 100;  // Diffuse intensity (%)
float specular  =   0;  // Specular intensity (%)
float shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
float zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[7]; // Texture names
int texChange = 0;

//Input Variables
char userInput[100];
string otherInput;
string STRS[5];

//Color Variables
double colOne = 0;
double colTwo = 0;
double colThree = 0;


//***** ROUTINE TO OUPUT TEXT *****//
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
    char    buf[LEN];
    char*   ch=buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args,format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}






//Computes a scaling value so that the strings
float computeScale(string strs[5]) {
    float maxWidth = 0;
    for(int i = 0; i < 1; i++) {
        float width = t3dDrawWidth(strs[i]);
        if (width > maxWidth) {
            maxWidth = width;
        }
    }
    
    return 1.0f;
}


float _angle = -30.0f;//for angle of text
float _scale; //to scale text to window size




//***** FREE MEMORY USED BY 3D TEXT *****//
void cleanup() {
    t3dCleanup();
}


static void A(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    
    //*****FRONT*****//
    
    // Left
    
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    //glColor3f(1,0,0);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    
    //Top
    //glColor3f(1,0,0);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    //Middle
    //glColor3f(1,0,0);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(-1,.1,1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    //glColor3f(1,0,0);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    
    
    //Top
    //glColor3f(1,0,0);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    //Middle
    //glColor3f(1,0,0);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0);glVertex3f(-1,.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    //glColor3f(1,1,0);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    //glColor3f(0,1,0);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    //glColor3f(0,1,0);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    //glColor3f(0,1,1);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    //glColor3f(0,1,1);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    
    //*****MIDDLE*****//
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+0.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+0.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+0.1,-1);
    
    //Bottem
    //glColor3f(1,0,1);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-0.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}


/*
 *  Draw B
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void B(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    //*****MIDDLE*****//
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+0.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+0.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+0.1,-1);
    
    //Bottem
    glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-0.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}


/////***** C *****/////

static void C(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    
    
    //*****LEFT*****//
    
    //Left
     glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}


////***** D *****////

static void D(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}


////***** E *****////

static void E(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, -1);
    
    
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    //*****MIDDLE*****//
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,+0.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+0.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+0.1,-1);
    
    //Bottem
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-0.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}


////***** F *****////

static void F(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, -1);
    
    
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    
    //*****MIDDLE*****//
    
    //Top
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+0.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+0.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+0.1,-1);
    
    //Bottem
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-0.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** G *****////

static void G(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+.1, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+.1, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    
    //Mid
    glTexCoord2f(0,0);glVertex3f(.5,.1,1);
    glTexCoord2f(1,0);glVertex3f(1,.1, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-.1, 1);
    glTexCoord2f(0,1);glVertex3f(.5,-.1, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
   glTexCoord2f(1,1); glVertex3f(+.8,+.1, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+.1, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    //Mid
    glTexCoord2f(0,0);glVertex3f(.5,.1,-1);
    glTexCoord2f(1,0);glVertex3f(1,.1, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-.1, -1);
    glTexCoord2f(0,1);glVertex3f(.5,-.1, -1);
    
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+.1,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+.1,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+.1,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+.1,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    
    //*****Mid*****//
    
    //Top
    glTexCoord2f(0,0);glVertex3f(.5,+.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+.1,-1);
    glTexCoord2f(0,1);glVertex3f(.5,+.1,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(.5,-.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,-.1,-1);
    glTexCoord2f(0,1);glVertex3f(.5,-.1,-1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}


////***** H *****////

static void H(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    
    //*****MIDDLE*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+0.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+0.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+0.1,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-0.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** I *****////

static void I(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Mid
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(.1,-2, 1);
    glTexCoord2f(1,1);glVertex3f(.1,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-.1,+2, 1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(.1,-2, -1);
    glTexCoord2f(1,1);glVertex3f(.1,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-.1,+2, -1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    
    
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(.1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(.1,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** J *****////

static void J(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(.1,-2, 1);
    glTexCoord2f(1,1);glVertex3f(.1,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-.1,+2, 1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+.1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+.1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(.1,-2, -1);
    glTexCoord2f(1,1);glVertex3f(.1,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-.1,+2, -1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+.1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+.1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    
    
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.1,+2,-1);
    
    //Right
      glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(.1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(.1,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+.1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.11,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+.1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+.1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
    
}

////***** K *****////

static void K(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    // Bottem Leg
    glTexCoord2f(0,0);glVertex3f(-1,0.1, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(.8,-2, 1);
    
    //  Top Leg
    glTexCoord2f(0,0);glVertex3f(+.8,2, 1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,-.1, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-.1, 1);
    
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    // Bottem Leg
    glTexCoord2f(0,0);glVertex3f(-1,0.1, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(.8,-2, 1);
    
    //  Top Leg
    glTexCoord2f(0,0);glVertex3f(+.8,2, -1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,-.1, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-.1, -1);
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****Bottem LEG*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+.1,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+.1,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+.1,-1);
    
    
    
    //*****Top Leg*****//
    
    //Top
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-.1,+1);
    glTexCoord2f(1,0);glVertex3f(-.8,-.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Bottem
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-.1,+1);
    glTexCoord2f(1,0);glVertex3f(-1,-.1,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** L *****////

static void L(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****BOTTOM*****//
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** M *****////

static void M(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-.25,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+.25,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+.25,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-.25,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
   glTexCoord2f(1,0); glVertex3f(-.25,2, 1);
    glTexCoord2f(1,1);glVertex3f(-.25,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    glTexCoord2f(0,0);glVertex3f(.25,2,1);
    glTexCoord2f(1,0);glVertex3f(1,2, 1);
    glTexCoord2f(1,1);glVertex3f(1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(.25,1.8, 1);
    
    // Mid Left
    glTexCoord2f(0,0);glVertex3f(-.45,2, 1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, 1);
    glTexCoord2f(1,1);glVertex3f(-.25,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-.45,-2, 1);
    
    //  Mid Right
    glTexCoord2f(0,0);glVertex3f(.25,2, 1);
    glTexCoord2f(1,0);glVertex3f(+.45,2, 1);
    glTexCoord2f(1,1);glVertex3f(+.45,-2, 1);
    glTexCoord2f(0,1);glVertex3f(+.25,-2, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-.25,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+.25,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+.25,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-.25,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, -1);
    glTexCoord2f(1,1);glVertex3f(-.25,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    glTexCoord2f(0,0);glVertex3f(.25,2,-1);
    glTexCoord2f(1,0);glVertex3f(1,2, -1);
    glTexCoord2f(1,1);glVertex3f(1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(.25,1.8, -1);
    
    // Mid Left
    glTexCoord2f(0,0);glVertex3f(-.45,2, -1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, -1);
    glTexCoord2f(1,1);glVertex3f(-.25,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-.45,-2, -1);
    
    //  Mid Right
    glTexCoord2f(0,0);glVertex3f(.25,2, -1);
    glTexCoord2f(1,0);glVertex3f(+.45,2, -1);
    glTexCoord2f(1,1);glVertex3f(+.45,-2, -1);
    glTexCoord2f(0,1);glVertex3f(+.25,-2, -1);
    
    
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(-.25,+2,+1);
    glTexCoord2f(1,1);glVertex3f(-.25,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    glTexCoord2f(0,0);glVertex3f(.25,+2,+1);
    glTexCoord2f(1,0);glVertex3f(1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(.25,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(-.25,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(-.25,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    glTexCoord2f(0,0);glVertex3f(.25,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(.25,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-.45,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+.45,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.45,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-.45,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-.25,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+.25,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+.25,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-.25,-1.8,+1);
    
    //*****MIDDLE*****//
    
    //MID RIGHT//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.45,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.45,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.45,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.45,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.25,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.25,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.25,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.25,+2,+1);
    
    
    //MID LEFT//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-.45,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.45,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.45,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.45,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.25,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.25,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.25,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.25,+2,-1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}


////***** N *****////

static void N(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-.25,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+.25,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+.25,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-.25,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, 1);
    glTexCoord2f(1,1);glVertex3f(-.25,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    // Mid Left
    glTexCoord2f(0,0);glVertex3f(-.45,2, 1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, 1);
    glTexCoord2f(1,1);glVertex3f(-.25,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-.45,-2, 1);
    
    //  Mid Right
    glTexCoord2f(0,0);glVertex3f(.25,2, 1);
    glTexCoord2f(1,0);glVertex3f(+.45,2, 1);
    glTexCoord2f(1,1);glVertex3f(+.45,-2, 1);
    glTexCoord2f(0,1);glVertex3f(+.25,-2, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-.25,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+.25,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+.25,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-.25,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, -1);
    glTexCoord2f(1,1);glVertex3f(-.25,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    // Mid Left
    glTexCoord2f(0,0);glVertex3f(-.45,2, -1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, -1);
    glTexCoord2f(1,1);glVertex3f(-.25,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-.45,-2, -1);
    
    //  Mid Right
    glTexCoord2f(0,0);glVertex3f(.25,2, -1);
    glTexCoord2f(1,0);glVertex3f(+.45,2, -1);
    glTexCoord2f(1,1);glVertex3f(+.45,-2, -1);
    glTexCoord2f(0,1);glVertex3f(+.25,-2, -1);
    
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(-.25,+2,+1);
    glTexCoord2f(1,1);glVertex3f(-.25,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);

    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(-.25,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(-.25,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-.45,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+.45,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.45,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-.45,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-.25,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+.25,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+.25,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-.25,-1.8,+1);
    
    //*****MIDDLE*****//
    
    //MID RIGHT//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.45,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.45,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.45,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.45,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.25,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.25,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.25,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.25,+2,+1);
    
    
    //MID LEFT//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-.45,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.45,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.45,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.45,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.25,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.25,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.25,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.25,+2,-1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** O *****////

static void O(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** P *****////

static void P(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-.1, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-.1, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-.1, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-.1, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-.1,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-.1,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    
    //*****MIDDLE*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+0.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+0.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+0.1,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-0.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** Q *****////

static void Q(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    // Mid
    glTexCoord2f(0,0);glVertex3f(-.1,-2.4, 1);
    glTexCoord2f(1,0);glVertex3f(.1,-2.4, 1);
    glTexCoord2f(1,1);glVertex3f(.1,-1.4, 1);
    glTexCoord2f(0,1);glVertex3f(-.1,-1.4, 1);
    
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    // Mid
    glTexCoord2f(0,0);glVertex3f(-.1,-2.4, -1);
    glTexCoord2f(1,0);glVertex3f(.1,-2.4, -1);
    glTexCoord2f(1,1);glVertex3f(.1,-1.4, -1);
    glTexCoord2f(0,1);glVertex3f(-.1,-1.4, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    
    //*****MID*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.1,-2.4,-1);
    glTexCoord2f(1,0);glVertex3f(-.1,-2.4,+1);
    glTexCoord2f(1,1);glVertex3f(-.1,-1.4,+1);
    glTexCoord2f(0,1);glVertex3f(-.1,-1.4,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.1,-2.4,-1);
    glTexCoord2f(1,0);glVertex3f(.1,-2.4,+1);
    glTexCoord2f(1,1);glVertex3f(.1,-1.4,+1);
    glTexCoord2f(0,1);glVertex3f(.1,-1.4,-1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** R *****////

static void R(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-.1, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-.1, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, 1);
    
    // Leg
    glTexCoord2f(0,0);glVertex3f(-1,0.1, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(.8,-2, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-.1, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-.1, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, -1);
    
    // Leg
    glTexCoord2f(0,0);glVertex3f(-1,0.1, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,0.1, -1);
    glTexCoord2f(1,1);glVertex3f(1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(.8,-2, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-.1,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-.1,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    
    //*****MIDDLE*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+0.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+0.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+0.1,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-0.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1,+1);
    
    
    //*****LEG*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+.1,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+.1,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+.1,-1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** S *****////

static void S(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-.1, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-.1, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+.1, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+.1, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-.1, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-.1, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+.1, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+.1, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    //Middle
    glTexCoord2f(0,0);glVertex3f(-1,.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,0.1, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+.1,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+.1,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+.1,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+.1,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-.1,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-.1,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-.1,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-.1,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    //*****MIDDLE*****//
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,+0.1,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+0.1,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+0.1,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+0.1,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-0.1,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-0.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-0.1,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-0.1,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** T *****////

static void T(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(.1,-2, 1);
    glTexCoord2f(1,1);glVertex3f(.1,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-.1,+2, 1);
    
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(.1,-2, -1);
    glTexCoord2f(1,1);glVertex3f(.1,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-.1,+2, -1);
    
    
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    
    
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(.1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(.1,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** U *****////

static void U(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    
    //*****BOTTOM*****//
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** V *****////

static void V(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,2, 1);
    glTexCoord2f(0,0);glVertex3f(-.8,2, 1);
    glTexCoord2f(0,0);glVertex3f(.1,-2, 1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+.8,2, 1);
    glTexCoord2f(0,0);glVertex3f(+1,2, 1);
    glTexCoord2f(0,0);glVertex3f(.1,-2, 1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, 1);
    
    
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,2, -1);
    glTexCoord2f(1,1);glVertex3f(.1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-.1,-2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+.8,2, -1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(.1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-.1,-2, -1);
    
    //*****RIGHT*****//
    
    //Top
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(.1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Bottem
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(-.1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    //
    //
    //    //*****TOP*****//
    //
    //    //Top
    //    glColor3f(0,1,1);
    //    glVertex3f(-1,+2,+1);
    //    glVertex3f(+1,+2,+1);
    //    glVertex3f(+1,+2,-1);
    //    glVertex3f(-1,+2,-1);
    //
    //    //Bottem
    //    glColor3f(0,1,1);
    //    glVertex3f(-1,+1.8,+1);
    //    glVertex3f(+1,+1.8,+1);
    //    glVertex3f(+1,+1.8,-1);
    //    glVertex3f(-1,+1.8,-1);
    //
    //    //*****BOTTOM*****//
    //
    //    //Bottem
    //    glColor3f(1,0,1);
    //    glVertex3f(-1,-2,-1);
    //    glVertex3f(+1,-2,-1);
    //    glVertex3f(+1,-2,+1);
    //    glVertex3f(-1,-2,+1);
    //
    //    //Top
    //    glColor3f(1,0,1);
    //    glVertex3f(-1,-1.8,-1);
    //    glVertex3f(+1,-1.8,-1);
    //    glVertex3f(+1,-1.8,+1);
    //    glVertex3f(-1,-1.8,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}


////***** W *****////

static void W(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glRotated(180,1,0,0);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, 1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, 1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-.25,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+.25,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+.25,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-.25,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0); glVertex3f(-.25,2, 1);
    glTexCoord2f(1,1); glVertex3f(-.25,+1.8, 1);
    glTexCoord2f(0,1);  glVertex3f(-1,1.8, 1);
   
    glTexCoord2f(0,0); glVertex3f(.25,2,1);
    glTexCoord2f(1,0);glVertex3f(1,2, 1);
    glTexCoord2f(1,1);glVertex3f(1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(.25,1.8, 1);
    
    // Mid Left
    glTexCoord2f(0,0);glVertex3f(-.45,2, 1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, 1);
    glTexCoord2f(1,1);glVertex3f(-.25,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-.45,-2, 1);
    
    //  Mid Right
    glTexCoord2f(0,0);glVertex3f(.25,2, 1);
    glTexCoord2f(1,0);glVertex3f(+.45,2, 1);
    glTexCoord2f(1,1);glVertex3f(+.45,-2, 1);
    glTexCoord2f(0,1);glVertex3f(+.25,-2, 1);
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,+2, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2, -1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2, -1);
    glTexCoord2f(0,1);glVertex3f(+1,+2, -1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-.25,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+.25,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+.25,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-.25,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, -1);
    glTexCoord2f(1,1);glVertex3f(-.25,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    glTexCoord2f(0,0);glVertex3f(.25,2,-1);
    glTexCoord2f(1,0);glVertex3f(1,2, -1);
    glTexCoord2f(1,1);glVertex3f(1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(.25,1.8, -1);
    
    // Mid Left
    glTexCoord2f(0,0);glVertex3f(-.45,2, -1);
    glTexCoord2f(1,0);glVertex3f(-.25,2, -1);
    glTexCoord2f(1,1);glVertex3f(-.25,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-.45,-2, -1);
    
    //  Mid Right
    glTexCoord2f(0,0);glVertex3f(.25,2, -1);
    glTexCoord2f(1,0);glVertex3f(+.45,2, -1);
    glTexCoord2f(1,1);glVertex3f(+.45,-2, -1);
    glTexCoord2f(0,1);glVertex3f(+.25,-2, -1);
    
    
    
    
    
    //*****RIGHT*****//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(-.25,+2,+1);
    glTexCoord2f(1,1);glVertex3f(-.25,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    glTexCoord2f(0,0);glVertex3f(.25,+2,+1);
    glTexCoord2f(1,0);glVertex3f(1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(.25,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(-.25,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(-.25,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    glTexCoord2f(0,0);glVertex3f(.25,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(.25,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-.45,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+.45,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.45,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-.45,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-.25,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+.25,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+.25,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-.25,-1.8,+1);
    
    //*****MIDDLE*****//
    
    //MID RIGHT//
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.45,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.45,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.45,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.45,+2,+1);
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(+.25,-2,+1);
    glTexCoord2f(1,0);glVertex3f(+.25,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.25,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.25,+2,+1);
    
    
    //MID LEFT//
    
    //Left
    glTexCoord2f(0,0);glVertex3f(-.45,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.45,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.45,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.45,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.25,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.25,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.25,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.25,+2,-1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** X *****////

static void X(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Left
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1,2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,2, 1);
    glTexCoord2f(1,1);glVertex3f(1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(.8,-2, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+.8,2, 1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(-.8,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    
    
    
    
    //*****BACK*****//
    
    // Left
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,2, -1);
    glTexCoord2f(1,1);glVertex3f(1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(.8,-2, -1);
    
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+.8,2, -1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(-.8,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //*****RIGHT*****//
    
    //Top
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.8,-2,+1);
    glTexCoord2f(1,0);glVertex3f(-.8,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Bottem
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-2,+1);
    glTexCoord2f(1,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.8,-2,-1);
    glTexCoord2f(1,0);glVertex3f(.8,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    //
    //
    //    //*****TOP*****//
    //
    //    //Top
    //    glColor3f(0,1,1);
    //    glVertex3f(-1,+2,+1);
    //    glVertex3f(+1,+2,+1);
    //    glVertex3f(+1,+2,-1);
    //    glVertex3f(-1,+2,-1);
    //
    //    //Bottem
    //    glColor3f(0,1,1);
    //    glVertex3f(-1,+1.8,+1);
    //    glVertex3f(+1,+1.8,+1);
    //    glVertex3f(+1,+1.8,-1);
    //    glVertex3f(-1,+1.8,-1);
    //
    //    //*****BOTTOM*****//
    //
    //    //Bottem
    //    glColor3f(1,0,1);
    //    glVertex3f(-1,-2,-1);
    //    glVertex3f(+1,-2,-1);
    //    glVertex3f(+1,-2,+1);
    //    glVertex3f(-1,-2,+1);
    //
    //    //Top
    //    glColor3f(1,0,1);
    //    glVertex3f(-1,-1.8,-1);
    //    glVertex3f(+1,-1.8,-1);
    //    glVertex3f(+1,-1.8,+1);
    //    glVertex3f(-1,-1.8,+1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** Y *****////

static void Y(double x,double y,double z, double dx,double dy,double dz, double th){
    
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    //*****FRONT*****//
    
    // Mid
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, 1);
    glTexCoord2f(1,0);glVertex3f(.1,-2, 1);
    glTexCoord2f(1,1);glVertex3f(.1,+.1, 1);
    glTexCoord2f(0,1);glVertex3f(-.1,+.1, 1);
    
    // Left
    glTexCoord2f(0,0);glVertex3f(-1,2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,2, 1);
    glTexCoord2f(1,1);glVertex3f(.1,-.1, 1);
    glTexCoord2f(0,1);glVertex3f(-.1,-.1, 1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+.8,2, 1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(.1,-.1, 1);
    glTexCoord2f(0,1);glVertex3f(-.1,-.1, 1);
    
    
    
    //*****BACK*****//
    
    //Mid
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-.1,-2, -1);
    glTexCoord2f(1,0);glVertex3f(.1,-2, -1);
    glTexCoord2f(1,1);glVertex3f(.1,+.1, -1);
    glTexCoord2f(0,1);glVertex3f(-.1,+.1, -1);
    
    // Left
    glTexCoord2f(0,0);glVertex3f(-1,2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,2, -1);
    glTexCoord2f(1,1);glVertex3f(.1,-.1, -1);
    glTexCoord2f(0,1);glVertex3f(-.1,-.1, -1);
    
    //  Right
    glTexCoord2f(0,0);glVertex3f(+.8,2, -1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(.1,-.1, -1);
    glTexCoord2f(0,1);glVertex3f(-.1,-.1, -1);
    
    
    
    //*****Mid*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(-.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.1,+.1,+1);
    glTexCoord2f(0,1);glVertex3f(-.1,+.1,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(.1,+.1,+1);
    glTexCoord2f(0,1);glVertex3f(.1,+.1,-1);
    
    
    //*****RIGHT*****//
    
    //Top
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.1,-.1,+1);
    glTexCoord2f(1,0);glVertex3f(.1,-.1,-1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+1,+2,+1);
    
    //Bottem
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.1,-.1,+1);
    glTexCoord2f(1,0);glVertex3f(-.1,-.1,-1);
    glTexCoord2f(1,1);glVertex3f(+.8,+2,-1);
    glTexCoord2f(0,1);glVertex3f(+.8,+2,+1);
    
    
    //*****LEFT*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(-.1,-.1,-1);
    glTexCoord2f(1,0);glVertex3f(-.1,-.1,+1);
    glTexCoord2f(1,1);glVertex3f(-1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.1,-.1,-1);
    glTexCoord2f(1,0);glVertex3f(.1,-.1,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

////***** Z *****////

static void Z(double x,double y,double z, double dx,double dy,double dz, double th){
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D,texture[texChange]);
    glRotated(180,0,1,0);
    glBegin(GL_QUADS);
    glColor3f(colOne,colTwo,colThree);//ADJUST COLOR HERE
    
    
    //*****FRONT*****//
    
    // Mid
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);glVertex3f(-1.1,2, 1);
    glTexCoord2f(1,0);glVertex3f(-.8,2, 1);
    glTexCoord2f(1,1);glVertex3f(1.1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(.8,-2, 1);
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, 1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, 1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, 1);
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,1);
    glTexCoord2f(1,0);glVertex3f(+1,2, 1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, 1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, 1);
    
    
    
    
    //*****BACK*****//
    
    
    //Bottem
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8, -1);
    glTexCoord2f(1,1);glVertex3f(+1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(-1,-2, -1);
    
    
    //Top
    glTexCoord2f(0,0);glVertex3f(-1,2,-1);
    glTexCoord2f(1,0);glVertex3f(+1,2, -1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8, -1);
    glTexCoord2f(0,1);glVertex3f(-1,1.8, -1);
    
    // Mid
    glTexCoord2f(0,0);glVertex3f(-1.1,2, -1);
    glTexCoord2f(1,0);glVertex3f(-.8,2, -1);
    glTexCoord2f(1,1);glVertex3f(1.1,-2, -1);
    glTexCoord2f(0,1);glVertex3f(.8,-2, -1);
    
    
    //*****TOP*****//
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1.1,+2,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+2,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+2,-1);
    glTexCoord2f(0,1);glVertex3f(-1.1,+2,-1);
    
    //Bottem
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,+1.8,+1);
    glTexCoord2f(1,0);glVertex3f(+1,+1.8,+1);
    glTexCoord2f(1,1);glVertex3f(+1,+1.8,-1);
    glTexCoord2f(0,1);glVertex3f(-1,+1.8,-1);
    
    //*****BOTTOM*****//
    
    //Bottem
    glTexCoord2f(0,0);glVertex3f(-1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(+1.1,-2,-1);
    glTexCoord2f(1,1);glVertex3f(+1.1,-2,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-2,+1);
    
    //Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);glVertex3f(-1,-1.8,-1);
    glTexCoord2f(1,0);glVertex3f(+1,-1.8,-1);
    glTexCoord2f(1,1);glVertex3f(+1,-1.8,+1);
    glTexCoord2f(0,1);glVertex3f(-1,-1.8,+1);
    
    //*****Mid*****//
    
    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(.9,-2,-1);
    glTexCoord2f(1,0);glVertex3f(.9,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-1.1,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-1.1,+2,-1);
    
    //Right
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);glVertex3f(1.1,-2,-1);
    glTexCoord2f(1,0);glVertex3f(1.1,-2,+1);
    glTexCoord2f(1,1);glVertex3f(-.8,+2,+1);
    glTexCoord2f(0,1);glVertex3f(-.8,+2,-1);
    
    
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}



//***** DRAW 3D TEXT *****//
void drawText() {
    
    
    //Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s",th,ph,dim,light?"On":"Off",ntex?"Replace":"Modulate");
    
    glMatrixMode(GL_MODELVIEW);
    
    glRotatef(1.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
    glScalef(_scale, _scale, _scale);
    glColor3f(colOne, colTwo, colThree);
    glEnable(GL_TEXTURE_2D);
    
    for(int i = 0; i < 1; i++) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texture[1]);
        glTranslatef(-.05*sizeof(userInput), 1, -1.1f / _scale);
        t3dDraw3D(STRS[i], -1, 2, 0.2f);//THIS TAKES IN STRINGS
        glPopMatrix();
    }
    
    //glutSwapBuffers();
}




 //***** GLUT CALLS THIS ROUTINE TO DISPLAY SCENE*****//
void display(){
    
   
    //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   
    //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
    
    glClearColor(.5,.5,.5,.5);
    
   
    //  Set perspective
   glLoadIdentity();
    if (fp) {
        cX = +2*dim*Sin(rot);
        cZ = -2*dim*Cos(rot);
        
        gluLookAt(eX,eY,eZ, cX+eX,eY,cZ+eZ, 0,1,0);
        
    }
    
    else if(mode)
    {
        double eX = -2*dim*Sin(th)*Cos(ph);
        double eY = +2*dim        *Sin(ph);
        double eZ = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(eX,eY,eZ , 0,0,0 , 0,Cos(ph),0);
    }
    
    
    //ORTHOGONAL
    else{
        //  Set view angle
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }
    
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      //float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
       float Position[]  = {-2,-2,2,1};
       Position[0] = 2;
       Position[1] = -4;
       Position[2] = 2;
       Position[3] = 1;
      //  Draw light position as ball (still no lighting here)
      //glColor3f(1,1,1);
      //ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
       
   }
   else
      glDisable(GL_LIGHTING);
    
    float setPos[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
    switch (p)
    {
        
            
        case 0:
            
            glRotatef(10,1,0,0);
            glRotatef(-55,0,1,0);
            texChange = 0;
            colOne = 1;
            colTwo = 1;
            colThree = 1;
            ambient = 30;
            drawText();
            glLightfv(GL_LIGHT0,GL_POSITION,setPos);
            glTranslatef(-0.05*sizeof(userInput), 0, 0);
            
    
            
            for(int i= 0; i < sizeof(userInput); i++){
                
                switch(userInput[i]){
                        
                    case 'A':
                        
                        //glColor3f(0,1,0);//adjust color here
                        A(i*0.75,0,0,.25,.25,.25,1);//adjust size and spacing here
                        break;
                        
                    case 'B':
                        B(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'C':
                        C(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'D':
                        D(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'E':
                        E(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'F':
                        F(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'G':
                        G(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'H':
                        H(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'I':
                        I(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'J':
                        J(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'K':
                        K(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'L':
                        L(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'M':
                        M(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'N':
                        N(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'O':
                        O(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'P':
                        P(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'Q':
                        Q(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'R':
                        R(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'S':
                        S(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'T':
                        T(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'U':
                        U(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'V':
                        V(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'W':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'X':
                        X(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'Y':
                        Y(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'Z':
                        Z(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'a':
                        //glColor3f(1,0,0);//adjust color here
                        A(i*0.75,0,0,.25,.25,.25,0);//adjust size and spacing here
                        break;
                    case 'b':
                        B(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'c':
                        C(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'd':
                        D(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'e':
                        E(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'f':
                        F(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'g':
                        G(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'h':
                        H(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'i':
                        I(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'j':
                        J(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'k':
                        K(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'l':
                        L(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'm':
                        M(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'n':
                        N(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'o':
                        O(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'p':
                        P(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'q':
                        Q(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'r':
                        R(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 's':
                        S(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 't':
                        T(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'u':
                        U(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'v':
                        V(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'w':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'x':
                        X(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'y':
                        Y(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'z':
                        Z(i*0.75,0,0,.25,.25,.25,0);
                        break;
                        
                }
                
            }
            
            //A(0,-3,0,1,1,1,0);
            
            
            break;
            
        case 1:
            
            glRotatef(10,1,0,0);
            glRotatef(-5,0,1,0);
            texChange = 1;
            colOne = 1;
            colTwo = 0;
            colThree = 0;
            drawText();
            setPos[0] = 2;
            setPos[1] = 2;
            setPos[2] = 2;
            setPos[3] = 1;
            glLightfv(GL_LIGHT0,GL_POSITION,setPos);
            ambient = 0;
            glTranslatef(-0.05*sizeof(userInput), 0, 0);
            
            for(int i= 0; i < sizeof(userInput); i++){
                
                switch(userInput[i]){
                        
                    case 'A':
                        //glColor3f(1,0,0);//adjust color here
                        A(i*0.75,0,0,.25,.25,.25,0);//adjust size and spacing here
                        break;
                        
                    case 'B':
                        B(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'C':
                        C(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'D':
                        D(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'E':
                        E(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'F':
                        F(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'G':
                        G(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'H':
                        H(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'I':
                        I(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'J':
                        J(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'K':
                        K(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'L':
                        L(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'M':
                        M(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'N':
                        N(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'O':
                        O(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'P':
                        P(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'Q':
                        Q(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'R':
                        R(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'S':
                        S(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'T':
                        T(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'U':
                        U(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'V':
                        V(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'W':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'X':
                        X(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'Y':
                        Y(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'Z':
                        Z(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'a':
                        //glColor3f(1,0,0);//adjust color here
                        A(i*0.75,0,0,.25,.25,.25,0);//adjust size and spacing here
                        break;
                    case 'b':
                        B(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'c':
                        C(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'd':
                        D(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'e':
                        E(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'f':
                        F(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'g':
                        G(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'h':
                        H(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'i':
                        I(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'j':
                        J(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'k':
                        K(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'l':
                        L(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'm':
                        M(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'n':
                        N(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'o':
                        O(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'p':
                        P(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'q':
                        Q(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'r':
                        R(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 's':
                        S(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 't':
                        T(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'u':
                        U(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'v':
                        V(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'w':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'x':
                        X(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'y':
                        Y(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'z':
                        Z(i*0.75,0,0,.25,.25,.25,0);
                        break;
                        
                }
                
            }

            
            break;
            
        case 2:
            
            glRotatef(10,1,0,0);
            glRotatef(-25,0,1,0);
            texChange = 2;
            colOne = 0;
            colTwo = 1;
            colThree = 0;
            drawText();
            setPos[0] = -2;
            setPos[1] = 4;
            setPos[2] = -2;
            setPos[3] = 1;
            glLightfv(GL_LIGHT0,GL_POSITION,setPos);
            ambient = 30;
            glTranslatef(-0.05*sizeof(userInput), 0, 0);
            
            for(int i= 0; i < sizeof(userInput); i++){
                
                switch(userInput[i]){
                        
                    case 'A':
                        //glColor3f(1,0,0);//adjust color here
                        A(i*1.1,0,0,.45,.45,.45,0);//adjust size and spacing here
                        break;
                        
                    case 'B':
                        B(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'C':
                        C(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'D':
                        D(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'E':
                        E(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'F':
                        F(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'G':
                        G(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'H':
                        H(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'I':
                        I(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'J':
                        J(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'K':
                        K(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'L':
                        L(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'M':
                        M(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'N':
                        N(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'O':
                        O(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'P':
                        P(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'Q':
                        Q(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'R':
                        R(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'S':
                        S(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'T':
                        T(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'U':
                        U(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'V':
                        V(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'W':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'X':
                        X(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'Y':
                        Y(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'Z':
                        Z(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'a':
                        //glColor3f(1,0,0);//adjust color here
                        A(i*1.1,0,0,.45,.45,.45,0);//adjust size and spacing here
                        break;
                    case 'b':
                        B(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'c':
                        C(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'd':
                        D(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'e':
                        E(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'f':
                        F(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'g':
                        G(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'h':
                        H(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'i':
                        I(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'j':
                        J(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'k':
                        K(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'l':
                        L(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'm':
                        M(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'n':
                        N(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'o':
                        O(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'p':
                        P(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'q':
                        Q(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'r':
                        R(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 's':
                        S(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 't':
                        T(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'u':
                        U(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'v':
                        V(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'w':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'x':
                        X(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'y':
                        Y(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'z':
                        Z(i*1.1,0,0,.45,.45,.45,0);
                        break;
                }
                
            }

            
            break;
            
        case 3:
            
            glRotatef(-30,1,0,0);
            glRotatef(-30,0,1,0);
            texChange = 3;
            colOne = .5;
            colTwo = .35;
            colThree = .05;
            drawText();
            setPos[0] = 2;
            setPos[1] = -4;
            setPos[2] = 2;
            setPos[3] = 1;
            glLightfv(GL_LIGHT0,GL_POSITION,setPos);
            glTranslatef(-0.05*sizeof(userInput), 0, 0);
            
            for(int i= 0; i < sizeof(userInput); i++){
                
                switch(userInput[i]){
                        
                    case 'A':
                        //glColor3f(1,0,0);//adjust color here
                        A(i*1.1,0,0,.45,.45,.45,0);//adjust size and spacing here
                        break;
                        
                    case 'B':
                        B(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'C':
                        C(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'D':
                        D(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'E':
                        E(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'F':
                        F(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'G':
                        G(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'H':
                        H(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'I':
                        I(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'J':
                        J(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'K':
                        K(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'L':
                        L(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'M':
                        M(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'N':
                        N(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'O':
                        O(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'P':
                        P(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'Q':
                        Q(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'R':
                        R(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'S':
                        S(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'T':
                        T(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'U':
                        U(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'V':
                        V(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'W':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'X':
                        X(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'Y':
                        Y(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'Z':
                        Z(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'a':
                        //glColor3f(1,0,0);//adjust color here
                        A(i*1.1,0,0,.45,.45,.45,0);//adjust size and spacing here
                        break;
                    case 'b':
                        B(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'c':
                        C(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'd':
                        D(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'e':
                        E(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'f':
                        F(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'g':
                        G(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'h':
                        H(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'i':
                        I(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'j':
                        J(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'k':
                        K(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'l':
                        L(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'm':
                        M(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'n':
                        N(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'o':
                        O(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'p':
                        P(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'q':
                        Q(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'r':
                        R(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 's':
                        S(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 't':
                        T(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'u':
                        U(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'v':
                        V(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'w':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'x':
                        X(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'y':
                        Y(i*1.1,0,0,.45,.45,.45,0);
                        break;
                    case 'z':
                        Z(i*1.1,0,0,.45,.45,.45,0);
                        break;
                        
                }
                
            }

            
            break;
            
        case 4:
            
            glRotatef(10,1,0,0);
            glRotatef(-55,0,1,0);
            texChange = 4;
            colOne = 1;
            colTwo = 1;
            colThree = 0;
            drawText();
            ambient = 75;
            setPos[0] = 0;
            setPos[1] = -4;
            setPos[2] = -2;
            setPos[3] = 1;
            glLightfv(GL_LIGHT0,GL_POSITION,setPos);
            glTranslatef(-0.05*sizeof(userInput), 0, 0);
            
            for(int i= 0; i < sizeof(userInput); i++){
                
                switch(userInput[i]){
                        
                    case 'A':
                        A(i*0.45,0,0,.15,.15,.15,0);//adjust size and spacing here
                        break;
                        
                    case 'B':
                        B(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'C':
                        C(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'D':
                        D(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'E':
                        E(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'F':
                        F(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'G':
                        G(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'H':
                        H(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'I':
                        I(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'J':
                        J(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'K':
                        K(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'L':
                        L(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'M':
                        M(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'N':
                        N(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'O':
                        O(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'P':
                        P(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'Q':
                        Q(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'R':
                        R(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'S':
                        S(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'T':
                        T(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'U':
                        U(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'V':
                        V(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'W':
                        W(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'X':
                        X(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'Y':
                        Y(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'Z':
                        Z(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'a':
                        A(i*0.45,0,0,.15,.15,.15,0);//adjust size and spacing here
                        break;
                        
                    case 'b':
                        B(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'c':
                        C(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'd':
                        D(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'e':
                        E(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'f':
                        F(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'g':
                        G(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'h':
                        H(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'i':
                        I(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'j':
                        J(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'k':
                        K(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'l':
                        L(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'm':
                        M(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'n':
                        N(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'o':
                        O(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'p':
                        P(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'q':
                        Q(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'r':
                        R(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 's':
                        S(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 't':
                        T(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'u':
                        U(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'v':
                        V(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'w':
                        W(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'x':
                        X(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'y':
                        Y(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'z':
                        Z(i*0.45,0,0,.15,.15,.15,0);
                        break;
                }
                
            }

            
            break;
           
        case 5:
            
            glRotatef(10,1,0,0);
            glRotatef(-5,0,1,0);
            texChange = 4;
            colOne = 1;
            colTwo = 0;
            colThree = 1;
            drawText();
            setPos[0] = 0;
            setPos[1] = 0;
            setPos[2] = 0;
            setPos[3] = 1;
            glLightfv(GL_LIGHT0,GL_POSITION,setPos);
            glTranslatef(-0.05*sizeof(userInput), 0, 0);
            
            for(int i= 0; i < sizeof(userInput); i++){
                
                switch(userInput[i]){
                        
                    case 'A':
                        A(i*0.45,0,0,.15,.15,.15,0);//adjust size and spacing here
                        break;
                        
                    case 'B':
                        B(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'C':
                        C(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'D':
                        D(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'E':
                        E(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'F':
                        F(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'G':
                        G(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'H':
                        H(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'I':
                        I(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'J':
                        J(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'K':
                        K(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'L':
                        L(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'M':
                        M(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'N':
                        N(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'O':
                        O(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'P':
                        P(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'Q':
                        Q(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'R':
                        R(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'S':
                        S(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'T':
                        T(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'U':
                        U(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'V':
                        V(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'W':
                        W(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'X':
                        X(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'Y':
                        Y(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'Z':
                        Z(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'a':
                        A(i*0.45,0,0,.15,.15,.15,0);//adjust size and spacing here
                        break;
                        
                    case 'b':
                        B(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'c':
                        C(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'd':
                        D(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'e':
                        E(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'f':
                        F(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'g':
                        G(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'h':
                        H(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'i':
                        I(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'j':
                        J(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'k':
                        K(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'l':
                        L(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'm':
                        M(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'n':
                        N(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'o':
                        O(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'p':
                        P(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'q':
                        Q(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'r':
                        R(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 's':
                        S(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 't':
                        T(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'u':
                        U(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'v':
                        V(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'w':
                        W(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'x':
                        X(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'y':
                        Y(i*0.45,0,0,.15,.15,.15,0);
                        break;
                    case 'z':
                        Z(i*0.45,0,0,.15,.15,.15,0);
                        break;
                        
                }
                
            }

            
            break;
           
        case 6:
            
            glRotatef(40,1,0,0);
            glRotatef(-55,0,1,0);
            texChange = 4;
            colOne = 0;
            colTwo = 1;
            colThree = 1;
            ambient = 40;
            drawText();
            setPos[0] = -2;
            setPos[1] = 2;
            setPos[2] = 0;
            setPos[3] = 1;
            glLightfv(GL_LIGHT0,GL_POSITION,setPos);
            glTranslatef(-0.05*sizeof(userInput), 0, 0);
            
            for(int i= 0; i < sizeof(userInput); i++){
                
                switch(userInput[i]){
                        
                    case 'A':
                        //glColor3f(1,0,0);//adjust color here
                        A(i*0.75,0,0,.25,.25,.25,0);//adjust size and spacing here
                        break;
                        
                    case 'B':
                        B(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'C':
                        C(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'D':
                        D(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'E':
                        E(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'F':
                        F(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'G':
                        G(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'H':
                        H(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'I':
                        I(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'J':
                        J(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'K':
                        K(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'L':
                        L(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'M':
                        M(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'N':
                        N(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'O':
                        O(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'P':
                        P(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'Q':
                        Q(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'R':
                        R(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'S':
                        S(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'T':
                        T(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'U':
                        U(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'V':
                        V(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'W':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'X':
                        X(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'Y':
                        Y(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'Z':
                        Z(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'a':
                        //glColor3f(1,0,0);//adjust color here
                        A(i*0.75,0,0,.25,.25,.25,0);//adjust size and spacing here
                        break;
                    case 'b':
                        B(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'c':
                        C(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'd':
                        D(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'e':
                        E(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'f':
                        F(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'g':
                        G(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'h':
                        H(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'i':
                        I(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'j':
                        J(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'k':
                        K(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'l':
                        L(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'm':
                        M(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'n':
                        N(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'o':
                        O(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'p':
                        P(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'q':
                        Q(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'r':
                        R(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 's':
                        S(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 't':
                        T(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'u':
                        U(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'v':
                        V(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'w':
                        W(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'x':
                        X(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'y':
                        Y(i*0.75,0,0,.25,.25,.25,0);
                        break;
                    case 'z':
                        Z(i*0.75,0,0,.25,.25,.25,0);
                        break;
                        
                }
                
            }

            
            break;
            

    }
    
   
   
 
   //  Display parameters
   glWindowPos2i(5,5);
   //Print("Angle=%d,%d  Dim=%.1f Light=%s PreSet=%s",th,ph,dim,light?"On":"Off",ntex?"Replace":"Modulate");
    Print("Angle=%d,%d  Dim=%.1f Light=%s PreSet=%d",th,ph,dim,light?"On":"Off",p+1);
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",int(ambient),int(diffuse),int(specular),int(emission),shiny);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}


//***** ALLOWS FOR REAL TIME MANIPULATION****//
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


 //GLUT CALLS THIS ROUTINVE WHEN AN ARROW KEY IS PRESSED*****//
void special(int key,int x,int y)
{
    if(!fp){
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
    Project(fov,asp,dim,fp,mode);
    
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
    }
}


 //***** GLUT CALLS THIS ROUTINE WHEN A KEY IS PRESSED*****//
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
    
    //  Toggle first person
   else if (ch == 'f' || ch == 'F'){
       fp = 1-fp;
   }
    
    if (fp) {
        double dt = 0.05;
        if (ch == 'w' || ch == 'W'){
            eX += cX*dt;
            eZ += cZ*dt;
        }
        else if (ch == 'a' || ch == 'A'){
            rot -= 3;
        }
        else if (ch == 's' || ch == 'S'){
            eX -= cX*dt;
            eZ -= cZ*dt;
        }
        else if (ch == 'd' || ch == 'D'){
            rot += 3;
        }
        //  Toggle lighting
        else if (ch == 'l' || ch == 'L')
            light = 1-light;
        
        rot %= 360;
    }
   
    
    else{
    //  Reset view angle
   if (ch == '0')
      th = ph = 0;
   //  Toggle Perspective mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
        //  Switch display mode
   else if (ch == 'p')
       p = (p+1)%7;
   else if (ch == 'P')
       p = (p+6)%7;
   
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle textures
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
    //  Change field of view angle
   else if (ch == '-' && ch>1)
       fov--;
   else if (ch == '+' && ch<179)
       fov++;
    }
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
    
   //  Reproject
   Project(fov,asp,dim,fp,mode);
   
    
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


//***** GLUT CALLS THIS ROUTINVE WHEN THE WINDOW IS RESIZED****//
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim,fp,mode);
   // Project();
}


 //***** START UP GLUT AND TELL IT WHAT TO DO*****//
int main(int argc,char* argv[]){
    
    cout << "Type Your Full Name: (letters only) " <<endl;
    cin.getline(userInput,sizeof(userInput));
    cout << "Type Your Class Number and Year: " <<endl;
    getline(cin,otherInput);
    STRS[0] = otherInput;
    
    
    
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(950,800);
   glutCreateWindow("3d Text Editor");
    
    // Set c++ Text
    t3dInit();
    _scale = computeScale(STRS);
   
    //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
    glutIdleFunc(idle);
   
    
    //  Load textures
   texture[0] = LoadTexBMP("tex1.bmp");
   texture[1] = LoadTexBMP("tex2.bmp");
   texture[2] = LoadTexBMP("tex3.bmp");
    texture[3] = LoadTexBMP("tex4.bmp");
    texture[4] = LoadTexBMP("tex5.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
