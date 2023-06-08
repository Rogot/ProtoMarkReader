#include "rtree.h"

template<class DATATYPE, class ELEMTYPE, int NUMDIMS,
		class ELEMTYPEREAL = ELEMTYPE, int TMAXNODES = 8, int TMINNODES =
				TMAXNODES / 2>
void RTree<DATATYPE, ELEMTYPE, NUMDIMS, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Insert(
		const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS],
		const DATATYPE &a_dataId) {
#ifdef _DEBUG
  for (int index = 0; index < NUMDIMS; ++index)
  {
    ASSERT(a_min[index] <= a_max[index]);
  }
#endif //_DEBUG

	Branch branch;
	branch.m_data = a_dataId;
	branch.m_child = NULL;

	for (int axis = 0; axis < NUMDIMS; ++axis) {
		branch.m_rect.m_min[axis] = a_min[axis];
		branch.m_rect.m_max[axis] = a_max[axis];
	}

	InsertRect(branch, &m_root, 0);
}

/**			ITERATOR		**/

template <class DATATYPE, class ELEMTYPE, int NUMDIMS,
		  class ELEMTYPEREAL = ELEMTYPE, int TMAXNODES = 8, int TMINNODES = TMAXNODES / 2>

void RTree<DATATYPE, ELEMTYPE, NUMDIMS, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Iterator::GetBounds(
		ELEMTYPE a_min[NUMDIMS], ELEMTYPE a_max[NUMDIMS]) {
	assert(IsNotNull());
	StackElement &curTos = m_stack[m_tos - 1];
	Branch &curBranch = curTos.m_node->m_branch[curTos.m_branchIndex];

	for (int index = 0; index < NUMDIMS; ++index) {
		a_min[index] = curBranch.m_rect.m_min[index];
		a_max[index] = curBranch.m_rect.m_max[index];
	}
}


/* Access the current data element. Caller must be sure iterator is not NULL first */
template <class DATATYPE, class ELEMTYPE, int NUMDIMS,
		  class ELEMTYPEREAL = ELEMTYPE, int TMAXNODES = 8, int TMINNODES = TMAXNODES / 2>

DATATYPE& RTree<DATATYPE, ELEMTYPE,NUMDIMS, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Iterator::operator *(void) {
	assert(IsNotNull());
	StackElement &curTos = m_stack[m_tos - 1];
	return curTos.m_node->m_branch[curTos.m_branchIndex].m_data;
}

/* Access the current data element. Caller must be sure iterator is not NULL first */
template <class DATATYPE, class ELEMTYPE, int NUMDIMS,
		  class ELEMTYPEREAL = ELEMTYPE, int TMAXNODES = 8, int TMINNODES = TMAXNODES / 2>

const DATATYPE& RTree<DATATYPE, ELEMTYPE,NUMDIMS, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Iterator::operator *(void)
			const {
//	assert(isNotNull());
	StackElement &curTos = m_stack[m_tos - 1];
	return curTos.m_node->m_branch[curTos.m_branchIndex].m_data;
}



/* Find the next data element in the tree (For internal use only) */
template <class DATATYPE, class ELEMTYPE, int NUMDIMS,
		  class ELEMTYPEREAL = ELEMTYPE, int TMAXNODES = 8, int TMINNODES = TMAXNODES / 2>

bool RTree<DATATYPE, ELEMTYPE,NUMDIMS, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Iterator::FindNextData() {
	for(;;) {
		if (m_tos <= 0){
			return false;
		}
	}
}

/**			ITERATOR		**/
