/* 
 * File:   Element.h
 * Author: Kyle
 *
 * Created on September 15, 2015, 6:45 PM
 */

#include <string>
#include <iostream>
#include <vector>

#ifndef ELEMENT_H
#define	ELEMENT_H

using namespace std;

class Element {
public:
    Element();
    Element(string str, int n);
    virtual ~Element();
    void SetStrElement(string strElement); // sets current tag
    string GetStrElement() const; // gets current tag
    string GetStrElementTrimmed(); // gets the current tag trimmed down
    void SetNLineNo(int nLineNo); // sets current line number
    int GetNLineNo() const; // gets current line number
    string trim( string& str );
    void SetStrTagName(string strTagName);
    string GetStrTagName() const;
private:
    string strElement; // current tag
    string strTagName; // name of element extracted from an element opening tag
    int nLineNo; // current line number
    vector<Element*> vecChildren; // vector of children
};

#endif	/* ELEMENT_H */

