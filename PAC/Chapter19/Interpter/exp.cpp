/*
 * File: exp.cpp
 * -------------
 * This file implements the exp.h interface.
 */
#include <string>
#include "error.h"
#include "exp.h"
#include "strlib.h"
using namespace std;

/*
 * Implementation notes: Expression
 * --------------------------------
 * The Expression class itself implements only those methods that
 * are not designated as pure virtual methods.
 */

Expression::Expression() {
   /* Empty */
}

Expression::~Expression() {
   /* Empty */
}

int Expression::getConstantValue() {
   error("getConstantValue: Illegal expression type");
   return 0;
}

std::string Expression::getIdentifierName() {
   error("getIdentifierName: Illegal expression type");
   return "";
}

std::string Expression::getOperator() {
   error("getOperator: Illegal expression type");
   return "";
}

Expression *Expression::getLHS() {
   error("getLHS: Illegal expression type");
   return NULL;
}

Expression *Expression::getRHS() {
   error("getRHS: Illegal expression type");
   return NULL;
}

Expression *Expression::getHS()
{
    error("getHS: Illeagal expression type");
    return NULL;
}

/*
 * Implementation notes: ConstantExp
 * ---------------------------------
 * The ConstantExp subclass represents an integer constant.  The eval
 * method simply returns that value.
 */

ConstantExp::ConstantExp(int value) {
   this->value = value;
}

int ConstantExp::eval(EvaluationContext & context) {
   return value;
}

string ConstantExp::toString() {
   return integerToString(value);
}

ExpressionType ConstantExp::getType() {
   return CONSTANT;
}

int ConstantExp::getConstantValue() {
   return value;
}

/*
 * Implementation notes: IdentifierExp
 * -----------------------------------
 * The IdentifierExp subclass represents a variable name.  The
 * implementation of eval looks up that name in the evaluation context.
 */

IdentifierExp::IdentifierExp(string name) {
   this->name = name;
}

int IdentifierExp::eval(EvaluationContext & context) {
   if (!context.isDefined(name)) error(name + " is undefined");
   return context.getValue(context.getAddress(name));
}

string IdentifierExp::toString() {
   return name;
}

ExpressionType IdentifierExp::getType() {
   return IDENTIFIER;
}

string IdentifierExp::getIdentifierName() {
   return name;
}

/*
 * Implementation notes: CompoundExp
 * ---------------------------------
 * The implementation of eval for CompoundExp evaluates the left and right
 * subexpressions recursively and then applies the operator.  Assignment is
 * treated as a special case because it does not evaluate the left operand.
 */

CompoundExp::CompoundExp(string op, Expression *lhs, Expression *rhs) {
   this->op = op;
   this->lhs = lhs;
   this->rhs = rhs;
}

CompoundExp::~CompoundExp() {
   delete lhs;
   delete rhs;
}

int CompoundExp::eval(EvaluationContext & context) {
   int right = rhs->eval(context);
   if (op == "=") {
       if(lhs->getType()==SINGLEOPEXP&&lhs->getOperator()=="*")
       {
           if(context.isDefined(lhs->getHS()->getIdentifierName()))
           context.setValue(context.getValue(context.getAddress(lhs->getHS()->getIdentifierName())),right);
           else
               error("Undefed refrence to"+lhs->getHS()->getIdentifierName());
       }
       else
           context.setValue(context.getAddress(lhs->getIdentifierName()), right);
      return right;
   }
   int left = lhs->eval(context);
   if (op == "+") return left + right;
   if (op == "-") return left - right;
   if (op == "*") return left * right;
   if (op == "/") {
      if (right == 0) error("Division by 0");
      return left / right;
   }
   error("Illegal operator in expression");
   return 0;
}

string CompoundExp::toString() {
   return '(' + lhs->toString() + ' ' + op + ' ' + rhs->toString() + ')';
}

ExpressionType CompoundExp::getType() {
   return COMPOUND;
}

string CompoundExp::getOperator() {
   return op;
}

Expression *CompoundExp::getLHS() {
   return lhs;
}

Expression *CompoundExp::getRHS() {
   return rhs;
}

/*
 * Implementation notes: EvaluationContext
 * ---------------------------------------
 * The methods in the EvaluationContext class simply call the appropriate
 * method on the map used to represent the symbol table.
 */



bool EvaluationContext::isDefined(string var) {
    return nameMap.containsKey(var);
}

int EvaluationContext::getAddress(string name)
{
    if(!nameMap.containsKey(name)){
        nameMap.add(name,1000+nameMap.size());
    }
    return nameMap.get(name);
}

int EvaluationContext::getValue(int address)
{
    return addressMap.get(address);
}

void EvaluationContext::setValue(int address, int value)
{
    if(!addressMap.containsKey(address)){
        addressMap.add(address,value);
    }
    else
        addressMap[address]=value;
}

Pointer::Pointer(string name)
{
    this->name=name;
}

string Pointer::toString()
{
    return name;
}

int Pointer::eval(EvaluationContext &context)
{
    return context.getValue(context.getAddress(name));
}

ExpressionType Pointer::getType()
{
    return POINTER;
}

string Pointer::getIdentifierName()
{
    return name;
}


SingleOpExp::SingleOpExp(Expression *exp_, string op_)
{
    this->exp=exp_;
    this->op=op_;
}

SingleOpExp::~SingleOpExp()
{
    if(exp!=NULL) delete exp;
}

string SingleOpExp::toString()
{
    return op+exp->toString();
}

int SingleOpExp::eval(EvaluationContext &context)
{
    if(op=="&"&&exp->getType()!=CONSTANT&&exp->getType()!=COMPOUND) return  context.getAddress(exp->getIdentifierName());
    if(op=="*"&&context.isDefined(this->getHS()->getIdentifierName())) return context.getValue(context.getValue(context.getAddress(exp->getIdentifierName())));
    error("Unexpected exp type!");
    cerr<<exp->getType()<<endl;
    return -1;
}

ExpressionType SingleOpExp::getType()
{
    return SINGLEOPEXP;
}

string SingleOpExp::getOperator()
{
    return op;
}

Expression *SingleOpExp::getHS()
{
    return exp;
}
