#include "MyOctant.h"
using namespace Simplex;
// Esatablishing values to begin with
uint MyOctant::m_uOctantCount = 0; //makes sure there are 0 octants before they're created
uint MyOctant::m_uMaxLevel = 3; //Sets the max level incase one isn't provided
uint MyOctant::m_uIdealEntityCount = 5; //Sets the target number of entities in each octant incase one isn't proivided

// Getters
uint MyOctant::GetOctantCount(void) { return m_uOctantCount; } //Number of octants in the whole tree
MyOctant* MyOctant::GetParent(void) { return m_pParent; } //The parent of this octant
float MyOctant::GetSize(void) { return m_fSize; } //The length of a side of this octant
vector3 MyOctant::GetCenterGlobal(void) { return m_v3Center; } //Center of the octant
vector3 MyOctant::GetMinGlobal(void) { return m_v3Min; } //The minimum point of this octant
vector3 MyOctant::GetMaxGlobal(void) { return m_v3Max; } //The maximum point of this octant
MyOctant* MyOctant::GetChild(uint a_nChild)
{
	//If the index is out of range, return a nullptr
	if (a_nChild > 7)
		return nullptr;
	//otherwise give the child at that index
	return m_pChild[a_nChild];
}

void MyOctant::Init(void)
{
	m_uChildren = 0;//Currently has no children

	m_fSize = 0.0f;//Currently does not have a size, just initializes the value

	m_uID = m_uOctantCount;//The id is the number octant that this is
	m_uLevel = 0;//This currently does not exist, just initializes
	
	//Initalizes the center, min, and max
	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	//Get the instance of the singleton for the mesh and entitity managers
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	//initalizes the root and parent of this node but does not assign them
	m_pRoot = nullptr;
	m_pParent = nullptr;
	//Initializes the pointers to the children even though there are none yet
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}
void MyOctant::Swap(MyOctant& other)
{
	//Swap all the values set up in Init()
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
void MyOctant::Release(void)
{
	//Removes all children and their children
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_uChildren = 0;//Octant now has no children
	m_fSize = 0.0f;//Octant no longer has a size
	m_EntityList.clear();//Nothing is in the octant now
	m_lChild.clear();//Nothing has this octant as a child anymore. It doesn't exist
}

MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	//Call the init method to initialize a bunch of values
	Init();

	//Establishes a lot of base values. Many are defaulted earlier but this makes sure they're right because it's the root of the tree
	m_uOctantCount = 0;//Make sure that this is the first octant since that's the only time this constructor is used
	m_uMaxLevel = a_nMaxLevel;//Sets the mamimum number of levels based on in provided value
	m_uIdealEntityCount = a_nIdealEntityCount;//Same with the ideal number of enitities
	m_uID = m_uOctantCount;//This thing is the first octant

	m_pRoot = this;//Establishes this as the root of the tree
	m_lChild.clear();//Since it's the root it's the child of nothing

	std::vector<vector3> lMinsAndMaxes;//A list of the minimums and maximums of all the entities in the world so that it can establish the minimum possible size for the root octant

	uint nObjects = m_pEntityMngr->GetEntityCount();//Get the total number of entities
	//Loop through all the entities in the entity manager
	//Get the minumum and maximum for each entity and push it to lMinsAndMaxes
	for (uint i = 0; i < nObjects; i++)
	{
		MyEntity* pEntity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* pRigid = pEntity->GetRigidBody();
		lMinsAndMaxes.push_back(pRigid->GetMinGlobal());
		lMinsAndMaxes.push_back(pRigid->GetMaxGlobal());
	}
	MyRigidBody* pRigid = new MyRigidBody(lMinsAndMaxes);//Make a ridigbody around all those mins and maxes

	vector3 v3HalfWidth = pRigid->GetHalfWidth();//Get the halfwidth of the rigidbody
	float fMax = v3HalfWidth.x;//The halfwidth determines the maximum and minimums of all enities combined which is needed for the octant
	//Get the maximum in each direction by comparing xyz to each other to make the largest square
	for (int i = 1; i < 3; i++)
	{
		if (fMax < v3HalfWidth[i])
			fMax = v3HalfWidth[i];
	}
	vector3 v3Center = pRigid->GetCenterLocal();//Gets the center of the rigidbody so that it can become the center of the octant
	//No longer need the mins and maxes or the rigidbody
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
	//Initializes the center and size as provided and calculated in subdivide
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
	//Establishes all the same stuff as in Swap() and Init() just copies it from the provided
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
	//Makes a temproary octant and swap all the data between it and the other one, then delete it
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
//Destructor calls Release()
MyOctant::~MyOctant(void) { Release(); }


void MyOctant::Display(uint a_nIndex, vector3 a_v3color)
{
	//If this is the octant that it currently is trying to display, then draw a cube with it's points
	if (m_uID == a_nIndex)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3color, RENDER_WIRE);
		return;
	}
	//Draw all the children
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3color);
	}
}
void MyOctant::Display(vector3 a_v3color)
{
	//Draw all the children
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3color);
	}
	//Draw this
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3color, RENDER_WIRE);
}
void MyOctant::DisplayLeafs(vector3 a_v3color)
{
	uint nLeafs = m_lChild.size();
	//Loops through all leaf children of this and then call this method on them
	for (int i = 0; i < nLeafs; i++)
	{
		m_lChild[i]->DisplayLeafs(a_v3color);
	}
	//Display the node
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

	//Since it's being subdivided, it will now have 8 children
	m_uChildren = 8;

	//Children are going to have a size of half this and a halfwidth of a 4th of this
	float fSize = m_fSize / 4.0f;
	float fSizeD = fSize * 2.0f;
	vector3 v3Center;

	//For all 8 octants, there will be a unique combination of + and - values equal to 1/4th this
	//octant's size that finds it's center, go through all the combinations and then at each child
	//make a new octant centered at that point with a size of 1/2 this size
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

	//Go through every child just created and establish the root, this as it's parent
	//that it's level is 1 more than this octant's level.
	//If the child has more than the ideal amount of entities in it, subdivide again
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
			m_pChild[i]->Subdivide();
	}
}
bool MyOctant::IsColliding(uint a_uRBIndex)
{
	uint nObjectCount = m_pEntityMngr->GetEntityCount();//Get the number of entities for reference

	//If index is greater than number of objects it doesn't exist ergo no collision
	if (a_uRBIndex >= nObjectCount)
		return false;

	//Get the info on the two objects we're checking
	MyEntity* pEntity = m_pEntityMngr->GetEntity(a_uRBIndex);//Get the entity to check
	MyRigidBody* pRigid = pEntity->GetRigidBody();//get it's rigid body to check against
	vector3 v3MinOther = pRigid->GetMinGlobal();//Get it's min
	vector3 v3MaxOther = pRigid->GetMaxGlobal();//get it's max

	//Relies on AABB to check for collisions but then it just checks in each axis
	//If any of the checks pass, then it's good and you can exit imediatly 
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
bool MyOctant::IsLeaf(void) { return m_uChildren == 0; }//Tells you if the octant has children
bool MyOctant::ContainsMoreThan(uint a_nEntities)
{
	uint nCount = 0;//Number of entities in this
	uint nObjectCount = m_pEntityMngr->GetEntityCount();//Number of entities
	//Loops through all entites
	for (int i = 0; i < nObjectCount; i++)
	{
		//If one of the entities is colliding with this octant then it is
		//contatined within it and is added to the number of entities this contatins
		if (IsColliding(i))
			nCount++;
		//If at any point the number of entities contained surpases the number of entities
		//it is shooting for, then it does contain more and this can be exited
		if (nCount > a_nEntities)
			return true;
	}
	//There are fewer entities in this octant than the goal so we're good.
	return false;
}
void MyOctant::KillBranches(void)
{
	//Go through all the children and kill their branches
	//Once a branch is killed, delete the child
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}
	m_uChildren = 0;//This now has no children
}
void MyOctant::ClearEntityList(void)
{
	//Loop through all children and call this
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ClearEntityList();
	}
	//Delete everything from the entity list so that nothing is inside of this octant
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
	//Establish lists for the octants
	ConstructList();
}
void MyOctant::AssignIDtoEntity(void)
{
	//Assign ID's to all the entities through recursion
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->AssignIDtoEntity();
	}

	//If the octant doesn't have children...
	if (m_uChildren == 0)
	{
		uint nEntities = m_pEntityMngr->GetEntityCount();
		//Loop through all entities and assign all colliding with this octant to this octant's dimention
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
	//Loop through all children and build their lists
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}

	//As long as there are entities, the root adds this to its list of children
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}

