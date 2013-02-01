
#include "stdafx.h"
#include "PropertyMaker.h"
#include "VisualizerParser.h"
#include "Variant.h"
#include "../Lib/DiaWrapper.h"
#include "DefaultPropertyMaker.h"

namespace visualizer
{
	using namespace visualizer_parser;
	using namespace sharedmemory;

	visualizer_parser::SVisualizerScript* FindVisualizer( const std::string &typeName );

	// search visualizer
	visualizer_parser::SVisualizerScript* SearchVisualizerScript(SType_Stmt *psymT);
	bool CompareTypes( SType_Stmt *psymT, SType_Stmts *pvisT);
	bool CompareType( SType_Stmt *psymT, SType_Stmt *pvisT,OUT bool &isAstrisk );
	bool CompareTemplateTypes( visualizer_parser::SType_TemplateArgs *psymT, 
		SType_TemplateArgs *pvisT );
	SType_Stmt* ParseType( INOUT std::string &typeName );
	SType_TemplateArgs* ParseTemplateType( INOUT std::string &typeName );


	// parse visualizer
	void MakeProperty_Visualizer( SVisualizer *pvis, 
		IDiaSymbol *pSymbol, const SMemoryInfo &memInfo );

	void MakeProperty_AutoExpand( SAutoExp *pautoexp, 
		IDiaSymbol *pSymbol, const SMemoryInfo &memInfo  );

	void MakePropertyStatements( SStatements *pstmt,
		IDiaSymbol *pSymbol, const SMemoryInfo &memInfo );

	void MakePropertyExpression( SExpression *pexp,
		IDiaSymbol *pSymbol, const SMemoryInfo &memInfo );

	void MakePropertyIfStmt( SIf_Stmt *pif_stmt,
		IDiaSymbol *pSymbol, const SMemoryInfo &memInfo );

	void MakePropertyIteratorStmt( SVisBracketIterator_Stmt *pItor_stmt,
		IDiaSymbol *pSymbol, const SMemoryInfo &memInfo );

	bool MakePropertyElifStmt( SElif_Stmt *pelif_stmt, 
		IDiaSymbol *pSymbol, const SMemoryInfo &memInfo );


	// evaluate
	CComVariant Eval_Expression( SExpression *pexp);

	CComVariant Eval_Variable( SExpression *pexp,
		IDiaSymbol *pSymbol=NULL, const SMemoryInfo *pMemInfo=NULL);

	CComVariant Eval_Variable_FromId( const std::string &varId, IDiaSymbol *pSymbol, 
		const SMemoryInfo *pMemInfo);


	// Find
	IDiaSymbol* Find_Variable( SExpression *pexp, IDiaSymbol *pSymbol, 
		OUT LONG &offset );

	IDiaSymbol* Find_Variable_FromId( const std::string &varId, IDiaSymbol *pSymbol, 
		OUT LONG &offset );

	void Disp_Expression( SExpression *pexp );
	char GetAsciiFromTokentype(Tokentype tok);
	void CheckError( bool condition, const std::string &msg="error" );

}

using namespace dia;
using namespace std;
using namespace visualizer;

visualizer_parser::SVisualizerScript *m_pVisScr = NULL;
std::string m_SymbolName;
sharedmemory::SMemoryInfo m_MemInfo;
CPropertiesWnd *m_pPropertiesWnd= NULL;
CMFCPropertyGridProperty *m_pParentProperty = NULL;


class VisualizerScriptError
{
public:
	VisualizerScriptError(const string &msg) : m_Msg(msg) {}
	string m_Msg;
};


//------------------------------------------------------------------------
// Visualizer ��ũ��Ʈ�� �д´�.
//------------------------------------------------------------------------
bool visualizer::OpenVisualizerScript( const std::string &fileName )
{
	visualizer_parser::CParser parser;
	m_pVisScr = parser.Parse(fileName);
	if (!m_pVisScr)
		return false;
	return true;
}

void visualizer::Release()
{
	visualizer_parser::RemoveVisualizerScript(m_pVisScr);

}


//------------------------------------------------------------------------
// Property ����
// symbolName : �����޸𸮿� ����� �ɺ��̸�
//------------------------------------------------------------------------
bool	visualizer::MakeVisualizerProperty( CPropertiesWnd *pPropertiesWnd, 
											   CMFCPropertyGridProperty *pParentProp, 
											   const SMemoryInfo &memInfo, const string &symbolName )
{
	const std::string str = sharedmemory::ParseObjectName(symbolName);
	m_SymbolName = str;
	m_MemInfo = memInfo;
	m_pPropertiesWnd = pPropertiesWnd;
	m_pParentProperty = pParentProp;

	SVisualizerScript *pVisScript = FindVisualizer(str);
	if (pVisScript)
	{
		IDiaSymbol *pSymbol = CDiaWrapper::Get()->FindType(str);
		if (!pSymbol)
			return false;

		try
		{
			if (VisualizerScript_Visualizer == pVisScript->kind)
				MakeProperty_Visualizer(pVisScript->vis, pSymbol, memInfo);
			else 
				MakeProperty_AutoExpand(pVisScript->autoexp, pSymbol, memInfo);
		}
		catch (VisualizerScriptError &)
		{

		}

		if (pSymbol)
			pSymbol->Release();

		return true;
	}
	else
	{
//		visualizer::MakeProperty(pPropertiesWnd, symbolName);
		// �ƹ��ϵ� ����
		return false;
	}
}


//------------------------------------------------------------------------
// visualizer ������Ƽ ����
//------------------------------------------------------------------------
void visualizer::MakeProperty_Visualizer( SVisualizer *pvis, 
													  IDiaSymbol *pSymbol, const SMemoryInfo &memInfo  )
{
	RET(!pvis);
	// �ϴ� preview�� ��� 
	MakePropertyStatements( pvis->preview, pSymbol, memInfo );
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void visualizer::MakeProperty_AutoExpand( SAutoExp *pautoexp, IDiaSymbol *pSymbol, 
										 const SMemoryInfo &memInfo )
{
	RET(!pautoexp);

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void visualizer::MakePropertyStatements( SStatements *pstmt, IDiaSymbol *pSymbol, 
										const SMemoryInfo &memInfo)
{
	RET(!pstmt);
	SStatements *node = pstmt;
	while (node)
	{
		switch (node->kind)
		{
		case Stmt_Expression: MakePropertyExpression(node->exp, pSymbol, memInfo); break;
		case Stmt_SimpleExpression: break;
		case Stmt_If: MakePropertyIfStmt(node->if_stmt, pSymbol, memInfo); break;
		case Stmt_Bracket_Iterator: MakePropertyIteratorStmt(node->itor_stmt, pSymbol, memInfo); break;
		}	
		node = node->next;
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void visualizer::MakePropertyIfStmt( SIf_Stmt *pif_stmt,
												  IDiaSymbol *pSymbol, const SMemoryInfo &memInfo )
{
	RET(!pif_stmt);
	if (Eval_Expression(pif_stmt->cond) == CComVariant(true))
	{
		MakePropertyStatements( pif_stmt->stmts, pSymbol, memInfo );
	}
	else
	{
		if (MakePropertyElifStmt( pif_stmt->elif_stmt, pSymbol, memInfo ))
		{
			// �ƹ��� ����
		}
		else
		{
			MakePropertyStatements( pif_stmt->else_stmts, pSymbol, memInfo );			
		}
	}
}


//------------------------------------------------------------------------
// �ݺ� ��ɹ� ó��
//------------------------------------------------------------------------
void visualizer::MakePropertyIteratorStmt( SVisBracketIterator_Stmt *pitor_stmt,
							  IDiaSymbol *pSymbol, const SMemoryInfo &memInfo )
{
	RET(!pitor_stmt);
	RET(!pitor_stmt->stmts);

	switch (pitor_stmt->kind)
	{
	case Iter_Array: break;

	case Iter_List:
		{
			CheckError( pitor_stmt->stmts->head && pitor_stmt->stmts->next, "#list head, next not setting" );
			CheckError( pitor_stmt->stmts->expr || pitor_stmt->disp_stmt , "#list expr, disp_stmt not setting" );

			// 
//  			IDiaSymbol *pEachSymbol = CDiaWrapper::Get()->FindType(m_SymbolName);
//  			CheckError( !pEachSymbol, "$e not found" );

			// $e = current object

			// $e ����, IDiaSymbol
			CComVariant node = Eval_Variable(pitor_stmt->stmts->head, pSymbol, &memInfo);
			CheckError( node.vt != VT_EMPTY, "#list head expression error" );

			CComVariant size;
			if (pitor_stmt->stmts->size)
			{
				size = Eval_Variable(pitor_stmt->stmts->size, pSymbol, &memInfo);
				CheckError( size.vt != VT_EMPTY, "#list size expression error" );
			}

			CComVariant skip;
			if (pitor_stmt->stmts->skip)
			{
				skip = Eval_Variable(pitor_stmt->stmts->skip, pSymbol, &memInfo);
				CheckError( skip.vt != VT_EMPTY, "#list skip expression error" );
			}

			LONG offset=0;
			IDiaSymbol *pEachSymbol = Find_Variable(pitor_stmt->stmts->head, pSymbol, offset);
			CheckError( !pEachSymbol, "#list head expression error, $e not found" );
			BYTE *eachPtr = (BYTE*)memInfo.ptr + offset;
			SMemoryInfo eachMemInfo( "$e",  eachPtr, 0);

			int count = 0;
//			while (node)
			{
				MakePropertyExpression( pitor_stmt->stmts->expr, pEachSymbol,  eachMemInfo );

				++count;

// 				CComVariant node = Eval_Variable(pitor_stmt->stmts->head, pSymbol, &memInfo);
// 				CheckError( node.vt != VT_EMPTY, "#list head expression error" );
			}

			
	

			pEachSymbol->Release();			
		}
		break;

	case Iter_Tree: break;
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool visualizer::MakePropertyElifStmt( SElif_Stmt *pelif_stmt,
													IDiaSymbol *pSymbol, const SMemoryInfo &memInfo )
{
	RETV(!pelif_stmt, false);
	if (Eval_Expression(pelif_stmt->cond) == CComVariant(true))
	{
		MakePropertyStatements( pelif_stmt->stmts, pSymbol, memInfo );
		return true;
	}
	else
	{
		return MakePropertyElifStmt( pelif_stmt->next, pSymbol, memInfo );
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void visualizer::MakePropertyExpression( SExpression *pexp,
													  IDiaSymbol *pSymbol, const SMemoryInfo &memInfo )
{
	RET(!pexp);

	switch (pexp->kind)
	{
	case CondExprK:
	case AddTermK:
	case MulTermK:
		break;

	case DispFormatK:
			MakePropertyExpression(pexp->rhs, pSymbol, memInfo);
		break;

	case VariableK:
		{
			LONG offset = 0;
			IDiaSymbol *pVarSymbol = Find_Variable(pexp, pSymbol, offset);
			if (pVarSymbol)
			{
				const string name = dia::GetSymbolName(pVarSymbol);
				ULONGLONG length = 0;
				HRESULT hr = pVarSymbol->get_length(&length);
				LONG symbolOffset=0;
				hr = pVarSymbol->get_offset(&symbolOffset);
				offset -= symbolOffset; // MakeProperty_DefaultForm �� �������� symbol �� offset��
													  // ����Ǳ� ������, �Լ��� ȣ���ϱ� ���� �̸� symbol offset
													  // ���� ���� �Ѵ�.
				BYTE *ptr = (BYTE*)memInfo.ptr + offset;

 				visualizer::MakeProperty_DefaultForm( m_pPropertiesWnd, m_pParentProperty, 
 					pVarSymbol, SMemoryInfo(name.c_str(), ptr, (size_t)length) );
				pVarSymbol->Release();
			}
		}
		break;

	case IndirectK:
	case NumberK:
	case StringK:
		break;
	}
}


//------------------------------------------------------------------------
// typeName�� �ش�Ǵ� Visualizer ��ũ��Ʈ ������
//------------------------------------------------------------------------
SVisualizerScript* visualizer::FindVisualizer(const std::string &typeName )
{
	RETV(!m_pVisScr, NULL);

	std::string parseName = typeName;
	SType_Stmt *pTypeStmt = ParseType(parseName);

	SVisualizerScript *pscr = SearchVisualizerScript(pTypeStmt);
	RemoveType_Stmt(pTypeStmt);
	return pscr;
}


//------------------------------------------------------------------------
// Dia ���̺귯������ �Ѱ��� Ÿ�� ��Ʈ���� SType_Stmt
// ���·� �м��ؼ� �����Ѵ�.
//------------------------------------------------------------------------
SType_Stmt* visualizer::ParseType( INOUT string &typeName  )
{
	SType_Stmt *p = new SType_Stmt;

	// ���� ��ū���� index�� ã�´�.
	list<int> indices;
	indices.push_back(typeName.find('<'));
	indices.push_back(typeName.find(','));
	indices.push_back(typeName.find('>'));
	indices.sort();
	int idx = indices.front();
	while (idx == string::npos && !indices.empty())
	{
		if (indices.size() == 1)
			break;
		indices.pop_front();
		idx = indices.front();
	}

	if (string::npos == idx)
	{
		p->id = typeName;
		typeName.clear();
		p->templateArgs = NULL;
	}
	else  if ('<' == typeName[ idx])
	{
		p->id = typeName.substr(0, idx);
		typeName = typeName.substr(idx+1, typeName.size()-1); // next string
		p->templateArgs = ParseTemplateType(typeName);

		ASSERT_RETV(!typeName.empty(), p);
		const int rtIdx = typeName.find('>');
		ASSERT_RETV(rtIdx < 2, p); // ��ĭ ����� '>' �� ������, >> �� �������� 
		typeName = typeName.substr(rtIdx, typeName.size()-1); // next string
	}
	else  
	{
		p->id = typeName.substr(0, idx);
		typeName = typeName.substr(idx, typeName.size()-1); // next string
		p->templateArgs = NULL;
	}
	return p;
}


//------------------------------------------------------------------------
// < typename, typename .. > parsing
//------------------------------------------------------------------------
SType_TemplateArgs* visualizer::ParseTemplateType( INOUT string &typeName )
{
	SType_TemplateArgs *p = new SType_TemplateArgs;
	p->type = ParseType(typeName);
	p->next = NULL;

	if (typeName.empty())
		return p;

	if (typeName[0] == ',')
	{
		typeName = typeName.substr(1, typeName.size()-1); // next string
		p->next = ParseTemplateType(typeName);
	}
	return p;
}


//------------------------------------------------------------------------
// psymT Ÿ�԰� ���� visualizer script�� �����Ѵ�.
//------------------------------------------------------------------------
SVisualizerScript* visualizer::SearchVisualizerScript(SType_Stmt *psymT)
{
	RETV(!m_pVisScr, NULL);

	SVisualizerScript *node = m_pVisScr;
	while (node)
	{
		switch (node->kind)
		{
		case VisualizerScript_AutoExp:
 			if (CompareTypes(psymT, node->autoexp->match_type))
 				return node;
			break;

		case VisualizerScript_Visualizer:
			if (CompareTypes(psymT, node->vis->matchType))
				return node;
			break;
		}
		node = node->next;
	}
	return NULL;
}


//------------------------------------------------------------------------
// visualizer script �� OR �������� �ϳ� �̻��� Ÿ����
// ������ �� �ִ�. ���߿� �� Ÿ���̶� ���ٸ�,
// �ش� visualizer �� ��Ī�ȴ�.
//------------------------------------------------------------------------
bool visualizer::CompareTypes( SType_Stmt *psymT, 
																	  SType_Stmts *pvisT)
{
	SType_Stmts *node = pvisT;
	while (node)
	{
		bool isAsterisk = false;
		if (CompareType(psymT, node->type, isAsterisk))
			return true;
		node = node->next;
	}
	return false;
}


//------------------------------------------------------------------------
// t1, t2 �ΰ��� Ÿ���� ���ؼ� ���ٸ� true�� �����Ѵ�.
// psymT : dia ���� �Ѿ���� Ÿ�Ը�
// pvisT : visualizer ��ũ��Ʈ Ÿ�� '*' �ƽ��׸�ũ�� �����Ѵ�.
// * �� �����ٸ� isAstrick ���� true�� ���ϵȴ�.
//------------------------------------------------------------------------
bool visualizer::CompareType( SType_Stmt *psymT, SType_Stmt *pvisT, OUT bool &isAstrisk )
{
	RETV(!psymT && !pvisT, true);
	RETV(!psymT, false);
	RETV(!pvisT, false);

	isAstrisk  = false;
	if (pvisT->id == "*")
	{
		isAstrisk = true;
		return true;
	}

	if (psymT->id == pvisT->id)
	{
		if (psymT->templateArgs && pvisT->templateArgs)
		{
			return CompareTemplateTypes(psymT->templateArgs, pvisT->templateArgs);
		}
		else if (!psymT->templateArgs && !pvisT->templateArgs)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}


//------------------------------------------------------------------------
// ���ø� Ÿ�Կ��� �ΰ��� 
//------------------------------------------------------------------------
bool visualizer::CompareTemplateTypes( SType_TemplateArgs *psymT, 
						  SType_TemplateArgs *pvisT )
{
	RETV(!psymT && !pvisT, true);
	RETV(!psymT, false);
	RETV(!pvisT, false);

	bool isAsterisk = false;
	if (CompareType(psymT->type, pvisT->type, isAsterisk))
	{
		if (isAsterisk)
			return true;
		return CompareTemplateTypes(psymT->next, pvisT->next);
	}
	return false;
}


//------------------------------------------------------------------------
//  SExpression ���
//------------------------------------------------------------------------
void visualizer::Disp_Expression( SExpression *pexp )
{

}


//------------------------------------------------------------------------
// SExpression ���� �����Ѵ�.
//------------------------------------------------------------------------
CComVariant visualizer::Eval_Expression( SExpression *pexp )
{
	CComVariant reval;
	RETV(!pexp, reval);

	switch (pexp->kind)
	{
	case CondExprK:
		{
			switch (pexp->op)
			{
			case LT: reval = Eval_Expression(pexp->lhs) < Eval_Expression(pexp->rhs); break;
			case RT: reval = Eval_Expression(pexp->lhs) > Eval_Expression(pexp->rhs); break;
			case LTEQ: /* <= */
				reval = (Eval_Expression(pexp->lhs) < Eval_Expression(pexp->rhs)) 
					|| Eval_Expression(pexp->lhs) == Eval_Expression(pexp->rhs);
				break;
			case RTEQ:	/* >= */
				reval = (Eval_Expression(pexp->lhs) > Eval_Expression(pexp->rhs)) 
					|| Eval_Expression(pexp->lhs) == Eval_Expression(pexp->rhs);
				break;
			case NEQ:	reval = Eval_Expression(pexp->lhs) != Eval_Expression(pexp->rhs); break; /* != */
			case EQ:	reval = Eval_Expression(pexp->lhs) == Eval_Expression(pexp->rhs); break; /* == */
			case OR: reval = Eval_Expression(pexp->lhs).llVal || Eval_Expression(pexp->rhs).llVal; break; /* || */
			case AND: reval = (int)Eval_Expression(pexp->lhs).llVal && (int)Eval_Expression(pexp->rhs).llVal; break; /* && */
			case NEG: reval = !Eval_Expression(pexp->lhs).llVal; break;
			}
		}	
		break;

	case AddTermK: 
	case MulTermK:
		reval =  visualizer::VariantCalc(GetAsciiFromTokentype(pexp->op), 
			Eval_Expression(pexp->lhs), Eval_Expression(pexp->rhs)); 
		break;

	case VariableK:
		reval = Eval_Variable(pexp, NULL, NULL);
		break;

	case IndirectK:
		break;

	case NumberK: reval = pexp->num; break;
	case StringK: reval = pexp->str.c_str(); break;
	}

	return reval;
}


//------------------------------------------------------------------------
// Variable �� �����Ѵ�.
//------------------------------------------------------------------------
CComVariant visualizer::Eval_Variable( SExpression *pexp,
													IDiaSymbol *pSymbol, const SMemoryInfo *pMemInfo )
													// pSymbol = NULL, pMemInfo = NULL
{
	CComVariant reval;
	RETV(!pexp, reval);
	RETV(pexp->kind != VariableK, reval);

	if (pexp->str == "$e")
	{
//		IDiaSymbol *pEachSymbol = CDiaWrapper::Get()->FindType(m_SymbolName);
		IDiaSymbol *pEachSymbol = pSymbol;
		if (!pEachSymbol)
			return reval;

		if (pexp->rhs)
		{
			reval = Eval_Variable(pexp->rhs, pEachSymbol,  &m_MemInfo);
		}
		else
		{
			reval = dia::GetValueFromSymbol(m_MemInfo.ptr, pEachSymbol);
		}
//		pEachSymbol->Release();
	}
	else if (pexp->str == "$r")
	{

	}
	else if (pexp->str == "$i")
	{

	}
	else
	{
		RETV(!pSymbol || !pMemInfo, reval);

		if (pexp->rhs)
		{
			LONG offset=0;
			IDiaSymbol *pChild = dia::FindChildSymbol(pexp->str, pSymbol, offset);
			RETV(!pChild, reval);

			// next variable ���� 
			BYTE *ptr = (BYTE*)pMemInfo->ptr + offset;
			const string typeName = dia::GetSymbolTypeName(pChild);
			ULONGLONG length = 0;
			HRESULT hr = pChild->get_length(&length);

			reval = Eval_Variable(pexp->rhs, pChild, &SMemoryInfo(pexp->str.c_str(), ptr, (size_t)length) );

			pChild->Release();
		}
		else
		{
			reval = Eval_Variable_FromId(pexp->str, pSymbol, pMemInfo);
		}
	}

	// ���߿� ó��
	switch (pexp->prefix_op)
	{
	case PLUS:
	case MINUS:
	case TIMES:
	case NEG:
	case INC: // ++
	case DEC: // --
		break;

	case REF: // &
		break;
	}

	return reval;
}


//------------------------------------------------------------------------
//  pSymbol ���� varId �� ���� ������ ����Ÿ�� �����Ѵ�.
//------------------------------------------------------------------------
CComVariant visualizer::Eval_Variable_FromId( const string &varId, 
												   IDiaSymbol *pSymbol, const SMemoryInfo *pMemInfo )
{
	CComVariant reval;
	RETV( !pSymbol || !pMemInfo, reval );

	LONG offset = 0;
	IDiaSymbol *pChildSymbol = dia::FindChildSymbol(varId, pSymbol, offset);
	RETV( !pChildSymbol, reval);

	enum SymTagEnum symTag;
	HRESULT hr = pChildSymbol->get_symTag((DWORD*)&symTag);
	if (SymTagData == symTag)
	{
		DWORD dwLocType;
		hr = pChildSymbol->get_locationType(&dwLocType);

		if (LocIsConstant == dwLocType) // ��� 
		{
			pChildSymbol->get_value(&reval);
		}
	}

	if (reval.vt == VT_EMPTY)
	{
		BYTE *ptr = (BYTE*)pMemInfo->ptr + offset;
		reval = dia::GetValueFromSymbol( ptr, pChildSymbol, false);
		pChildSymbol->Release();
	}

	return reval;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
IDiaSymbol* visualizer::Find_Variable( SExpression *pexp, IDiaSymbol *pSymbol, 
						  OUT LONG &offset )
{
	IDiaSymbol *reval = NULL;
	RETV(!pexp, reval);
	RETV(pexp->kind != VariableK, reval);

	if (pexp->str == "$e")
	{
		IDiaSymbol *pEachSymbol = CDiaWrapper::Get()->FindType(m_SymbolName);
		if (!pEachSymbol)
			return reval;

		if (pexp->rhs)
		{
			reval = Find_Variable(pexp->rhs, pEachSymbol, offset);
		}
		else
		{
			reval = pEachSymbol;
			offset = 0;
		}
	}
	else if (pexp->str == "$r")
	{

	}
	else if (pexp->str == "$i")
	{

	}
	else
	{
		RETV(!pSymbol, reval);

		if (pexp->rhs)
		{
			LONG childOffset=0;
			IDiaSymbol *pChild = dia::FindChildSymbol(pexp->str, pSymbol, childOffset);
			RETV(!pChild, reval);

			const string typeName = dia::GetSymbolTypeName(pChild); // debug ��
			LONG nextOffset=0;
			reval = Find_Variable(pexp->rhs, pChild, nextOffset);
			offset = childOffset + nextOffset;

			pChild->Release();
		}
		else
		{
			reval = Find_Variable_FromId(pexp->str, pSymbol, offset);
		}
	}

	return reval;
}

//------------------------------------------------------------------------
// pSymbol �ȿ��� varId �� ���� �ɺ��� �����Ѵ�.
//------------------------------------------------------------------------
IDiaSymbol* visualizer::Find_Variable_FromId( const std::string &varId, 
											 IDiaSymbol *pSymbol, OUT LONG &offset )
{
	RETV( !pSymbol, NULL);

	LONG childOffset = 0;
	IDiaSymbol *pChildSymbol = dia::FindChildSymbol(varId, pSymbol, offset);
	RETV( !pChildSymbol, NULL);
	return pChildSymbol;
}


//------------------------------------------------------------------------
// ���б�ȣ�� �����Ѵ�.
//------------------------------------------------------------------------
char visualizer::GetAsciiFromTokentype(Tokentype tok)
{
	char c = NULL;
	switch (tok)
	{
	case PLUS: c = '+'; break;
	case MINUS: c = '-'; break;
	case TIMES: c = '*'; break;
	case DIV: c = '/'; break;
	}
	return c;
}


//------------------------------------------------------------------------
// ���� üũ
// condition �� true �̸� ���� ����
//------------------------------------------------------------------------
void visualizer::CheckError( bool condition, const string &msg ) // msg="error"
{
	if (condition) // success
		return;

	// error occur
	throw VisualizerScriptError( m_SymbolName + " " + msg);
}
