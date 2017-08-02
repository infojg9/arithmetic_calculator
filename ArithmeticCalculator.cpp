/**
 * Copyright: jg.info9@gmail.com
 */

#include "ArithmeticCalculator.h"

ArithmeticCalculator::ArithmeticCalculator(char cDelimitInput, char cTermInput)
: m_cDelimit(cDelimitInput), m_cTerm(cTermInput)
{
  memset(m_acBuf, '\0', sizeof(m_acBuf));
}

ArithmeticCalculator::~ArithmeticCalculator()
{}

float ArithmeticCalculator::evaluateOperation(char operate, float op1,
                                              float op2)
{
  switch (operate)
  {
    case '*':
      return op2 * op1;
    case '/':
      return op2 / op1;
    case '+':
      return op2 + op1;
    case '-':
      return op2 - op1;
    case '^':
      return evaluateExponential(op1, op2);
    case '!':
      return evaluateFactorial(op1);
    default:
      cout << "Operator not supported:" << operate << endl;
      return 0;
  }
}

int ArithmeticCalculator::evaluateFactorial(int n) const
{
  return (n == 1 || n == 0) ? 1 : evaluateFactorial(n - 1) * n;
}

bool ArithmeticCalculator::processUserInput()
{
  cout << "Enter Arithmetic Infix Expression(terminate with '_' ): " << endl;
  getline(cin, m_sInfix, '_');
  /// User sanity test cases
  //m_sInfix = "(123+321)*4_";
  //m_sInfix = "9!_";
  //m_sInfix = "2^10_";
  //m_sInfix = "12/(2*12)_";
  cout << "Received Infix Expression: " << m_sInfix << endl;
  return true;
}

bool ArithmeticCalculator::evaluateInfixExp(float &fOutResult) {
  float expLeft = 1, expRight = 1;
  char  operate = m_cTerm, operateNext = m_cTerm, operateLast = m_cTerm;
  bool bExpLeftNeeded = true;
  deque <float> expLeftMemory;
  deque <char> operatorMemory;

  if (m_operandQueue.empty())
  {
    cout << "Info: No valid operand found in input" <<endl;
    fOutResult = 0;
    return true;
  }

  if (m_operatorQueue.empty())
  {
    cout << "Info: No valid operator found in input" <<endl;
    fOutResult = m_operandQueue.front();
    m_operandQueue.pop_front();
    return true;
  }
  else
  {
    operate = m_operatorQueue.front();
    m_operatorQueue.pop_front();
    operateNext = m_operatorQueue.front();
    m_operatorQueue.pop_front();
  }

  while(operate && (!m_operatorQueue.empty() || !m_operandQueue.empty()))
  {
    switch (operate)
    {
      case '(':
      {
        if (operateLast == '+' || operateLast == '-' || operateLast == '*'
            || operateLast == '/' || operateLast == '^')
        {
          expLeftMemory.push_back(expLeft);
          operatorMemory.push_back(operateLast);
          operatorMemory.push_back(operate);
        }

        if (operateLast != '_')
        {
          operateLast = operate;
          operate = m_operatorQueue.front();
          m_operatorQueue.pop_front();
          operateNext = m_operatorQueue.front();
          m_operatorQueue.pop_front();
        }
        else
        {
          operate = operateNext;
          m_operatorQueue.pop_front();
          operateNext = m_operatorQueue.front();
        }
        break;
      }

      case ')':
      {
        if (operateLast == '+' || operateLast == '-' || operateLast == '*'
            || operateLast == '/' || operateLast == '^')
        {
          if (!expLeftMemory.empty() && !operatorMemory.empty())
          {
            expLeft = evaluateOperation(operatorMemory.back(),
                                        expLeftMemory.back(), expLeft);
            expLeftMemory.pop_back();
            operatorMemory.pop_back();
          }
        }
        operateLast = operate;
        operate = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        operateNext = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        break;
      }

      case '_':
      { ///operator stack finished, process only operand
        fOutResult = expLeft;
        return true;
      }

      case '!':
      {
        if (bExpLeftNeeded)
        {
          if(!m_operandQueue.empty())
          {
            expLeft = m_operandQueue.front();
            m_operandQueue.pop_front();
          }
          else
          {
            cout <<"Error: Invalid evaluateFactorial exp: left operand is missing"<<endl;
            fOutResult = expLeft;
            return false;
          }
          bExpLeftNeeded = false;
        }

        expLeft = evaluateOperation(operate, expLeft);
        operateLast = operate;
        operate = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        operateNext = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        break;
      }

      case '^':
      {
        if (operateNext == '(')
        {
          expLeftMemory.push_back(expLeft);
          operatorMemory.push_back(operate);
          operatorMemory.push_back(operateNext);

          operate = m_operatorQueue.front();
          m_operatorQueue.pop_front();
          operateNext = m_operatorQueue.front();
          m_operatorQueue.pop_front();
        }

        if (bExpLeftNeeded || (!operatorMemory.empty() && operatorMemory.back() == '('))
        {
          if (!m_operandQueue.empty()) {
            expLeft = m_operandQueue.front();
            m_operandQueue.pop_front();
          } else
          {
            cout << "Error: Invalid power exp: left power operand is missing" << endl;
            fOutResult = expLeft;
            return false;
          }
          bExpLeftNeeded = false;
        }

        if(!m_operandQueue.empty())
        {
          expRight = m_operandQueue.front();
          m_operandQueue.pop_front();
        }
        else
        {
          cout <<"Error: Invalid power exp: right power operand is missing"<<endl;
          fOutResult = expLeft;
          return false;
        }
        expLeft = evaluateOperation(operate, expLeft, expRight);
        operateLast = operate;
        operate = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        operateNext = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        break;
      }

      case '+':
      {
        if (operateNext == '(')
        {
          expLeftMemory.push_back(expLeft);
          operatorMemory.push_back(operate);
          operatorMemory.push_back(operateNext);

          operate = m_operatorQueue.front();
          m_operatorQueue.pop_front();
          operateNext = m_operatorQueue.front();
          m_operatorQueue.pop_front();
        }

        if (bExpLeftNeeded || (!operatorMemory.empty() && operatorMemory.back() == '(')
            || operateNext == ')')
        {
          if (!m_operandQueue.empty()) {
            expLeft = m_operandQueue.front();
            m_operandQueue.pop_front();
          } else
          {
            cout << "Error: Invalid + exp: left + operand is missing" << endl;
            fOutResult = expLeft;
            return false;
          }
          bExpLeftNeeded = false;
        }

        if(!m_operandQueue.empty())
        {
          expRight = m_operandQueue.front();
          m_operandQueue.pop_front();
        }
        else
        {
          cout <<"Error: Invalid + exp: right + operand is missing"<<endl;
          fOutResult = expLeft;
          return false;
        }
        expLeft = evaluateOperation(operate, expLeft, expRight);
        operateLast = operate;
        operate = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        operateNext = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        break;
      }

      case '-':
      {
        if (operateNext == '(')
        {
          expLeftMemory.push_back(expLeft);
          operatorMemory.push_back(operate);
          operatorMemory.push_back(operateNext);

          operate = m_operatorQueue.front();
          m_operatorQueue.pop_front();
          operateNext = m_operatorQueue.front();
          m_operatorQueue.pop_front();
        }

        if (bExpLeftNeeded || (!operatorMemory.empty() && operatorMemory.back() == '('))
        {
          if(!m_operandQueue.empty())
          {
            expLeft = m_operandQueue.front();
            m_operandQueue.pop_front();
          }
          else
          {
            cout <<"Error: Invalid - exp: left - operand is missing"<<endl;
            fOutResult = expLeft;
            return false;
          }
          bExpLeftNeeded = false;
        }

        if(!m_operandQueue.empty())
        {
          expRight = m_operandQueue.front();
          m_operandQueue.pop_front();
        }
        else
        {
          cout <<"Error: Invalid - exp: right - operand is missing"<<endl;
          fOutResult = expLeft;
          return false;
        }
        expLeft = evaluateOperation(operate, expLeft, expRight);
        operateLast = operate;
        operate = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        operateNext = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        break;
      }

      case '*':
      {
        if (operateNext == '(')
        {
          expLeftMemory.push_back(expLeft);
          operatorMemory.push_back(operate);
          operatorMemory.push_back(operateNext);

          operate = m_operatorQueue.front();
          m_operatorQueue.pop_front();
          operateNext = m_operatorQueue.front();
          m_operatorQueue.pop_front();
        }

        if (bExpLeftNeeded || (!operatorMemory.empty() && operatorMemory.back() == '('))
        {
          if(!m_operandQueue.empty())
          {
            expLeft = m_operandQueue.front();
            m_operandQueue.pop_front();
          }
          else
          {
            cout <<"Error: Invalid * exp: left * operand is missing"<<endl;
            fOutResult = expLeft;
            return false;
          }
          bExpLeftNeeded = false;
        }

        if(!m_operandQueue.empty())
        {
          expRight = m_operandQueue.front();
          m_operandQueue.pop_front();
        }
        else
        {
          cout <<"Error: Invalid * exp: right * operand is missing"<<endl;
          fOutResult = expLeft;
          return false;
        }
        expLeft = evaluateOperation(operate, expLeft, expRight);
        operateLast = operate;
        operate = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        operateNext = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        break;
      }

      case '/':
      {
        if (operateNext == '(')
        {
          expLeftMemory.push_back(expLeft);
          operatorMemory.push_back(operate);
          operatorMemory.push_back(operateNext);

          operate = m_operatorQueue.front();
          m_operatorQueue.pop_front();
          operateNext = m_operatorQueue.front();
          m_operatorQueue.pop_front();
        }

        if (bExpLeftNeeded || (!operatorMemory.empty() && operatorMemory.back() == '('))
        {
          if(!m_operandQueue.empty())
          {
            expLeft = m_operandQueue.front();
            m_operandQueue.pop_front();
          }
          else
          {
            cout <<"Error: Invalid / exp: left / operand is missing"<<endl;
            fOutResult = expLeft;
            return false;
          }
          bExpLeftNeeded = false;
        }

        if(!m_operandQueue.empty())
        {
          expRight = m_operandQueue.front();
          m_operandQueue.pop_front();
        }
        else
        {
          cout <<"Error: Invalid / expression: right / operand is missing"<<endl;
          fOutResult = expLeft;
          return false;
        }
        expLeft = evaluateOperation(operate, expLeft, expRight);
        operateLast = operate;
        operate = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        operateNext = m_operatorQueue.front();
        m_operatorQueue.pop_front();
        break;
      }

      default:
        break;
    } /// switch (operate) end
    fOutResult = expLeft;
  } /// while end

  return true;
}

bool ArithmeticCalculator::parseAndEvalInfixExp(float &fOutResult)
{
  int value = 0;
  const char *cInfixExp = m_sInfix.c_str();
  const char *cInfixExpPrev = cInfixExp;
  const char *cMsDigit      = cInfixExp;
  bool error = false;

  if (!m_cDelimit || !m_cTerm)
  {
    fOutResult = 0;
    return false;
  }

  if (m_sInfix.empty())
  {
    fOutResult = 0;
    return true;
  }

  if (*cInfixExp == m_cTerm)
  {
    fOutResult = 0;
    return true;
  }

  if (cInfixExp[1] == m_cTerm)
  {
    value = atof(cInfixExp);
    (isdigit(value) ? fOutResult = value : fOutResult = 0);
    return true;
  }

  while (cInfixExp && *cInfixExp && m_cTerm != *cInfixExp && !error) {

    if (m_cDelimit == *cInfixExp)
    {
      cInfixExpPrev = cInfixExp;
      cInfixExp++;
      continue;
    }

    if (!isdigit(*cInfixExp))
    {
      m_operatorQueue.push_back(*cInfixExp);
      cout << "Parsed operator: " << m_operatorQueue.back() << endl;
      cInfixExpPrev = cInfixExp;
      cInfixExp++;
      continue;
    }
    /// Multidigit integer parser
    if (isdigit(*cInfixExp) && !isdigit(*cInfixExpPrev))
    { /// Single digit operand
      if (!isdigit(*(cInfixExp + 1)))
      {
        m_operandQueue.push_back(atof(cInfixExp));
        cout << "Parsed operand: " << m_operandQueue.back() << endl;
        if (m_cTerm != *(cInfixExp + 1))
        {
          m_operatorQueue.push_back(*(cInfixExp + 1));
          cout<<"Parsed operator: "<<m_operatorQueue.back()<<endl;
        }
        else
        {
          break;
        }
        cInfixExp += 2;
        cInfixExpPrev = cInfixExp - 1;
        continue;
      }
      else
      { ///Starting Multidigit operand token
        cMsDigit = cInfixExp;
        cInfixExpPrev = cInfixExp;
        cInfixExp++;
        continue;
      }
    }

    if (isdigit(*cInfixExp) && isdigit(*cInfixExpPrev))
    {
      if (!isdigit(*(cInfixExp + 1)))
      { ///Ending Multidigit operand token
        memcpy((void*)m_acBuf, cMsDigit, cInfixExp - cMsDigit + 1);
        m_operandQueue.push_back(atof(m_acBuf));
        cout<<"Parsed operand: "<<m_operandQueue.back()<<endl;
        if (m_cTerm != *(cInfixExp + 1))
        {
          m_operatorQueue.push_back(*(cInfixExp + 1));
          cout<<"Parsed operator: "<<m_operatorQueue.back()<<endl;
        }
        else
        {
          break;
        }

        cInfixExp += 2;
        cInfixExpPrev = cInfixExp - 1;
        memset(m_acBuf, '\0', sizeof(m_acBuf));
        continue;
      }
      else
      { ///Continue Multidigit operand token
        cInfixExpPrev = cInfixExp;
        cInfixExp++;
        continue;
      }
    }

    cInfixExp = strpbrk(cInfixExp, m_pcSeparator); // find separator
    if (cInfixExp)
    {
      if (isdigit(*cInfixExp))
      {
        cInfixExpPrev = cInfixExp;
      }
      else
      {
        //int val2 = cInfixExp - cInfixExpPrev;
        cout << "Digit length: " << cInfixExp - cInfixExpPrev<<endl;
        memcpy((void*)m_acBuf, cInfixExpPrev, cInfixExp - cInfixExpPrev);
        cInfixExpPrev = cInfixExp;
        value = atof(m_acBuf);
        memset(m_acBuf, '\0', sizeof(m_acBuf));
        m_operandQueue.push_back(value);
        cout<<"Parsed operand: "<<m_operandQueue.back()<<endl;
        if (m_cTerm != *cInfixExp)
        {
          m_operatorQueue.push_back(*cInfixExp);
          cout<<"Parsed operator: "<<m_operatorQueue.back()<<endl;
        }
        else
        {
          break;
        }
        cInfixExp++;
      }
      continue;
      //cInfixExp += strspn(cInfixExp, m_pcSeparator); // skip separator
      //cout << "after skip separator:" << cInfixExp << endl;
    }
  } // while

  /// Sanity code for Parser tokens verification
  deque<char> operatorsCopy = m_operatorQueue;
  deque<int>  operandsCopy = m_operandQueue;

  cout << "Parsed operator: " << endl;
  cout << endl << "Displaying m_operandQueue in LIFO order: " << endl;
  while (!operandsCopy.empty())
  {
    std::cout << ' ' << operandsCopy.front();
    operandsCopy.pop_front();
  }

  cout <<endl<<"Displaying m_operatorQueue in LIFO order: "<<endl;
  while (!operatorsCopy.empty())
  {
    std::cout << ' ' << operatorsCopy.front();
    operatorsCopy.pop_front();
  }
  cout<<endl;

  /// Final Infix stack evaluation
  error = evaluateInfixExp(fOutResult);
  return (error ? true : false);

}

float ArithmeticCalculator::evaluateExponential(float base, int exponent)
{
  float result = 1;
  while (exponent != 0) {
    result *= base;
    --exponent;
  }
  cout << "Processing evaluateExponential(): " << base << " ^ " << exponent
       << " = " << result << endl;
  return result;
}

