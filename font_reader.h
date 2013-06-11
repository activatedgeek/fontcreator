/*********Class to read created fonts' data***********/

#include "file_system.h"
#include "md5.h"

//GL headers
#include<GL/gl.h>

#include<vector>
using namespace std;

class Font{
  public:
		string name;
		string fontFilePath;
		char charOfInterest;
		Font(string fontName, char input);
		bool getRowData(vector<GLubyte> *dataSet);
	private:
		
};
