//Custom Headers
#include "timer.h"
#include "file_system.h"
#include "md5.h"

//C++ Standard headers
#include<iostream>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<cmath> 
#include<cstdio>
using namespace std;

//OpenGL and Utility Headers
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>


//Defines the density of horizontal and vertical lines
#define WIDTH_STEP 5
#define HEIGHT_STEP 5
#define CURR_STATUS "Currently editing character:   \'"
#define X_OFFSET 303 
#define Y_OFFSET 612
#define FRAME_SIZE 485


/***********************Active parameters*****************/
Timer timer;
string fontName;
string currChar="[]";
char currentEdit;
fstream currFile;
string currFilePath;
//Current editing raster
vector<vector<int> > currRaster;
//For combined functionality with arrow keys: true = leftmousebutton
bool buttonType;

//Location pointers
int xLoc, yLoc;
/***********************Active parameters*****************/

//Dropping points off a vector row (on right mouse clicks)
bool findAndErase(vector<int> *array, int num){
	
if(!(*array).empty()){
	
	for(int i=0; i<(*array).size();i++){
		if((*array)[i] == num){
			(*array).erase((*array).begin() + i);
			return true;
		}
	}
}

return false;
}

/************Operations for saving/editing file data***************/
//Get the bitstream for each row
string getBitString(vector<int> rowPoints){
string rowBits="";

for(int i = 1; i<FRAME_SIZE/WIDTH_STEP ; i++){
	rowBits+="0";
}

if(rowPoints.empty())
	return rowBits;

for(int i=0;i<rowPoints.size();i++){
	rowBits.replace((rowPoints[i]-X_OFFSET)/WIDTH_STEP - 1, 1, "1");
}

return rowBits;
}

//Get an 8-bit binary into integer
int decimal8Bit(string input){
	if(input.length()!=8)
		return 0;
	
	double num=0;
	for(int i=0;i<8;i++){
		if(input[i]!='0')
			num += pow(2, 7-i);
	}
	return (int)num;
}

//Reset the data in the file for current character
bool resetCharInFile(){
	currFile.open(currFilePath.c_str(), ios::in);
	if(!currFile.good())
		return false;
		
	string temp;
	fstream tempFile("temp~", ios::out | ios::app);
	while(!currFile.eof()){
		getline(currFile, temp);
		if(temp[0] == '#' && temp[1] == currentEdit){
			for(int i=0;i<FRAME_SIZE/WIDTH_STEP-1;i++)
				getline(currFile, temp);
		}
		else if(temp!=""){
			tempFile<<temp<<endl;
		}
	}
	//tempFile<<endl;
	currFile.close();
	tempFile.close();
	remove(currFilePath.c_str());
	rename("temp~",currFilePath.c_str());
	return true;
}

//Store the raster data into file
bool resolveBitstoFile(){
	resetCharInFile();
	
	currFile.open(currFilePath.c_str(), ios::out | ios::app);
	if(!currFile.good())
		return false;
	string bitRow;
	currFile<<"#"<<currentEdit<<endl;
	for(int j=0;j< FRAME_SIZE/HEIGHT_STEP-1;j++){
		bitRow = getBitString(currRaster[j]);
		for(int i =0; i < bitRow.length()/8 ; i++)
			currFile<<decimal8Bit(bitRow.substr(8*i,8))<<" ";
				
		currFile<<endl;
	}
	//currFile<<endl;
	currFile.close();
	currRaster.clear();
	for(int i=1; i< FRAME_SIZE/HEIGHT_STEP ; i++)
			currRaster.push_back(vector<int>());

	return true;
}

/*****************Operations for saving/editing file***************/

//Print string onto the screen
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
	for(int i=X_OFFSET; i-X_OFFSET<=FRAME_SIZE; i+=WIDTH_STEP)
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
	glColor3f(0.0f, 0.0f, 1.0f);
glBegin(GL_POINTS);
	glVertex2f(xLoc, yLoc);
glEnd();

glutSwapBuffers();	

timer.start();
	while(1){
		if(timer.getElapsedTime(MILLISEC) == 50){
			timer.stop();
			break;
		}
	}
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

/******************User Interface Functions*******************/
//Post redisplay is necessary
void keyboard(unsigned char key, int x, int y){
currChar = key;
currentEdit = key;

if (key == 27){
	exit(0);
}

glutPostRedisplay();
}


void processModifiers(int key, int x, int y){
	if(key == GLUT_KEY_F5){
		//Reset the screen data for the same character
		currRaster.clear();
		for(int i=1; i< FRAME_SIZE/HEIGHT_STEP ; i++)
			currRaster.push_back(vector<int>());
		glClear(GL_COLOR_BUFFER_BIT);
		glutPostRedisplay();
	}
	
	//Save the raster parameters and clear the raster vector array
	else if( key == GLUT_KEY_END){
		//Save parameters into the file and reset active parameters
		resolveBitstoFile();
		currChar = "[]";
		glClear(GL_COLOR_BUFFER_BIT);
		glutPostRedisplay();
	}
	
	//Reset the whole program
	else if(key == 'r' || key == 'R'){
		int mod = glutGetModifiers();
		if(mod == GLUT_ACTIVE_CTRL)
			glClear(GL_COLOR_BUFFER_BIT);
		currChar="[]";
		currRaster.clear();
		for(int i=1; i< FRAME_SIZE/HEIGHT_STEP ; i++)
			currRaster.push_back(vector<int>());
		glutPostRedisplay();
	}

	//Start movements from old position using arrow keys
	if(key == GLUT_KEY_LEFT && xLoc-WIDTH_STEP!=X_OFFSET){
		xLoc -= WIDTH_STEP;
		int row = (yLoc - Y_OFFSET)/HEIGHT_STEP + FRAME_SIZE/HEIGHT_STEP - 1;
		if(buttonType == true){
			currRaster[row].push_back(xLoc);
			colorPoint(GLUT_LEFT_BUTTON);
		}
		else{
			findAndErase(&currRaster[row], xLoc);
			colorPoint(GLUT_RIGHT_BUTTON);
		}
	}
	
	else if(key == GLUT_KEY_UP && yLoc+HEIGHT_STEP!=Y_OFFSET){
		yLoc += HEIGHT_STEP;
		int row = (yLoc - Y_OFFSET)/HEIGHT_STEP + FRAME_SIZE/HEIGHT_STEP - 1;
		if(buttonType == true){
			currRaster[row].push_back(xLoc);
			colorPoint(GLUT_LEFT_BUTTON);
		}
		else{
			findAndErase(&currRaster[row], xLoc);
			colorPoint(GLUT_RIGHT_BUTTON);
		}
	}
	
	else if(key == GLUT_KEY_RIGHT && xLoc+WIDTH_STEP!=X_OFFSET+FRAME_SIZE){
		xLoc += WIDTH_STEP;
		int row = (yLoc - Y_OFFSET)/HEIGHT_STEP + FRAME_SIZE/HEIGHT_STEP - 1;
		if(buttonType == true){
			currRaster[row].push_back(xLoc);
			colorPoint(GLUT_LEFT_BUTTON);
		}
		else{
			findAndErase(&currRaster[row], xLoc);
			colorPoint(GLUT_RIGHT_BUTTON);
		}
	}
	
	else if(key == GLUT_KEY_DOWN && yLoc-HEIGHT_STEP!=Y_OFFSET-FRAME_SIZE){
		yLoc -= HEIGHT_STEP;
		int row = (yLoc - Y_OFFSET)/HEIGHT_STEP + FRAME_SIZE/HEIGHT_STEP - 1;
		if(buttonType == true){
			currRaster[row].push_back(xLoc);
			colorPoint(GLUT_LEFT_BUTTON);
		}
		else{
			findAndErase(&currRaster[row], xLoc);
			colorPoint(GLUT_RIGHT_BUTTON);
		}
	}

}

void mouse(int button, int state, int x , int y){
	
//Calculations a little hit and trial, need to work out more until better solution
int actX = WIDTH_STEP*((x - x%WIDTH_STEP)/WIDTH_STEP)+WIDTH_STEP-2;
int actY = HEIGHT_STEP*((glutGet(GLUT_WINDOW_HEIGHT)-y - (glutGet(GLUT_WINDOW_HEIGHT)-y)%HEIGHT_STEP)/HEIGHT_STEP)+HEIGHT_STEP-3;

if(actX > X_OFFSET && actX < X_OFFSET + FRAME_SIZE && actY < Y_OFFSET && actY > Y_OFFSET - FRAME_SIZE){
	if(button==GLUT_LEFT_BUTTON){
		buttonType =true;
		if(state==GLUT_DOWN){
		xLoc = actX;
		yLoc = actY;
		colorPoint(button);
		int row = (actY - Y_OFFSET)/HEIGHT_STEP + FRAME_SIZE/HEIGHT_STEP - 1;
		currRaster[row].push_back(xLoc);
		}
	}
	else if(button==GLUT_RIGHT_BUTTON){
		buttonType =false;
		if(state==GLUT_DOWN){
		xLoc = actX;
		yLoc = actY;
		colorPoint(button);
		int row = (actY - Y_OFFSET)/HEIGHT_STEP + FRAME_SIZE/HEIGHT_STEP - 1;
		findAndErase(&currRaster[row], xLoc);
		}
    }
}
		
}
/******************User Interface Functions*******************/

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
		string finalDisp = CURR_STATUS + currChar +string("\'");
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
//int actX = WIDTH_STEP*((x - x%WIDTH_STEP)/WIDTH_STEP)+WIDTH_STEP-2;
//int actY = HEIGHT_STEP*((glutGet(GLUT_WINDOW_HEIGHT)-y - (glutGet(GLUT_WINDOW_HEIGHT)-y)%HEIGHT_STEP)/HEIGHT_STEP)+HEIGHT_STEP-3;
//int row = (actY - Y_OFFSET)/HEIGHT_STEP + FRAME_SIZE/HEIGHT_STEP - 1;
//cout<<actX<<" "<<actY<<" row: "<<row<<endl;
}

int main(int arg, char **args){
cout<<"Enter the name of the font: ";
cin>>fontName;
currFilePath = SXresolveDir(md5(fontName), FONTS);
SXcreateDir(currFilePath, FONTS);
currFilePath += fontName + string(".txt");

for(int i=1; i< FRAME_SIZE/HEIGHT_STEP ; i++)
	currRaster.push_back(vector<int>());

glutInit(&arg, args);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
glDisable(GL_DEPTH_TEST);
glutInitWindowPosition(0,0);
glutInitWindowSize(1197,693);
glutCreateWindow("Font Creator");
glutSetCursor(GLUT_CURSOR_SPRAY);
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
