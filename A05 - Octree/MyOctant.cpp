#include "MyOctant.h"
using namespace Simplex;

Simplex::MyOctant::MyOctant(uint a_uMaxLevel, uint a_uIdealEntityCount)
{
	m_uMaxLevel = a_uMaxLevel;
	m_uIdealEntityCount = a_uIdealEntityCount;

	m_pRoot = this;
	m_uID = 0;
	m_uLevel = 0;

	//m_fSize = 0;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{

}

Simplex::MyOctant::MyOctant(MyOctant const& other)
{

}

MyOctant& Simplex::MyOctant::operator=(MyOctant const& other)
{
	// TODO: insert return statement here
}

Simplex::MyOctant::~MyOctant(void)
{

}

void Simplex::MyOctant::Swap(MyOctant& other)
{
}

int Simplex::MyOctant::GetData(void)
{
	return 0;
}

void Simplex::MyOctant::SetData(int a_nData)
{
}

void Simplex::MyOctant::SetDataOnVector(int a_nData)
{
}

int& Simplex::MyOctant::GetDataOnVector(int a_nIndex)
{
	// TODO: insert return statement here
}

void Simplex::MyOctant::Release(void)
{
}

void Simplex::MyOctant::Init(void)
{
}
