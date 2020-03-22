#include <fstream>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#ifndef ERR
#define ERR cout << "ERROR: " <<
#endif
using std::cout;
using std::endl;
bool hive = false;
bool inFunction = false;
std::string interpret;
std::string hiveType;

int createHive() {
  hive = true;
  hiveType = interpret.substr(1, interpret.length() - 1);
  return 0;
}

int main() {
  std::string fileName;
  cout << "Please enter name of file." << endl;
  std::getline(std::cin, fileName);
  if (boost::algorithm::ends_with(fileName, ".BUZZ") == false) {
    if (boost::algorithm::iends_with(fileName, ".BUZZ") == true) {
      fileName.erase(fileName.length() - 5, fileName.length());
    }
    fileName += ".BUZZ";
  }
  std::ifstream buzzFile(fileName.c_str());
  if (!buzzFile) {
    ERR "File does not exist!" << endl;
    return 0;
  }
  while (getline(buzzFile, interpret)) {
    if (interpret.at(0) == '^') {
      if (hive == false) {
        if (inFunction == false) {
          createHive();
        }
        else {
          ERR "Cannot declare hive type inside of function!" << endl;
          return 0;
        }
      }
      else {
        ERR "Hive type already declared." << endl;
        return 0;
      }
    }
  }
  return 0;
}