#include "Octant.h"
using namespace Simplex;


Simplex::Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
}

Simplex::Octant::Octant(vector3 a_v3Center, float a_fSize)
{
}

Simplex::Octant::Octant(Octant const& other)
{
}

Octant& Simplex::Octant::operator=(Octant const& other)
{
	// TODO: insert return statement here
}

Simplex::Octant::~Octant(void)
{
}

void Simplex::Octant::Swap(Octant& other)
{
}

float Simplex::Octant::GetSize(void)
{
	return 0.0f;
}

vector3 Simplex::Octant::GetCenterGlobal(void)
{
	return vector3();
}

vector3 Simplex::Octant::GetMinGlobal(void)
{
	return vector3();
}

vector3 Simplex::Octant::GetMaxGlobal(void)
{
	return vector3();
}

bool Simplex::Octant::IsColliding(uint a_uRBIndex)
{
	return false;
}

void Simplex::Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
}

void Simplex::Octant::Display(vector3 a_v3Color)
{
}

void Simplex::Octant::DisplayLeafs(vector3 a_v3Color)
{
}

void Simplex::Octant::ClearEntityList(void)
{
}

void Simplex::Octant::Subdivide(void)
{
}

Octant* Simplex::Octant::GetChild(uint a_nChild)
{
	return m_pChild[a_nChild];
}

Octant* Simplex::Octant::GetParent(void)
{
	return m_pParent;
}

bool Simplex::Octant::IsLeaf(void)
{
	for (int i = 0; i < 8; i++)
	{
		if (m_pChild[i] != nullptr)
		{
			return false;
		}
	}
	return true;
}

bool Simplex::Octant::ContainsMoreThan(uint a_nEntities)
{
	if (m_EntityList.size() <= a_nEntities)
		return false;
	else
		return true;
}

void Simplex::Octant::KillBranches(void)
{
}

void Simplex::Octant::ConstructTree(uint a_nMaxLevel)
{
}

void Simplex::Octant::AssignIDtoEntity(void)
{
}

uint Simplex::Octant::GetOctantCount(void)
{
	return uint();
}

void Simplex::Octant::Release(void)
{
}

void Simplex::Octant::Init(void)
{
}

void Simplex::Octant::ConstructList(void)
{
}
