/*
Takes a text file containing log entries and parses them.
Theoretically, these entries could then be inserted into an SQLite database using its C/C++ interface
However, I've had trouble getting the interface to work, so instead it just outputs the text to standard out.
*/

#include "sqlite3.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

void removeSpaces(istream &source){
  // removes whitespace from the stream
	char c;
	char p = source.peek();
	while (isspace(p)){
		source.get(c);
		p = source.peek();
	}
}

string getToken(istream &source){
	// get the first "token" (the priority level)
  char c;
  string token = "";
  source.get(c);
  token.push_back(c);

	char p = source.peek();
  while(!isspace(p)){
    source.get(c);
    token.push_back(c);
    p = source.peek();
  }
  return token;
}

string getString(istream &source){
  // Returns the entire quoted string
  char c;
  string inQuotes = "";
  source.get(c);
  inQuotes.push_back(c);
  while (c != '\"'){
    source.get(c);
    inQuotes.push_back(c);
  }
  return inQuotes;
}

void parseEntry(istream &source){
  // Parses the next entry in the log file, according to the format [priority level] || [text of log]


  // Priority level is first word of the line, grab it and store
  removeSpaces(source);
  string priority = getToken(source);

  // Parse the rest of the string until a newline is found
  char c;
  char p;
  string text;

  removeSpaces(source);
  p = source.peek();
  while (p != '\n'){
    // If double quotes is encountered before newline, parse until other quote is found and paste to end of text
    if (p == '\"'){
      source.get(c);
      text.push_back(c);
      string t = getString(source);
      text = text + t;
      p = source.peek();
    }
    else{
      source.get(c);
      text.push_back(c);
      p = source.peek();
    }
  }
  cout << priority << "  ||  " << text << endl;
}

void parseUntilDone(istream &source){
  while(source.good()){
    parseEntry(source);
  }
}

int main (int argc, char *argv[]){
  ifstream fin;
  fin.open("event_logs.txt");
  parseUntilDone(fin);
  fin.close();
  return 0;
}
