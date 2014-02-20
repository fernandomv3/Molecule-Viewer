#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
using namespace std;

char* substr(const char* source, int i, int n){
	char* substr = new char[n+1];
	int len = strlen(source);
	if(i > len) i= 0;
	strncpy(substr,source + i,n);
	substr[n]='\0';
	return substr;
}

int main (int argv, char** argc){
	ifstream pdbFile;
	char line [90];
	pdbFile.open("water.pdb");
	if (pdbFile.is_open()){
	    while (!pdbFile.eof()){
	      pdbFile.getline(line,81);
	      int len = strlen(line);
	      if(len == 80){
	      	char* recordName = substr(line,0,6);
	      	if(!strcmp(recordName,"ATOM  ") || !strcmp(recordName,"HETATM")){
	      		char* element;
	      		element = isdigit(line[12])? substr(line,13,1): substr(line,12,2);
	      		cout << "Symbol: " << element << '\n'; 
	      		char* x = substr(line,30,8);
	      		cout << "x: " << atof(x) << '\n';
	      		delete x;
	      		char* y = substr(line,38,8);
	      		cout << "y: " << atof(y) << '\n';
	      		delete y;
	      		char* z = substr(line,46,8);
	      		cout << "z: " << atof(z) << '\n';
	      		delete z;
	      	}
	      	cout << "----------------------------\n";
	      	delete recordName;
	      }
	    }
	    pdbFile.close();
  	}
  	return 0;
}