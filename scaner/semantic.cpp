#include "semantic.h"
///////////////////////////////////////////////////
//                 Node                          //
///////////////////////////////////////////////////
//конструктор дл€ переменной
Node::Node(LEXs id_, TypeData dt) 
	:id(id_), DataType(dt), ObjectType(TypeObject::ObjVar),
	 Params(NULL), ParamsQuantity(0), FlagInit(false)
{
	Value.dataAsFloat = 0;
}
//конструктор дл€ функции
Node::Node(LEXs id_, TypeData dt, Tree *prms, int prmsQuantity) 
	:id(id_), DataType(dt), ObjectType(TypeObject::ObjVar),
	 Params(prms), ParamsQuantity(prmsQuantity), FlagInit(false)
{
	Value.dataAsFloat = 0;
}

void Node::InitVar()
{
	FlagInit = true;
}

bool Node::getFlagInit() const
{
	return FlagInit;
}
void Node::setDataValue(TDataValue val, TypeData dtype)
{
	switch (dtype)
	{
	case TypeData::TypeFloat:
		Value.dataAsFloat = val.dataAsFloat;
		break;
	case TypeData::TypeShortInt:
		Value.dataAsShortInt = val.dataAsShortInt;
		break;
	default:
		Value.dataAsShortInt = val.dataAsShortInt;
		break;
	}
}
Tree *Node::getParams()
{
	return Params;
}
int Node::getParamsQuantity() const 
{
	return ParamsQuantity;
}
TypeObject Node::getObjectType() const 
{
	return ObjectType;
}
TypeData Node::getDataType() const
{
	return DataType;
}
TDataValue Node::getDataValue()const 
{
	return Value;
}
LEXs Node::getID() const
{
	return id;
}
//конструктор дл€ неименованной константы
Node::Node(TypeData dt)
	:DataType(dt),ParamsQuantity(0),Params(NULL),FlagInit(false)
{
	Value.dataAsShortInt = 0;
	ObjectType = TypeObject::ObjConst;
}
//конструктор дл€ пустого узла
Node::Node()
	:DataType(TypeData::TypeShortInt), ParamsQuantity(0), Params(NULL), FlagInit(false)
{
	Value.dataAsShortInt = 0;
	ObjectType = TypeObject::ObjEmpty;
}
void Node::incParamsQuantity()
{
	ParamsQuantity++;
}
void Node::setParams(TreeControl *t)
{
	Params = t->getRoot();
}

///////////////////////////////////////////////////
//                 Tree                          //
///////////////////////////////////////////////////
TypeData Tree::getDataType() const
{
	return data->getDataType();
}
TDataValue Tree::getDataValue() const
{
	return data->getDataValue();
}
void Tree::setDataValue(TDataValue val, TypeData dtype)
{
	data->setDataValue(val, dtype);
}
LEXs Tree::getID() const
{
	if (data->getObjectType() != TypeObject::ObjEmpty)
		return data->getID();
	else
		return "*";
}
Tree::Tree(Tree * l, Tree * r, Tree * u, Node * Data)
{
	Left = l;
	Right = r;
	Up = u;
	data = Data;
}
Tree::Tree()
{
	Left = nullptr;
	Right = nullptr;
	Up = nullptr;
	data = new Node();
}

void Tree::deleteLeft()
{
	Left = nullptr;
}
void Tree::deleteRight()
{
	Right = nullptr;
}
void Tree::deleteUp()
{
	Up = nullptr;
}

void Tree::SetLeft(Node * data_)
{
	Left = new Tree(nullptr, nullptr, this, data_);
}
void Tree::SetRight(Node * data_)
{
	Right = new Tree(nullptr, nullptr, this, data_);
}
Tree *  Tree::FindUp(Tree * from, Node *id, bool &parentOrHigher)
{
	parentOrHigher = false;
	Tree *temp = from;
	if (id->getObjectType() == TypeObject::ObjVar)
	{
		while (temp != nullptr)
		{
			if ( (temp->data->getObjectType() != TypeObject::ObjEmpty) && (temp->data->getID() == id->getID()) )
				return temp;
			//если сверху еще остались узлы
			if (temp->Up != nullptr)
			{
				//если сверху родительский узел, отмечаем, что он в блоке выше
				// (чтобы знать, можем мы переобъ€вл€ть идентификатор или нет)
				if (temp->Up->Right == temp)
					parentOrHigher = true;
				if ( 
					(temp->Up->data->getObjectType() == TypeObject::ObjFunc) && //если сверху узел функции
					(temp->Up->Right == temp) && //если этот узел функции €вл€етс€ родительским
					(temp->Up->data->getParamsQuantity()) //если у этой функции есть параметры
				)
				{
					//просматриваем параметры функции - может там найдетс€ нужный узел
					Tree *temp2 = temp->Up->data->getParams();
					while (temp2 != nullptr)
					{
						if (temp2->data->getID() == id->getID())
							return temp2;
						temp2 = temp2->Left;
					}
				}
			}

			temp = temp->Up;
		}
	}
	else //иначе это точно функци€
	{
		//у функций не может быть родителей, так что мы точно не можем ее переобъ€вл€ть
		while (temp != nullptr)
		{
			if (
				(temp->data->getObjectType() != TypeObject::ObjEmpty) &&
				(temp->data->getID() == id->getID())
			)
				return temp;
			temp = temp->Up;
		}
	}
	return nullptr;
}

TypeObject Tree::getObjectType() const
{
	return data->getObjectType();
}
bool Tree::checkIfTypesOK(Tree *checkWithThis) const
{
	//позже возможны изменени€, но сейчас все типы приводимы друг к другу
	return true;
}
bool Tree::getFlagInit() const
{
	return data->getFlagInit();
}
void Tree::initVar()
{
	data->InitVar();
}
Tree *Tree::getLeft()
{
	return Left;
}
Tree *Tree::getRight()
{
	return Right;
}
Tree *Tree::getUp()
{
	return Up;
}
Tree *Tree::getParams()
{
	return data->getParams();
}
int Tree::getParamsQuantity() const
{
	return data->getParamsQuantity();
}
Node * Tree::getData()
{
	return data;
}

///////////////////////////////////////////////////
//              TreeControl                      //
///////////////////////////////////////////////////
TreeControl::TreeControl()
{
	temp = root = nullptr;
}
void TreeControl::AddNeighbour(Node *neighbour, int tabs)
{
	if (temp == nullptr)
	{
		temp = root = new Tree(nullptr, nullptr, nullptr, neighbour);
	}
	else
	{
		temp->SetLeft(neighbour);
		temp = temp->getLeft();
	}
	if (neighbour->getObjectType() != TypeObject::ObjEmpty)
		printTC(tabs);
}
void TreeControl::AddChild(Node *child, int tabs)
{
	if (temp == nullptr)
	{
		temp = root = new Tree(nullptr, nullptr, nullptr, child);
	}
	else
	{
		temp->SetRight(child);
		temp = temp->getRight();
	}
	if(child->getObjectType() != TypeObject::ObjEmpty)
		printTC(tabs);
}
Tree *TreeControl::findUp(Node *id, bool &parentOrHigher)
{
	return temp->FindUp(temp, id, parentOrHigher);
}

int TreeControl::Sem1(LEXs &id, TypeData dt, bool init,int tabs)
{
	Node *newnode = new Node(id, dt);
	if (init)
	{
		newnode->InitVar();
	}
	bool parentOrHigher = false;
	Tree *foundOldIdent = findUp(newnode, parentOrHigher);
	if (foundOldIdent != nullptr)
	{
		if (parentOrHigher == true)
		{
			AddNeighbour(newnode,tabs);
		}
		else
			return T_VAR_ALREADY_EXISTS;
	}
	else
	{
		AddNeighbour(newnode,tabs);
	}
	return 0;
}
int TreeControl::Sem2_0(Node *newnode)
{
	bool parentOrHigher = false;
	Tree *foundOldIdent = findUp(newnode, parentOrHigher);
	if (foundOldIdent != nullptr)
	{
		return T_FUNC_ALREADY_EXISTS;
	}
	return 0;
}
Tree *TreeControl::Sem3_0(LEXs &id)
{
	bool parentOrHigher = false;
	Node *toFind = new Node(id, TypeData::TypeFloat, nullptr, 0);
	Tree *foundOldIdent = findUp(toFind, parentOrHigher);
	return foundOldIdent;
}
Tree *TreeControl::Sem4_0(LEXs &id)
{
	bool parentOrHigher = false;
	Node *toFind = new Node(id, TypeData::TypeFloat);
	Tree *foundOldIdent = findUp(toFind, parentOrHigher);
	return foundOldIdent;
}
Tree * TreeControl::getRoot()
{
	return root;
}
void TreeControl::setTemp(Tree *newtemp)
{
	temp = newtemp;
}
Tree *TreeControl::getTemp()
{
	return temp;
}

void TreeControl::printFuncParams(Tree *temp)
{
	cout << "( ";
	if (temp->getParamsQuantity())
	{
		map <TypeData, string>::iterator it;

		Tree *tempP = temp->getParams();		
		it = types.find(tempP->getDataType());
		//cout << it->second << " " << tempP->getID();
		cout << tempP->getID();
		tempP = tempP->getLeft();
		while (tempP != nullptr)
		{
			it = types.find(tempP->getDataType());
			//cout << ", " << it->second << " " << tempP->getID();
			cout << ", " << tempP->getID();
			tempP = tempP->getLeft();
		}
	}
	cout << " )";
}
void TreeControl::printTree(Tree *tempT)
{
	Tree *t = tempT;
	if (t->getObjectType() != TypeObject::ObjEmpty)
	{
		map <TypeData, string>::iterator it;
		it = types.find(tempT->getDataType());
		cout << t->getID();
	}
	
	if (t->getObjectType() == TypeObject::ObjFunc)
	{
		printFuncParams(t);
		cout << ", ";
	}
	if (t->getObjectType() == TypeObject::ObjVar)
	{
		cout << ", ";
	}
	
	if (t->getRight() != nullptr)
	{
		Tree *func = t->getRight();
		printTree(func);
	}
	t = t->getLeft();
	if (t != nullptr)
	{
		printTree(t);
	}
}
void TreeControl::printTC(int tabs)
{
	for (int tabsi=0; tabsi <= tabs; tabsi++)
		cout << "\t";
	printTree(root);
	cout << endl;
}

void TreeControl::deleteUpToEmpty()
{
	Tree * tempToDelete = temp;

	while (temp->getObjectType() != TypeObject::ObjEmpty)
	{
		temp = tempToDelete->getUp();
		if (temp->getLeft() == tempToDelete)
		{
			temp->deleteLeft();
			delete (tempToDelete);
		}
		else
		{
			temp->deleteRight();
			delete (tempToDelete);
		}
		tempToDelete = temp;
	}
	temp = tempToDelete->getUp();
	temp->deleteRight();
	delete (tempToDelete);
	tempToDelete = temp;

	if (tempToDelete->getObjectType() == TypeObject::ObjEmpty)
	{
		temp = tempToDelete->getUp();
		temp->deleteLeft();
		delete (tempToDelete);
	}
}