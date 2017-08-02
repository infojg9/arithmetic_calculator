/**
 * Copyright: jg.info9@gmail.com
 */

#include <iostream>
#include <cstdlib>
#include <deque>
#include <cstring>
#include <string>

using namespace std;

#pragma once

/// Generic Calculator Interface
class Calculator
{
public:
  Calculator()  = default;
  ~Calculator() = default;

  virtual bool processUserInput() = 0;
  virtual bool parseAndEvalInfixExp(float &fOutResult) = 0;
};

/// Arithmetic Calculator: Final Derived Implementation
class ArithmeticCalculator : virtual public Calculator
{
public:
  ArithmeticCalculator(char cDelimitInput = ' ', char cTermInput = '_');
  virtual ~ArithmeticCalculator();

  bool processUserInput() override final;
  bool parseAndEvalInfixExp(float &fOutResult) override final;

private:
  int   evaluateFactorial(int n = 0) const;
  float evaluateExponential(float base, int exponent);
  float evaluateOperation(char operate, float op1 = 1, float op2 = 1);
  bool  evaluateInfixExp(float &fOutResult);

  string m_sInfix;
  char   m_cDelimit;
  char   m_cTerm;
  char   m_acBuf[20];
  const  char * const m_pcSeparator = " (),+-*/^!_";
  deque<char> m_operatorQueue;
  deque<int>  m_operandQueue;
};
