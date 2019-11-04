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
	class SimplexDLL MyOctant
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
		MyOctant* m_pChildren[8];

		//Entities in this octant
		std::vector<uint> m_EntityList; 
		
		MyOctant* m_pRoot = nullptr;
		std::vector<MyOctant*> m_lChild;
	public:
		// Root constructor
		MyOctant(uint a_uMaxLevel = 2, uint a_uIdealEntityCount = 5);
		// Child constructor
		MyOctant(vector3 a_v3Center, float a_fSize);
		// Copy constructor
		MyOctant(MyOctant const& other);
		// Copy Assignment Operator
		MyOctant& operator=(MyOctant const& other);
		// Destructor
		~MyOctant(void);

		void Swap(MyOctant& other);

		float GetSize(void);

		vector3 GetCenterGlobal(void);

		vector3 GetMinGlobal(void);

		vector3 GetMaxGlobal(void);

		bool IsColliding(uint a_uRBIndex);

		void Display(uint a_nIndex, vector3 a_v3color = C_YELLOW);
		void Display(vector3 a_v3color = C_YELLOW);

		void DisplayLeafs(vector3 a_v3color = C_YELLOW);

		void ClearEntityList(void);

		void Subdivide(void);

		MyOctant* GetChild(uint a_nChild);

		MyOctant* GetParent(void);

		bool IsLeaf(void);

		bool ContainsMoreThan(uint a_nEntities);

		void KillBranches(void);

		void ConstructTree(uint a_nMaxLevel = 3);

		void AssignIDtoEntity(void);

		uint GetOctantCount(void);

	private:
		void Release(void);

		void Init(void);

		void ConstructList(void);
	};//class

} //namespace Simplex

#endif //__MyOctantCLASS_H_

  /*
  USAGE:
  ARGUMENTS: ---
  OUTPUT: ---
  */