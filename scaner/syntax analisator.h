#pragma once
#include "defs.h"
#include "scaner.h"
#include "semantic.h"

using namespace std;
class   TDiagram {
private:    // ������ � ��� ������� ��, ����� �������
	TScaner *sc;
	void  functionDef1(LEXs &nameIdent, TypeData dt);
	void  listInFuncDef14(TreeControl *listInFuncTree, Node *funcNode);
	TypeData  type2();
	void  list3(TypeData dt);
	Tree *variable4(TypeData dt, bool init);
	void  parameters6(Tree *T);
	void  while7(Tree *funcNode);
	Tree *expression9();
	Tree *shift_res10();
	Tree *addendum11();
	Tree *multiplier12();
	Tree *simple_expression13();
	void return_operator16(Tree *funcNode);
	Tree *parameter17();

	int  operator5(bool makeChild, Tree *funcNode);
	int  opers_desrps8(Tree *funcNode);
	int  sostavnoi_operator15(Tree *funcNode);

	void PrintError(int i);

	LEXs *lex;
	int type;
	int uk, ukLine, ukInLine;
	int ukTemp, ukLineTemp, ukInLineTemp;
	TreeControl *TC;
	int tabs;

	map <int, string> ErrorMessages = {
							 {ERROR_EXPECTED_SEMICOLON,				"��������� ;" },
							 {ERROR_EXPECTED_BRACE_OPEN,			"��������� ("},
							 {ERROR_EXPECTED_BRACE_CLOSED,			"��������� )"},
							 {ERROR_EXPECTED_CUR_BRACE_OPEN,		"��������� {"},
							 {ERROR_EXPECTED_CUR_BRACE_CLOSED,		"��������� }"},
							 {ERROR_EXPECTED_IDENT,					"�������� �������������"},
							 {ERROR_EXPECTED_TYPE,					"�������� ��� (��������� ����: short int, long int, float)"},
							 {ERROR_EXPECTED_INT,					"��������� ����: short int, long int"},
							 {ERROR_EXPECTED_FUNCTION_OR_ASSIGNING,	"��������� ������� ��� ������������"},
							 {ERROR_EXPECTED_OPERATOR,				"�������� ��������"},
							 {ERROR_EXPECTED_EXPRESSION,			"��������� ���������"},
							 {ERROR_EXPECTED_SEMICOLON_OR_ASSIGN,	"��������� ������������ ��� ;"},
							 {ERROR_FUNC_NO_RETURNS,				"������� �� ���������� ��������"},
							 {ERROR_MAIN_IS_FUNC,					"������������� main ����� �������������� ������ ��� �������� ������� ������� ���������"},
							 {ERROR_MAIN_ISNT_DEF,					"�� ��������� ������� ������� main"},
							 {ERROR_EXPECTED_OPER_OR_VAR,			"�������� �������� ��� ����������"},

							 {T_FUNC_ALREADY_EXISTS,				"������� ��� ���������" },
							 {T_VAR_ALREADY_EXISTS,					"���������� ��� ���������"},
							 
							 {ERROR_FUNC_DOES_NOT_EXIST,			"������� �� ���������"},
							 {ERROR_VAR_DOES_NOT_EXIST,				"���������� �� ���������"},
							 {ERROR_VAR_IS_NOT_INIT,				"���������� �� ����������������"},
							 {ERROR_FUNC_HAS_MORE_PARAMS,			"������� ���� ����������"},
							 {ERROR_FUNC_HAS_LESS_PARAMS,			"������� ����� ����������"},
							 {ERROR_TYPES_DONT_MATCH,				"���������� ����� ����������"},
							 {ERROR_TYPES_DONT_MATCH_RET,			"��� ������������� �������� ���������� �������� � ���� �������"}
	};

public:     //   �� ��   �  ������  �������
	TDiagram(TScaner * s): sc (s), lex ( new LEXs()), tabs (-1), TC (new TreeControl()) {}
	~TDiagram() { 
		delete(lex);
		delete(TC);
	}
	void maincpp(); // ���������  - ������� ���
};
