#include "antlr4-runtime.h"
#include "../MITScript.h"
#include "CST.h"
#include <cassert>
ProgramNode* Program(antlr4::CommonTokenStream &tokens); // DONE
StatementNode* Statement(antlr4::CommonTokenStream &tokens); // NOT fully implemented
GlobalNode* Global(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
AssignmentNode* Assignment(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
CallStatementNode* CallStatement(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
BlockNode* Block(antlr4::CommonTokenStream &tokens); // DONE UNTESTED -> deal w EOF
BlockPrimeNode* BlockPrime(antlr4::CommonTokenStream &tokens); // DONE UNTESTED -> deal w EOF
IfStatementNode* IfStatement(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
WhileNode* While(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
ReturnNode* Return(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
ExpressionNode* Expression(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
FunctionNode* Function(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
BooleanNode* Boolean(antlr4::CommonTokenStream &tokens); // DONE
ConjunctionNode* Conjunction(antlr4::CommonTokenStream &tokens); // DONE
BoolUnitNode* BoolUnit(antlr4::CommonTokenStream &tokens); // DONE
PredicateNode* Predicate(antlr4::CommonTokenStream &tokens); // DONE
ArithmeticNode* Arithmetic(antlr4::CommonTokenStream &tokens); // DONE
ProductNode* Product(antlr4::CommonTokenStream &tokens); // DONE
UnitNode* Unit(antlr4::CommonTokenStream &tokens); // NOT fully implemented
LHSNode* LHS(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
CallNode* Call(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
RecordNode* Record(antlr4::CommonTokenStream &tokens); // DONE UNTESTED
ConstantNode* Constant(antlr4::CommonTokenStream &tokens); // DONE

StatementListNode* StatementList(antlr4::CommonTokenStream &tokens);

