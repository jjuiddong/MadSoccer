
#include "Global.h"
#include "ProtocolParser.h"
#include "GenerateProtocolCode.h"


cProtocolParser::cProtocolParser() 
{
	m_pScan = new cProtocolScanner();
	m_bTrace = FALSE;
	m_bError = FALSE;

}


cProtocolParser::~cProtocolParser()
{
	SAFE_DELETE(m_pScan);
//	DeleteParseTree(m_pParseTree);

}


//---------------------------------------------------------------------
// Ʃ�丮�� ��ũ��Ʈ�� �Ľ��Ѵ�.
//---------------------------------------------------------------------
BOOL cProtocolParser::Parse( char *szFileName, BOOL bTrace )
{
	if( !m_pScan->LoadFile(szFileName, bTrace) )
		return FALSE;

	strcpy_s( m_FileName, sizeof(m_FileName), szFileName );
	printf( "%s file Compile\n", szFileName );

	m_Token = m_pScan->GetToken();
	if( ENDFILE == m_Token )
	{
		m_pScan->Clear();
		return NULL;
	}

	sRmi *rmiList = rmi_list();

	if( ENDFILE != m_Token )
	{
		SyntaxError( " code ends before file " );
		PrintToken( m_Token, m_pScan->GetTokenStringQ(0) );
		m_pScan->Clear();
		return FALSE;
	}

	//WritePIDLMacro(m_FileName, rmiList);
	compiler::WriteProtocolCode(rmiList);
	ReleaseRmi(rmiList);

	return TRUE;
}


// rmi_list -> (rmi)*
sRmi* cProtocolParser::rmi_list()
{
	sRmi *p = rmi();
	sRmi *first = p;
	while (p && (p->next = rmi()))
	{
		p = p->next;
	}
	return first;
}

// rmi -> global id number '{' stmt_list '}'
sRmi* cProtocolParser::rmi()
{
	sRmi *p = NULL;
	
	if (PROTOCOL == m_Token)
	{
		Match(PROTOCOL);
		p = new sRmi;
		p->name = id();
		p->number = num();
		Match(LBRACE);
		p->protocol = stmt_list();
		Match(RBRACE);
	}

	return p;
}

// stmt_list -> (stmt)*
sProtocol* cProtocolParser::stmt_list()
{
	sProtocol *p = stmt();
	sProtocol *first = p;
	while (p && (p->next = stmt()))
	{
		p = p->next;
	}
	return first;
}

// stmt -> protocol semicolon
sProtocol* cProtocolParser::stmt()
{
	sProtocol *p = protocol();
	if (p)
		Match(SEMICOLON);
	return p;
}

// protocol -> id '(' arg_list ')'
sProtocol* cProtocolParser::protocol()
{
	sProtocol*p=NULL;
	if (ID == m_Token)
	{
		p = new sProtocol;
		p->name = id();
		Match(LPAREN);
		p->argList = arg_list();
		Match(RPAREN);
		p->next = NULL;
	}
	return p;
}


// arg_list -> [arg (',' arg)*]
sArg* cProtocolParser::arg_list()
{
	sArg *p = arg();
	if (!p)
		return NULL;

	sArg *first = p;
	while (COMMA == m_Token)
	{
		Match(COMMA);
		p->next = arg();
		p = p->next;
	}

	return first;
}

// arg -> type
sArg* cProtocolParser::arg()
{
	sArg *p = NULL;
	if (ID == m_Token)
	{
		p = new sArg;
		p->var = type();
		p->next = NULL;
	}
	return p;
}

// type -> type_sub (var)?
sTypeVar* cProtocolParser::type()
{
	sTypeVar *p=NULL;
	if (ID != m_Token)
		return NULL;

	Tokentype nextTok = m_pScan->GetTokenQ(1);
	p = new sTypeVar;
	p->type = type_sub();
	p->var = var();

	return p;
}

// type_sub -> id '<' type_sub '>'
//			| id::id
//			| id
string cProtocolParser::type_sub()
{
	string str = "";

	if (ID == m_Token)
	{
		Tokentype nextTok = m_pScan->GetTokenQ(1);
		if (LT == nextTok)
		{
			str += id();
			str += "<";
			Match(LT);
			str += type_sub();
			str += ">";
			Match(RT);
		}
		else if (SCOPE == nextTok)
		{
			str += id();
			str += "::";
			Match(SCOPE);
			str += type_sub();
		}
		else
		{
			str += id();
		}
	}

	return str;
}

// var -> '*' id (index)?
//	    | '&' id (index)?
//		| id (index)?
//	    | '*'
//		| '&'
string cProtocolParser::var()
{
	string str = "";
	Tokentype nextTok = m_pScan->GetTokenQ(1);

	if (TIMES == m_Token && ID == nextTok)
	{
		Match(TIMES);
		str += "*";
		str += id();
		str += index();
	}
	else if (REF == m_Token && ID == nextTok)
	{
		Match(REF);
		str += "&";
		str += id();
		str += index();
	}
	else if (ID == m_Token)
	{
		str += id();
		str += index();
	}
	else if (TIMES == m_Token)
	{
		Match(TIMES);
		str += "*";
	}
	else if (REF == m_Token)
	{
		Match(REF);
		str += "&";
	}

	return str;
}

string cProtocolParser::index()
{
	string str = "";
	if (LBRACKET == m_Token)
	{
		Match(LBRACKET);
		str += "[";
		str += number();
		str += "]";
		Match(RBRACKET);
	}
	return str;
}

string cProtocolParser::number()
{
	string str = "";
	str = m_pScan->GetTokenStringQ(0);
	Match(NUM);
	return str;
}

int cProtocolParser::num()
{
	int n = atoi(m_pScan->GetTokenStringQ(0));
	Match(NUM);
	return n;
}

string cProtocolParser::id()
{
	string str = m_pScan->GetTokenStringQ(0);
	Match( ID );
	return str;
}

BOOL cProtocolParser::Match( Tokentype t )
{
	if( m_Token == t )
	{
		m_Token = m_pScan->GetToken();
	}
	else
	{
		SyntaxError( "unexpected token -> " );
		PrintToken( m_Token, m_pScan->GetTokenStringQ(0) );
		printf( "\n" );
	}
	return TRUE;
}


void cProtocolParser::SyntaxError( char *szMsg, ... )
{
	m_bError = TRUE;
	char buf[ 256];
	va_list marker;
	va_start(marker, szMsg);
	vsprintf_s(buf, sizeof(buf), szMsg, marker);
	va_end(marker);
 	printf( ">>>" );
 	printf( "Syntax error at line %s %d: %s", m_FileName, m_pScan->GetLineNo(), buf );
}

void cProtocolParser::ReleaseRmi(sRmi *p)
{
	if (!p) return;
	ReleaseProtocol(p->protocol);
	ReleaseRmi(p->next);
	delete p;
}

void cProtocolParser::ReleaseProtocol(sProtocol *p)
{
	if (!p) return;
	ReleaseArg(p->argList);
	ReleaseProtocol(p->next);
	delete p;
}

void cProtocolParser::ReleaseArg(sArg *p)
{
	if (!p) return;
	if (p->var) delete p->var;
	ReleaseArg(p->next);
	delete p;
}

void cProtocolParser::WritePIDLMacro(string PIDLFileName, sRmi *rmi)
{
// 	string fileName = PIDLFileName;
// 	fileName += "_procstub";

	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, PIDLFileName.c_str() );
	char *name = strtok_s(srcFileName, ".", NULL);

	string fileName = name;
	fileName += "_procstub.h";

//	OFSTRUCT of;
// 	HFILE hFile = OpenFile(fileName.c_str(), &of, OF_WRITE);
// 	if (hFile != EOF)

	FILE *fp;
	fopen_s(&fp, fileName.c_str(), "w" );
	if (!fp)
		return;

	fprintf( fp, "// Compiled by PIDLCompiler.exe\n" );
	fprintf( fp, "// CNetListener ���� ���ȴ�. ��Ŷ ó�� ���� ó���� �� �� �и��ϱ� ���� ���������.\n" );
	fprintf( fp, "// jjuiddong \n\n" );
	fprintf( fp, "#pragma once\n\n" );
	WriteRmi(fp, rmi);

	fclose(fp);
}

void cProtocolParser::WriteRmi(FILE *fp, sRmi *p)
{
	if (!p) return;
	
	fprintf( fp, "namespace %s {\n", p->name.c_str(), p->number );
	fprintf( fp, "\tclass ProcessStub {\n" );
	fprintf( fp, "\tpublic:\n" );

	WriteProtocol(fp, p, p->protocol);

	// BeforeRmiInvocation �������̽� �߰�
	fprintf( fp, "virtual void BeforeRmiInvocation(const Proud::BeforeRmiSummary& summary) {} \n" );

	fprintf( fp, "\t};\n" );
	fprintf( fp, "}\n\n" );

	WriteRmi(fp, p->next);
}

void cProtocolParser::WriteProtocol(FILE *fp, sRmi *rmi, sProtocol *p)
{
	if (!p) return;
	
//	printf( "\t\t%s( ", p->name.c_str() );

	// Before
	fprintf( fp, "#define DECRMI_%s_Before_%s bool Before_%s(", rmi->name.c_str(), p->name.c_str(), p->name.c_str() );
	WriteFirstArg(fp, p->argList);
	fprintf( fp, ")\n" );

	fprintf( fp, "#define DEFRMI_%s_Before_%s(DerivedClass) bool DerivedClass::Before_%s(", rmi->name.c_str(), 
		p->name.c_str(), p->name.c_str() );
	WriteFirstArg(fp, p->argList);
	fprintf( fp, ")\n" );

	fprintf( fp, "#define CALL_%s_Before_%s Before_%s(", rmi->name.c_str(), 
		p->name.c_str(), p->name.c_str() );
	WriteFirstArgVar(fp, p->argList);
	fprintf( fp, ")\n" );

	fprintf( fp, "virtual bool Before_%s(", p->name.c_str() );
	WriteFirstArg(fp, p->argList);
	fprintf( fp, ")\n{\n\nreturn false;\n}\n" );



	// After
	fprintf( fp, "#define DECRMI_%s_After_%s bool After_%s(", rmi->name.c_str(), p->name.c_str(), p->name.c_str() );
	WriteFirstArg(fp, p->argList);
	fprintf( fp, ")\n" );

	fprintf( fp, "#define DEFRMI_%s_After_%s(DerivedClass) bool DerivedClass::After_%s(", rmi->name.c_str(), 
		p->name.c_str(), p->name.c_str() );
	WriteFirstArg(fp, p->argList);
	fprintf( fp, ")\n" );

	fprintf( fp, "#define CALL_%s_After_%s After_%s(", rmi->name.c_str(), 
		p->name.c_str(), p->name.c_str() );
	WriteFirstArgVar(fp, p->argList);
	fprintf( fp, ")\n" );
	fprintf( fp, "\n" );

	fprintf( fp, "virtual bool After_%s(", p->name.c_str() );
	WriteFirstArg(fp, p->argList);
	fprintf( fp, ")\n{\n\nreturn false;\n}\n" );

	fprintf( fp, "\n" );

	WriteProtocol(fp, rmi, p->next);
}

// ���ڰ� ó�� ��� 
// Proud::HostID remote,Proud::RmiContext &rmiContext �� �־���� �Ѵ�.
void cProtocolParser::WriteFirstArg(FILE *fp, sArg*p)
{
	fprintf( fp, "Proud::HostID remote, Proud::RmiContext &rmiContext" );
	WriteArg(fp, p, true);
}

// ������ Ÿ�԰� �̸��� ����Ѵ�.
void cProtocolParser::WriteArg(FILE *fp, sArg*p, bool isComma)
{
	if (!p) return;
	if (isComma)
		fprintf( fp, ", " );
	fprintf( fp, "const %s &%s", p->var->type.c_str(), p->var->var.c_str());
	WriteArg(fp, p->next, true);
}

// �����̸� ��� ó��
// Proud::HostID remote,Proud::RmiContext &rmiContext �� �־���� �Ѵ�.
void cProtocolParser::WriteFirstArgVar(FILE *fp, sArg*p)
{
	fprintf( fp, "remote, rmiContext" );
	WriteArgVar(fp, p, true);
}

// ���� �̸��� ����Ѵ�.
void cProtocolParser::WriteArgVar(FILE *fp, sArg*p, bool isComma)
{
	if (!p) return;
	if (isComma)
		fprintf( fp, ", " );
	fprintf( fp, "%s", p->var->var.c_str());
	WriteArgVar(fp, p->next, true);
}
