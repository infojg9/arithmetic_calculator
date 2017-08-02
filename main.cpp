/**
 * Copyright: jg.info9@gmail.com
 */

#include "ArithmeticCalculator.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

using namespace std;

int main()
{
  int status = 0;
  float result = 0;

  ArithmeticCalculator *pArithmeticCalculator = new ArithmeticCalculator(' ', '_');
  if (!pArithmeticCalculator)
  {
    cout <<"Memory Error: Heap allocation failed"<<endl;
    return EXIT_FAILURE;
  }

  pArithmeticCalculator->processUserInput();
  status = pArithmeticCalculator->parseAndEvalInfixExp(result);

  if (pArithmeticCalculator)
  {
    delete(pArithmeticCalculator);
    pArithmeticCalculator = nullptr;
  }

  cout <<endl<<"Expression evaluation: " << result << ", Evaluation status: "
       << (status ? "Success" : "Error") << endl;
  getchar();

  return EXIT_SUCCESS;
}
