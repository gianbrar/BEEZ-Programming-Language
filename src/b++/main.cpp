#define ERR std::cout << "ERROR: " <<
#define WARN std::cout << "WARNING: " <<
#define ENDL << std::endl
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

int debug(std::string msg) {
  if (debugMode == true) {
    std::cout << msg ENDL;
  }
  return 0;
}

int combUp() {
  ++currentFuncDef;
  if (currentFuncDef == maxFuncDef) {
    ERR "Maximum function definition exceeded!" ENDL;
    exit(0);
  }
  return 0;
}

std::string removeCS(std::string ogString, char CS) {
  debug("Inside std::string removeCS");
  std::string newOG;
  if (CS == 'C') {
    debug("Removing all comments.");
    newOG = ogString;
    if (newOG.find("?") != std::string::npos) {
	    newOG.erase(newOG.find("?"));
    }
  }
  else if (CS == 'S') {
    debug("Removing all spaces.");
    newOG = ogString;
    newOG.erase(std::remove_if(newOG.begin(), newOG.end(), ::isspace), newOG.end());
  }
  else {
    WARN "Compilation error; char CS defined incorrectly in std::string removeCS(std::string ogString, char CS)" ENDL;
    return "0";
  }
   debug("Returning newOG and exiting to main.");
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
    while (CSHiveType.find(startingChar) != std::string::npos) {
      if (CSHiveType.find(endingChar) == std::string::npos) {
        ERR "Cannot declare start token '" << startingChar << "' without ending token '" << endingChar << "'" ENDL;
        exit(0);
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
    if (hiveType == "COMB") {
      if (debugMode == true) {
      	std::cout << "Entered case for COMB." ENDL;
      }
      if (hiveTypeContent.size() > 1) {
        ERR "More than one size provided despite being comb." ENDL;
        return 0;
      }
      else if (hiveTypeContent.size() < 1) {
        WARN "No arguments provided for COMB; defaulting to CELL." ENDL;
        hiveType = "CELL";
        return 0;
      }
      if (debugMode == true) {
	      std::cout << "Correct size provided." ENDL;
      }
      maxFuncDef = hiveTypeContent.front();
      pFunc = new function[maxFuncDef];
      if (debugMode == true) {
	      std::cout << "Dynamic array pFunc declared with maximum size of " << maxFuncDef ENDL;
    }
  }
  if (debugMode == true) {
	  std::cout << "Detecting hiveType as " << hiveType << "\nSize of hiveTypeContent is " << hiveTypeContent.size() << "\nValues in hiveTypeContent are:" ENDL;
    for (int i = 0; i < hiveTypeContent.size(); i++) {
      std::cout << hiveTypeContent.at(i);
      if (i != hiveTypeContent.size() - 1) {
        std::cout << ", ";
      }
      else {
        std::cout ENDL;
      }
    }
  }
  hiveType = CSHiveType.substr(0, CSHiveType.find(">") - 1);
  if (debugMode == true) {
    std::cout << "Final hiveType is " << hiveType ENDL;
  }
  }
  return 0;
}

int main(int argc, char** argv) {
  std::string fileName;
  std::string secondary;
  bool connectionStart = false;
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
      ERR "No input files given." ENDL;
      return 0;
  }
  std::string argv1;
  std::stringstream ssArgv1;
  ssArgv1 << argv[1];
  ssArgv1 >> argv1;
  if (argv1.at(0) == '-') {
    possibleCommand = true;
    if (fileName == "-m" || fileName == "--man") {
      std::cout << "Welcome to b++, an interpreter created for the BEEZ Programming Language.\nGENERAL STRUCTURE OF COMMAND:\nb++ {file name or optional command here} {secondary optional command here}\nOPTIONAL COMMANDS:\nb++ -m or b++ --man: Brings up this help page.\nb++ {file name} -d or b++ {file name} --debug: Runs file in debug mode. (for compiler maintainers)" ENDL;
      return 0;
    }
    else if (fileName == "-d" || fileName == "--debug") {
      ERR "Debug mode cannot be activated without input file." ENDL;
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
    ERR "File '" << fileName << "' does not exist!" ENDL;
    if (possibleCommand == true) {
      std::cout << "Did you mean to type in a command?" ENDL;
    }
    return 0;
  }
  if (buzzFile.peek() == std::ifstream::traits_type::eof()) {
    ERR "File is blank." ENDL;
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
    if (debugMode == true) {
    	if (firstWhile == true) {
	      std::cout << "Began while loop" ENDL;
    	}
    	else if (firstWhile == false) {
	      ++currentIteration;
	      std::cout << "Ran while loop for the " << currentIteration << " time." ENDL;
    	}
	    std::cout << "Reading line as " << interpret ENDL;
    }
    firstWhile = false;
    if (interpret == "") {
       if (debugMode == true) {
	       std::cout << "Skipping current loop" ENDL;
       }
       continue;
    }
    std::string varCheck = removeCS(interpret, 'C');
    if (debugMode == true) {
	    std::cout << "Successfully removed all comments." ENDL;
    }
    if (varCheck == "") {
      if (debugMode == true) {
      	std::cout << "Possible comment joke trigger detected." ENDL;
      }
      commentJoke = true;
      commentI += 1;
    }
    varCheck = removeCS(varCheck, 'S');
    if (debugMode == true) {
	    std::cout << "Successfully removed all spsaces." ENDL;
    }
    if (varCheck == "") {
      if (commentJoke == false) {
	      if (debugMode == true) {
	        std::cout << "Possible space joke trigger detected." ENDL;
	      }
        spaceJoke = true;
        spaceI += 1;
      }
      possibleJoke = true;
      continue;
    }
    if (varCheck.at(0) == '^') {
      commandRecognized = true;
      if (debugMode == true) {
	      std::cout << "Detecting hive type declaration." ENDL;
      }
      if (hive == false) {
        if (inFunction == false) {
          createHive();
        }
        else {
          ERR "Cannot declare hive type inside of function." ENDL;
          return 0;
        }
      }
      else {
        ERR "Hive type already declared." ENDL;
        return 0;
      }
    }
    if (varCheck.substr(0, 3) == "BUZZ") {
      commandRecognized = true;
      varCheck.erase(0, 3);
      if (varCheck.find("(") != std::string::npos) {
        std::string funcName = interpret.substr(0, varCheck.find("("));
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
          	function uFunc(funcName, functionArgs);
	        }
          else if (hiveType == "COMB") {
            combUp();
            pFunc[currentFuncDef].name = funcName;
            for (int i = 0; i < functionArgs.size(); i++) {
              pFunc[currentFuncDef].args.push_back(functionArgs.at(i));
            }
          }
	      }
        else {
          int commaF = varCheck.find(',');
	        if (hiveType == "CELL") {
          	function uFunc(funcName);
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
          WARN "Function declared without curly brackets." ENDL;
        }  
      }
      continue;
    }
  }
  if (interpret.find("(") != std::string::npos) {
    if (interpret.substr(interpret.find("("), interpret.find("(") + 1) != ")") {
      if (connectionStart == true) {
        ERR "Cannot define connection inside connection. You just smashed two bees into each other. You monster." ENDL;
        return 0;
      }
      connectionStart = true;
      continue;
    }
  }
  else {
    ERR "No closing paranthese found!" ENDL;
  }
  }
  buzzFile.close();
  std::string jokeTest;
  if (possibleJoke == true) {
    if (spaceI == lineCount) {
      ERR "File is just spaces.\nYou may be interested in the completely serious programming language, whitespace." ENDL;
    }
    else if (commentI == lineCount || spaceI + commentI == lineCount) {
      ERR "File is just comments.\nCouldn't you have just used a .txt file?" ENDL;
    }
    else {
      ERR "Unknown compilation error." ENDL;
    }
  }
  return 0;
}
