#include "Obj.h"

Obj::Obj()
{
}

Obj::~Obj()
{
}

Vector2 Obj::GetPos()
{
	return pos_;
}

bool Obj::GetAlive()
{

	return isAlive;

}

bool Obj::GetIsDeath()
{
	return isDeath;
}

bool Obj::SetIsDeath(bool flag)
{
	isDeath = flag;
	return true;
}

int Obj::GetID()
{
	return id_;
}

int Obj::GetZorder()
{
	return zorder_;
}




