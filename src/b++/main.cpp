#define ERR std::cout << "ERROR: " <<
#define WARN std::cout << "WARNING: " <<
#define ENDL << std::endl
#include "libraries.hpp"
#include "variables.hpp"
bool hive = false;
bool inFunction = false;
bool debugMode = false;
int combMax;
std::string fValue;
std::string hiveType;
std::vector<function> vFunc;
function uFunc;
function* pFunc;
char startingChar;
char endingChar;
int iStartingChar;
int currentFuncDef = -1;
int maxFuncDef;
int maxVarDef;
bool possibleJoke = false;
bool commandRecognized = false;
std::vector<std::string> stripes;
bool firstWingDefined = false;
bool firstWhile = true;
bool commentJoke = false;
bool spaceJoke = false;
int spaceI = 0;
int commentI = 0;
int currentIteration = 0;
int lineCount = 0;
std::string empty;
std::string firstWingContent;
int stripeCount = 0;
std::string fileName;
std::string secondary;
std::string secondWingContent;
bool connectionStart = false;
bool bodyDefined = false;
int currentVarDef = 0;
std::string tempVariable;
bool possibleCommand = false;
std::vector<variable> varList;
variable* pVar;
variable var;

int debug(std::string msg) {
  if (debugMode == true) {
    std::cout << msg ENDL;
  }
  return 0;
}



int combUp(std::string type) {
  if (type == "function") {
    ++currentFuncDef;
    if (currentFuncDef == maxFuncDef) {
      ERR "Maximum function definition exceeded!" ENDL;
      exit(0);
    }
  }
  else if (type == "var") {
    ++currentVarDef;
    if (currentVarDef == maxVarDef) {
      ERR "Maximum variable definition exceeded!" ENDL;
      exit(0);
    }
  }
  return 0;
}

std::string removeCS(std::string ogString, char CS) {
  debug("Inside std::string removeCS");
  std::string newOG;
  if (CS == 'C') {
    debug("Removing all comments.");
    newOG = ogString;
    if (newOG.find("??") != std::string::npos) {
      if (newOG.at(newOG.find("??") - 1) != '\\') {
	      newOG.erase(newOG.find("??"));
      }
      else {
        newOG.erase(newOG.find("??") - 1, 1);
      }
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
  hiveType = fValue.substr(1, fValue.length() - 1);
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
      maxVarDef = maxFuncDef;
      pFunc = new function[maxFuncDef];
      pVar = new variable[maxVarDef];
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

int process(std::string interpret) {
  interpret = removeCS(interpret, 'C');
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
       return 0;
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
	  debug("Successfully removed all spaces.");
    if (varCheck == "") {
      if (commentJoke == false) {
	      debug("Possible space joke trigger detected.");
        spaceJoke = true;
        spaceI += 1;
      }
      possibleJoke = true;
      return 0;
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
      return 0;
    }
    bool fileWrite = false;
    if (interpret.find("BUZZ@") != std::string::npos) {
      debug("Detecting BUZZ@");
      interpret = interpret.substr(interpret.find("BUZZ@") + 5, interpret.length() - 1);
      if (interpret.substr(0, 7) == "console") {
        debug("Sending message to console");
        interpret.erase(0, 8);
        varCheck.erase(0, 12);
      }
      if (varCheck.at(0) == '"') {
          debug("Found string");
          int firstQuotation = interpret.find('"');
          std::string secondQuotation = interpret.substr(interpret.find('"') + 1, interpret.length() - 1);
          interpret = interpret.substr(firstQuotation + 1, secondQuotation.find('"'));
          while (interpret.find("</") != std::string::npos && interpret.substr(interpret.find("</"), interpret.find("</") - 1) != "\\") {
            tempVariable = interpret.substr(interpret.find("</") + 2, interpret.length() - 1);
            debug("Temp variable is " + tempVariable);
            tempVariable = tempVariable.substr(0, tempVariable.find(" "));
            debug("Temp variable changed to " + tempVariable);
            bool replaceSuccess = false;
            if (tempVariable.find("\\") != std::string::npos) {
              tempVariable.erase(tempVariable.find("\\"), 2);
              interpret.erase(interpret.find("\\"), 1);
            }
            if (hiveType == "NEST") {
              for (int i = 0; i < varList.size(); i++) {
                if (varList.at(i).name == tempVariable) {
                  debug("Matching variable is " + varList.at(i).name);
                  interpret.replace(interpret.find("</"), tempVariable.length() + 2, varList.at(i).name);
                  replaceSuccess = true;
                }
              }
            }
            else if (hiveType == "COMB") {
              for (int i = 0; i < sizeof(pVar); i++) {
                if (pVar[i].name == tempVariable) {
                  debug("Matching variable is " + pVar[i].name);
                  replaceSuccess = true;
                  interpret.replace(interpret.find("</"), tempVariable.length() + 2, pVar[i].contents);
                }
              }
            }
            else if (hiveType == "CELL") {
              debug("var.contents is " + var.contents);
              debug("var.name is " + var.name);
              if (var.name == tempVariable) {
                debug("Matching variable is " + var.contents);
                replaceSuccess = true;
                interpret.replace(interpret.find("</"), var.name.length() + 2, var.contents);
              }
              else {
                debug("Unsuccessful cell attempt in string variable reference");
              }
            }
            if (replaceSuccess == false) {
              WARN "Ignoring instance of 'Unknown variable mentioned.'" ENDL;
              std::cout << interpret << std::endl;
              return 0;
            }
            else {
              debug("Replace successful with " + interpret + "!");
            }
          }
      }
      if (fileWrite == false) {
        std::cout << interpret << std::endl;
      }
      return 0;
    }
    if (varCheck.substr(0, 4) == "BUZZ" && interpret.find("BUZZ") != std::string::npos) {
      debug("Detecting variable or function declaration.");
      commandRecognized = true;
      varCheck.erase(0, 4);
      interpret.erase(0, 4);
      if (varCheck.find("(") != std::string::npos) {
        std::string funcName = interpret.substr(0, varCheck.find("(") - 1);
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
          	function nFunc(funcName, functionArgs);
            uFunc.name = nFunc.name;
            uFunc.contents = nFunc.contents;
	        }
          else if (hiveType == "COMB") {
            combUp("function");
            pFunc[currentFuncDef].name = funcName;
            for (int i = 0; i < functionArgs.size(); i++) {
              pFunc[currentFuncDef].args.push_back(functionArgs.at(i));
            }
          }
	      }
        else {
          int commaF = varCheck.find(',');
	        if (hiveType == "CELL") {
          	  uFunc.name = funcName;
	          }
          else if (hiveType == "COMB") {
            combUp("function");
            pFunc[currentFuncDef].name = funcName;
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
            combUp("function");
            pFunc[currentFuncDef].contents = interpret.substr(interpret.find('{') + 1, interpret.find('}') - 1);
          }
        else if (interpret.find('{') == std::string::npos) {
          WARN "Function declared without curly brackets." ENDL;
        }
        else {
          ERR "Unknown error." ENDL;
        }  
      }
    }
    else {
      debug("Detecting variable declaration.");
      std::string varName = varCheck.substr(0, varCheck.find("="));
      if (varName == "STR:") {
        ERR "Variable name cannot be data type!" ENDL;
        return 0;
      }
      varCheck = varCheck.substr(varCheck.find("=") + 1, varCheck.length() - 1);
      if (varCheck.substr(0, 4) == "STR:" && interpret.find("STR:") != std::string::npos) {
        debug("Detecting string declaration.");
        interpret = interpret.substr(interpret.find("\"") + 1, std::distance(interpret.begin(), boost::find_nth(interpret, "\"", 2).begin()));
        interpret.erase(interpret.length() - 1);
        if (hiveType == "CELL") {
          var.name = varName;
          var.contents = interpret;
        }
        else if (hiveType == "COMB") {
          combUp("var");
          pVar[currentVarDef].name = varName;
          pVar[currentVarDef].contents = interpret;
        }
      }
    }
    return 0;
  }
  if (interpret.find("}") != std::string::npos) {
    if (inFunction) {
      inFunction = false;
    }
    else {
      WARN "Ignoring '}' character." ENDL;
    }
  }
  if (interpret.find(":(") != std::string::npos) {
    debug("Inside bee body");
    if (!connectionStart) {
      ERR "Cannot start connection without wings." ENDL;
      return 0;
    }
    if (interpret.find("|") == std::string::npos) {
      ERR "No stripes found. Bee is entirely yellow and currently facing an existential crisis." ENDL;
      return 0;
    }
    bodyDefined = true;
    while (interpret.find("|") != std::string::npos) {
      ++stripeCount;
      interpret = interpret.substr(interpret.find("|") + 1, interpret.length() - 1);
      if (interpret.find("|") != std::string::npos) {
        stripes.push_back(interpret.substr(0, interpret.find("|") - 1));
      }
      else if (stripeCount < 2) {
        ERR "At least two stripes must be given." ENDL;
      }
    }
    return 0;
  }
  if (interpret.find("(") != std::string::npos) {
    if (interpret.find(")") != std::string::npos) {
      if (connectionStart && !bodyDefined) {
        ERR "Cannot define connection inside connection. You just smashed two bees into each other. You monster." ENDL;
        return 0;
      }
      else if (!connectionStart && !bodyDefined) {
        debug("Defining first wings");
        if (varCheck.substr(varCheck.find("(") + 1) != ")") {
          firstWingContent = varCheck.substr(varCheck.find("(") + 1, varCheck.find(")") - 1);
        }
        else {
          firstWingContent = "BLANK";
        }
        connectionStart = true;
        return 0;
      }
      else if (connectionStart && bodyDefined) {
        debug("Defining second wings");
        secondWingContent = varCheck.substr(varCheck.find("(") + 1, varCheck.find(")") - 1);
        for (int i = 0; i < stripes.size(); i++) {
          debug("Running process with stripes instead of fValue.");
          process(stripes.at(i));
        }
        connectionStart = false;
        bodyDefined = false;
      }
      else {
        ERR "This literally shouldn't be possible, but why not put an error here just for debugging purposes? Debeeing, one could even say!\n...comedy is not an interpreter's specialty." ENDL;
      }
    }
    else {
        ERR "No closing paranthese found!" ENDL;
        return 0;
    }  
  }
  if (hiveType == "CELL") {
    if (varCheck.find(uFunc.name + "(") != std::string::npos && varCheck.find(")") != std::string::npos) {
      process(uFunc.contents);
    }
  }
  return 0;
}

int main(int argc, char** argv) {
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
  buzzFile.open(fileName.c_str());
  while (getline(buzzFile, empty)) {
    lineCount++;
  }
  buzzFile.close();
  buzzFile.open(fileName.c_str());
  bool possibleJoke = false;
  std::vector<std::string> stripes;
  bool firstWingDefined = false;
  while (getline(buzzFile, fValue)) {
    process(fValue);
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
