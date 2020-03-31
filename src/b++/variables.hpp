#include "libraries.hpp"

class function {
  public:
    std::vector<std::string> args;
    std::string name;
    std::string contents;
    function(std::string arrayFuncName, std::vector<std::string> givenArgs) : name(arrayFuncName) {
      for (int i = 0; i < givenArgs.size(); i++) {
        args.push_back(givenArgs.at(i));
      }
    }
    function (std::string funcName) : name(funcName) {}
    function () {}
};

class variable {
  public:
    std::string name;
    std::string contents;
};