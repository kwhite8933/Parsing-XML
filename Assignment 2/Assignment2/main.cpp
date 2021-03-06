/* 
 * File: main.cpp
 * Description: This file reads in an xml file and extracts the tags
 *              from the source code to list the number of occurrences
 *              of each one.
 * Author: Kyle White - kyle_white@student.uml.edu - kwhite8933@gmail.com
 * (c) UML 2015
 * Created on September 28, 2015, 6:51 PM
 * Updated on October 6, 2015, 8:47 PM
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "Element.h"

using namespace std;

/*
 * 
 */

Element* root; // Pointer to the root element of the XML document

vector<Element*> vecElementStack; // The stack of elements in the XML hierarchy.

int main(int argc, char** argv) {

  // creates a vector to be populated with xml elements and their line numbers
  vector<Element*> ElementFound;

  // accesses the file to be read
  ifstream infile("allTheWay.xml"); //gets the xml file
  string strLine; // string of the line we are currently on
  int nLineNo = 1; // current line number being read
  int nStartPos = 0; // position in the string of the first '<'
  int nEndPos = 0; // position in the string of the first '>'
  string strTag; // XML Element without the angle brackets
  string strTrimmedTag; // saves the element after it has been trimmed of white space

  // populates ElementFound vector with the entire xml file
  while (getline(infile, strLine) != NULL) {

    ElementFound.push_back(new Element(strLine, nLineNo));
    nLineNo++;

  }
  
  cout << "OUTPUT OF POPULATED ARRAY" << endl;
  for (vector<Element*>::iterator it = ElementFound.begin(); 
          it != ElementFound.end(); ++it) {

    //trims the string for easier manipulation
    strTrimmedTag = (*it)->GetStrElementTrimmed();
    
    // finds the opening angle bracket that starts the tag
    //nStartPos = (*it)->GetStrElement().find_first_of('<', 0);
    nStartPos = strTrimmedTag.find_first_of('<',0);
    
    // end position of the tag is initialized to be the first '>' in the string
    // if there are no closing angle brackets, then the end of the string is set to the first space for an attribute
    if( strTrimmedTag.find_first_of( ' ', 0 ) == string::npos ){
      
      nEndPos = strTrimmedTag.find_first_of( '>', 0 );
      
      // if there is both a space and an '>' on the same line, find whichever one comes first
      // this value becomes the end of the tag
    } else if( strTrimmedTag.find_first_of( ' ',0 ) > strTrimmedTag.find_first_of( '>',0 ) ){
      
      nEndPos = strTrimmedTag.find_first_of( '>',0 );
      
    } else {
      
      nEndPos = strTrimmedTag.find_first_of( ' ',0 );
      
    }
    
    // strips off angle brackets to just access the element itself
    strTag = strTrimmedTag.substr( nStartPos + 1, nEndPos-nStartPos-1 );
    
    // ignores the line/tag if it is a closing tag
    if (strTrimmedTag.at(nStartPos + 1) != ('/') ){
      
      // ignores the line/tag if it is a special case tag
      if (strTrimmedTag.at(nStartPos + 1) != ('?')) {
        
        // prints the tag out with an offset of 4 spaces from the right for easier reading
        cout << setw(4) << (*it)->GetNLineNo() << " : " << strTag << endl;

      }
    }
  }

  // closes the file being read from/used
  infile.close();

  return EXIT_SUCCESS;
}

