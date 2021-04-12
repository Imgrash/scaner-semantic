#pragma once
#include "defs.h"
#include "scaner.h"

enum TypeObject {
	ObjVar = 1,		// ������� ����������
	ObjFunc,		// �������
	
	ObjConst,		//������������� ���������

	ObjEmpty		//������ ����
};

enum TypeData {
	TypeShortInt = 1,
	TypeLongInt,
	TypeFloat,

	TypeConstInt,
	TypeConstChar
};
union TDataValue {
	short int	dataAsShortInt;
	float		dataAsFloat;
};

class Node;
class Tree;
class TreeControl;

class Node
{
private:
	// ������������� �������
	LEXs   id;
	// ���  ������� (�������/����������/������������� ���������/������ ����)
	TypeObject   ObjectType;
	// ��� (long int/short int/float/constInt/constChar)
	TypeData DataType;
	TDataValue Value;
	// ���������� ���������� �������
	int ParamsQuantity;
	//��������� ���������� �������
	Tree *Params;
	// ���� ������������� ����������
	bool FlagInit;
public:
	//�������� ������ � ���� �������
	TypeObject getObjectType() const;
	//�������� ��� ������
	TypeData getDataType() const;
	//�������� �������� ���������� / ������������ �������� �������
	TDataValue getDataValue() const;
	//���������� �������� ���������� / ������������ �������� �������
	void setDataValue(TDataValue val, TypeData dtype);
	//��� ������� � ���������� �������� �������������
	LEXs getID() const;
	//����������� ��� ����������
	Node(LEXs id_, TypeData dt);
	//��� ���������� - ���������������� ����������
	void InitVar();
	//������, ���������������� �� ����������
	bool getFlagInit() const;

	//����������� ��� �������
	Node(LEXs id_, TypeData dt, Tree *prms, int prmsQuantity);
	//�������� ������ �� ������ ���������� �������
	Tree *getParams();
	//��������� ������� ������ ����������
	void setParams(TreeControl *t);
	//�������� ���������� ���������� �������
	int getParamsQuantity() const;
	//��������� ���������� ���������� ���������� �������
	void incParamsQuantity();

	//����������� ��� ������������� ���������
	Node(TypeData dt);
	//����������� ��� ������� ����
	Node();

	~Node() {};
};

class Tree {
private:
	// ������ �������
	Node * data;          
	// ��������, ����� � ������ �������
	Tree * Up, *Left, *Right;
public:
	//������������
	Tree(Tree * l, Tree * r, Tree * u, Node * Data);
	Tree();
		
		TypeObject getObjectType() const;
		TypeData getDataType() const;
		TDataValue getDataValue() const;
		void setDataValue(TDataValue val, TypeData dtype);

		bool getFlagInit() const;
		void initVar();
		Tree *getParams();
		int getParamsQuantity() const;
		LEXs getID() const;
		
	//��������� ������������ �����
	bool checkIfTypesOK(Tree *checkWithThis) const;
	//�������� ������
	void SetLeft(Node * data);
	//�������� �������
	void SetRight(Node * data);

	void deleteLeft();
	void deleteRight();
	void deleteUp();

	//�������� ������ �� ������
	Tree *getLeft();
	//�������� ������ �� �������
	Tree *getRight();
	//�������� ������ �� ������ ������/��������
	Tree *getUp();
	//����� � ������������� ������ ����, ��� ��� �������� ���� �������������
	Tree *FindUp(Tree * from, Node *id, bool &parentOrHigher);
		
	~Tree() 
	{
		delete(Right);
		delete(Left);
		delete(data);
	};

	Node * getData();
};
//�����, � ������� ������������ �������� ������ ��� ������������� �������
//� ��� �������� ��� ������������� ������
class TreeControl
{
private:
	//������� ���� ������
	Tree *root;
	//�������� ���� ������
	Tree *temp;

	map <TypeData, string> types = {
							 {TypeData::TypeFloat,		"float" },
							 {TypeData::TypeLongInt,	"long int" },
							 {TypeData::TypeShortInt,	"short int" },
	};
	//������ ���������� �� ���� �������
	void printFuncParams(Tree *temp);
	//������ ������
	void printTree(Tree *temp);
public:
	//�����������
	TreeControl();
	//�������� ������
	void AddNeighbour(Node *neighbour, int tabs);
	//�������� �������
	void AddChild(Node *child, int tabs);
	//����� ���� ���� - ����� ����������, ����� ������������� � ���� ����� ��� ����
	Tree *findUp(Node *id, bool &parentOrHigher);	
	//��� ����������� ���������� ����������
	//��� �������� ���������� ������
	int Sem1(LEXs &id, TypeData dt, bool init, int tabs);
	//���� ������� ��� ���������, ���������� ������
	int Sem2_0(Node *newnode);
	//���������� ���� ������� � ���� ������ (���� NULL, ���� ����� ���)
	Tree *Sem3_0(LEXs &id);
	//���������� ���� ���������� � ���� ������ (���� NULL, ���� ����� ���)
	Tree *Sem4_0(LEXs &id);

	//���������� ������� ���� �������������� ������
	Tree * getRoot();
	//���������� ������ �� �������� ����
	Tree *getTemp();

	//������� ��� ���������� ��������� ����
	//(������������ ����� ��������� ����� ��� �����������)
	void setTemp(Tree *newtemp);

	//������ �������������� ������
	void printTC(int tabs);

	~TreeControl() {};

	void deleteUpToEmpty();
};