
#ifndef __SCRIPTDEFINE_H__
#define __SCRIPTDEFINE_H__

#include <string>
using namespace std;

enum
{
	MAX_STRING = 256,
	MAX_ARGUMENT = 10,
};

enum Tokentype
{
	_ERROR, ENDFILE, ID, NUM, FNUM, STRING, ASSIGN, LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET, COMMA, COLON, SEMICOLON,
	PLUS, MINUS, TIMES, DIV, REMAINDER, REF, ARROW,
	LT/* < */, RT/* > */, LTEQ/* <= */, RTEQ/* >= */, NEQ/* != */, EQ/* == */, OR/* || */, AND/* && */, NEG/* ! */, SCOPE/*::*/,
	EVENT, UIEVENT, CUSTOMEVENT, IF, ELSE, WHILE, FUNCTION, ARG_IN, PROTOCOL,
};

enum NodeKind { Stmt, Exp };
enum Kind
{
	ProgramK, TutorialK, RoleTutorialK, TrainingK, PracticeK, StatementK, AIK, EventK, UIEventK, 
	CustomEventK, SuccessK, FailK, AssignK,
	ScriptWaitK, ScriptEndK, ScriptExitK, // statement
	TypeK, ConditionOpK, ConditionTreeK, OpK, IdK, FuncK, ParamK, ConstIntK, ConstFloatK, ConstStrK, CallK, // exp
};

enum { MAXCHILD=8, };

enum eCONDITION_OP { OP_NONE, OP_AND, OP_OR, OP_NEG, OP_LT, OP_RT, OP_LTEQ, OP_RTEQ, OP_EQ, OP_NEQ };




typedef struct _sTypeVar
{
	string type;
	string var;

} sTypeVar;


typedef struct _sArg
{
	sTypeVar *var;
	_sArg *next;
} sArg;


typedef struct _sProtocol
{
	string name;
	sArg *argList;
	_sProtocol *next;
} sProtocol;

typedef struct _sRmi
{
	string name;
	int number;
	_sProtocol *protocol;
	_sRmi *next;
} sRmi;

void PrintToken( Tokentype token, char *szTokenString );


#ifndef SAFE_DELETE
#	define SAFE_DELETE(p) { if (p) delete (p); }
#endif 


#endif // __SCRIPTDEFINE_H__
