#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;
uint MyOctant::GetOctantCount(void) { return m_uOctantCount; }

void MyOctant::Init(void)
{
	m_uChildren = 0;

	m_fSize = 0.0f;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}
void MyOctant::Swap(MyOctant& other)
{
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);

	for (int i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}
MyOctant* MyOctant::GetParent(void) { return m_pParent; }
void MyOctant::Release(void)
{
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}

MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	//Call the init method to initialize a bunch of values
	Init();

	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	std::vector<vector3> lMinsAndMaxes;

	uint nObjects = m_pEntityMngr->GetEntityCount();
	for (uint i = 0; i < nObjects; i++)
	{
		MyEntity* pEntity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* pRigid = pEntity->GetRigidBody();
		lMinsAndMaxes.push_back(pRigid->GetMinGlobal());
		lMinsAndMaxes.push_back(pRigid->GetMaxGlobal());
	}
	MyRigidBody* pRigid = new MyRigidBody(lMinsAndMaxes);

	vector3 v3HalfWidth = pRigid->GetHalfWidth();
	float fMax = v3HalfWidth.x;
	for (int i = 1; i < 3; i++)
	{
		if (fMax < v3HalfWidth[i])
			fMax = v3HalfWidth[i];
	}
	vector3 v3Center = pRigid->GetCenterLocal();
	lMinsAndMaxes.clear();
	SafeDelete(pRigid);

	//Assign calculated values to the octant
	m_fSize = fMax * 2.0f;
	m_v3Center = v3Center;
	m_v3Min = m_v3Center - (vector3(fMax));
	m_v3Max = m_v3Center + (vector3(fMax));

	//Finished with this octant, move to the next
	m_uOctantCount++;
	ConstructTree(m_uMaxLevel);
}
MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	//Establish the basic values
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	//get the max and min
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	//Done with this, increment count
	m_uOctantCount++;
}
MyOctant::MyOctant(MyOctant const& other)
{
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;

	m_pRoot = other.m_pRoot;
	m_pRoot = other.m_pRoot;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant(void) { Release(); }

float MyOctant::GetSize(void) { return m_fSize; }
vector3 MyOctant::GetCenterGlobal(void) { return m_v3Center; }
vector3 MyOctant::GetMinGlobal(void) { return m_v3Min; }
vector3 MyOctant::GetMaxGlobal(void) { return m_v3Max; }
void MyOctant::Display(uint a_nIndex, vector3 a_v3color)
{
	if (m_uID == a_nIndex)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3color, RENDER_WIRE);
		return;
	}

	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3color);
	}
}
void MyOctant::Display(vector3 a_v3color)
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3color, RENDER_WIRE);
}
void MyOctant::Subdivide(void)
{
	//Max number of levels reached
	if (m_uLevel >= m_uMaxLevel)
		return;
	//Already subdivided
	if (m_uChildren != 0)
		return;

	m_uChildren = 8;

	float fSize = m_fSize / 4.0f;
	float fSizeD = fSize * 2.0f;
	vector3 v3Center;

	//Octant 1
	v3Center = m_v3Center;
	v3Center.x -= fSize;
	v3Center.y -= fSize;
	v3Center.z -= fSize;
	m_pChild[0] = new MyOctant(v3Center, fSizeD);
	//Octant 2
	v3Center.x += fSizeD;
	m_pChild[1] = new MyOctant(v3Center, fSizeD);
	//Octant 3
	v3Center.z += fSizeD;
	m_pChild[2] = new MyOctant(v3Center, fSizeD);
	//Octant 4
	v3Center.x -= fSizeD;
	m_pChild[3] = new MyOctant(v3Center, fSizeD);
	//Octant 5
	v3Center.y += fSizeD;
	m_pChild[4] = new MyOctant(v3Center, fSizeD);
	//Octant 6
	v3Center.z -= fSizeD;
	m_pChild[5] = new MyOctant(v3Center, fSizeD);
	//Octant 7
	v3Center.x += fSizeD;
	m_pChild[6] = new MyOctant(v3Center, fSizeD);
	//Octant 8
	v3Center.z += fSizeD;
	m_pChild[7] = new MyOctant(v3Center, fSizeD);

	for (int i = 0; i < 8; i++)
	{
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
			m_pChild[i]->Subdivide();
	}
}
MyOctant* MyOctant::GetChild(uint a_nChild)
{
	if (a_nChild > 7)
		return nullptr;

	return m_pChild[a_nChild];
}
bool MyOctant::IsColliding(uint a_uRBIndex)
{
	uint nObjectCount = m_pEntityMngr->GetEntityCount();

	//If index is greater than number of objects it doesn't exist ergo no collision
	if (a_uRBIndex >= nObjectCount)
		return false;

	//Get the info on the two objects we're checking
	MyEntity* pEntity = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* pRigid = pEntity->GetRigidBody();
	vector3 v3MinOther = pRigid->GetMinGlobal();
	vector3 v3MaxOther = pRigid->GetMaxGlobal();

	//Check x
	if (m_v3Max.x < v3MinOther.x)
		return false;
	if (m_v3Min.x > v3MaxOther.x)
		return false;
	//Check y
	if (m_v3Max.y < v3MinOther.y)
		return false;
	if (m_v3Min.y > v3MaxOther.y)
		return false;
	//Check z
	if (m_v3Max.z < v3MinOther.z)
		return false;
	if (m_v3Min.z > v3MaxOther.z)
		return false;

	//If nothing said they weren't colliding then they are
	return true;
}
bool MyOctant::IsLeaf(void) { return m_uChildren == 0; }
bool MyOctant::ContainsMoreThan(uint a_nEntities)
{
	uint nCount = 0;
	uint nObjectCount = m_pEntityMngr->GetEntityCount();
	for (int i = 0; i < nObjectCount; i++)
	{
		if (IsColliding(i))
			nCount++;
		if (nCount > a_nEntities)
			return true;
	}
	return false;
}
void MyOctant::KillBranches(void)
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}
	m_uChildren = 0;
}
void MyOctant::DisplayLeafs(vector3 a_v3color)
{
	uint nLeafs = m_lChild.size();
	for (int i = 0; i < nLeafs; i++)
	{
		m_lChild[i]->DisplayLeafs(a_v3color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3color, RENDER_WIRE);
}
void MyOctant::ClearEntityList(void)
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ClearEntityList();
	}
	m_EntityList.clear();
}
void MyOctant::ConstructTree(uint a_nMaxLevel)
{
	//No Tree if there's no level
	if (m_uLevel != 0)
		return;
	//Set the max level
	m_uMaxLevel = a_nMaxLevel;
	//Reset values
	m_uOctantCount = 1;
	m_EntityList.clear();
	KillBranches();
	m_lChild.clear();

	//Build tree
	if (ContainsMoreThan(m_uIdealEntityCount))
		Subdivide();

	//Assign entities to octants
	AssignIDtoEntity();

	ConstructList();
}
void MyOctant::AssignIDtoEntity(void)
{
	//Assign ID's to all the entities through recursion
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->AssignIDtoEntity();
	}

	if (m_uChildren == 0)
	{
		uint nEntities = m_pEntityMngr->GetEntityCount();
		for (uint i = 0; i < nEntities; i++)
		{
			if (IsColliding(i))
			{
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}
void MyOctant::ConstructList(void)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}

	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}

