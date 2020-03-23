#define ERR cout << "ERROR: " <<
#define WARN cout << "WARNING: " <<
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

char** removeCS(std::string ogString, char CS) {
  if (CS == 'C') {
    newOG = ogString;
    newOG.erase(newOG.find("?"));
    return newOG;
  }
  else if (CS == 'S') {
    std::string newOg = ogString;
    newOG.erase(std::remove_if(newOG.begin(), newOG.end(), ::isspace), newOG.end());
    return newOG;
  }
  else {
    WARN "Compilation error; char CS defined incorrectly in char** removeCS(std::string ogString, char CS)" << endl;
    return "";
  }
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
          ERR "Cannot declare hive type inside of function." << endl;
          return 0;
        }
      }
      else {
        ERR "Hive type already declared." << endl;
        return 0;
      }
    }
    if (interpret.substr(0, 3) == "BUZZ") {
      interpret.erase(0, 4);
      std::string varCheck = removeCS(removeCS(interpret, 'S'), 'C');
      if (varCheck.at(0) == '(') {
        if (varCheck.find(',') != string::npos) {
          std::string functionArgs[];
          int varCheckFindI = 0;
          while (varCheck.find(',') != string::npos) {
            secondVarCheckFind = std::distance(varCheck.substr(varCheck.find(',') + 1), boost::algorithm::string::find_nth(varCheck, ',', 2))
            if (secondVarCheckFind != 0) {
              functionArgs[varCheckFindI] = varCheck.substr(varCheck.find(',') + 1, secondVarCheckFind);
            }
            else {
              functionArgs[varCheckFindI] = varCheck.substr(varCheck.find(','), varCheck.find_last_of(")"))
            }
            varCheckFindI += 1;
          }
          function uFunc(varCheck.substr(1, commaF - 1), functionArgs);
        }
        else {
          function uFunc(varCheck.substr(1, commaF - 1));
        }
        if (varCheck.back() == '{') {
          inFunction = true
        }
        else if (varCheck.back() == '}') {
          uFunc.contents = interpret.substr(interpret.find('{') + 1, interpret.find('}') - 1);
        }
        else {
          WARN "Function declared without brackets." << endl;
        }  
      }
    }
  }
  return 0;
}