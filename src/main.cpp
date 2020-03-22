#define ERR cout << "ERROR: " <<
#define MAIN_CPP
#include "libraries.hpp"
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

char** removeSpace(std::string ogString) {
  return ogString.substr(std::remove_if(ogString.begin(), ogString.end(), ::isspace), ogString.end());
}

int main(int argc, char** argv) {
  std::string fileName = argv[1];
  if (boost::algorithm::ends_with(fileName, ".BUZZ") == false) {
    if (boost::algorithm::iends_with(fileName, ".BUZZ") == true) {
      fileName.erase(fileName.length() - 5, fileName.length());
    }
    fileName += ".BUZZ";
  }
  std::ifstream buzzFile(fileName.c_str());
  if (!buzzFile) {
    ERR "File " << "'" << fileName << "' does not exist!" << endl;
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
    if (interpret.substr(0, 4) == "BUZZ") {
      interpret.erase(0, 4);
      std::string varCheck = removeSpace(interpret);
      if (varCheck.at(0) == '(') {
        if (varCheck.find(',') != string::npos) {
          std::string functionArgs[];
          int varCheckFindI = 0;
          while (varCheck.find(',') != string::npos) {
            functionArgs[varCheckFindI] = varCheck.substr(varCheck.find(',') + 1, ;
            varCheckFindI += 1;
          }
          function uFunc(varCheck.substr(1, commaF - 1), functionArgs);
        }
        else {
          function uFunc(varCheck.substr(1, commaF - 1));
        }  
      }
    }
  }
  return 0;
}