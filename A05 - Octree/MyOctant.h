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
		static uint m_uOctantCount; //will store the number of octants instantiated
		static uint m_uMaxLevel;//will store the maximum level an octant can go to
		static uint m_uIdealEntityCount; //will tell how many ideal Entities this object will contain

		uint m_uID = 0; //Will store the current ID for this octant
		uint m_uLevel = 0; //Will store the current level of the octant
		uint m_uChildren = 0;// Number of children on the octant (either 0 or 8)

		float m_fSize = 0.0f; //Size of the octant

		MeshManager* m_pMeshMngr = nullptr;//Mesh Manager singleton
		MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton

		vector3 m_v3Center = vector3(0.0f); //Will store the center point of the octant
		vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the octant
		vector3 m_v3Max = vector3(0.0f); //Will store the maximum vector of the octant

		MyOctant* m_pParent = nullptr;
		MyOctant* m_pChildren[8];

		std::vector<uint> m_EntityList; //List of Entities under this octant (Index in Entity Manager)

		MyOctant* m_pRoot = nullptr;//Root octant
		std::vector<MyOctant*> m_lChild; //list of nodes that contain objects (this will be applied to root only)

	public:
		/*
		Usage: Constructor
		Arguments: ---
		Output: class object instance
		*/
		MyOctant(uint a_uMaxLevel = 2, uint a_uIdealEntityCount = 5);
		/*
		USAGE: Constructor
		ARGUMENTS:
		- vector3 a_v3Center -> Center of the octant in global space
		- float a_fSize -> size of each side of the octant volume
		OUTPUT: class object
		*/
		MyOctant(vector3 a_v3Center, float a_fSize);
		/*
		Usage: Copy Constructor
		Arguments: class object to copy
		Output: class object instance
		*/
		MyOctant(MyOctant const& other);
		/*
		Usage: Copy Assignment Operator
		Arguments: class object to copy
		Output: ---
		*/
		MyOctant& operator=(MyOctant const& other);
		/*
		Usage: Destructor
		Arguments: ---
		Output: ---
		*/
		~MyOctant(void);

		/*
		Usage: Changes object contents for other object's
		Arguments: other -> object to swap content from
		Output: ---
		*/
		void Swap(MyOctant& other);

		/*
		Usage: Gets data member
		Arguments: ---
		Output: data
		*/
		int GetData(void);
		/*
		Usage: Sets data member
		Arguments: int a_nData = 1 -> data to set
		Output: ---
		*/
		void SetData(int a_nData = 1);
		/* Property */
		//__declspec(property(get = GetData, put = SetData)) int Data;

		/*
		Usage: adds data on vector
		Arguments: int a_nData -> data to add
		Output: ---
		*/
		void SetDataOnVector(int a_nData);
		/*
		Usage: gets data on vector at the specified entry
		Arguments: int a_nIndex -> entry index
		Output: entry's content
		*/
		int& GetDataOnVector(int a_nIndex);

	private:
		/*
		Usage: Deallocates member fields
		Arguments: ---
		Output: ---
		*/
		void Release(void);
		/*
		Usage: Allocates member fields
		Arguments: ---
		Output: ---
		*/
		void Init(void);
	};//class

} //namespace Simplex

#endif //__MyOctantCLASS_H_

  /*
  USAGE:
  ARGUMENTS: ---
  OUTPUT: ---
  */