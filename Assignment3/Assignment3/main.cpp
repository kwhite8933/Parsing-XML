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

enum ParserState {
    UNKNOWN, PROCESSING_LINE, STARTING_DOCUMENT, DIRECTIVE, ELEMENT_OPENING_TAG,
    ELEMENT_CONTENT, ELEMENT_NAME_AND_CONTENT, ELEMENT_CLOSING_TAG,
    SELF_CLOSING_TAG, STARTING_COMMENT, IN_COMMENT, ENDING_COMMENT, ONE_LINE_COMMENT,
    ERROR
};

/**
 * Code Taken From Jesse Heines
 * This function is used during debugging to display the parser state.
 * @param ps the parser state
 */
void ShowState(ParserState ps) {
    cout << "ParserState = ";
    switch (ps) {
        case UNKNOWN: cout << "UNKNOWN";
            break;
        case PROCESSING_LINE: cout << "PROCESSING_LINE";
            break;
        case STARTING_DOCUMENT: cout << "STARTING_DOCUMENT";
            break;
        case DIRECTIVE: cout << "DIRECTIVE";
            break;
        case ELEMENT_OPENING_TAG: cout << "ELEMENT_OPENING_TAG";
            break;
        case ELEMENT_CONTENT: cout << "ELEMENT_CONTENT";
            break;
        case ELEMENT_NAME_AND_CONTENT: cout << "ELEMENT_CONTENT";
            break;
        case ELEMENT_CLOSING_TAG: cout << "ELEMENT_CLOSING_TAG";
            break;
        case SELF_CLOSING_TAG: cout << "SELF_CLOSING_TAG";
            break;
        case STARTING_COMMENT: cout << "STARTING_COMMENT";
            break;
        case IN_COMMENT: cout << "IN_COMMENT";
            break;
        case ENDING_COMMENT: cout << "ENDING_COMMENT";
            break;
        case ONE_LINE_COMMENT: cout << "ONE_LINE_COMMENT";
            break;
        case ERROR: cout << "ERROR";
            break;
    }
    cout << endl;
}

ParserState GetXMLData(string strLine, int nLineNo, string strElementName, string& strContent,
    int nStartPos, int nEndPos, ParserState currentState) {
    
    // if no closing tag is found on the line, the line must be part of a multi-
    // line comment.  In this case, we must check if the multi-line comment has
    // just started or if it has already begun to be processed
    if (strLine.find(">") == string::npos) {

        // returns IN_COMMENT when the the parser already knows if the line being
        // processed is a comment
        if ((currentState == STARTING_COMMENT) || (currentState == IN_COMMENT)) {

            strContent = strLine;
            cout << strContent << endl;
            return IN_COMMENT;
        }
        
        // Parser returns STARTING_COMMENT when the opening tag of a line is found
        // and is followed by the '!--' denoting a comment
        if (strLine.find("!--")) {

            return STARTING_COMMENT;
        }
      // if there is no opening tag found on the line, the line must be a member
      // of a multi-line comment.  In which case, search for an ending comment tag
      // and return ENDING_COMMENT
    } else if ((strLine.find_first_of('<') == string::npos) && (strLine.find("--"))) {

        return ENDING_COMMENT;

    }
    
    // An XML directive must the first in an XML file and is wrapped in '?'
    // characters.  Search for these to find the XML directive and return
    // the DIRECTIVE state.
    else if ( (strLine.find_first_of('<') != string::npos ) && (strLine.find("<?") != string::npos) ){
        
        size_t start = strLine.find_first_of('?');
        size_t end = strLine.find_last_of('?');
        strContent = strLine.substr( start, start-end );
        return DIRECTIVE;
        
    }
    
    // if the opening tag on a line is actually a comment ('<!--'), check to see,
    // if there is a closing tag ('-->') on the same line.  If so, the current
    // line being processed is a one line comment so return ONE_LINE_COMMENT
    else if ( (strLine.find_first_of('<') != string::npos ) && (strLine.find("<!--") != string::npos) ){
        
        
        size_t start = strLine.find_first_of(' ');
        size_t end = strLine.find("-->");
        strContent = strLine.substr( start + 1, end-start-1 );
        return ONE_LINE_COMMENT;
        
    }
    
    // If there is an opening angle bracket and that angle bracket is a closing
    // tag, then return ELEMENT_CLOSING__TAG
    else if( (strLine.find_first_of('<') != string::npos ) && (strLine.find_first_of('/') == 
                                                              strLine.find_first_of('<') + 1) ){
        
    return ELEMENT_CLOSING_TAG;    
        
    }
    
    // Otherwise, an opening tag has been found and should be processed as such
    else
    {
        
        // Check if there is a closing tag on the same line and return ELEMENT_NAME_AND_CONTENT
        string strOpening = strElementName;
        if ( strLine.rfind("</" + strElementName) != string::npos )
        {
            
            size_t start = strLine.find_first_of('>');
            size_t end = strLine.find("</");
            strContent = strLine.substr( start + 1, end-start-1 );
            return ELEMENT_NAME_AND_CONTENT;
            
        }
        // if the opening and closing tag are not on the same line, check
        // to see a self-closing tag is on that line.  If so, return SELF_CLOSING_TAG
        else if ( (strLine.find_first_of('>') != string::npos ) && (strLine.find_first_of('>') ==
                                                                    strLine.find_first_of('/') + 1) )
        {
        
            strContent = "";
            return SELF_CLOSING_TAG;
        
        }
        // If none of these other cases are true but an opening tag is found,
        // the line contains just an opening tag so return ELEMENT_OPENING_TAG
        else
        {
        
            return ELEMENT_OPENING_TAG;
        
        }
    }
    
    return UNKNOWN;

}

int main(int argc, char** argv) {

    // creates a vector to be populated with xml elements and their line numbers
    vector<Element*> ElementFound;

    // accesses the file to be read
    ifstream infile("allTheWay.xml"); //gets the xml file
    string strLine; // string of the line we are currently on
    int nLineNo = 1; // current line number being read
    int nStartPos = 0; // position in the string of the first '<'
    int nEndPos = 0; // position in the string of the first '>'
    ParserState state = STARTING_DOCUMENT; // current state of the parser
    string strTag; // XML Element without the angle brackets
    string strTrimmedTag; // saves the element after it has been trimmed of white space
    string strContent = ""; // saves the content of each opening tag
    // populates ElementFound vector with the entire xml file
    while (getline(infile, strLine) != NULL) {

        ElementFound.push_back(new Element(strLine, nLineNo));
        nLineNo++;

    }

    ShowState(state);

    for (vector<Element*>::iterator it = ElementFound.begin();
        it != ElementFound.end(); ++it) {

        //trims the white space from the string for easier manipulation
        strTrimmedTag = (*it)->GetStrElementTrimmed();

        // finds the opening angle bracket that starts the tag
        //nStartPos = (*it)->GetStrElement().find_first_of('<', 0);
        nStartPos = strTrimmedTag.find_first_of('<', 0);

        // end position of the tag is initialized to be the first '>' in the string
        // if there are no closing angle brackets, then the end of the string is set to the first space for an attribute
        if (strTrimmedTag.find_first_of(' ', 0) == string::npos) {

            nEndPos = strTrimmedTag.find_first_of('>', 0);

            // if there is both a space and an '>' on the same line, find whichever one comes first
            // this value becomes the end of the tag
        } else if (strTrimmedTag.find_first_of(' ', 0) > strTrimmedTag.find_first_of('>', 0)) {

            nEndPos = strTrimmedTag.find_first_of('>', 0);

        } else {

            nEndPos = strTrimmedTag.find_first_of(' ', 0);
            strContent = strTrimmedTag.substr(nEndPos, strTrimmedTag.find_first_of('>') -
                strTrimmedTag.find_first_of(' '));

        }

        // strips off angle brackets to just access the element itself
        strTag = strTrimmedTag.substr(nStartPos + 1, nEndPos - nStartPos - 1);
        nLineNo = (*it)->GetNLineNo();
        state = GetXMLData((*it)->GetStrElement(), nLineNo, strTag, strContent,
            nStartPos, nEndPos, state);

        cout << nLineNo << " : ";
        ShowState(state);
        strContent = ""; // empties the string of content to allow for tags without
        // any attributes to print correctly

        // ignores the line/tag if it is a closing tag
        //        if ((strTrimmedTag.at(nStartPos + 1) != ('/')) &&
        //          (strTrimmedTag.at(nStartPos + 1) != ('?'))) {
        //


        //                // gets the current line number
        //                nLineNo = (*it)->GetNLineNo();
        //                // prints the tag out with an offset of 4 spaces from the right for easier reading
        //                cout << setw(4) << nLineNo << " : " << strTag << endl;
        //                
        //                // if there are attributes in the tag, print them
        //                if( strContent != "" ){
        //                    cout << "******" << strContent << endl << endl;
        //                    
        //                    // empties the string containing any attributes for the next
        //                    // check to be made
        //                    strContent = "";
        //    }
        //        }
    }

    // closes the file being read from/used
    infile.close();

    return EXIT_SUCCESS;
}