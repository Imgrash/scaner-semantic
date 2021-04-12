#include "syntax analisator.h"

void TDiagram::PrintError(int er_type)
{
	cout << endl << "________________________________________________" << endl;
	cout << "������: " << endl;
		type = sc->GetTypeOfNextLex(lex);
		cout << "\"" << *lex << "\": " << endl;
	cout << " ������: " << sc->GetUKLine() << " �������: " << sc->GetUKInLine() << endl;
	map <int, string>::iterator it;
	it = ErrorMessages.find(er_type);
	if(it->first != ERROR_FUNC_NO_RETURNS)
		cout << it->second << endl;
	else
		cout << "������� \"" << *lex << "\" �� ���������� ��������" << endl;
	cout << "________________________________________________" << endl;
	cout << "������� �����-������ ������ ��� ���������� ���������" << endl;
	_getche();
	exit(1);
}

void TDiagram::maincpp()
{
	bool mainIsDef = false;
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type != TYPE_END)
	{
		//��������� ������� �� ������ �������� ";"
		if (type == TYPE_SEMICOLON)	{;}
		else
		{
			sc->SetUK(uk, ukLine, ukInLine);
			TypeData dt = type2();
			int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
			
			ukTemp = sc->GetUK(); ukLineTemp = sc->GetUKLine(); ukInLineTemp = sc->GetUKInLine();
			type = sc->scan(lex);

			LEXs nameIdent = *lex;

			//���� ��� ���������� main
			if (type == MAIN)
			{
				mainIsDef = true;
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				//���� ������� �������� - ��� ���������� �������
				if (type == TYPE_BRACE_OPEN)
				{
					functionDef1(nameIdent,dt);
				}
				else
				{
					sc->SetUK(ukTemp, ukLineTemp, ukInLineTemp);	PrintError(ERROR_MAIN_IS_FUNC);
				}
			}
			else if (type == TYPE_IDENT)
			{
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				//���� ������� �������� - ��� ���������� �������
				if (type == TYPE_BRACE_OPEN)
				{	functionDef1(nameIdent,dt);}
				//���� ������� ������� ��� ����� - ��� ������
				else if (type == TYPE_COMMA || type == TYPE_ASSIGN)
				{
					sc->SetUK(ukT, ukTl, ukTil);
					//��������� ������
					list3(dt);
					//������ ������ ������������� ������ � �������
					uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
					type = sc->scan(lex);
					if (type != TYPE_SEMICOLON)
					{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON);}
				}
				//���� ������� ����� � ������� - ��� ���������� ����� ����������, ������ ��������� ������
				else if (type == TYPE_SEMICOLON) 
				{
					int er = TC->Sem1(nameIdent,dt,false,tabs); 
					if (er)
					{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(er);}
				}
				//����� ������, ��� ��� ���������� ������ ��������������, ����� �������� ��������� ����� � �������
				else
				{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON);}
			}
			else
			{	sc->SetUK(ukTemp, ukLineTemp, ukInLineTemp);	PrintError(ERROR_EXPECTED_IDENT);}
			//���� ������� ������������� - ��� ������ ������ ��� ���������� �������	
			
		}
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	if(!mainIsDef)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_MAIN_ISNT_DEF);}
	cout << "��������� ��������� �����" << endl;
	TC->printTC(0);
}

void TDiagram::functionDef1(LEXs &nameIdent, TypeData dt)
{
	Node *newnode = new Node(nameIdent, dt, NULL, 0);
	int er = TC->Sem2_0(newnode);
	if (er)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(er);}
	//� ������� ����� �� ���� ����������
	//�� �� ������ ���������, ���� �� ���
	TreeControl *listInFuncTree = new TreeControl();
	uk = sc->GetUK();ukLine = sc->GetUKLine();ukInLine = sc->GetUKInLine();		type = sc->scan(lex);
	sc->SetUK(uk, ukLine, ukInLine);
	//���� ������ �� ��������� �����, �� ������ ������ ���� ���������
	if (type != TYPE_BRACE_CLOSED)
	{	listInFuncDef14(listInFuncTree,newnode);}

	//����� ���������� ����� ������ ����������� ������
	uk = sc->GetUK();ukLine = sc->GetUKLine();ukInLine = sc->GetUKInLine();		type = sc->scan(lex);
	if (type != TYPE_BRACE_CLOSED)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED);}

	TC->AddNeighbour(newnode,tabs);
	Tree *funcNode = TC->getTemp();
	
	Tree *oldTemp = TC->getTemp();

	tabs++;
	for (int tabsi = 0; tabsi <= tabs; tabsi++)
		cout << "\t";
	cout << "�������� ���������� ���������" << endl;
	TC->AddChild(new Node(),tabs); //�������� ���������� ���������

	//����� ������ ������ ���� ��������� ��������, ������� ����������
	//� �������� ������
	uk = sc->GetUK();ukLine = sc->GetUKLine();ukInLine = sc->GetUKInLine();		type = sc->scan(lex);
	if (type != TYPE_CUR_BR_OPEN)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_CUR_BRACE_OPEN);}

int returnQuantity = sostavnoi_operator15(funcNode);
	if (returnQuantity == 0)
	{	sc->SetUK(ukTemp, ukLineTemp, ukInLineTemp);	PrintError(ERROR_FUNC_NO_RETURNS);}

	TC->deleteUpToEmpty();	
	for (int tabsi = 0; tabsi <= tabs; tabsi++)
		cout << "\t";
	cout << "�������� ���������� ���������:" << endl;
	TC->printTC(tabs);
	cout << endl;
	tabs--;
}

TypeData TDiagram::type2()
{
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	TypeData ret;
	if (type == SHORT || type == LONG || type == FLOAT)
	{
		if (type == SHORT)
			ret = TypeData::TypeShortInt;
		else if (type == LONG)
			ret = TypeData::TypeLongInt;
		else
			ret = TypeData::TypeFloat;

		if (type == SHORT || type == LONG)
			type = sc->scan(lex);
		if (type != INT && type != FLOAT)
		{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_TYPE);}
	}
	else
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_TYPE);}
	return ret;
}

void TDiagram::list3(TypeData dt)
{
	variable4(dt,false);
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);

	while (type == TYPE_COMMA)
	{
		variable4(dt,false);
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	} 
	sc->SetUK(uk, ukLine, ukInLine);
}

Tree *TDiagram::variable4(TypeData dt, bool init)
{
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type != TYPE_IDENT)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_IDENT);}
	int er = TC->Sem1(*lex, dt, init,tabs);
	if (er)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(er);}

	Tree *var = TC->getTemp();

	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type == TYPE_ASSIGN)
	{	
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *tr = expression9();
		if (var->checkIfTypesOK(tr))
		{	var->initVar();}
		else
		{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH);}
	}
	else
	{	sc->SetUK(uk, ukLine, ukInLine);}

	return var;
}

Tree *TDiagram::parameter17()
{
	Tree *param;

	int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	TypeData dt;
	
	if (type == FLOAT || type == SHORT || type == LONG)
	{
		if (type == SHORT || type == LONG)
		{
			if (type == SHORT)
				dt = TypeData::TypeShortInt;
			else
				dt = TypeData::TypeLongInt;
			type = sc->scan(lex);
		}
		else
			dt = TypeData::TypeFloat;
		if (type != INT && type != FLOAT)
		{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_TYPE);}
		int uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		param = variable4(dt,false);
		if (!param->getFlagInit())
		{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_VAR_IS_NOT_INIT);}
	}
	else if (type == TYPE_IDENT || type == MAIN)
	{
		LEXs name = *lex;
		Tree *T = TC->Sem3_0(name);

		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		
		//���� ������� ������ - �� ��� �������
		if (type == TYPE_BRACE_OPEN)
		{			
			if (T == NULL || T->getObjectType() != TypeObject::ObjFunc)
			{	sc->SetUK(ukT, ukTl, ukTil);	PrintError(ERROR_FUNC_DOES_NOT_EXIST);}
			//� ������� ����� �� ���� ����������
			//�� �� ������ ���������, ���� �� ���
			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);
			
			//���� ������ �� ��������� �����, �� ������ ������ ���� ���������
			if (type != TYPE_BRACE_CLOSED)
			{
				if (T->getParamsQuantity() == 0)
				{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_LESS_PARAMS);}
				
				sc->SetUK(uk, ukLine, ukInLine); 				
				parameters6(T);

				//����� ���������� ����� ������ ����������� ������
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				if (type != TYPE_BRACE_CLOSED)
				{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED);}
			}
			else if (T->getParamsQuantity() != 0)
			{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_MORE_PARAMS);}
			param = T;
		}
		//����� ��� ����� ���� ������������
		else if (type == TYPE_ASSIGN)
		{	
			if (T == NULL || T->getObjectType() != TypeObject::ObjVar)
			{	sc->SetUK(ukT, ukTl, ukTil);	PrintError(ERROR_VAR_DOES_NOT_EXIST);}

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			param = expression9();

			if (!T->checkIfTypesOK(param))
			{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_TYPES_DONT_MATCH);}
			param->initVar();
		}
		//����� ��� ���� ����������
		else
		{
			sc->SetUK(ukT, ukTl, ukTil);
			param = expression9();
		}
	}
	//����� ��� ���������
	else
	{
		//��������������� ��������� �� ��������� �������
		sc->SetUK(uk, ukLine, ukInLine);
		//��������� �������
		param = expression9();
	}
	return param;
}

void TDiagram::parameters6(Tree *T)
{
	int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
	Tree *tempParamInFunc = T->getParams();

	Tree *param = parameter17();
	if (!tempParamInFunc->checkIfTypesOK(param))
	{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH);}

	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type == TYPE_COMMA)
	{
		ukT = sc->GetUK(); ukTl = sc->GetUKLine(); ukTil = sc->GetUKInLine();
		
		tempParamInFunc = tempParamInFunc->getLeft();
		if (tempParamInFunc == NULL)
		{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_FUNC_HAS_LESS_PARAMS);}
		
		param = parameter17();
		if (!tempParamInFunc->checkIfTypesOK(param))
		{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH);}

		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);

	//����� ���������� ���������� ���������, ���� ���������, �������� �� �
	//���������� ������� ��� ���������.
	tempParamInFunc = tempParamInFunc->getLeft();
	if (tempParamInFunc != NULL)
	{	PrintError(ERROR_FUNC_HAS_MORE_PARAMS);}
}

void TDiagram::while7(Tree *funcNode)
{
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	//��� ������� "while", ������ ������ ���� "("
	if (type != TYPE_BRACE_OPEN)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_OPEN);}
	
	//������ ������ ���� ���������, ���� ��� ���, ������� ������
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	sc->SetUK(uk, ukLine, ukInLine);
	if (type == TYPE_BRACE_CLOSED)
	{	PrintError(ERROR_EXPECTED_EXPRESSION);}

	TC->AddNeighbour(new Node(),tabs);
	Tree *oldTemp = TC->getTemp();
	tabs++;
	for (int tabsi = 0; tabsi <= tabs; tabsi++)
		cout << "\t";
	cout << "�������� ���������� ���������" << endl;
	TC->AddChild(new Node(),tabs);//�������� ���������� ���������
	
	//��������� ���������
	parameter17();

	//������ ������ ���� ����������� ������
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type != TYPE_BRACE_CLOSED)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED);}
	//� ������� �������� � �����
	operator5(false,funcNode);

	TC->deleteUpToEmpty();
	for (int tabsi = 0; tabsi <= tabs; tabsi++)
		cout << "\t";
	cout << "�������� ���������� ���������:" << endl;
	TC->printTC(tabs);
	cout << endl;
	tabs--;
	//TC->setTemp(oldTemp);//�������� ���������� ���������
}

int TDiagram::operator5(bool makeChild,Tree *funcNode)
{
	int returnQuantity = 0;
	int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	//���� ��� ����� � �������, �� ������ �� ������, � ��������� ������������ ����
	if (type == TYPE_SEMICOLON){;}
	else if (type == RETURN)
	{
		return_operator16(funcNode);
		returnQuantity++;
	}
	else if (type == WHILE)
	{	while7(funcNode);}
	else if (type == TYPE_CUR_BR_OPEN)
	{	
		Tree *oldTemp;
		if (makeChild)
		{
			TC->AddNeighbour(new Node(),tabs);
			oldTemp = TC->getTemp();
			tabs++;
			for (int tabsi = 0; tabsi <= tabs; tabsi++)
				cout << "\t";
			cout << "�������� ���������� ���������" << endl;
			TC->AddChild(new Node(),tabs); //�������� ���������� ���������

			returnQuantity += sostavnoi_operator15(funcNode);

			TC->deleteUpToEmpty();
			for (int tabsi = 0; tabsi <= tabs; tabsi++)
				cout << "\t";
			cout << "�������� ���������� ���������:" << endl;
			TC->printTC(tabs);
			cout << endl;
			tabs--;
			//TC->setTemp(oldTemp); //�������� ���������� ���������
		}
		else
			returnQuantity += sostavnoi_operator15(funcNode);
	}
	//���� ������ ���� �������������, �� ��� ���� ���������� ���������� ��������,
	//���� ����� �������
	else if (type == TYPE_IDENT || type == MAIN)
	{		
		LEXs name = *lex;
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		//���� ������� ������ - �� ��� �������
		if (type == TYPE_BRACE_OPEN)
		{
			Tree *T = TC->Sem3_0(name);
			if (T == NULL || T->getObjectType() != TypeObject::ObjFunc)
			{	sc->SetUK(ukT, ukTl, ukTil);	PrintError(ERROR_FUNC_DOES_NOT_EXIST);}
			//� ������� ����� �� ���� ����������
			//�� �� ������ ���������, ���� �� ���
			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);
			
			//���� ������ �� ��������� �����, �� ������ ������ ���� ���������
			if (type != TYPE_BRACE_CLOSED)
			{
				if (T->getParamsQuantity() == 0)
				{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_LESS_PARAMS);}
				
				sc->SetUK(uk, ukLine, ukInLine); 				
				parameters6(T);

				//����� ���������� ����� ������ ����������� ������
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				if (type != TYPE_BRACE_CLOSED)
				{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED);}
			}
			else if (T->getParamsQuantity() != 0)
			{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_MORE_PARAMS);}
			
		}
		//����� ��� ������ ���� ���������� 
		else if (type == TYPE_ASSIGN)
		{
			Tree *expr = TC->Sem4_0(name);
			if (expr == NULL || expr->getObjectType() != TypeObject::ObjVar)
			{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_VAR_DOES_NOT_EXIST);}

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();

			if (!expr->checkIfTypesOK(expression9()))
			{	sc->SetUK(uk, ukLine, ukInLine); PrintError(ERROR_TYPES_DONT_MATCH);}
			expr->initVar();
		}
		else
		{	sc->SetUK(ukTemp, ukLineTemp, ukInLineTemp);	PrintError(ERROR_EXPECTED_FUNCTION_OR_ASSIGNING);}
		//� ������������� ��� ������ � �������
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		if (type != TYPE_SEMICOLON)
		{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON);}
	}
	else
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_OPERATOR);}
	return returnQuantity;
}

int TDiagram::opers_desrps8(Tree *funcNode)
{
	int returnQuantity = 0;
	
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type != TYPE_CUR_BR_CLOSED)
	{
		//���� ������� ����� ���, �� 
		//��� ������ ���� ���������� ����������/������
		if (type == SHORT || type == LONG || type == FLOAT)
		{
			TypeData dt;
			if (type == SHORT || type == LONG)
			{
				if (type == SHORT)

					dt = TypeData::TypeShortInt;
				else
					dt = TypeData::TypeLongInt;
				type = sc->scan(lex);
			}
			else
				dt = TypeData::TypeFloat;
			if (type != INT && type != FLOAT)
			{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_TYPE);}			
			int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);

			LEXs nameIdent = *lex;
			//���� ������� ������������� - ��� ������ ������
			//������� �� ����
			if (type != TYPE_IDENT)
			{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_IDENT);}

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);
			if (type == TYPE_COMMA || type == TYPE_ASSIGN)
			{
				sc->SetUK(ukT, ukTl, ukTil);
				//��������� ������
				list3(dt);
				//������ ������ ������������� ������ � �������
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				if (type != TYPE_SEMICOLON)
				{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON);}
			}
			//���� ������� ����� � ������� - ��� ���������� ����� ����������, ������ ��������� ������
			else if (type == TYPE_SEMICOLON)
			{
				int er = TC->Sem1(nameIdent, dt, false,tabs);
				if (er)
				{	sc->SetUK(ukT, ukTl, ukTil);	PrintError(er);}
			}
			//����� ������, ��� ��� ���������� ������ ��������������, ����� �������� ��������� ����� � �������
			else
			{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON);}
		}
		//���� ��� ��������
		else if (type == TYPE_SEMICOLON || type == RETURN || type == WHILE || type == TYPE_CUR_BR_OPEN || type == TYPE_IDENT || type == MAIN)
		{	
			sc->SetUK(uk, ukLine, ukInLine);	
			returnQuantity += operator5(true, funcNode);
		}
		//���� ������
		else 
		{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_OPER_OR_VAR);}
		//��������� ��������� �������
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);
	return returnQuantity;
}

Tree * TDiagram::expression9()
{
	Tree *expr1 = shift_res10();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type == TYPE_GT || type == TYPE_GE || type == TYPE_LT || type == TYPE_LE || type == TYPE_EQ || type == TYPE_NEQ)
	{
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *expr2 = shift_res10();
		if (!expr1->checkIfTypesOK(expr2))
		{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH);}
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);
	return expr1;
}

Tree * TDiagram::shift_res10()
{
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type != TYPE_PLUS && type != TYPE_MINUS)
	{	sc->SetUK(uk, ukLine, ukInLine);}
	
	Tree *expr1 = addendum11();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type == TYPE_LS || type == TYPE_GS)
	{
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *expr2 = addendum11();
		if (!expr1->checkIfTypesOK(expr2))
		{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH);}

		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	} 
	sc->SetUK(uk, ukLine, ukInLine);
	return expr1;
}

Tree * TDiagram::addendum11()
{
	Tree *expr1 = multiplier12();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);

	while (type == TYPE_PLUS || type == TYPE_MINUS)
	{
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *expr2 = multiplier12();
		if (!expr1->checkIfTypesOK(expr2))
		{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH);}
		
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	} 
	sc->SetUK(uk, ukLine, ukInLine);
	return expr1;
}

Tree * TDiagram::multiplier12()
{
	Tree *expr1 = simple_expression13();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);

	while (type == TYPE_MUL || type == TYPE_DIV)
	{
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *expr2 = simple_expression13();
		if (!expr1->checkIfTypesOK(expr2))
		{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH);}
		
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	} 
	sc->SetUK(uk, ukLine, ukInLine);
	return expr1;
}

Tree * TDiagram::simple_expression13()
{
	Tree *simple_expr = NULL;
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	int ukT = uk, ukTl = ukLine, ukTil = ukInLine;
	type = sc->scan(lex);
	if (type != TYPE_CONST_10 && type != TYPE_CONST_16 && type != TYPE_CONST_8 && type != TYPE_CONST_2 && type != TYPE_CONST_CHAR)
	{
		if (type == TYPE_BRACE_OPEN)
		{
			simple_expr = expression9();
			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);
			if (type != TYPE_BRACE_CLOSED)
			{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED);}
		}
		else if (type == TYPE_IDENT || type == MAIN)
		{
			LEXs name = *lex;
			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);
			//���� ������� ������ - �� ��� �������
			if (type == TYPE_BRACE_OPEN)
			{
				Tree *T = TC->Sem3_0(name);
				if (T == NULL || T->getObjectType() != TypeObject::ObjFunc)
				{	sc->SetUK(ukT, ukTl, ukTil);	PrintError(ERROR_FUNC_DOES_NOT_EXIST);}
				//� ������� ����� �� ���� ����������
				//�� �� ������ ���������, ���� �� ���
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);

				//���� ������ �� ��������� �����, �� ������ ������ ���� ���������
				if (type != TYPE_BRACE_CLOSED)
				{
					if (T->getParamsQuantity() == 0)
					{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_LESS_PARAMS);}

					sc->SetUK(uk, ukLine, ukInLine);
					parameters6(T);

					//����� ���������� ����� ������ ����������� ������
					uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
					type = sc->scan(lex);
					if (type != TYPE_BRACE_CLOSED)
					{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED);}
				}
				else if (T->getParamsQuantity() != 0)
				{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_MORE_PARAMS);}
				simple_expr = T;
			}
			//����� ��� ������ ����������, ���������� ��������� ����� ����������
			else
			{
				sc->SetUK(ukT, ukTl, ukTil);
				sc->scan(lex);
				//� ����� ������� � ��� ��������� ����� ����� ����������
				//� � lex ������ ������������� ����������
				simple_expr = TC->Sem4_0(*lex);
				if (simple_expr == NULL || simple_expr->getObjectType() != TypeObject::ObjVar)
				{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_VAR_DOES_NOT_EXIST);}
				else if (!simple_expr->getFlagInit())
				{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_VAR_IS_NOT_INIT);}
			}
		}
		else
		{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_EXPRESSION);}
	}
	else
	{
		if (type == TYPE_CONST_10 || type == TYPE_CONST_16 || type == TYPE_CONST_8 || type == TYPE_CONST_2)
		{	simple_expr = new Tree(NULL,NULL,NULL,new Node(TypeData::TypeConstInt));}
		else if (type != TYPE_CONST_CHAR)
		{	simple_expr = new Tree(NULL, NULL, NULL, new Node(TypeData::TypeConstChar));}
	}
	return simple_expr;
}

void TDiagram::listInFuncDef14(TreeControl *listInFuncTree, Node *funcNode)
{
	cout << endl;
	for (int tabsi = 0; tabsi <= tabs; tabsi++)
		cout << '\t';
	cout << "��������� ������ ���������� ��� ���������� �������" << endl;
	TreeControl *oldTree = TC;
	TC = listInFuncTree;

	TypeData dt = type2();
	variable4(dt,true);
	funcNode->incParamsQuantity();

	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type == TYPE_COMMA)
	{
		dt = type2();
		variable4(dt,true);
		funcNode->incParamsQuantity();

		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);

	funcNode->setParams(TC);
	TC = oldTree;
	for (int tabsi = 0; tabsi <= tabs; tabsi++)
		cout << '\t';
	cout << "������ ���������� ��� ���������� ������� ������" << endl << endl;
}

int TDiagram::sostavnoi_operator15(Tree *funcNode)
{
	int returnQuantity = 0;
	//����� ��������� ���������� ��� ������� �������� ��������
	//������� �������� ����� � ������������� ���������
	returnQuantity = opers_desrps8(funcNode);
	//� ������������� �� �������� ���������
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type != TYPE_CUR_BR_CLOSED)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_CUR_BRACE_CLOSED);}
	return returnQuantity;
}

void TDiagram::return_operator16(Tree *funcNode)
{
	Tree *param;
	//��� ������ ���� ������� "return", ������� �������, ��� �� ���
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	int ukT = uk, ukTl = ukLine, ukTil = ukInLine;
	type = sc->scan(lex);
	//�� ��� ����� ���� ���� ����������, ���� ������������ ���������
	//���� �������������, �� ��� ����� ���� ���� ����������, ���� ������ ����������, ���� �������
	if (type == TYPE_IDENT || type == MAIN)
	{
		LEXs name = *lex;
		
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		//���� ������� =, �� ��� ����������
		if (type == TYPE_ASSIGN)
		{	
			param = TC->Sem4_0(name);
			if (param == NULL || param->getObjectType() != TypeObject::ObjVar)
			{	sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_VAR_DOES_NOT_EXIST);}

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();

			if ( !param->checkIfTypesOK(expression9()) )
			{	sc->SetUK(uk, ukLine, ukInLine); PrintError(ERROR_TYPES_DONT_MATCH);}
			param->initVar();
		}
		//����� ��� ��������� 
		else
		{	
			sc->SetUK(ukT, ukTl, ukTil); 
			param = expression9();
		}
	}
	//����� ��� ���������
	else
	{
		sc->SetUK(ukT, ukTl, ukTil);
		param = expression9();
	}

	if (!funcNode->checkIfTypesOK(param))
	{
		sc->SetUK(ukT, ukTl, ukTil);  PrintError(ERROR_TYPES_DONT_MATCH_RET);
	}

	//����� ������������� �������� ����������� ������ ������ ";"
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type != TYPE_SEMICOLON)
	{	sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON);}
}
