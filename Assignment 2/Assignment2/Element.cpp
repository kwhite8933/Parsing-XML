/* 
 * File:   Element.cpp
 * Author: Kyle
 * 
 * Created on September 15, 2015, 6:45 PM
 */

#include "Element.h"

Element::Element() {
}

Element::Element(string str, int n) {

  strElement = str;
  nLineNo = n;
}

Element::~Element() {
}

void Element::SetStrElement(string strElement) {
  this->strElement = strElement;
}

string Element::GetStrElement() const {
  return strElement;
}

string Element::GetStrElementTrimmed(){
  return trim(strElement);
}

void Element::SetNLineNo(int nLineNo) {
  this->nLineNo = nLineNo;
}

int Element::GetNLineNo() const {
  return nLineNo;
}

/**
 * Trim leading and trailing white space (spaces and tabs) from the 
 *    string passed as an argument and return the trimmed string.
 * @param str string to trim
 * @return trimmed string
 */
string Element::trim(string& str) {

  // cout << "Trimming |" << str << "|" << endl ;  // for debugging
  while (str[0] == ' ' || str[0] == '\t') {
    str.erase(str.begin()); // must use an iterator
  }
  while (str[str.length() - 1] == ' ' || str[str.length() - 1] == '\t') {
    str.erase(str.end() - 1); // must use an iterator
  }
  return str;
}

void Element::SetStrTagName(string strTagName) {
    this->strTagName = strTagName;
}

string Element::GetStrTagName() const {
    return strTagName;
}
