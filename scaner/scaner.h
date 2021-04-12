#pragma once
#include "defs.h"
#include <map>
using namespace std;

typedef char IM[MaxText]; // ����� ��
typedef char LEX[MaxLex];
typedef string LEXs;

class TScaner
{
	IM t; // �������� ������
	int uk; // ��������� ������� ������� � ��
	int ukLine;
	int ukInLine;
	int line;
	int pos; //������, �������
	map <string, int> KW = { {"return", RETURN},
							 {"main", MAIN},
							 {"while", WHILE},
							 {"int", INT},
							 {"short", SHORT},
							 {"long", LONG},
							 {"float",FLOAT}
											};

	map <int, string> ErrorMessages = { 
							 {ERROR_OPEN_COMMENT_IN_THE_END,"�������� ����������� � ����� ����" },
							 {ERROR_UNKNOWN_LEKSEMA,"����������� �������"},
							 {ERROR_OPEN_CHAR_CONST,"���������� ���������� �������"},
							 {ERROR_WRONG_CHAR_CONST,"�������� ���������� �������"},
							 {ERROR_LEXEMA_IS_TOO_LONG,"������� ������� �������"}
							};
public:
	TScaner();	
	void SetUK(int newUK,int newUKLine, int newUKInLine);
	int GetUK() const;
	int GetUKLine() const;
	int GetUKInLine() const;
	int GetTypeOfNextLex(LEXs *l);
	void PrintError(int er_type, int i, LEX &l) const; // ������ ��������� �� ������;
	void PrintError(int oldukLine, int oldUkInLine, int er_type, LEXs &l); // ������ ��������� �� ������;
	int scan(LEX &l); // �������� ������� �������
	int scan(LEXs *l); // �������� ������� �������
};