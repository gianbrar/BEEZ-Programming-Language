#define ERR cout << "ERROR: " <<
#define WARN cout << "WARNING: " << 
#include "libraries.hpp"
#include "function.hpp"
using std::cout;
using std::endl;
bool hive = false;
bool inFunction = false;
bool debugMode = false;
std::string interpret;
std::string hiveType;
std::vector<function> uFuncs;
function uFunc;

int createHive() {
  hive = true;
  hiveType = interpret.substr(1, interpret.length() - 1);
  if (hiveType.substr(0, 3) == "COMB") {
  }
  if (debugMode == true) {
	  cout << "Detecting hiveType as " << hiveType << endl;
  }
  return 0;
}

std::string removeCS(std::string ogString, char CS) {
  std::string newOG;
  if (CS == 'C') {
    newOG = ogString;
    newOG.erase(newOG.find("?"));
  }
  else if (CS == 'S') {
    newOG = ogString;
    newOG.erase(std::remove_if(newOG.begin(), newOG.end(), ::isspace), newOG.end());
  }
  else {
    WARN "Compilation error; char CS defined incorrectly in std::string removeCS(std::string ogString, char CS)" << endl;
    return "0";
  }
  return newOG;
}

int main(int argc, char** argv) {
  std::string fileName;
  std::string secondary;
  bool possibleCommand = false;
  if (argc > 1) {
      fileName = argv[1];
      if (argc > 2) {
        secondary = argv[2];
        if (secondary == "-d" || secondary == "--debug") {
          debugMode = true;
        }
      }
    }
  else {
      ERR "No input files given." << endl;
      return 0;
  }
  std::string argv1;
  std::stringstream ssArgv1;
  ssArgv1 << argv[1];
  ssArgv1 >> argv1;
  if (argv1.at(0) == '-') {
    possibleCommand = true;
    if (fileName == "-m" || fileName == "--man") {
      cout << "Welcome to b++, an interpreter created for the BEEZ Programming Language.\nGENERAL STRUCTURE OF COMMAND:\nb++ {file name or optional command here} {secondary optional command here}\nOPTIONAL COMMANDS:\nb++ -m or b++ --man: Brings up this help page.\nb++ {file name} -d or b++ {file name} --debug: Runs file in debug mode. (for compiler maintainers)" << endl;
      return 0;
    }
  }
  if (boost::algorithm::ends_with(fileName, ".BUZZ") == false) {
    if (boost::algorithm::iends_with(fileName, ".BUZZ") == true) {
      fileName.erase(fileName.length() - 5, fileName.length());
    }
    fileName += ".BUZZ";
  }
  std::ifstream buzzFile(fileName.c_str());
  if (!buzzFile) {
    ERR "File '" << fileName << "' does not exist!" << endl;
    if (possibleCommand == true) {
      cout << "Did you mean to type in a command?" << endl;
    }
    return 0;
  }
  if (buzzFile.peek() == std::ifstream::traits_type::eof()) {
    ERR "File is blank." << endl;
    return 0;
  }
  bool firstWhile = true;
  int currentIteration = 0;
  buzzFile.open(fileName.c_str());
  while (getline(buzzFile, interpret)) {
    if (debugMode == true && firstWhile == true) {
	    cout << "Began while loop" << endl;
    }
    else if (debugMode == true && firstWhile == false) {
	    ++currentIteration;
	    cout << "Ran while loop for the " << currentIteration << " time." << endl;
    }
    std::string varCheck = removeCS(interpret, 'S');
    if (varCheck == "" && firstWhile == true) {
      ERR "Input file is literally just spaces.\nYou might be interested in another joke programming language: whitespace." << endl;
      return 0;
    }
    varCheck = removeCS(varCheck, 'C');
    if (varCheck == "" && firstWhile == true) {
      ERR "Input file is literally just comments.\nWhat are you thinking? Just write something down in a txt file, jesus..." << endl;
      return 0;
    }
    firstWhile = false;
    if (varCheck.at(0) == '^') {
      if (debugMode == true) {
	      cout << "Detecting hive type declaration." << endl;
      }
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
    if (varCheck.substr(0, 3) == "BUZZ") {
      varCheck.erase(0, 3);
      if (varCheck.at(0) == '(') {
        if (varCheck.find(',') != std::string::npos) {
          int secondVarCheckFind;
          int varCheckFindI = 1;
          std::vector<std::string> functionArgs;
          while (varCheck.find(',') != std::string::npos) {
            varCheckFindI++;
            boost::iterator_range<std::string::iterator> varCheckFindMultiComma = boost::find_nth(varCheck, ",", varCheckFindI);
            char charVarCheckGoTo = varCheck.at(varCheck.find(','));
            std::stringstream ssVarCheckGoTo;
            ssVarCheckGoTo << charVarCheckGoTo;
            std::string varCheckGoTo;
            ssVarCheckGoTo >> varCheckGoTo;
            secondVarCheckFind = std::distance(varCheckGoTo.begin(), varCheckFindMultiComma.begin());
            if (secondVarCheckFind != 0) {
              functionArgs.push_back(varCheck.substr(varCheck.find(',') + 1, secondVarCheckFind));
            }
            else {
              functionArgs.push_back(varCheck.substr(varCheck.find(','), varCheck.find_last_of(")")));
            }
          }
          int commaF = varCheck.find(',');
	  if (hiveType == "CELL") {
          	function uFunc(varCheck.substr(1, commaF - 1), functionArgs);
	  }
	}
        else {
          int commaF = varCheck.find(',');
	  if (hiveType == "CELL") {
          	function uFunc(varCheck.substr(1, commaF - 1));
	  }
        }
        if (varCheck.back() == '{') {
          inFunction = true;
        }
        else if (varCheck.back() == '}') {
	  if (hiveType == "CELL") {
          	uFunc.contents = interpret.substr(interpret.find('{') + 1, interpret.find('}') - 1);
          }
	  else if (hiveType == "COMB") {

	}
        else {
          WARN "Function declared without curly brackets." << endl;
        }  
      }
    }
  }
  buzzFile.close();
  return 0;
}
