#include<iostream>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

//Defines the density of horizontal and vertical lines
#define WIDTH_STEP 5
#define HEIGHT_STEP 5
#define CURR_STATUS "Currently editing character:   \'"

//Parameters for frame location & size
#define X_OFFSET 303 
#define Y_OFFSET 612
#define FRAME_SIZE 480

using namespace std;

string additions="";

//For combined functionality with arrow keys: true = leftmousebutton
bool buttonType;

//Location pointers
int xLoc, yLoc;

void printString(const char *str, int x, int y)
{
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); 
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);          
    glRasterPos2i(x, y);

    // loop all characters in the string
    while(*str)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
        ++str;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}

void init(void){
glClearColor (0.0, 0.0, 0.0, 0.0);
}

//Set the drawing frame boundaries
void frameSet(){
	glColor3ub(255, 0, 255);
	glPointSize(3.0f);
glBegin(GL_POINTS);
	for(int i=X_OFFSET; i-X_OFFSET<=FRAME_SIZE ; i+=WIDTH_STEP)
		glVertex2f(i, Y_OFFSET);
	for(int i=Y_OFFSET; Y_OFFSET-i<=FRAME_SIZE ; i-=HEIGHT_STEP)
		glVertex2f(X_OFFSET, i);
	for(int i=X_OFFSET; i-X_OFFSET<=FRAME_SIZE ; i+=WIDTH_STEP)
		glVertex2f(i, Y_OFFSET-FRAME_SIZE);
	for(int i=Y_OFFSET; Y_OFFSET-i<=FRAME_SIZE ; i-=HEIGHT_STEP)
		glVertex2f(X_OFFSET+FRAME_SIZE, i);
glEnd();

//Swap buffer necessary to display the new pixels
glutSwapBuffers();
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

//Post redisplay is necessary
void keyboard(unsigned char key, int x, int y){
additions.clear();
additions = key;

if (key == 27){
	exit(0);
}	
else if(key == 32){
	
}

glutPostRedisplay();
}

//In the arrow keys, it works without 
void processModifiers(int key, int x, int y){
	if(key == GLUT_KEY_F5){
		glClear(GL_COLOR_BUFFER_BIT);
		additions="";
		glutPostRedisplay();
	}
	else if(key == 'r' || key == 'R'){
		int mod = glutGetModifiers();
		if(mod == GLUT_ACTIVE_CTRL)
			glClear(GL_COLOR_BUFFER_BIT);
		additions="";
		glutPostRedisplay();
	}

	if(key == GLUT_KEY_LEFT && xLoc-WIDTH_STEP!=X_OFFSET){
		xLoc -= WIDTH_STEP;
		if(buttonType == true)
			colorPoint(GLUT_LEFT_BUTTON);
		else
			colorPoint(GLUT_RIGHT_BUTTON);
	}
	
	else if(key == GLUT_KEY_UP && yLoc+HEIGHT_STEP!=Y_OFFSET){
		yLoc += HEIGHT_STEP;
		if(buttonType == true)
			colorPoint(GLUT_LEFT_BUTTON);
		else
			colorPoint(GLUT_RIGHT_BUTTON);
	}
	
	else if(key == GLUT_KEY_RIGHT && xLoc+WIDTH_STEP!=X_OFFSET+FRAME_SIZE){
		xLoc += WIDTH_STEP;
		if(buttonType == true)
			colorPoint(GLUT_LEFT_BUTTON);
		else
			colorPoint(GLUT_RIGHT_BUTTON);
	}
	
	else if(key == GLUT_KEY_DOWN && yLoc-HEIGHT_STEP!=Y_OFFSET-FRAME_SIZE){
		yLoc -= HEIGHT_STEP;
		if(buttonType == true)
			colorPoint(GLUT_LEFT_BUTTON);
		else
			colorPoint(GLUT_RIGHT_BUTTON);
	}

}
void mouse(int button, int state, int x , int y){
	
//Calculations a little hit and trial, need to work out more until better solution
int actX = WIDTH_STEP*((x - x%WIDTH_STEP)/WIDTH_STEP)+WIDTH_STEP-2;
int actY = HEIGHT_STEP*((glutGet(GLUT_WINDOW_HEIGHT)-y - (glutGet(GLUT_WINDOW_HEIGHT)-y)%HEIGHT_STEP)/HEIGHT_STEP)+HEIGHT_STEP-3;

if(actX != X_OFFSET && actY != Y_OFFSET){
	if(button==GLUT_LEFT_BUTTON){
		buttonType =true;
		if(state==GLUT_DOWN){
		xLoc = actX;
		yLoc = actY;
		colorPoint(button);
		}
	}
	else if(button==GLUT_RIGHT_BUTTON){
		buttonType =false;
		if(state==GLUT_DOWN){
		xLoc = actX;
		yLoc = actY;
		colorPoint(button);
		}
    }
}
		
}

//Main display on Context
void display(void){
glClear(GL_COLOR_BUFFER_BIT);
glEnable(GL_BLEND); 
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glEnable(GL_LINE_SMOOTH);
glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  

baseGrid();
frameSet();

glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();                  
		glLoadIdentity();            
		gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
		string finalDisp = CURR_STATUS + additions +string("\'");
		printString(finalDisp.c_str(), glutGet(GLUT_WINDOW_WIDTH)-325, glutGet(GLUT_WINDOW_HEIGHT) - 30);
	glPopMatrix();
glPopMatrix();

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
int actX = WIDTH_STEP*((x - x%WIDTH_STEP)/WIDTH_STEP)+WIDTH_STEP-2;
int actY = HEIGHT_STEP*((glutGet(GLUT_WINDOW_HEIGHT)-y - (glutGet(GLUT_WINDOW_HEIGHT)-y)%HEIGHT_STEP)/HEIGHT_STEP)+HEIGHT_STEP-3;

cout<<actX<<" "<<actY<<endl;
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
glutKeyboardFunc(keyboard);
glutSpecialFunc(processModifiers);	
glutMouseFunc(mouse);
glutPassiveMotionFunc(passive);
glutMainLoop();

return 0;
}
