/*
Author: Rene Rodrigues Veloso

Inspired on Hilmi Yildirim's code.
(https://code.google.com/p/grail/)

For research purposes.
There is no additional support offered, nor are the author(s)
or their institutions liable under any circumstances.
*/
#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <deque>
#include <algorithm>
#include <utility>
#include <cmath>
#include <ext/hash_map>
#include <functional>
#include <ctime>

namespace std {using namespace __gnu_cxx;}
using namespace std;

#define MAX_VAL 10000000
#define MIN_VAL -10000000

//#define MAXDIMS 20

struct Vertex {
	int id;
	int top_level;	// topological level
	int min_parent_level;	// level of the highest parent in top_order
	int topo_id;	// topological order
	int min_int;
	int tmp_inDegree;
	int tmp_outDegree;
	//int x;
	//int y;
//	long volume;
//	double adj_vol;
	bool visited;
	int dfs_order;
	int pre_order;
	int post_order;
	double tcs;
	int mingap;
	int pre;//vector<int> *pre;
	int post;//vector<int> *post;
	int middle;//vector<int> *middle;
	Vertex(int ID) : id(ID) {
		top_level = -1;
		visited = false;
	}
	Vertex(){
		top_level = -1;
		visited = false;
	};
};

struct VertexCompare {
  bool operator() ( const Vertex p1, const Vertex p2 ) const
  {
    return p1.id < p2.id;
  }
};

typedef vector<int> EdgeList;	// edge list represented by vertex id list
typedef vector<Vertex> VertexList;	// vertices list (store real vertex property) indexing by id

struct Level_list {
	int level;
	vector<int> listv;
};

typedef vector<Level_list> LevelList;
typedef vector<int> TopoSortList;

struct Coords{
	int x, y;
};

struct IndexInvTable{
	int id;
	int level;
	Coords coord;

	IndexInvTable(int ID) : id(ID){
                level = -1;
        }
        IndexInvTable(){
                level = -1;
        };
	
};

struct IndexTable{
	int id;
	int level;
	
	Coords coord;

	IndexTable(int ID) : id(ID){
		level = -1;
	}
	IndexTable(){
		level = -1;
	}

//	int operator< (IndexTable &v1){
//		return (coord.x < v1.coord.x);
//	}

};

struct IndexTableCompare {
  bool operator() ( const IndexTable p1, const IndexTable p2 ) const
  {
    return p1.coord.x < p2.coord.x;
  }
};


//int IndexTable::operator<(IndexTable &v1){
//	return (coord.x < v1.coord.x);
//}

typedef vector<IndexTable> Index;
//typedef vector<IndexTable> IndexInv;

struct In_OutList {
	EdgeList inList;
	EdgeList outList;
};
typedef vector<In_OutList> GRA;	// index graph

typedef vector<IndexTable> SG;


class Graph {
	private:
		GRA graph;
		VertexList vl;
		LevelList ll;
		TopoSortList ts;
		Index index;
		Index indexInv;

		int maxlevels;

		int *visited;
		int QueryCnt;

		int vsize;
		int esize;


		int DIMS;

		unsigned int hopsTotal, ConstantCut, PositiveCut, NegativeCut, TotalCall, TotalDepth, CurrentDepth;


		int contador;

	public:
		SG rootRank;
		SG rootRankInv;
		Graph();
		Graph(int);
		Graph(GRA&, VertexList&);
		Graph(istream&);
		~Graph();
		bool contains(int src, int trg);
		bool incrementalContains(int src, int trg, int cur);
		void readGraph(istream&);
		void writeGraph(ostream&);
		void writeGraph_inv(ostream&);
		void writeGraph_forInterval(ostream&);
		void writeIndex(ostream&);
		void printGraph();
		void addVertex(int);
		void addVertexLl(Vertex&);
		void printLevelList();
		void addEdge(int, int);
		int num_vertices();
		int num_edges();
		void initializeVisited(int);
		void printAvgInOutDegreDist();
		int getVisited(int);
		void setQueryCnt(int);
		int getQueryCnt();
		void setNegativeCut(int n);
		int getNegativeCut();
		int getConstantCut();
		int getPositiveCut();
		int getHopsTotal();
		int getMaxLevels();
		void setMaxLevels(int max);
		int getContador();
		void setContador(int c);
		bool go_for_reach(int src, int trg, bool LEVEL_FILTER);
		bool reach(int src,int trg, bool LEVEL_FILTER);
		bool go_for_reach_inv(int src, int trg, bool LEVEL_FILTER);
                bool reach_inv(int src,int trg, bool LEVEL_FILTER);
		bool contains_inv(int src, int trg);
		int contains_pc(int src,int trg);
		bool go_for_reach_pc(int src, int trg, bool LEVEL_FILTER, int hop);
		bool reach_pc(int src,int trg, bool LEVEL_FILTER);
		int contains_pc_inv(int src,int trg);
                bool go_for_reach_pc_inv(int src, int trg, bool LEVEL_FILTER, int hop);
                bool reach_pc_inv(int src,int trg, bool LEVEL_FILTER);
		void calcPrune(int src, int trg);
		int calcPruneTree(int src, int trg, int& vertices, int& edges, vector<bool>& visited);
		VertexList& vertices();
		Index& getIndex();
		Index& getIndexInv();
		LevelList& getLevelList();
		TopoSortList& getTopoSortList();
		void setTopoSortList(vector<int>&);
		EdgeList& out_edges(int);
		EdgeList& in_edges(int);
		int out_degree(int);
		int in_degree(int);
		vector<int> getRoots();
		vector<int> getLeafs();
		vector<int> getBranches();
		vector<int> getAloneVertices();
		bool hasEdge(int, int);
		Graph& operator=(const Graph&);
		Vertex& operator[](const int);
		const double actualgap(const int);
		const double tcs(const int);

		void clear();
		void strTrimRight(string& str);

		void writeGraph_levels(ostream& out, int level);

		Graph(hash_map<int,vector<int> >& inlist, hash_map<int,vector<int> >& outlist);
		void extract(hash_map<int,vector<int> >& inlist, hash_map<int,vector<int> >& outlist);
		void printMap(hash_map<int,vector<int> >& inlist, hash_map<int,vector<int> >& outlist);

};

#endif
