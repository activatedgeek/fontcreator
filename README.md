fontcreator
===========

//Things left: Minor fixes to multiples pushes to vector

A utility to create raster bits for fonts into openGL (Makefile for compile)

Certain external utilities used:
md5.h ( used from the internet )
timer.h (created in another repository named timer)
file_system.h (operations on the file, generating and creating directories)

Below are prototypes of my own defined functions in the above files:

bool SXresolveDir(string, dir_type);  //dir_type is an ENUM, resolves durectory from type and a hash string value

bool SXcreateDir(string, dir_type);  //creates directories

string md5(string); //generated md5 hash of a string
