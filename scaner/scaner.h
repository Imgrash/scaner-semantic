#pragma once
#include "defs.h"
#include <map>
using namespace std;

typedef char IM[MaxText]; // текст ИМ
typedef char LEX[MaxLex];
typedef string LEXs;

class TScaner
{
	IM t; // исходный модуль
	int uk; // указатель текущей позиции в ИМ
	int ukLine;
	int ukInLine;
	int line;
	int pos; //строка, позиция
	map <string, int> KW = { {"return", RETURN},
							 {"main", MAIN},
							 {"while", WHILE},
							 {"int", INT},
							 {"short", SHORT},
							 {"long", LONG},
							 {"float",FLOAT}
											};

	map <int, string> ErrorMessages = { 
							 {ERROR_OPEN_COMMENT_IN_THE_END,"Открытый комментарий в конце кода" },
							 {ERROR_UNKNOWN_LEKSEMA,"Неизвестная лексема"},
							 {ERROR_OPEN_CHAR_CONST,"Незакрытая символьная лексема"},
							 {ERROR_WRONG_CHAR_CONST,"Неверная символьная лексема"},
							 {ERROR_LEXEMA_IS_TOO_LONG,"Слишком длинная лексема"}
							};
public:
	TScaner();	
	void SetUK(int newUK,int newUKLine, int newUKInLine);
	int GetUK() const;
	int GetUKLine() const;
	int GetUKInLine() const;
	int GetTypeOfNextLex(LEXs *l);
	void PrintError(int er_type, int i, LEX &l) const; // выдать сообщение об ошибке;
	void PrintError(int oldukLine, int oldUkInLine, int er_type, LEXs &l); // выдать сообщение об ошибке;
	int scan(LEX &l); // основная функция сканера
	int scan(LEXs *l); // основная функция сканера
};