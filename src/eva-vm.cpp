#include "./vm/EvaVM.h"

int main(int argc, char const *argv[]) {
  EvaVM vm;

  auto result = vm.exec(R"(

  42
  
  )");
  log(result.number);
  return 0;
}
