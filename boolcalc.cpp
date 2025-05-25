#include <fstream>
#include "boolexpr.h"
#include "Error.h"

int main(int argc, char* argv[]){
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " [expression] [table]" << std::endl;
    return 1;
  }
  std::ifstream infin(argv[2]);
  if (!infin.is_open()) {
    std::cout << "Can not open to file " << argv[2] << std::endl;
    infin.close();
    return 2;
  }
  std::ofstream outfin(argv[3]);
  if (!outfin.is_open()) {
    std::cout << "Can not open to file " << argv[3] << std::endl;
    outfin.close();
    return 3;
  }
  if (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"?") == 0) {}
  try{
    std::string expression;
    if (strcmp(argv[1],"-isfull") == 0)
    {
      std::vector<BooleanExpression> v;
      BooleanExpression c1;
      while (std::getline(infin, expression))
      {
        BooleanExpression c(expression.c_str());
        v.push_back(c);
      }
      bool check = c1.isFullSystem(v);
      if (check)
      {
        outfin << "yes";
      }
      else
      {
        outfin << "no";
      }
    }
    else
    {
      while (std::getline(infin, expression))
      {
        BooleanExpression c(expression.c_str());
        if (strcmp(argv[1],"-table") == 0)
        {
          std::string s = c.table();
          outfin << s << std::endl;
        }
        else if (strcmp(argv[1],"-zh") == 0)
        {
          outfin << c.GetZhegalkin() << std::endl;
        }
      }
    }
  } catch (const Error& a) {
    outfin << "error";
    std::cout<< a.what() << std::endl;
  }
  return 0;
}