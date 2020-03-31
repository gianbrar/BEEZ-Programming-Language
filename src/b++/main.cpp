#define ERR std::cout << "ERROR: " <<
#define WARN std::cout << "WARNING: " <<
#include "libraries.hpp"
#include "variables.hpp"
bool hive = false;
bool inFunction = false;
bool debugMode = false;
int combMax;
std::string interpret;
std::string hiveType;
std::vector<function> vFunc;
function uFunc;
function* pFunc;
char startingChar;
char endingChar;
int iStartingChar;
int currentFuncDef = -1;
int maxFuncDef;

int combUp() {
  ++currentFuncDef;
  if (currentFuncDef == maxFuncDef) {
    ERR "Maximum function definition reached!" << std::endl;
    exit(0);
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
    WARN "Compilation error; char CS defined incorrectly in std::string removeCS(std::string ogString, char CS)" << std::endl;
    return "0";
  }
  return newOG;
}

int charCheck() {
  if (iStartingChar % 2 == 0) {
    startingChar = '>';
    endingChar = '<';
  }
  else {
    startingChar = '<';
    endingChar = '>';
  }
  return 0;
}

int createHive() {
  hive = true;
  hiveType = interpret.substr(1, interpret.length() - 1);
  if (hiveType.substr(0, 3) != "CELL") {
    std::string CSHiveType = removeCS(removeCS(hiveType, 'S'), 'C');
    std::vector<int> hiveTypeContent;
    iStartingChar = 2;
    charCheck();
    while (hiveType.find(startingChar) != std::string::npos) {
      if (CSHiveType.find("<") == std::string::npos) {
        ERR "Cannot declare start token '" << startingChar << "' without ending token '" << endingChar << "'" << std::endl;
        return 0;
      }
      std::string tempHiveTypeContent = CSHiveType.substr(CSHiveType.find(">") + 1, CSHiveType.find("<") - 1);
      std::stringstream ssHiveTypeContent(tempHiveTypeContent);
      int iHiveTypeContent;
      ssHiveTypeContent >> iHiveTypeContent;
      hiveTypeContent.push_back(iHiveTypeContent);
      CSHiveType.erase(CSHiveType.find(startingChar));
      ++iStartingChar;
      charCheck();
    }
    if (hiveType.substr(0, 3) == "COMB") {
      if (hiveTypeContent.size() > 1) {
        ERR "More than one size provided despite being comb." << std::endl;
        return 0;
      }
      maxFuncDef = hiveTypeContent.at(0);
      pFunc = new function[maxFuncDef];
    }
  }
  if (debugMode == true) {
	  std::cout << "Detecting hiveType as " << hiveType.substr(0, 3) << std::endl;
  }
  return 0;
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
      ERR "No input files given." << std::endl;
      return 0;
  }
  std::string argv1;
  std::stringstream ssArgv1;
  ssArgv1 << argv[1];
  ssArgv1 >> argv1;
  if (argv1.at(0) == '-') {
    possibleCommand = true;
    if (fileName == "-m" || fileName == "--man") {
      std::cout << "Welcome to b++, an interpreter created for the BEEZ Programming Language.\nGENERAL STRUCTURE OF COMMAND:\nb++ {file name or optional command here} {secondary optional command here}\nOPTIONAL COMMANDS:\nb++ -m or b++ --man: Brings up this help page.\nb++ {file name} -d or b++ {file name} --debug: Runs file in debug mode. (for compiler maintainers)" << std::endl;
      return 0;
    }
    else if (fileName == "-d" || fileName == "--debug") {
      ERR "Debug mode cannot be activated without input file." << std::endl;
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
    ERR "File '" << fileName << "' does not exist!" << std::endl;
    if (possibleCommand == true) {
      std::cout << "Did you mean to type in a command?" << std::endl;
    }
    return 0;
  }
  if (buzzFile.peek() == std::ifstream::traits_type::eof()) {
    ERR "File is blank." << std::endl;
    return 0;
  }
  bool firstWhile = true;
  bool commentJoke = false;
  bool spaceJoke = false;
  int spaceI = 0;
  int commentI = 0;
  int currentIteration = 0;
  buzzFile.open(fileName.c_str());
  int lineCount = 0;
  std::string empty;
  while (getline(buzzFile, empty)) {
    lineCount++;
  }
  buzzFile.close();
  buzzFile.open(fileName.c_str());
  bool possibleJoke = false;
  bool commandRecognized = false;
  while (getline(buzzFile, interpret)) {
    if (debugMode == true && firstWhile == true) {
	    std::cout << "Began while loop" << std::endl;
    }
    else if (debugMode == true && firstWhile == false) {
	    ++currentIteration;
	    std::cout << "Ran while loop for the " << currentIteration << " time." << std::endl;
    }
    firstWhile = false;
    if (interpret == "") {
      continue;
    }
    std::string varCheck = removeCS(interpret, 'C');
    if (varCheck == "") {
      commentJoke = true;
      commentI += 1;
    }
    varCheck = removeCS(varCheck, 'S');
    if (varCheck == "") {
      if (commentJoke == false) {
        spaceJoke = true;
        spaceI += 1;
      }
      possibleJoke = true;
      continue;
    }
    if (varCheck.at(0) == '^') {
      commandRecognized = true;
      if (debugMode == true) {
	      std::cout << "Detecting hive type declaration." << std::endl;
      }
      if (hive == false) {
        if (inFunction == false) {
          createHive();
        }
        else {
          ERR "Cannot declare hive type inside of function." << std::endl;
          return 0;
        }
      }
      else {
        ERR "Hive type already declared." << std::endl;
        return 0;
      }
    }
    if (varCheck.substr(0, 3) == "BUZZ") {
      commandRecognized = true;
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
              functionArgs.push_back(varCheck.substr(varCheck.find(','), varCheck.find_last_of(')')));
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
            combUp();
            pFunc[currentFuncDef] = interpret.substr(interpret.find('{') + 1, interpret.find('}') - 1);
          }
        else {
          WARN "Function declared without curly brackets." << std::endl;
        }  
      }
    }
  }
  }
  buzzFile.close();
  std::string jokeTest;
  if (possibleJoke == true) {
    if (spaceI == lineCount) {
      ERR "File is just spaces.\nYou may be interested in the joke programming language, whitespace." << std::endl;
    }
    else if (commentI == lineCount || spaceI + commentI == lineCount) {
      ERR "File is just comments.\nCouldn't you have just used a .txt file?" << std::endl;
    }
    else {
      ERR "Unknown compilation error." << std::endl;
    }
  }
  return 0;
}
