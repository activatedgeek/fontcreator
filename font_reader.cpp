#include "font_reader.h"

#include<fstream>
#include<sstream>

Font::Font(string fontName, char input){
  //Set Font Name
	name = fontName;
	
	//Set character of interest
	charOfInterest = input;
	
	//Set the path to the font file
	fontFilePath = SXresolveDir(md5(fontName), FONTS);
	fontFilePath += fontName + string(".txt");
}

bool Font::getRowData(vector<GLubyte> *dataSet){
	fstream dataFile(fontFilePath.c_str(), ios::in);
	if(!dataFile.good())
		return false;
	string tempRead;
	stringstream input;
	while(!dataFile.eof()){
		getline(dataFile, tempRead);
		if(tempRead[0]=='#' && tempRead[1] == charOfInterest){
			for(int i=0;i<96;i++){
				getline(dataFile, tempRead);
				input<<tempRead;
				while(input.peek() != EOF){
					int t;
					input>>t;
					(*dataSet).push_back(t);
				}			
			}
			break;
		}
	}
	
	dataFile.close();
}
