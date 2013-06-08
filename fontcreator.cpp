#include<iostream>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

//Defines the density of horizontal and vertical lines
#define WIDTH_STEP 5
#define HEIGHT_STEP 5

using namespace std;

//Location pointers
int xLoc, yLoc;

//Checks the state of operation, first click done or not, first click done then true
bool start = false, carry=false;

void init(void){
glClearColor (0.0, 0.0, 0.0, 0.0);
}

//Draw a point
void colorPoint(int button){

glPointSize(3.0f);

if(button == GLUT_LEFT_BUTTON){
  glColor3ub(255, 0, 0);
glBegin(GL_POINTS);
	glVertex2f(xLoc, yLoc);
glEnd();
}
else if(button == GLUT_RIGHT_BUTTON){
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex2f(xLoc, yLoc);
glEnd();
}
//Swap buffer necessary to display the new pixels
glutSwapBuffers();	

}

//Make the base grid
void baseGrid(){
glColor3ub(23, 45, 68);

glLineWidth(3.0f);
glBegin(GL_LINES);
	for(int j=0;j<= glutGet(GLUT_WINDOW_HEIGHT); j+=HEIGHT_STEP){
		glVertex2f(0,j);
		glVertex2f(glutGet(GLUT_WINDOW_WIDTH),j);
	}
glEnd();	

glBegin(GL_LINES);
	for(int j=0;j<= glutGet(GLUT_WINDOW_WIDTH); j+=WIDTH_STEP){
		glVertex2f(j,0);
		glVertex2f(j,glutGet(GLUT_WINDOW_WIDTH));
	}
glEnd();

}

/*
void draw(){
glEnable(GL_BLEND); 
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glEnable(GL_LINE_SMOOTH);
glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 

glColor3f(1.0f, 0.0f, 1.0f);	
glLineWidth(2.0f);
glBegin(GL_LINES);
glVertex2f(xLoc1, yLoc1);
glVertex2f(xLoc2, yLoc2);
glEnd();

glutSwapBuffers();

}
*/

void key(unsigned char key, int x, int y){

if (key == 27){
	exit(0);
}	

//Reset the screen by clearing the buffer bit
if(key == 'r' || key == 'R'){
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

}

void mouse(int button, int state, int x , int y){
	
//Calculations a little hit and trial, need to work out more until better solution
int actX = WIDTH_STEP*((x - x%WIDTH_STEP)/WIDTH_STEP)+WIDTH_STEP-2;
int actY = HEIGHT_STEP*((glutGet(GLUT_WINDOW_HEIGHT)-y - (glutGet(GLUT_WINDOW_HEIGHT)-y)%HEIGHT_STEP)/HEIGHT_STEP)+HEIGHT_STEP-3;

	if(button==GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON){
		if(state==GLUT_DOWN){
		xLoc = actX;
		yLoc = actY;
		colorPoint(button);
	}
	}
		
}

void display(void){
glClear(GL_COLOR_BUFFER_BIT);
glEnable(GL_BLEND); 
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glEnable(GL_LINE_SMOOTH);
glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  

baseGrid();

glutSwapBuffers();
}

void reshape(int w, int h){
glViewport(0,0,(GLsizei)w,(GLsizei)h);
glMatrixMode (GL_PROJECTION);
glLoadIdentity();
glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT), 0.0, 60.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}

void passive(int x, int y){

}

int main(int arg, char **args){
glutInit(&arg, args);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
glDisable(GL_DEPTH_TEST);
glutInitWindowPosition(0,0);
glutInitWindowSize(1197,693);
glutCreateWindow("snap");
init();

glutDisplayFunc(display);
glutReshapeFunc(reshape);
glutKeyboardFunc(key);
glutMouseFunc(mouse);
glutPassiveMotionFunc(passive);
glutMainLoop();
return 0;
}
