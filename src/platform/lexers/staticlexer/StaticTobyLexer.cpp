/*
 * TOBY -- An abstract interpreter engine and system for learning.
 * Copyright (C) 1999  Ryan C. Gordon.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/*
 * This is computer-generated code. DO NOT MANUALLY EDIT.
 *
 * This is generated from an XML file that specifies lexing rules for
 *  a given language.
 *
 *    Language : toby
 *    Language's file extension: *.toby
 *
 *   Code generator written by Ryan C. Gordon (icculus@icculus.org)
 */

#include "util/TobyObject.h"
#include "util/TobyLanguage.h"
#include "lexer/Lexer.h"
#include "lexer/LexerRules.h"
#include "lexer/LanguageRules.h"
#include "lexer/ElementRules.h"
#include "lexer/TokenizerRules.h"
#include "lexer/RepeatRules.h"
#include "lexer/PickOneRules.h"
#include "lexer/ReqWordRules.h"
#include "lexer/ReqEOFRules.h"
#include "lexer/ReqNewlineRules.h"
#include "lexer/ReqLiteralStringRules.h"
#include "lexer/ReqWhitespaceRules.h"
#include "lexer/ReqSingleLineCommentRules.h"
#include "lexer/ReqMultiLineCommentRules.h"
#include "lexer/ReqNumberRules.h"
#include "lexer/ReqWordCharsRules.h"
#include "lexer/ReqCharRules.h"
#include "lexer/ReqElementRules.h"


    // Declarations...

static LanguageRules *language_toby = NULL;
static LanguageRules *buildLanguage_toby(void);

static ElementRules *element_EXPRESSION = NULL;
static ElementRules *buildElement_EXPRESSION(void);

static ReqElementRules *reqelement_0 = NULL;
static ReqElementRules *buildReqElement_0(void);

static RepeatRules *buildRepeat_0(void);

static ReqElementRules *reqelement_1 = NULL;
static ReqElementRules *buildReqElement_1(void);

static ElementRules *element_OPERAND = NULL;
static ElementRules *buildElement_OPERAND(void);

static RepeatRules *buildRepeat_1(void);

static ReqElementRules *reqelement_2 = NULL;
static ReqElementRules *buildReqElement_2(void);

static PickOneRules *buildPickOne_0(void);

static ReqElementRules *reqelement_3 = NULL;
static ReqElementRules *buildReqElement_3(void);

static ReqElementRules *reqelement_4 = NULL;
static ReqElementRules *buildReqElement_4(void);

static ReqElementRules *reqelement_5 = NULL;
static ReqElementRules *buildReqElement_5(void);

static ReqElementRules *reqelement_6 = NULL;
static ReqElementRules *buildReqElement_6(void);

static ElementRules *element_PREFIXEDUNARYOPERATOR = NULL;
static ElementRules *buildElement_PREFIXEDUNARYOPERATOR(void);

static ElementRules *element_PARENTHESES = NULL;
static ElementRules *buildElement_PARENTHESES(void);

static ReqElementRules *reqelement_7 = NULL;
static ReqElementRules *buildReqElement_7(void);

static ElementRules *element_BOOLEANVALUE = NULL;
static ElementRules *buildElement_BOOLEANVALUE(void);

static PickOneRules *buildPickOne_1(void);

static ElementRules *element_BINARYEXPRESSION = NULL;
static ElementRules *buildElement_BINARYEXPRESSION(void);

static ReqElementRules *reqelement_8 = NULL;
static ReqElementRules *buildReqElement_8(void);

static ReqElementRules *reqelement_9 = NULL;
static ReqElementRules *buildReqElement_9(void);

static RepeatRules *buildRepeat_2(void);

static ReqElementRules *reqelement_10 = NULL;
static ReqElementRules *buildReqElement_10(void);

static ElementRules *element_BINARYOPERATOR = NULL;
static ElementRules *buildElement_BINARYOPERATOR(void);

static PickOneRules *buildPickOne_2(void);

static ElementRules *element_IDENTIFIER = NULL;
static ElementRules *buildElement_IDENTIFIER(void);

static ReqWordCharsRules *buildReqWordChars_0(void);

static RepeatRules *buildRepeat_3(void);

static PickOneRules *buildPickOne_3(void);

static ElementRules *element_FUNCTIONCALL = NULL;
static ElementRules *buildElement_FUNCTIONCALL(void);

static ReqElementRules *reqelement_11 = NULL;
static ReqElementRules *buildReqElement_11(void);

static RepeatRules *buildRepeat_4(void);

static ReqElementRules *reqelement_12 = NULL;
static ReqElementRules *buildReqElement_12(void);

static RepeatRules *buildRepeat_5(void);

static ReqElementRules *reqelement_13 = NULL;
static ReqElementRules *buildReqElement_13(void);

static ElementRules *element_VARDEFINE = NULL;
static ElementRules *buildElement_VARDEFINE(void);

static ReqElementRules *reqelement_14 = NULL;
static ReqElementRules *buildReqElement_14(void);

static ReqElementRules *reqelement_15 = NULL;
static ReqElementRules *buildReqElement_15(void);

static RepeatRules *buildRepeat_6(void);

static ReqElementRules *reqelement_16 = NULL;
static ReqElementRules *buildReqElement_16(void);

static ElementRules *element_VARIABLETYPE = NULL;
static ElementRules *buildElement_VARIABLETYPE(void);

static PickOneRules *buildPickOne_4(void);

static ReqElementRules *reqelement_17 = NULL;
static ReqElementRules *buildReqElement_17(void);

static ReqElementRules *reqelement_18 = NULL;
static ReqElementRules *buildReqElement_18(void);

static ElementRules *element_INTRINSICTYPE = NULL;
static ElementRules *buildElement_INTRINSICTYPE(void);

static PickOneRules *buildPickOne_5(void);

static ElementRules *element_ARRAYTYPE = NULL;
static ElementRules *buildElement_ARRAYTYPE(void);

static ReqElementRules *reqelement_19 = NULL;
static ReqElementRules *buildReqElement_19(void);

static ElementRules *element_VARINIT = NULL;
static ElementRules *buildElement_VARINIT(void);

static ReqElementRules *reqelement_20 = NULL;
static ReqElementRules *buildReqElement_20(void);

static ElementRules *element_TOPLEVEL = NULL;
static ElementRules *buildElement_TOPLEVEL(void);

static RepeatRules *buildRepeat_7(void);

static ReqElementRules *reqelement_21 = NULL;
static ReqElementRules *buildReqElement_21(void);

static RepeatRules *buildRepeat_8(void);

static ReqElementRules *reqelement_22 = NULL;
static ReqElementRules *buildReqElement_22(void);

static ElementRules *element_GLOBALVARS = NULL;
static ElementRules *buildElement_GLOBALVARS(void);

static RepeatRules *buildRepeat_9(void);

static ReqElementRules *reqelement_23 = NULL;
static ReqElementRules *buildReqElement_23(void);

static ElementRules *element_FUNCTION = NULL;
static ElementRules *buildElement_FUNCTION(void);

static ReqElementRules *reqelement_24 = NULL;
static ReqElementRules *buildReqElement_24(void);

static RepeatRules *buildRepeat_10(void);

static ReqElementRules *reqelement_25 = NULL;
static ReqElementRules *buildReqElement_25(void);

static RepeatRules *buildRepeat_11(void);

static ReqElementRules *reqelement_26 = NULL;
static ReqElementRules *buildReqElement_26(void);

static ElementRules *element_FUNCTIONDECLARATION = NULL;
static ElementRules *buildElement_FUNCTIONDECLARATION(void);

static ReqElementRules *reqelement_27 = NULL;
static ReqElementRules *buildReqElement_27(void);

static ReqElementRules *reqelement_28 = NULL;
static ReqElementRules *buildReqElement_28(void);

static ReqElementRules *reqelement_29 = NULL;
static ReqElementRules *buildReqElement_29(void);

static ElementRules *element_FUNCTIONPARAMETERLIST = NULL;
static ElementRules *buildElement_FUNCTIONPARAMETERLIST(void);

static RepeatRules *buildRepeat_12(void);

static ReqElementRules *reqelement_30 = NULL;
static ReqElementRules *buildReqElement_30(void);

static RepeatRules *buildRepeat_13(void);

static ReqElementRules *reqelement_31 = NULL;
static ReqElementRules *buildReqElement_31(void);

static ElementRules *element_FUNCTIONPARAMETER = NULL;
static ElementRules *buildElement_FUNCTIONPARAMETER(void);

static ReqElementRules *reqelement_32 = NULL;
static ReqElementRules *buildReqElement_32(void);

static ReqElementRules *reqelement_33 = NULL;
static ReqElementRules *buildReqElement_33(void);

static ElementRules *element_RETURNTYPE = NULL;
static ElementRules *buildElement_RETURNTYPE(void);

static PickOneRules *buildPickOne_6(void);

static ReqElementRules *reqelement_34 = NULL;
static ReqElementRules *buildReqElement_34(void);

static ElementRules *element_LOCALVARS = NULL;
static ElementRules *buildElement_LOCALVARS(void);

static RepeatRules *buildRepeat_14(void);

static ReqElementRules *reqelement_35 = NULL;
static ReqElementRules *buildReqElement_35(void);

static ElementRules *element_STATEMENT = NULL;
static ElementRules *buildElement_STATEMENT(void);

static PickOneRules *buildPickOne_7(void);

static ReqElementRules *reqelement_36 = NULL;
static ReqElementRules *buildReqElement_36(void);

static ReqElementRules *reqelement_37 = NULL;
static ReqElementRules *buildReqElement_37(void);

static ReqElementRules *reqelement_38 = NULL;
static ReqElementRules *buildReqElement_38(void);

static ReqElementRules *reqelement_39 = NULL;
static ReqElementRules *buildReqElement_39(void);

static ReqElementRules *reqelement_40 = NULL;
static ReqElementRules *buildReqElement_40(void);

static ReqElementRules *reqelement_41 = NULL;
static ReqElementRules *buildReqElement_41(void);

static ElementRules *element_ASSIGNMENTSTATEMENT = NULL;
static ElementRules *buildElement_ASSIGNMENTSTATEMENT(void);

static ReqElementRules *reqelement_42 = NULL;
static ReqElementRules *buildReqElement_42(void);

static ReqElementRules *reqelement_43 = NULL;
static ReqElementRules *buildReqElement_43(void);

static ElementRules *element_FORSTATEMENT = NULL;
static ElementRules *buildElement_FORSTATEMENT(void);

static ReqElementRules *reqelement_44 = NULL;
static ReqElementRules *buildReqElement_44(void);

static ReqElementRules *reqelement_45 = NULL;
static ReqElementRules *buildReqElement_45(void);

static ReqElementRules *reqelement_46 = NULL;
static ReqElementRules *buildReqElement_46(void);

static RepeatRules *buildRepeat_15(void);

static ReqElementRules *reqelement_47 = NULL;
static ReqElementRules *buildReqElement_47(void);

static RepeatRules *buildRepeat_16(void);

static ReqElementRules *reqelement_48 = NULL;
static ReqElementRules *buildReqElement_48(void);

static ElementRules *element_FORSTEP = NULL;
static ElementRules *buildElement_FORSTEP(void);

static ReqElementRules *reqelement_49 = NULL;
static ReqElementRules *buildReqElement_49(void);

static ElementRules *element_WHILESTATEMENT = NULL;
static ElementRules *buildElement_WHILESTATEMENT(void);

static ReqElementRules *reqelement_50 = NULL;
static ReqElementRules *buildReqElement_50(void);

static RepeatRules *buildRepeat_17(void);

static ReqElementRules *reqelement_51 = NULL;
static ReqElementRules *buildReqElement_51(void);

static ElementRules *element_IFSTATEMENT = NULL;
static ElementRules *buildElement_IFSTATEMENT(void);

static ReqElementRules *reqelement_52 = NULL;
static ReqElementRules *buildReqElement_52(void);

static RepeatRules *buildRepeat_18(void);

static ReqElementRules *reqelement_53 = NULL;
static ReqElementRules *buildReqElement_53(void);

static RepeatRules *buildRepeat_19(void);

static ReqElementRules *reqelement_54 = NULL;
static ReqElementRules *buildReqElement_54(void);

static RepeatRules *buildRepeat_20(void);

static ReqElementRules *reqelement_55 = NULL;
static ReqElementRules *buildReqElement_55(void);

static ElementRules *element_ELSEIFSTATEMENT = NULL;
static ElementRules *buildElement_ELSEIFSTATEMENT(void);

static ReqElementRules *reqelement_56 = NULL;
static ReqElementRules *buildReqElement_56(void);

static RepeatRules *buildRepeat_21(void);

static ReqElementRules *reqelement_57 = NULL;
static ReqElementRules *buildReqElement_57(void);

static ElementRules *element_ELSESTATEMENT = NULL;
static ElementRules *buildElement_ELSESTATEMENT(void);

static RepeatRules *buildRepeat_22(void);

static ReqElementRules *reqelement_58 = NULL;
static ReqElementRules *buildReqElement_58(void);

static ElementRules *element_RETURNSTATEMENT = NULL;
static ElementRules *buildElement_RETURNSTATEMENT(void);

static ReqElementRules *reqelement_59 = NULL;
static ReqElementRules *buildReqElement_59(void);

static ElementRules *element_FUNCTIONCALLSTATEMENT = NULL;
static ElementRules *buildElement_FUNCTIONCALLSTATEMENT(void);

static ReqElementRules *reqelement_60 = NULL;
static ReqElementRules *buildReqElement_60(void);

    // Functions...

static ReqElementRules *buildReqElement_0(void)
{
    assert(reqelement_0 == NULL);

    reqelement_0 = new ReqElementRules();
    return(reqelement_0);
} // buildReqElement_0

static ReqElementRules *buildReqElement_1(void)
{
    assert(reqelement_1 == NULL);

    reqelement_1 = new ReqElementRules();
    return(reqelement_1);
} // buildReqElement_1

static RepeatRules *buildRepeat_0(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_1();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_0

static ElementRules *buildElement_EXPRESSION(void)
{
    assert(element_EXPRESSION == NULL);

    LexerRules **kids = new LexerRules *[2];
    kids[0] = buildReqElement_0();
    kids[1] = buildRepeat_0();

    element_EXPRESSION = new ElementRules(0, 2, kids);
    return(element_EXPRESSION);
} // buildElement_EXPRESSION

static ReqElementRules *buildReqElement_2(void)
{
    assert(reqelement_2 == NULL);

    reqelement_2 = new ReqElementRules();
    return(reqelement_2);
} // buildReqElement_2

static RepeatRules *buildRepeat_1(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_2();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_1

static ReqElementRules *buildReqElement_3(void)
{
    assert(reqelement_3 == NULL);

    reqelement_3 = new ReqElementRules();
    return(reqelement_3);
} // buildReqElement_3

static ReqElementRules *buildReqElement_4(void)
{
    assert(reqelement_4 == NULL);

    reqelement_4 = new ReqElementRules();
    return(reqelement_4);
} // buildReqElement_4

static ReqElementRules *buildReqElement_5(void)
{
    assert(reqelement_5 == NULL);

    reqelement_5 = new ReqElementRules();
    return(reqelement_5);
} // buildReqElement_5

static ReqElementRules *buildReqElement_6(void)
{
    assert(reqelement_6 == NULL);

    reqelement_6 = new ReqElementRules();
    return(reqelement_6);
} // buildReqElement_6

static PickOneRules *buildPickOne_0(void)
{
    LexerRules **kids = new LexerRules *[6];
    kids[0] = buildReqElement_3();
    kids[1] = buildReqElement_4();
    kids[2] = buildReqElement_5();
    kids[3] = buildReqElement_6();
    kids[4] = new ReqNumberRules();
    kids[5] = new ReqLiteralStringRules();
    return(new PickOneRules(6, kids));
} // buildPickOne_0

static ElementRules *buildElement_OPERAND(void)
{
    assert(element_OPERAND == NULL);

    LexerRules **kids = new LexerRules *[2];
    kids[0] = buildRepeat_1();
    kids[1] = buildPickOne_0();

    element_OPERAND = new ElementRules(0, 2, kids);
    return(element_OPERAND);
} // buildElement_OPERAND

static ElementRules *buildElement_PREFIXEDUNARYOPERATOR(void)
{
    assert(element_PREFIXEDUNARYOPERATOR == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = new ReqWordRules(TOBYI18N_OPER_SUBTRACT);

    element_PREFIXEDUNARYOPERATOR = new ElementRules(0, 1, kids);
    return(element_PREFIXEDUNARYOPERATOR);
} // buildElement_PREFIXEDUNARYOPERATOR

static ReqElementRules *buildReqElement_7(void)
{
    assert(reqelement_7 == NULL);

    reqelement_7 = new ReqElementRules();
    return(reqelement_7);
} // buildReqElement_7

static ElementRules *buildElement_PARENTHESES(void)
{
    assert(element_PARENTHESES == NULL);

    LexerRules **kids = new LexerRules *[3];
    kids[0] = new ReqWordRules(TOBYI18N_OPER_LPAREN);
    kids[1] = buildReqElement_7();
    kids[2] = new ReqWordRules(TOBYI18N_OPER_RPAREN);

    element_PARENTHESES = new ElementRules(0, 3, kids);
    return(element_PARENTHESES);
} // buildElement_PARENTHESES

static PickOneRules *buildPickOne_1(void)
{
    LexerRules **kids = new LexerRules *[2];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_TRUE);
    kids[1] = new ReqWordRules(TOBYI18N_KEYWORD_FALSE);
    return(new PickOneRules(2, kids));
} // buildPickOne_1

static ElementRules *buildElement_BOOLEANVALUE(void)
{
    assert(element_BOOLEANVALUE == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildPickOne_1();

    element_BOOLEANVALUE = new ElementRules(0, 1, kids);
    return(element_BOOLEANVALUE);
} // buildElement_BOOLEANVALUE

static ReqElementRules *buildReqElement_8(void)
{
    assert(reqelement_8 == NULL);

    reqelement_8 = new ReqElementRules();
    return(reqelement_8);
} // buildReqElement_8

static ReqElementRules *buildReqElement_9(void)
{
    assert(reqelement_9 == NULL);

    reqelement_9 = new ReqElementRules();
    return(reqelement_9);
} // buildReqElement_9

static ReqElementRules *buildReqElement_10(void)
{
    assert(reqelement_10 == NULL);

    reqelement_10 = new ReqElementRules();
    return(reqelement_10);
} // buildReqElement_10

static RepeatRules *buildRepeat_2(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_10();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_2

static ElementRules *buildElement_BINARYEXPRESSION(void)
{
    assert(element_BINARYEXPRESSION == NULL);

    LexerRules **kids = new LexerRules *[3];
    kids[0] = buildReqElement_8();
    kids[1] = buildReqElement_9();
    kids[2] = buildRepeat_2();

    element_BINARYEXPRESSION = new ElementRules(0, 3, kids);
    return(element_BINARYEXPRESSION);
} // buildElement_BINARYEXPRESSION

static PickOneRules *buildPickOne_2(void)
{
    LexerRules **kids = new LexerRules *[10];
    kids[0] = new ReqWordRules(TOBYI18N_OPER_EQUALS);
    kids[1] = new ReqWordRules(TOBYI18N_OPER_MULTIPLY);
    kids[2] = new ReqWordRules(TOBYI18N_OPER_DIVIDE);
    kids[3] = new ReqWordRules(TOBYI18N_OPER_MODULO);
    kids[4] = new ReqWordRules(TOBYI18N_OPER_ADD);
    kids[5] = new ReqWordRules(TOBYI18N_OPER_SUBTRACT);
    kids[6] = new ReqWordRules(TOBYI18N_OPER_GREATER);
    kids[7] = new ReqWordRules(TOBYI18N_OPER_LESS);
    kids[8] = new ReqWordRules(TOBYI18N_OPER_GREATEREQL);
    kids[9] = new ReqWordRules(TOBYI18N_OPER_LESSEQL);
    return(new PickOneRules(10, kids));
} // buildPickOne_2

static ElementRules *buildElement_BINARYOPERATOR(void)
{
    assert(element_BINARYOPERATOR == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildPickOne_2();

    element_BINARYOPERATOR = new ElementRules(0, 1, kids);
    return(element_BINARYOPERATOR);
} // buildElement_BINARYOPERATOR

static PickOneRules *buildPickOne_3(void)
{
    LexerRules **kids = new LexerRules *[2];
    kids[0] = new ReqCharRules(ReqCharRules::CTYPE_ALPHANUMERIC, (char) 0);
    kids[1] = new ReqCharRules(ReqCharRules::CTYPE_SPECIFIC, (char) 95);
    return(new PickOneRules(2, kids));
} // buildPickOne_3

static RepeatRules *buildRepeat_3(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildPickOne_3();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_3

static ReqWordCharsRules *buildReqWordChars_0(void)
{
    LexerRules **kids = new LexerRules *[2];
    kids[0] = new ReqCharRules(ReqCharRules::CTYPE_LETTER, (char) 0);
    kids[1] = buildRepeat_3();
    return(new ReqWordCharsRules(2, kids));
} // buildReqWordChars_0

static ElementRules *buildElement_IDENTIFIER(void)
{
    assert(element_IDENTIFIER == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqWordChars_0();

    element_IDENTIFIER = new ElementRules(0, 1, kids);
    return(element_IDENTIFIER);
} // buildElement_IDENTIFIER

static ReqElementRules *buildReqElement_11(void)
{
    assert(reqelement_11 == NULL);

    reqelement_11 = new ReqElementRules();
    return(reqelement_11);
} // buildReqElement_11

static ReqElementRules *buildReqElement_12(void)
{
    assert(reqelement_12 == NULL);

    reqelement_12 = new ReqElementRules();
    return(reqelement_12);
} // buildReqElement_12

static RepeatRules *buildRepeat_4(void)
{
    LexerRules **kids = new LexerRules *[2];
    kids[0] = buildReqElement_12();
    kids[1] = new ReqWordRules(TOBYI18N_OPER_SEPARATOR);
    return(new RepeatRules(0, -1, 2, kids));
} // buildRepeat_4

static ReqElementRules *buildReqElement_13(void)
{
    assert(reqelement_13 == NULL);

    reqelement_13 = new ReqElementRules();
    return(reqelement_13);
} // buildReqElement_13

static RepeatRules *buildRepeat_5(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_13();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_5

static ElementRules *buildElement_FUNCTIONCALL(void)
{
    assert(element_FUNCTIONCALL == NULL);

    LexerRules **kids = new LexerRules *[5];
    kids[0] = buildReqElement_11();
    kids[1] = new ReqWordRules(TOBYI18N_OPER_LARRAY);
    kids[2] = buildRepeat_4();
    kids[3] = buildRepeat_5();
    kids[4] = new ReqWordRules(TOBYI18N_OPER_RARRAY);

    element_FUNCTIONCALL = new ElementRules(0, 5, kids);
    return(element_FUNCTIONCALL);
} // buildElement_FUNCTIONCALL

static ReqElementRules *buildReqElement_14(void)
{
    assert(reqelement_14 == NULL);

    reqelement_14 = new ReqElementRules();
    return(reqelement_14);
} // buildReqElement_14

static ReqElementRules *buildReqElement_15(void)
{
    assert(reqelement_15 == NULL);

    reqelement_15 = new ReqElementRules();
    return(reqelement_15);
} // buildReqElement_15

static ReqElementRules *buildReqElement_16(void)
{
    assert(reqelement_16 == NULL);

    reqelement_16 = new ReqElementRules();
    return(reqelement_16);
} // buildReqElement_16

static RepeatRules *buildRepeat_6(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_16();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_6

static ElementRules *buildElement_VARDEFINE(void)
{
    assert(element_VARDEFINE == NULL);

    LexerRules **kids = new LexerRules *[4];
    kids[0] = buildReqElement_14();
    kids[1] = buildReqElement_15();
    kids[2] = buildRepeat_6();
    kids[3] = new ReqNewlineRules();

    element_VARDEFINE = new ElementRules(0, 4, kids);
    return(element_VARDEFINE);
} // buildElement_VARDEFINE

static ReqElementRules *buildReqElement_17(void)
{
    assert(reqelement_17 == NULL);

    reqelement_17 = new ReqElementRules();
    return(reqelement_17);
} // buildReqElement_17

static ReqElementRules *buildReqElement_18(void)
{
    assert(reqelement_18 == NULL);

    reqelement_18 = new ReqElementRules();
    return(reqelement_18);
} // buildReqElement_18

static PickOneRules *buildPickOne_4(void)
{
    LexerRules **kids = new LexerRules *[2];
    kids[0] = buildReqElement_17();
    kids[1] = buildReqElement_18();
    return(new PickOneRules(2, kids));
} // buildPickOne_4

static ElementRules *buildElement_VARIABLETYPE(void)
{
    assert(element_VARIABLETYPE == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildPickOne_4();

    element_VARIABLETYPE = new ElementRules(0, 1, kids);
    return(element_VARIABLETYPE);
} // buildElement_VARIABLETYPE

static PickOneRules *buildPickOne_5(void)
{
    LexerRules **kids = new LexerRules *[3];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_NUMBER);
    kids[1] = new ReqWordRules(TOBYI18N_KEYWORD_BOOLEAN);
    kids[2] = new ReqWordRules(TOBYI18N_KEYWORD_STRING);
    return(new PickOneRules(3, kids));
} // buildPickOne_5

static ElementRules *buildElement_INTRINSICTYPE(void)
{
    assert(element_INTRINSICTYPE == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildPickOne_5();

    element_INTRINSICTYPE = new ElementRules(0, 1, kids);
    return(element_INTRINSICTYPE);
} // buildElement_INTRINSICTYPE

static ReqElementRules *buildReqElement_19(void)
{
    assert(reqelement_19 == NULL);

    reqelement_19 = new ReqElementRules();
    return(reqelement_19);
} // buildReqElement_19

static ElementRules *buildElement_ARRAYTYPE(void)
{
    assert(element_ARRAYTYPE == NULL);

    LexerRules **kids = new LexerRules *[8];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_ARRAY);
    kids[1] = new ReqWordRules(TOBYI18N_KEYWORD_OF);
    kids[2] = buildReqElement_19();
    kids[3] = new ReqWordRules(TOBYI18N_OPER_LARRAY);
    kids[4] = new ReqNumberRules();
    kids[5] = new ReqWordRules(TOBYI18N_KEYWORD_TO);
    kids[6] = new ReqNumberRules();
    kids[7] = new ReqWordRules(TOBYI18N_OPER_RARRAY);

    element_ARRAYTYPE = new ElementRules(0, 8, kids);
    return(element_ARRAYTYPE);
} // buildElement_ARRAYTYPE

static ReqElementRules *buildReqElement_20(void)
{
    assert(reqelement_20 == NULL);

    reqelement_20 = new ReqElementRules();
    return(reqelement_20);
} // buildReqElement_20

static ElementRules *buildElement_VARINIT(void)
{
    assert(element_VARINIT == NULL);

    LexerRules **kids = new LexerRules *[2];
    kids[0] = new ReqWordRules(TOBYI18N_OPER_ASSIGNMENT);
    kids[1] = buildReqElement_20();

    element_VARINIT = new ElementRules(0, 2, kids);
    return(element_VARINIT);
} // buildElement_VARINIT

static ReqElementRules *buildReqElement_21(void)
{
    assert(reqelement_21 == NULL);

    reqelement_21 = new ReqElementRules();
    return(reqelement_21);
} // buildReqElement_21

static RepeatRules *buildRepeat_7(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_21();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_7

static ReqElementRules *buildReqElement_22(void)
{
    assert(reqelement_22 == NULL);

    reqelement_22 = new ReqElementRules();
    return(reqelement_22);
} // buildReqElement_22

static RepeatRules *buildRepeat_8(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_22();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_8

static ElementRules *buildElement_TOPLEVEL(void)
{
    assert(element_TOPLEVEL == NULL);

    LexerRules **kids = new LexerRules *[3];
    kids[0] = buildRepeat_7();
    kids[1] = buildRepeat_8();
    kids[2] = new ReqEOFRules();

    element_TOPLEVEL = new ElementRules(0, 3, kids);
    return(element_TOPLEVEL);
} // buildElement_TOPLEVEL

static ReqElementRules *buildReqElement_23(void)
{
    assert(reqelement_23 == NULL);

    reqelement_23 = new ReqElementRules();
    return(reqelement_23);
} // buildReqElement_23

static RepeatRules *buildRepeat_9(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_23();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_9

static ElementRules *buildElement_GLOBALVARS(void)
{
    assert(element_GLOBALVARS == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildRepeat_9();

    element_GLOBALVARS = new ElementRules(0, 1, kids);
    return(element_GLOBALVARS);
} // buildElement_GLOBALVARS

static ReqElementRules *buildReqElement_24(void)
{
    assert(reqelement_24 == NULL);

    reqelement_24 = new ReqElementRules();
    return(reqelement_24);
} // buildReqElement_24

static ReqElementRules *buildReqElement_25(void)
{
    assert(reqelement_25 == NULL);

    reqelement_25 = new ReqElementRules();
    return(reqelement_25);
} // buildReqElement_25

static RepeatRules *buildRepeat_10(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_25();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_10

static ReqElementRules *buildReqElement_26(void)
{
    assert(reqelement_26 == NULL);

    reqelement_26 = new ReqElementRules();
    return(reqelement_26);
} // buildReqElement_26

static RepeatRules *buildRepeat_11(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_26();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_11

static ElementRules *buildElement_FUNCTION(void)
{
    assert(element_FUNCTION == NULL);

    LexerRules **kids = new LexerRules *[5];
    kids[0] = buildReqElement_24();
    kids[1] = buildRepeat_10();
    kids[2] = buildRepeat_11();
    kids[3] = new ReqWordRules(TOBYI18N_KEYWORD_ENDFUNC);
    kids[4] = new ReqNewlineRules();

    element_FUNCTION = new ElementRules(0, 5, kids);
    return(element_FUNCTION);
} // buildElement_FUNCTION

static ReqElementRules *buildReqElement_27(void)
{
    assert(reqelement_27 == NULL);

    reqelement_27 = new ReqElementRules();
    return(reqelement_27);
} // buildReqElement_27

static ReqElementRules *buildReqElement_28(void)
{
    assert(reqelement_28 == NULL);

    reqelement_28 = new ReqElementRules();
    return(reqelement_28);
} // buildReqElement_28

static ReqElementRules *buildReqElement_29(void)
{
    assert(reqelement_29 == NULL);

    reqelement_29 = new ReqElementRules();
    return(reqelement_29);
} // buildReqElement_29

static ElementRules *buildElement_FUNCTIONDECLARATION(void)
{
    assert(element_FUNCTIONDECLARATION == NULL);

    LexerRules **kids = new LexerRules *[8];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_BEGINFUNC);
    kids[1] = buildReqElement_27();
    kids[2] = new ReqWordRules(TOBYI18N_OPER_LPAREN);
    kids[3] = buildReqElement_28();
    kids[4] = new ReqWordRules(TOBYI18N_OPER_RPAREN);
    kids[5] = new ReqWordRules(TOBYI18N_KEYWORD_RETURNS);
    kids[6] = buildReqElement_29();
    kids[7] = new ReqNewlineRules();

    element_FUNCTIONDECLARATION = new ElementRules(0, 8, kids);
    return(element_FUNCTIONDECLARATION);
} // buildElement_FUNCTIONDECLARATION

static ReqElementRules *buildReqElement_30(void)
{
    assert(reqelement_30 == NULL);

    reqelement_30 = new ReqElementRules();
    return(reqelement_30);
} // buildReqElement_30

static RepeatRules *buildRepeat_12(void)
{
    LexerRules **kids = new LexerRules *[2];
    kids[0] = buildReqElement_30();
    kids[1] = new ReqWordRules(TOBYI18N_OPER_SEPARATOR);
    return(new RepeatRules(0, -1, 2, kids));
} // buildRepeat_12

static ReqElementRules *buildReqElement_31(void)
{
    assert(reqelement_31 == NULL);

    reqelement_31 = new ReqElementRules();
    return(reqelement_31);
} // buildReqElement_31

static RepeatRules *buildRepeat_13(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_31();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_13

static ElementRules *buildElement_FUNCTIONPARAMETERLIST(void)
{
    assert(element_FUNCTIONPARAMETERLIST == NULL);

    LexerRules **kids = new LexerRules *[2];
    kids[0] = buildRepeat_12();
    kids[1] = buildRepeat_13();

    element_FUNCTIONPARAMETERLIST = new ElementRules(0, 2, kids);
    return(element_FUNCTIONPARAMETERLIST);
} // buildElement_FUNCTIONPARAMETERLIST

static ReqElementRules *buildReqElement_32(void)
{
    assert(reqelement_32 == NULL);

    reqelement_32 = new ReqElementRules();
    return(reqelement_32);
} // buildReqElement_32

static ReqElementRules *buildReqElement_33(void)
{
    assert(reqelement_33 == NULL);

    reqelement_33 = new ReqElementRules();
    return(reqelement_33);
} // buildReqElement_33

static ElementRules *buildElement_FUNCTIONPARAMETER(void)
{
    assert(element_FUNCTIONPARAMETER == NULL);

    LexerRules **kids = new LexerRules *[2];
    kids[0] = buildReqElement_32();
    kids[1] = buildReqElement_33();

    element_FUNCTIONPARAMETER = new ElementRules(0, 2, kids);
    return(element_FUNCTIONPARAMETER);
} // buildElement_FUNCTIONPARAMETER

static ReqElementRules *buildReqElement_34(void)
{
    assert(reqelement_34 == NULL);

    reqelement_34 = new ReqElementRules();
    return(reqelement_34);
} // buildReqElement_34

static PickOneRules *buildPickOne_6(void)
{
    LexerRules **kids = new LexerRules *[2];
    kids[0] = buildReqElement_34();
    kids[1] = new ReqWordRules(TOBYI18N_KEYWORD_NOTHING);
    return(new PickOneRules(2, kids));
} // buildPickOne_6

static ElementRules *buildElement_RETURNTYPE(void)
{
    assert(element_RETURNTYPE == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildPickOne_6();

    element_RETURNTYPE = new ElementRules(0, 1, kids);
    return(element_RETURNTYPE);
} // buildElement_RETURNTYPE

static ReqElementRules *buildReqElement_35(void)
{
    assert(reqelement_35 == NULL);

    reqelement_35 = new ReqElementRules();
    return(reqelement_35);
} // buildReqElement_35

static RepeatRules *buildRepeat_14(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_35();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_14

static ElementRules *buildElement_LOCALVARS(void)
{
    assert(element_LOCALVARS == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildRepeat_14();

    element_LOCALVARS = new ElementRules(0, 1, kids);
    return(element_LOCALVARS);
} // buildElement_LOCALVARS

static ReqElementRules *buildReqElement_36(void)
{
    assert(reqelement_36 == NULL);

    reqelement_36 = new ReqElementRules();
    return(reqelement_36);
} // buildReqElement_36

static ReqElementRules *buildReqElement_37(void)
{
    assert(reqelement_37 == NULL);

    reqelement_37 = new ReqElementRules();
    return(reqelement_37);
} // buildReqElement_37

static ReqElementRules *buildReqElement_38(void)
{
    assert(reqelement_38 == NULL);

    reqelement_38 = new ReqElementRules();
    return(reqelement_38);
} // buildReqElement_38

static ReqElementRules *buildReqElement_39(void)
{
    assert(reqelement_39 == NULL);

    reqelement_39 = new ReqElementRules();
    return(reqelement_39);
} // buildReqElement_39

static ReqElementRules *buildReqElement_40(void)
{
    assert(reqelement_40 == NULL);

    reqelement_40 = new ReqElementRules();
    return(reqelement_40);
} // buildReqElement_40

static ReqElementRules *buildReqElement_41(void)
{
    assert(reqelement_41 == NULL);

    reqelement_41 = new ReqElementRules();
    return(reqelement_41);
} // buildReqElement_41

static PickOneRules *buildPickOne_7(void)
{
    LexerRules **kids = new LexerRules *[6];
    kids[0] = buildReqElement_36();
    kids[1] = buildReqElement_37();
    kids[2] = buildReqElement_38();
    kids[3] = buildReqElement_39();
    kids[4] = buildReqElement_40();
    kids[5] = buildReqElement_41();
    return(new PickOneRules(6, kids));
} // buildPickOne_7

static ElementRules *buildElement_STATEMENT(void)
{
    assert(element_STATEMENT == NULL);

    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildPickOne_7();

    element_STATEMENT = new ElementRules(0, 1, kids);
    return(element_STATEMENT);
} // buildElement_STATEMENT

static ReqElementRules *buildReqElement_42(void)
{
    assert(reqelement_42 == NULL);

    reqelement_42 = new ReqElementRules();
    return(reqelement_42);
} // buildReqElement_42

static ReqElementRules *buildReqElement_43(void)
{
    assert(reqelement_43 == NULL);

    reqelement_43 = new ReqElementRules();
    return(reqelement_43);
} // buildReqElement_43

static ElementRules *buildElement_ASSIGNMENTSTATEMENT(void)
{
    assert(element_ASSIGNMENTSTATEMENT == NULL);

    LexerRules **kids = new LexerRules *[4];
    kids[0] = buildReqElement_42();
    kids[1] = new ReqWordRules(TOBYI18N_OPER_ASSIGNMENT);
    kids[2] = buildReqElement_43();
    kids[3] = new ReqNewlineRules();

    element_ASSIGNMENTSTATEMENT = new ElementRules(0, 4, kids);
    return(element_ASSIGNMENTSTATEMENT);
} // buildElement_ASSIGNMENTSTATEMENT

static ReqElementRules *buildReqElement_44(void)
{
    assert(reqelement_44 == NULL);

    reqelement_44 = new ReqElementRules();
    return(reqelement_44);
} // buildReqElement_44

static ReqElementRules *buildReqElement_45(void)
{
    assert(reqelement_45 == NULL);

    reqelement_45 = new ReqElementRules();
    return(reqelement_45);
} // buildReqElement_45

static ReqElementRules *buildReqElement_46(void)
{
    assert(reqelement_46 == NULL);

    reqelement_46 = new ReqElementRules();
    return(reqelement_46);
} // buildReqElement_46

static ReqElementRules *buildReqElement_47(void)
{
    assert(reqelement_47 == NULL);

    reqelement_47 = new ReqElementRules();
    return(reqelement_47);
} // buildReqElement_47

static RepeatRules *buildRepeat_15(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_47();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_15

static ReqElementRules *buildReqElement_48(void)
{
    assert(reqelement_48 == NULL);

    reqelement_48 = new ReqElementRules();
    return(reqelement_48);
} // buildReqElement_48

static RepeatRules *buildRepeat_16(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_48();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_16

static ElementRules *buildElement_FORSTATEMENT(void)
{
    assert(element_FORSTATEMENT == NULL);

    LexerRules **kids = new LexerRules *[11];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_BEGINFOR);
    kids[1] = buildReqElement_44();
    kids[2] = new ReqWordRules(TOBYI18N_OPER_ASSIGNMENT);
    kids[3] = buildReqElement_45();
    kids[4] = new ReqWordRules(TOBYI18N_KEYWORD_TO);
    kids[5] = buildReqElement_46();
    kids[6] = buildRepeat_15();
    kids[7] = new ReqNewlineRules();
    kids[8] = buildRepeat_16();
    kids[9] = new ReqWordRules(TOBYI18N_KEYWORD_ENDFOR);
    kids[10] = new ReqNewlineRules();

    element_FORSTATEMENT = new ElementRules(0, 11, kids);
    return(element_FORSTATEMENT);
} // buildElement_FORSTATEMENT

static ReqElementRules *buildReqElement_49(void)
{
    assert(reqelement_49 == NULL);

    reqelement_49 = new ReqElementRules();
    return(reqelement_49);
} // buildReqElement_49

static ElementRules *buildElement_FORSTEP(void)
{
    assert(element_FORSTEP == NULL);

    LexerRules **kids = new LexerRules *[2];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_STEP);
    kids[1] = buildReqElement_49();

    element_FORSTEP = new ElementRules(0, 2, kids);
    return(element_FORSTEP);
} // buildElement_FORSTEP

static ReqElementRules *buildReqElement_50(void)
{
    assert(reqelement_50 == NULL);

    reqelement_50 = new ReqElementRules();
    return(reqelement_50);
} // buildReqElement_50

static ReqElementRules *buildReqElement_51(void)
{
    assert(reqelement_51 == NULL);

    reqelement_51 = new ReqElementRules();
    return(reqelement_51);
} // buildReqElement_51

static RepeatRules *buildRepeat_17(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_51();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_17

static ElementRules *buildElement_WHILESTATEMENT(void)
{
    assert(element_WHILESTATEMENT == NULL);

    LexerRules **kids = new LexerRules *[6];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_BEGINWHILE);
    kids[1] = buildReqElement_50();
    kids[2] = new ReqNewlineRules();
    kids[3] = buildRepeat_17();
    kids[4] = new ReqWordRules(TOBYI18N_KEYWORD_ENDWHILE);
    kids[5] = new ReqNewlineRules();

    element_WHILESTATEMENT = new ElementRules(0, 6, kids);
    return(element_WHILESTATEMENT);
} // buildElement_WHILESTATEMENT

static ReqElementRules *buildReqElement_52(void)
{
    assert(reqelement_52 == NULL);

    reqelement_52 = new ReqElementRules();
    return(reqelement_52);
} // buildReqElement_52

static ReqElementRules *buildReqElement_53(void)
{
    assert(reqelement_53 == NULL);

    reqelement_53 = new ReqElementRules();
    return(reqelement_53);
} // buildReqElement_53

static RepeatRules *buildRepeat_18(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_53();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_18

static ReqElementRules *buildReqElement_54(void)
{
    assert(reqelement_54 == NULL);

    reqelement_54 = new ReqElementRules();
    return(reqelement_54);
} // buildReqElement_54

static RepeatRules *buildRepeat_19(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_54();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_19

static ReqElementRules *buildReqElement_55(void)
{
    assert(reqelement_55 == NULL);

    reqelement_55 = new ReqElementRules();
    return(reqelement_55);
} // buildReqElement_55

static RepeatRules *buildRepeat_20(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_55();
    return(new RepeatRules(0, 1, 1, kids));
} // buildRepeat_20

static ElementRules *buildElement_IFSTATEMENT(void)
{
    assert(element_IFSTATEMENT == NULL);

    LexerRules **kids = new LexerRules *[8];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_IF);
    kids[1] = buildReqElement_52();
    kids[2] = new ReqNewlineRules();
    kids[3] = buildRepeat_18();
    kids[4] = buildRepeat_19();
    kids[5] = buildRepeat_20();
    kids[6] = new ReqWordRules(TOBYI18N_KEYWORD_ENDIF);
    kids[7] = new ReqNewlineRules();

    element_IFSTATEMENT = new ElementRules(0, 8, kids);
    return(element_IFSTATEMENT);
} // buildElement_IFSTATEMENT

static ReqElementRules *buildReqElement_56(void)
{
    assert(reqelement_56 == NULL);

    reqelement_56 = new ReqElementRules();
    return(reqelement_56);
} // buildReqElement_56

static ReqElementRules *buildReqElement_57(void)
{
    assert(reqelement_57 == NULL);

    reqelement_57 = new ReqElementRules();
    return(reqelement_57);
} // buildReqElement_57

static RepeatRules *buildRepeat_21(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_57();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_21

static ElementRules *buildElement_ELSEIFSTATEMENT(void)
{
    assert(element_ELSEIFSTATEMENT == NULL);

    LexerRules **kids = new LexerRules *[4];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_ELSEIF);
    kids[1] = buildReqElement_56();
    kids[2] = new ReqNewlineRules();
    kids[3] = buildRepeat_21();

    element_ELSEIFSTATEMENT = new ElementRules(0, 4, kids);
    return(element_ELSEIFSTATEMENT);
} // buildElement_ELSEIFSTATEMENT

static ReqElementRules *buildReqElement_58(void)
{
    assert(reqelement_58 == NULL);

    reqelement_58 = new ReqElementRules();
    return(reqelement_58);
} // buildReqElement_58

static RepeatRules *buildRepeat_22(void)
{
    LexerRules **kids = new LexerRules *[1];
    kids[0] = buildReqElement_58();
    return(new RepeatRules(0, -1, 1, kids));
} // buildRepeat_22

static ElementRules *buildElement_ELSESTATEMENT(void)
{
    assert(element_ELSESTATEMENT == NULL);

    LexerRules **kids = new LexerRules *[3];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_ELSE);
    kids[1] = new ReqNewlineRules();
    kids[2] = buildRepeat_22();

    element_ELSESTATEMENT = new ElementRules(0, 3, kids);
    return(element_ELSESTATEMENT);
} // buildElement_ELSESTATEMENT

static ReqElementRules *buildReqElement_59(void)
{
    assert(reqelement_59 == NULL);

    reqelement_59 = new ReqElementRules();
    return(reqelement_59);
} // buildReqElement_59

static ElementRules *buildElement_RETURNSTATEMENT(void)
{
    assert(element_RETURNSTATEMENT == NULL);

    LexerRules **kids = new LexerRules *[3];
    kids[0] = new ReqWordRules(TOBYI18N_KEYWORD_RETURN);
    kids[1] = buildReqElement_59();
    kids[2] = new ReqNewlineRules();

    element_RETURNSTATEMENT = new ElementRules(0, 3, kids);
    return(element_RETURNSTATEMENT);
} // buildElement_RETURNSTATEMENT

static ReqElementRules *buildReqElement_60(void)
{
    assert(reqelement_60 == NULL);

    reqelement_60 = new ReqElementRules();
    return(reqelement_60);
} // buildReqElement_60

static ElementRules *buildElement_FUNCTIONCALLSTATEMENT(void)
{
    assert(element_FUNCTIONCALLSTATEMENT == NULL);

    LexerRules **kids = new LexerRules *[2];
    kids[0] = buildReqElement_60();
    kids[1] = new ReqNewlineRules();

    element_FUNCTIONCALLSTATEMENT = new ElementRules(0, 2, kids);
    return(element_FUNCTIONCALLSTATEMENT);
} // buildElement_FUNCTIONCALLSTATEMENT

static LanguageRules *buildLanguage_toby(void)
{
    assert(language_toby == NULL);

    LexerRules **kids = new LexerRules *[32];
    kids[0] = buildElement_EXPRESSION();
    kids[1] = buildElement_OPERAND();
    kids[2] = buildElement_PREFIXEDUNARYOPERATOR();
    kids[3] = buildElement_PARENTHESES();
    kids[4] = buildElement_BOOLEANVALUE();
    kids[5] = buildElement_BINARYEXPRESSION();
    kids[6] = buildElement_BINARYOPERATOR();
    kids[7] = buildElement_IDENTIFIER();
    kids[8] = buildElement_FUNCTIONCALL();
    kids[9] = buildElement_VARDEFINE();
    kids[10] = buildElement_VARIABLETYPE();
    kids[11] = buildElement_INTRINSICTYPE();
    kids[12] = buildElement_ARRAYTYPE();
    kids[13] = buildElement_VARINIT();
    kids[14] = buildElement_TOPLEVEL();
    kids[15] = buildElement_GLOBALVARS();
    kids[16] = buildElement_FUNCTION();
    kids[17] = buildElement_FUNCTIONDECLARATION();
    kids[18] = buildElement_FUNCTIONPARAMETERLIST();
    kids[19] = buildElement_FUNCTIONPARAMETER();
    kids[20] = buildElement_RETURNTYPE();
    kids[21] = buildElement_LOCALVARS();
    kids[22] = buildElement_STATEMENT();
    kids[23] = buildElement_ASSIGNMENTSTATEMENT();
    kids[24] = buildElement_FORSTATEMENT();
    kids[25] = buildElement_FORSTEP();
    kids[26] = buildElement_WHILESTATEMENT();
    kids[27] = buildElement_IFSTATEMENT();
    kids[28] = buildElement_ELSEIFSTATEMENT();
    kids[29] = buildElement_ELSESTATEMENT();
    kids[30] = buildElement_RETURNSTATEMENT();
    kids[31] = buildElement_FUNCTIONCALLSTATEMENT();

    language_toby = new LanguageRules("toby", "toby", element_TOPLEVEL, new TokenizerRules(false, " \t", 2, true, false, "//", true, NULL, NULL, true, "\"", 1, true, '\\', false), 32, kids);
    return(language_toby);
} // buildLanguage_toby


    // Call this to build the language...

LexerRules *staticlang_toby(void)
{
    LexerRules *retval = buildLanguage_toby();
    if (retval == NULL)
        return(NULL);

    // resolve dependencies...

    assert(reqelement_0 != NULL);
    assert(element_OPERAND != NULL);
    reqelement_0->setElement(element_OPERAND);
    assert(reqelement_1 != NULL);
    assert(element_BINARYEXPRESSION != NULL);
    reqelement_1->setElement(element_BINARYEXPRESSION);
    assert(reqelement_2 != NULL);
    assert(element_PREFIXEDUNARYOPERATOR != NULL);
    reqelement_2->setElement(element_PREFIXEDUNARYOPERATOR);
    assert(reqelement_3 != NULL);
    assert(element_PARENTHESES != NULL);
    reqelement_3->setElement(element_PARENTHESES);
    assert(reqelement_4 != NULL);
    assert(element_FUNCTIONCALL != NULL);
    reqelement_4->setElement(element_FUNCTIONCALL);
    assert(reqelement_5 != NULL);
    assert(element_IDENTIFIER != NULL);
    reqelement_5->setElement(element_IDENTIFIER);
    assert(reqelement_6 != NULL);
    assert(element_BOOLEANVALUE != NULL);
    reqelement_6->setElement(element_BOOLEANVALUE);
    assert(reqelement_7 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_7->setElement(element_EXPRESSION);
    assert(reqelement_8 != NULL);
    assert(element_BINARYOPERATOR != NULL);
    reqelement_8->setElement(element_BINARYOPERATOR);
    assert(reqelement_9 != NULL);
    assert(element_OPERAND != NULL);
    reqelement_9->setElement(element_OPERAND);
    assert(reqelement_10 != NULL);
    assert(element_BINARYEXPRESSION != NULL);
    reqelement_10->setElement(element_BINARYEXPRESSION);
    assert(reqelement_11 != NULL);
    assert(element_IDENTIFIER != NULL);
    reqelement_11->setElement(element_IDENTIFIER);
    assert(reqelement_12 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_12->setElement(element_EXPRESSION);
    assert(reqelement_13 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_13->setElement(element_EXPRESSION);
    assert(reqelement_14 != NULL);
    assert(element_VARIABLETYPE != NULL);
    reqelement_14->setElement(element_VARIABLETYPE);
    assert(reqelement_15 != NULL);
    assert(element_IDENTIFIER != NULL);
    reqelement_15->setElement(element_IDENTIFIER);
    assert(reqelement_16 != NULL);
    assert(element_VARINIT != NULL);
    reqelement_16->setElement(element_VARINIT);
    assert(reqelement_17 != NULL);
    assert(element_INTRINSICTYPE != NULL);
    reqelement_17->setElement(element_INTRINSICTYPE);
    assert(reqelement_18 != NULL);
    assert(element_ARRAYTYPE != NULL);
    reqelement_18->setElement(element_ARRAYTYPE);
    assert(reqelement_19 != NULL);
    assert(element_INTRINSICTYPE != NULL);
    reqelement_19->setElement(element_INTRINSICTYPE);
    assert(reqelement_20 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_20->setElement(element_EXPRESSION);
    assert(reqelement_21 != NULL);
    assert(element_GLOBALVARS != NULL);
    reqelement_21->setElement(element_GLOBALVARS);
    assert(reqelement_22 != NULL);
    assert(element_FUNCTION != NULL);
    reqelement_22->setElement(element_FUNCTION);
    assert(reqelement_23 != NULL);
    assert(element_VARDEFINE != NULL);
    reqelement_23->setElement(element_VARDEFINE);
    assert(reqelement_24 != NULL);
    assert(element_FUNCTIONDECLARATION != NULL);
    reqelement_24->setElement(element_FUNCTIONDECLARATION);
    assert(reqelement_25 != NULL);
    assert(element_LOCALVARS != NULL);
    reqelement_25->setElement(element_LOCALVARS);
    assert(reqelement_26 != NULL);
    assert(element_STATEMENT != NULL);
    reqelement_26->setElement(element_STATEMENT);
    assert(reqelement_27 != NULL);
    assert(element_IDENTIFIER != NULL);
    reqelement_27->setElement(element_IDENTIFIER);
    assert(reqelement_28 != NULL);
    assert(element_FUNCTIONPARAMETERLIST != NULL);
    reqelement_28->setElement(element_FUNCTIONPARAMETERLIST);
    assert(reqelement_29 != NULL);
    assert(element_RETURNTYPE != NULL);
    reqelement_29->setElement(element_RETURNTYPE);
    assert(reqelement_30 != NULL);
    assert(element_FUNCTIONPARAMETER != NULL);
    reqelement_30->setElement(element_FUNCTIONPARAMETER);
    assert(reqelement_31 != NULL);
    assert(element_FUNCTIONPARAMETER != NULL);
    reqelement_31->setElement(element_FUNCTIONPARAMETER);
    assert(reqelement_32 != NULL);
    assert(element_VARIABLETYPE != NULL);
    reqelement_32->setElement(element_VARIABLETYPE);
    assert(reqelement_33 != NULL);
    assert(element_IDENTIFIER != NULL);
    reqelement_33->setElement(element_IDENTIFIER);
    assert(reqelement_34 != NULL);
    assert(element_VARIABLETYPE != NULL);
    reqelement_34->setElement(element_VARIABLETYPE);
    assert(reqelement_35 != NULL);
    assert(element_VARDEFINE != NULL);
    reqelement_35->setElement(element_VARDEFINE);
    assert(reqelement_36 != NULL);
    assert(element_ASSIGNMENTSTATEMENT != NULL);
    reqelement_36->setElement(element_ASSIGNMENTSTATEMENT);
    assert(reqelement_37 != NULL);
    assert(element_FORSTATEMENT != NULL);
    reqelement_37->setElement(element_FORSTATEMENT);
    assert(reqelement_38 != NULL);
    assert(element_WHILESTATEMENT != NULL);
    reqelement_38->setElement(element_WHILESTATEMENT);
    assert(reqelement_39 != NULL);
    assert(element_IFSTATEMENT != NULL);
    reqelement_39->setElement(element_IFSTATEMENT);
    assert(reqelement_40 != NULL);
    assert(element_RETURNSTATEMENT != NULL);
    reqelement_40->setElement(element_RETURNSTATEMENT);
    assert(reqelement_41 != NULL);
    assert(element_FUNCTIONCALLSTATEMENT != NULL);
    reqelement_41->setElement(element_FUNCTIONCALLSTATEMENT);
    assert(reqelement_42 != NULL);
    assert(element_IDENTIFIER != NULL);
    reqelement_42->setElement(element_IDENTIFIER);
    assert(reqelement_43 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_43->setElement(element_EXPRESSION);
    assert(reqelement_44 != NULL);
    assert(element_IDENTIFIER != NULL);
    reqelement_44->setElement(element_IDENTIFIER);
    assert(reqelement_45 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_45->setElement(element_EXPRESSION);
    assert(reqelement_46 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_46->setElement(element_EXPRESSION);
    assert(reqelement_47 != NULL);
    assert(element_FORSTEP != NULL);
    reqelement_47->setElement(element_FORSTEP);
    assert(reqelement_48 != NULL);
    assert(element_STATEMENT != NULL);
    reqelement_48->setElement(element_STATEMENT);
    assert(reqelement_49 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_49->setElement(element_EXPRESSION);
    assert(reqelement_50 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_50->setElement(element_EXPRESSION);
    assert(reqelement_51 != NULL);
    assert(element_STATEMENT != NULL);
    reqelement_51->setElement(element_STATEMENT);
    assert(reqelement_52 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_52->setElement(element_EXPRESSION);
    assert(reqelement_53 != NULL);
    assert(element_STATEMENT != NULL);
    reqelement_53->setElement(element_STATEMENT);
    assert(reqelement_54 != NULL);
    assert(element_ELSEIFSTATEMENT != NULL);
    reqelement_54->setElement(element_ELSEIFSTATEMENT);
    assert(reqelement_55 != NULL);
    assert(element_ELSESTATEMENT != NULL);
    reqelement_55->setElement(element_ELSESTATEMENT);
    assert(reqelement_56 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_56->setElement(element_EXPRESSION);
    assert(reqelement_57 != NULL);
    assert(element_STATEMENT != NULL);
    reqelement_57->setElement(element_STATEMENT);
    assert(reqelement_58 != NULL);
    assert(element_STATEMENT != NULL);
    reqelement_58->setElement(element_STATEMENT);
    assert(reqelement_59 != NULL);
    assert(element_EXPRESSION != NULL);
    reqelement_59->setElement(element_EXPRESSION);
    assert(reqelement_60 != NULL);
    assert(element_FUNCTIONCALL != NULL);
    reqelement_60->setElement(element_FUNCTIONCALL);

    return(retval);
} // staticlang_toby


