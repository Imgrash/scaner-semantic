#pragma once
#include "defs.h"
#include "scaner.h"
#include "semantic.h"

using namespace std;
class   TDiagram {
private:    // сканер и все функции СД, кроме аксиомы
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
							 {ERROR_EXPECTED_SEMICOLON,				"Ожидалось ;" },
							 {ERROR_EXPECTED_BRACE_OPEN,			"Ожидалось ("},
							 {ERROR_EXPECTED_BRACE_CLOSED,			"Ожидалось )"},
							 {ERROR_EXPECTED_CUR_BRACE_OPEN,		"Ожидалось {"},
							 {ERROR_EXPECTED_CUR_BRACE_CLOSED,		"Ожидалось }"},
							 {ERROR_EXPECTED_IDENT,					"Ожидался идентификатор"},
							 {ERROR_EXPECTED_TYPE,					"Ожидался тип (разрешены типы: short int, long int, float)"},
							 {ERROR_EXPECTED_INT,					"Разрешены типы: short int, long int"},
							 {ERROR_EXPECTED_FUNCTION_OR_ASSIGNING,	"Ожидалась функция или присваивание"},
							 {ERROR_EXPECTED_OPERATOR,				"Ожидался оператор"},
							 {ERROR_EXPECTED_EXPRESSION,			"Ожидалось выражение"},
							 {ERROR_EXPECTED_SEMICOLON_OR_ASSIGN,	"Ожидалось присваивание или ;"},
							 {ERROR_FUNC_NO_RETURNS,				"Функция не возвращает значений"},
							 {ERROR_MAIN_IS_FUNC,					"Идентификатор main может использоваться только для описания главной функции программы"},
							 {ERROR_MAIN_ISNT_DEF,					"Не объявлена главная функция main"},
							 {ERROR_EXPECTED_OPER_OR_VAR,			"Ожидался оператор или переменная"},

							 {T_FUNC_ALREADY_EXISTS,				"Функция уже объявлена" },
							 {T_VAR_ALREADY_EXISTS,					"Переменная уже объявлена"},
							 
							 {ERROR_FUNC_DOES_NOT_EXIST,			"Функция не объявлена"},
							 {ERROR_VAR_DOES_NOT_EXIST,				"Переменная не объявлена"},
							 {ERROR_VAR_IS_NOT_INIT,				"Переменная не инициализирована"},
							 {ERROR_FUNC_HAS_MORE_PARAMS,			"Слишком мало параметров"},
							 {ERROR_FUNC_HAS_LESS_PARAMS,			"Слишком много параметров"},
							 {ERROR_TYPES_DONT_MATCH,				"Приведение типов невозможно"},
							 {ERROR_TYPES_DONT_MATCH_RET,			"Тип возвращаемого значения невозможно привести к типу функции"}
	};

public:     //   из СД   –  только  аксиома
	TDiagram(TScaner * s): sc (s), lex ( new LEXs()), tabs (-1), TC (new TreeControl()) {}
	~TDiagram() { 
		delete(lex);
		delete(TC);
	}
	void maincpp(); // программа  - аксиома КСГ
};
