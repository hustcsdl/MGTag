/*
Author: Rene Rodrigues Veloso

Inspired on Hilmi Yildirim's code.
(https://code.google.com/p/grail/)

For research purposes.
There is no additional support offered, nor are the author(s)
or their institutions liable under any circumstances.
*/
#ifndef _GRAPH_UTIL_H_
#define _GRAPH_UTIL_H_

#include "Graph.h"
#include <sys/time.h>


class GraphUtil {
	public:

	static void dfs(Graph& g, int vid, vector<int>& order1, vector<int>& order2, vector<bool>& visited);
	static void dfs(Graph& g, int vid, vector<int>& order2, vector<bool>& visited, char direction);
	static void dfs_inv(Graph& g, int vid, vector<int>& order2, vector<bool>& visited, char direction);	
	static void topo_leveler(Graph& g, bool PC_FILTER, bool INV, bool LEVEL_LIST, int spread);
		static int topo_level(Graph& g, int vid);
		static int topo_level_inv(Graph& g, int vid);
		static void topological_sort(Graph& g, char direction);
		static void topological_sort_inv(Graph& g, char direction);
		static void SpTreelabeling(Graph& tree);
		static int visit(Graph& tree, int vid, int& pre_post, vector<bool>& visited);
		static void topological_sort(Graph g, vector<int>& tso);
		static void traverse(Graph& tree, int vid, int& pre_post, vector<bool>& visited);
		static void pre_post_labeling(Graph& tree);

		static void genRandomGraph(int n, double c, char* filename);
};
#endif
