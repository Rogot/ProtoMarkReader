#pragma once
#ifndef RTREE_H
#define RTREE_H

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include <algorithm>
#include <functional>


template <class DATATYPE, class ELEMTYPE, int NUMDIMS,
		  class ELEMTYPEREAL = ELEMTYPE, int TMAXNODES = 8, int TMINNODES = TMAXNODES / 2>
class RTree {
protected:
	struct Node; /* Fwd decl.  Used by other internal structs and iterator */

public:
	/* These constant must be declared after Branch and before Node struct
	 * Stuck up here for MSVC 6 compiler.  NSVC .NET 2003 is much happier.
	 */
	enum {
		MAXNODES = TMAXNODES, /* < Max elements in node */
		MINNODES = TMINNODES, /* < Min elements in node */
	};

public:
	RTree() {
		assert(MAXNODES > MINNODES);
		assert(MINNODES > 0);

		// Precomputed volumes of the unit spheres for the first few dimensions
		const float UNIT_SPHERE_VOLUMES[] = {
				0.000000f, 2.000000f, 3.141593f, // Dimension  0,1,2
				4.188790f, 4.934802f, 5.263789f, // Dimension  3,4,5
				5.167713f, 4.724766f, 4.058712f, // Dimension  6,7,8
				3.298509f, 2.550164f, 1.884104f, // Dimension  9,10,11
				1.335263f, 0.910629f, 0.599265f, // Dimension  12,13,14
				0.381443f, 0.235331f, 0.140981f, // Dimension  15,16,17
				0.082146f, 0.046622f, 0.025807f, // Dimension  18,19,20
				};

		m_root = AllocNode();
		m_root->m_level = 0;
		m_unitSphereVolume = (ELEMTYPEREAL)UNIT_SPHERE_VOLUMES[NUMDIMS];
	}

	RTree(const RTree &other) {
		CopyRec(m_root, other.m_root);
	}
	virtual ~RTree() {
		Reset();
	}

	int Search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS],
			std::function<bool(const DATATYPE &)> callback) const;

	/* Remove all elements */
	void RemoveAll();

	/* Count the data elements in this container */
	int Count();


	class Iterator {
	private:
		enum {
			MAX_STACK = 32 /* Max stack size. Allows almost n^32 where n is number of branches in node */
		};

		struct StackElement {
			Node *m_node;
			int m_branchIndex;
		};

		StackElement m_stack[MAX_STACK]; /* Stack as we are doing iteration instead of recursion */
		int m_tos; /* Top of Stack index */

	public:
		Iterator() { Init(); }
		~Iterator() {}

		/* Is iterator invalid */
		bool IsNull() {
			return (m_tos <= 0);
		}

		/* Is iterator pointing to valid data */
		bool IsNotNull() {
			return (m_tos > 0);
		}

		DATATYPE& operator*(void);

		const DATATYPE& operator*() const;

		/* Find the next data element */
		bool operator++() { return FindNextData(); }

		void GetBounds(ELEMTYPE a_min[NUMDIMS], ELEMTYPE a_max[NUMDIMS]);

	private:
		void Init() { m_tos = 0; }
		bool FindNextData(void);


		/// Push node and branch onto iteration stack (For internal use only)
		void Push(Node *a_node, int a_branchIndex);

		  /// Pop element off iteration stack (For internal use only)
		StackElement&Pop();

		friend class RTree; // Allow hiding of non-public functions while allowing manipulation by logical owner
	};

	/// Get Next for iteration
	void GetNext(Iterator &a_it) { ++a_it; }

	/// Is iterator NULL, or at end?
	bool isNull(Iterator &a_it) { return a_it.IsNull(); }
	bool isNotNull(Iterator &a_it) { return a_it.IsNotNull(); }

	/// Get object at iterator position
	DATATYPE& GetAt(Iterator &a_it) { return *a_it; }

protected:

	struct Rect {
		ELEMTYPE m_min[NUMDIMS]; /* Min dimensions of bounding box */
		ELEMTYPE m_max[NUMDIMS]; /* Max dimensions of bounding box */
	};

	/*
	 * May be data or may be another subtree
	 * The parents level determines this.
	 * If the parents level is 0, then this is data
	 * */

	struct Branch {
		Rect m_rect; /* Bounds */
		Node *m_child; /* Child mode */
		DATATYPE m_data; /* Data ID */
	};

	/* Node for each branch level */
	struct Node {
		bool isIternalNode() { return (m_level > 0); }
		bool isLeaf() { return (m_level == 0); }

		int m_count;               /* Count */
		int m_level;               /* Leaf is zero, others positive */
		Branch m_branch[MAXNODES]; /* Branch */
	};

	struct ListNode {
		ListNode *m_next;
		Node *m_node;
	};

	struct PartitionVars {
		enum {
			NOT_TAKEN = -1
		}; // indicates that position

		int m_partition[MAXNODES + 1];
		int m_total;
		int m_minFill;
		int m_count[2];
		Rect m_cover[2];
		ELEMTYPEREAL m_area[2];

		Branch m_branchBuf[MAXNODES + 1];
		int m_branchCount;
		Rect m_coverSplit;
		ELEMTYPEREAL m_coverSplitArea;
	};

	Node* AllocNode();
	void Insert(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], const DATATYPE &a_dataId);
	void FreeNode(Node *a_node);
	void InitNode(Node *a_node);
	void InitRect(Rect *a_rect);
	bool InsertRectRec(const Branch &a_branch, Node *a_node, Node **a_newNode,
			int a_level);
	bool InsertRect(const Branch &a_branch, Node **a_root, int a_level);
	Rect NodeCover(Node *a_node);
	bool AddBranch(const Branch *a_branch, Node *a_node, Node **a_newNode);
	void DisconnectBranch(Node *a_node, int a_index);
	int PickBranch(const Rect *a_rect, Node *a_node);
	Rect CombineRect(const Rect *a_rectA, const Rect *a_rectB);
	void SplitNode(Node *a_node, const Branch *a_branch, Node **a_newNode);
	ELEMTYPEREAL RectSphericalVolume(Rect *a_rect);
	ELEMTYPEREAL RectVolume(Rect *a_rect);
	ELEMTYPEREAL CalcRectVolume(Rect *a_rect);
	void GetBranches(Node *a_node, const Branch *a_branch,
			PartitionVars *a_parVars);
	void ChoosePartition(PartitionVars *a_parVars, int a_minFill);
	void LoadNodes(Node *a_nodeA, Node *a_nodeB, PartitionVars *a_parVars);
	void InitParVars(PartitionVars *a_parVars, int a_maxRects, int a_minFill);
	void PickSeeds(PartitionVars *a_parVars);
	void Classify(int a_index, int a_group, PartitionVars *a_parVars);
	bool RemoveRect(Rect *a_rect, const DATATYPE &a_id, Node **a_root);
	bool RemoveRectRec(Rect *a_rect, const DATATYPE &a_id, Node *a_node,
			ListNode **a_listNode);
	ListNode* AllocListNode();
	void FreeListNode(ListNode *a_listNode);
	bool Overlap(Rect *a_rectA, Rect *a_rectB) const;
	void ReInsert(Node *a_node, ListNode **a_listNode);
	bool Search(Node *a_node, Rect *a_rect, int &a_foundCount,
			std::function<bool(const DATATYPE&)> callback) const;
	void RemoveAllRec(Node *a_node);
	void Reset();
	void CountRec(Node *a_node, int &a_count);

//	bool SaveRec(Node *a_node, RTFileStream &a_stream);
//	bool LoadRec(Node *a_node, RTFileStream &a_stream);
	void CopyRec(Node *current, Node *other);

	Node *m_root;                    ///< Root of tree
	ELEMTYPEREAL m_unitSphereVolume; ///< Unit sphere constant for required number of dimensions
};

#endif // !RTREE_H
