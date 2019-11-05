/*----------------------------------------------
Programmer: Alex Herman (ajh1624@rit.edu)
Date: 2017/07
----------------------------------------------*/
#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{

	//System Class
	class MyOctant
	{
		// Stuff shared between all Octants
		static uint m_uOctantCount;
		static uint m_uMaxLevel;
		static uint m_uIdealEntityCount;

		//Info about this octant
		uint m_uID = 0;
		uint m_uLevel = 0;
		uint m_uChildren = 0;

		//Size of one side of the octant
		float m_fSize = 0.0f;

		//Stuff for tracking the entities in the world
		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr;

		//Info about the octant in space
		vector3 m_v3Center = vector3(0.0f);
		vector3 m_v3Min = vector3(0.0f);
		vector3 m_v3Max = vector3(0.0f);

		//Tree structure stuff
		MyOctant* m_pParent = nullptr;
		MyOctant* m_pChild[8];

		//Entities in this octant
		std::vector<uint> m_EntityList;

		MyOctant* m_pRoot = nullptr;
		std::vector<MyOctant*> m_lChild;
	public:
		// Root constructor
		MyOctant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);
		// Child constructor
		MyOctant(vector3 a_v3Center, float a_fSize);
		// Copy constructor
		MyOctant(MyOctant const& other);
		// Copy Assignment Operator
		MyOctant& operator=(MyOctant const& other);
		// Destructor
		~MyOctant(void);
		
		//Does as the name implies, Swaps data with another one
		void Swap(MyOctant& other);
		//Gets for size, center, min, and max
		float GetSize(void);
		vector3 GetCenterGlobal(void);
		vector3 GetMinGlobal(void);
		vector3 GetMaxGlobal(void);
		//Determines if the object is colliding with the object at the provided index
		bool IsColliding(uint a_uRBIndex);
		//Renders the octant and its children
		void Display(uint a_nIndex, vector3 a_v3color = C_YELLOW);
		void Display(vector3 a_v3color = C_YELLOW);
		void DisplayLeafs(vector3 a_v3color = C_YELLOW);
		//makes sure the enitiy list is clear
		void ClearEntityList(void);
		//Subdivides an octant down to the lowest allowed level
		void Subdivide(void);
		//Gets the child at the index
		MyOctant* GetChild(uint a_nChild);
		//gets the parent
		MyOctant* GetParent(void);
		//Tells you if the octant has children
		bool IsLeaf(void);
		//Checks if there are more enitities in the box than the provided number
		bool ContainsMoreThan(uint a_nEntities);
		//Deletes the octant and it's children
		void KillBranches(void);
		//Starts building the whole tree from scratch
		void ConstructTree(uint a_nMaxLevel = 3);
		//Give each entity an id for the octant(s) it's in
		void AssignIDtoEntity(void);
		//Gets the number of octants
		uint GetOctantCount(void);

	private:
		//Used in deleteing the octant
		void Release(void);
		//Used in initializing the octant
		void Init(void);
		//Used in making the list of the entities for the leaves
		void ConstructList(void);
	};//class

} //namespace Simplex

#endif //__MyOctantCLASS_H_

  /*
  USAGE:
  ARGUMENTS: ---
  OUTPUT: ---
  */