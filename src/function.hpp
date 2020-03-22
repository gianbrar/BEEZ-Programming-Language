#include "libraries.hpp"

class function {
  public:
    std::string args[];
    std::string name;
    function(std::string arrayFuncName, std::string givenArgs[]) {
      for (int i = 0; i < givenArgs.size(); i++) {
        args[i] = givenArgs[i];
      }
    }
    function (std::string funcName) : name(funcName) {}
};