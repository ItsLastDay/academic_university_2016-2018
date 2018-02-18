# Generated from LParser.g4 by ANTLR 4.7
from antlr4 import *
if __name__ is not None and "." in __name__:
    from .LParser import LParser
else:
    from LParser import LParser

# This class defines a complete listener for a parse tree produced by LParser.
class LParserListener(ParseTreeListener):

    # Enter a parse tree produced by LParser#program.
    def enterProgram(self, ctx:LParser.ProgramContext):
        pass

    # Exit a parse tree produced by LParser#program.
    def exitProgram(self, ctx:LParser.ProgramContext):
        pass


    # Enter a parse tree produced by LParser#statement.
    def enterStatement(self, ctx:LParser.StatementContext):
        pass

    # Exit a parse tree produced by LParser#statement.
    def exitStatement(self, ctx:LParser.StatementContext):
        pass


    # Enter a parse tree produced by LParser#simple_statement.
    def enterSimple_statement(self, ctx:LParser.Simple_statementContext):
        pass

    # Exit a parse tree produced by LParser#simple_statement.
    def exitSimple_statement(self, ctx:LParser.Simple_statementContext):
        pass


    # Enter a parse tree produced by LParser#compound_statement.
    def enterCompound_statement(self, ctx:LParser.Compound_statementContext):
        pass

    # Exit a parse tree produced by LParser#compound_statement.
    def exitCompound_statement(self, ctx:LParser.Compound_statementContext):
        pass


    # Enter a parse tree produced by LParser#skip.
    def enterSkip(self, ctx:LParser.SkipContext):
        pass

    # Exit a parse tree produced by LParser#skip.
    def exitSkip(self, ctx:LParser.SkipContext):
        pass


    # Enter a parse tree produced by LParser#assignment.
    def enterAssignment(self, ctx:LParser.AssignmentContext):
        pass

    # Exit a parse tree produced by LParser#assignment.
    def exitAssignment(self, ctx:LParser.AssignmentContext):
        pass


    # Enter a parse tree produced by LParser#write.
    def enterWrite(self, ctx:LParser.WriteContext):
        pass

    # Exit a parse tree produced by LParser#write.
    def exitWrite(self, ctx:LParser.WriteContext):
        pass


    # Enter a parse tree produced by LParser#read.
    def enterRead(self, ctx:LParser.ReadContext):
        pass

    # Exit a parse tree produced by LParser#read.
    def exitRead(self, ctx:LParser.ReadContext):
        pass


    # Enter a parse tree produced by LParser#while_stmt.
    def enterWhile_stmt(self, ctx:LParser.While_stmtContext):
        pass

    # Exit a parse tree produced by LParser#while_stmt.
    def exitWhile_stmt(self, ctx:LParser.While_stmtContext):
        pass


    # Enter a parse tree produced by LParser#if_stmt.
    def enterIf_stmt(self, ctx:LParser.If_stmtContext):
        pass

    # Exit a parse tree produced by LParser#if_stmt.
    def exitIf_stmt(self, ctx:LParser.If_stmtContext):
        pass


    # Enter a parse tree produced by LParser#if_stmt_while_stmt_condition.
    def enterIf_stmt_while_stmt_condition(self, ctx:LParser.If_stmt_while_stmt_conditionContext):
        pass

    # Exit a parse tree produced by LParser#if_stmt_while_stmt_condition.
    def exitIf_stmt_while_stmt_condition(self, ctx:LParser.If_stmt_while_stmt_conditionContext):
        pass


    # Enter a parse tree produced by LParser#if_stmt_while_stmt_body.
    def enterIf_stmt_while_stmt_body(self, ctx:LParser.If_stmt_while_stmt_bodyContext):
        pass

    # Exit a parse tree produced by LParser#if_stmt_while_stmt_body.
    def exitIf_stmt_while_stmt_body(self, ctx:LParser.If_stmt_while_stmt_bodyContext):
        pass


    # Enter a parse tree produced by LParser#valueExpr.
    def enterValueExpr(self, ctx:LParser.ValueExprContext):
        pass

    # Exit a parse tree produced by LParser#valueExpr.
    def exitValueExpr(self, ctx:LParser.ValueExprContext):
        pass


    # Enter a parse tree produced by LParser#andBinop.
    def enterAndBinop(self, ctx:LParser.AndBinopContext):
        pass

    # Exit a parse tree produced by LParser#andBinop.
    def exitAndBinop(self, ctx:LParser.AndBinopContext):
        pass


    # Enter a parse tree produced by LParser#multDivModBinop.
    def enterMultDivModBinop(self, ctx:LParser.MultDivModBinopContext):
        pass

    # Exit a parse tree produced by LParser#multDivModBinop.
    def exitMultDivModBinop(self, ctx:LParser.MultDivModBinopContext):
        pass


    # Enter a parse tree produced by LParser#orBinop.
    def enterOrBinop(self, ctx:LParser.OrBinopContext):
        pass

    # Exit a parse tree produced by LParser#orBinop.
    def exitOrBinop(self, ctx:LParser.OrBinopContext):
        pass


    # Enter a parse tree produced by LParser#greaterLessBinop.
    def enterGreaterLessBinop(self, ctx:LParser.GreaterLessBinopContext):
        pass

    # Exit a parse tree produced by LParser#greaterLessBinop.
    def exitGreaterLessBinop(self, ctx:LParser.GreaterLessBinopContext):
        pass


    # Enter a parse tree produced by LParser#plusMinusBinop.
    def enterPlusMinusBinop(self, ctx:LParser.PlusMinusBinopContext):
        pass

    # Exit a parse tree produced by LParser#plusMinusBinop.
    def exitPlusMinusBinop(self, ctx:LParser.PlusMinusBinopContext):
        pass


    # Enter a parse tree produced by LParser#parenExpr.
    def enterParenExpr(self, ctx:LParser.ParenExprContext):
        pass

    # Exit a parse tree produced by LParser#parenExpr.
    def exitParenExpr(self, ctx:LParser.ParenExprContext):
        pass


    # Enter a parse tree produced by LParser#equalityBinop.
    def enterEqualityBinop(self, ctx:LParser.EqualityBinopContext):
        pass

    # Exit a parse tree produced by LParser#equalityBinop.
    def exitEqualityBinop(self, ctx:LParser.EqualityBinopContext):
        pass


    # Enter a parse tree produced by LParser#idExpr.
    def enterIdExpr(self, ctx:LParser.IdExprContext):
        pass

    # Exit a parse tree produced by LParser#idExpr.
    def exitIdExpr(self, ctx:LParser.IdExprContext):
        pass


    # Enter a parse tree produced by LParser#value.
    def enterValue(self, ctx:LParser.ValueContext):
        pass

    # Exit a parse tree produced by LParser#value.
    def exitValue(self, ctx:LParser.ValueContext):
        pass


    # Enter a parse tree produced by LParser#identifier.
    def enterIdentifier(self, ctx:LParser.IdentifierContext):
        pass

    # Exit a parse tree produced by LParser#identifier.
    def exitIdentifier(self, ctx:LParser.IdentifierContext):
        pass


