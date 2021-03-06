/*
 * File: Interpreter.cpp
 * ---------------------
 * This program simulates the top level of an expression interpreter.  The
 * program reads an expression, evaluates it, and then displays the result.
 */

#include <iostream>
#include <string>
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "tokenscanner.h"
#include"console.h"
using namespace std;

bool expMatch(Expression *e1,Expression *e2);

void unparse(Expression* src);
void doUnparse(Expression* src,string op);


int main() {
   EvaluationContext context;
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   Expression* exp;
   while(true){
       exp=NULL;
      try {
         string line;
         cout << "=> ";
         getline(cin, line);
         if (line == "quit") break;
         scanner.setInput(line);
         exp = parseExp(scanner);
         unparse(exp);
         cout<<endl;
      } catch (ErrorException ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
       if(exp!=NULL)
       delete exp;
   }

   return 0;
}

void unparse(Expression* src){
    doUnparse(src,"");
}


void doUnparse(Expression* src,string op){
    if(src==NULL) return;
    if(src->getType()==CONSTANT) cout<<src->toString();
    if(src->getType()==IDENTIFIER) cout<<src->toString();
    if(src->getType()==COMPOUND){
        bool isNeedBlance=precedence(op)>precedence(src->getOperator());
        if(isNeedBlance) cout<<"(";
        doUnparse(src->getLHS(),src->getOperator());
        cout<<src->getOperator();
        doUnparse(src->getRHS(),src->getOperator());
        if(isNeedBlance) cout<<")";
    }

}

