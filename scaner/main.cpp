#include "scaner.h"
#include "syntax analisator.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	TScaner *sc = new TScaner();
	TDiagram *diagramm = new TDiagram(sc);
	diagramm->maincpp();
	cout << "������� �����-������ ������ ��� ���������� ���������" << endl;
	_getche();
	return 0;
}
