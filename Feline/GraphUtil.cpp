/* 
Author: Rene Rodrigues Veloso

Inspired on Hilmi Yildirim's code. 
(https://code.google.com/p/grail/)

For research purposes.
There is no additional support offered, nor are the author(s)
or their institutions liable under any circumstances.
*/
#include "GraphUtil.h"
#include <queue>


void GraphUtil::dfs(Graph& g, int vid, vector<int>& order1, vector<int>& order2, vector<bool>& visited) {
	visited[vid] = true;
	order1.push_back(vid);
	EdgeList el = g.out_edges(vid);
	EdgeList::iterator eit;
	int nextid = -1;

	srand ( unsigned ( time (NULL) ) );

	random_shuffle ( el.begin(), el.end() );

	for (eit = el.begin(); eit != el.end(); eit++) {
		if (!visited[*eit]) {
			nextid = *eit;
			dfs(g, nextid, order1, order2, visited);
		}
	}
	order2.push_back(vid);
}

void GraphUtil::dfs(Graph& g, int vid, vector<int>& order2, vector<bool>& visited, char direction) {
	visited[vid] = true;
	EdgeList el = g.out_edges(vid);
	EdgeList::iterator eit;
	EdgeList::reverse_iterator eite;
	int nextid = -1;

	if(direction == 'd'){
		for (eit = el.begin(); eit != el.end(); eit++) {
			if (!visited[*eit]) {
				nextid = *eit;
				dfs(g, nextid, order2, visited, direction);
			}
		}
	}
	else if(direction == 'e') { 
		for (eite = el.rbegin(); eite != el.rend(); eite++) {
                        if (!visited[*eite]) {
                                nextid = *eite;
                                dfs(g, nextid, order2, visited, direction);
                        }
                }
	}

//	copy.clear();
	order2.push_back(vid);
}

void GraphUtil::dfs_inv(Graph& g, int vid, vector<int>& order2, vector<bool>& visited, char direction) {
        visited[vid] = true;
        //order1.push_back(vid);
        EdgeList el = g.in_edges(vid);
//      EdgeList copy = el;
        EdgeList::iterator eit;
	 EdgeList::reverse_iterator eite;
        int nextid = -1;

        // check whether all child nodes are visited
	if(direction == 'd'){
        	for (eit = el.begin(); eit != el.end(); eit++) {
                	if (!visited[*eit]) {
                        	nextid = *eit;
                        	dfs_inv(g, nextid, order2, visited, direction);
                	}
        	}
	}
	else if(direction == 'e') {
		for (eite = el.rbegin(); eite != el.rend(); eite++) {
                        if (!visited[*eite]) {
                                nextid = *eite;
                                dfs_inv(g, nextid, order2, visited, direction);
                        }
                }
	}
//      copy.clear();
        order2.push_back(vid);
}

void GraphUtil::topo_leveler(Graph& g, bool PC_FILTER, bool INV, bool LEVEL_LIST, int spread){
	int N = g.num_vertices(), i;
	int levels = 0, maxlevels=0, maiorPai_x, maiorPai_y;
	vector<int>::iterator sit;
	double avg_diff_degree=0;

	g.getTopoSortList().clear();

    //encontra a primeira ordenacao topologica
	if(INV) topological_sort_inv(g,'d');
	else topological_sort(g, 'e');
	cout<<"1"<<endl;

	//if(INV) {
		for(i=0; i<N; i++)
			g[i].top_level = -1;
	//}

		for (i=0; i < N ; i++) {
				if(INV) g[i].tmp_outDegree = g.out_degree(i); 
				else g[i].tmp_inDegree = g.in_degree(i);
				if(INV) levels = topo_level_inv(g, i);
				else levels = topo_level(g, i);
				if(levels > maxlevels) maxlevels = levels;
		}

		g.setMaxLevels(maxlevels);
		cout << "# Levels: "<<maxlevels<<endl;
	//}


    //encontra a segunda ordenacao topologica
	priority_queue<IndexTable, vector<IndexTable>, IndexTableCompare> pq;
	int cont=1;
	vector<int>::reverse_iterator vrit;
	for (vrit = g.getTopoSortList().rbegin(); vrit != g.getTopoSortList().rend(); vrit++){
		if(!INV){
			g.getIndex()[*vrit].coord.x = cont++;
			g.getIndex()[*vrit].coord.y = 0;
			g.getIndex()[*vrit].level = g[(*vrit)].top_level;
		
			if(g.in_degree(*vrit) == 0 && g.out_degree(*vrit) > 0){
				pq.push(g.getIndex()[*vrit]);
   	        }
		}
		else {
             g.getIndexInv()[*vrit].coord.x = cont++;
             g.getIndexInv()[*vrit].coord.y = 0;
             g.getIndexInv()[*vrit].level = g[(*vrit)].top_level;
             
			if(g.out_degree(*vrit) == 0 && g.in_degree(*vrit) > 0){
             	 pq.push(g.getIndexInv()[*vrit]);
  	         }
		}
	}

	EdgeList el;
    EdgeList::iterator eit;
	if(!INV){
		for (i=0; i < N ; i++) {
			if(!pq.empty()){
			IndexTable u = pq.top();
			pq.pop();
			
			el = g.out_edges(u.id);
            for (eit = el.begin(); eit != el.end(); eit++){
				g[*eit].tmp_inDegree--;
				if(g[*eit].tmp_inDegree == 0){
					pq.push(g.getIndex()[*eit]);
				}
			}
			g.getIndex()[u.id].coord.y = i+1;
			}
		}
	}
	else {
		 for (i=0; i < N ; i++) {
			if(!pq.empty()){
                        IndexTable u = pq.top();
			            pq.pop();
                        el = g.in_edges(u.id);
                        for (eit = el.begin(); eit != el.end(); eit++){
                                g[*eit].tmp_outDegree--;
                                if(g[*eit].tmp_outDegree == 0){
                  					 pq.push(g.getIndexInv()[*eit]);
                                }
                        }
                        g.getIndexInv()[u.id].coord.y = i+1;
                	}
		}
	}

	if(PC_FILTER){
		for(i = 0 ; i< N; i++){
			g[i].pre = 0;//new vector<int>();
			g[i].post = 0;//new vector<int>();
			g[i].middle = 0;//new vector<int>();
			//visited[i]=-1;
		}

		SpTreelabeling(g);
	}

}

// compute interval label for each node of tree (pre_order, post_order)
void GraphUtil::SpTreelabeling(Graph& tree) {
	vector<int> roots = tree.getRoots();
	vector<int>::iterator sit;
	int pre_post = 0;
	vector<bool> visited(tree.num_vertices(), false);
//	random_shuffle(roots.begin(),roots.end());	
	for (sit = roots.begin(); sit != roots.end(); sit++) {
		pre_post++;
		visit(tree, *sit, pre_post, visited);
	}
}

// traverse tree to label node with pre and post order by giving a start node
int GraphUtil::visit(Graph& tree, int vid, int& pre_post, vector<bool>& visited) {
//	cout << "entering " << vid << endl;
	visited[vid] = true;
	EdgeList el = tree.out_edges(vid);
//	random_shuffle(el.begin(),el.end());
	EdgeList::iterator eit;
	int pre_order = tree.num_vertices()+1;
	tree[vid].middle = pre_post;//->push_back(pre_post);
	for (eit = el.begin(); eit != el.end(); eit++) {
		if (!visited[*eit]){
			pre_order=min(pre_order,visit(tree, *eit, pre_post, visited));
		}else
			pre_order=min(pre_order,tree[*eit].pre);//->back());
			//pre_order=min(pre_order,tree[*eit].pre->at(tree[*eit].pre->size()-1));
	}
	
	pre_order=min(pre_order,pre_post);
	tree[vid].pre = pre_order;//->push_back(pre_order);
	tree[vid].post = pre_post;//->push_back(pre_post);
	pre_post++;
	return pre_order;
}

int GraphUtil::topo_level(Graph& g, int vid){
	if(g[vid].top_level != -1){
		return g[vid].top_level;
	}
	int min = g.num_vertices();
	int max = -1;
	g[vid].top_level = 0;
	EdgeList el = g.in_edges(vid);
	EdgeList::iterator eit;
	for(eit = el.begin(); eit != el.end(); eit++){
		max = max > topo_level(g,*eit) ? max : g[*eit].top_level;
		min = min < g[*eit].top_level ? min : g[*eit].top_level;
	}
	g[vid].top_level = max + 1;
	g[vid].min_parent_level = (min == g.num_vertices() ? -1 : min );
	return g[vid].top_level;
}

int GraphUtil::topo_level_inv(Graph& g, int vid){
        if(g[vid].top_level != -1){
                return g[vid].top_level;
        }
        int min = g.num_vertices();
        int max = -1;
        g[vid].top_level = 0;
        EdgeList el = g.out_edges(vid);
        EdgeList::iterator eit;
        for(eit = el.begin(); eit != el.end(); eit++){
                max = max > topo_level_inv(g,*eit) ? max : g[*eit].top_level;
                min = min < g[*eit].top_level ? min : g[*eit].top_level;
        }
        g[vid].top_level = max + 1;
        g[vid].min_parent_level = (min == g.num_vertices() ? -1 : min );
        return g[vid].top_level;
}

// topological sorting by depth-first search
// return reverse order of topological_sorting list
void GraphUtil::topological_sort(Graph& g, char direction) {
    g.setContador(0);
	vector<bool> visited(g.num_vertices(), false);
	//vector<int> order1;
	vector<int> order2;
	vector<int> roots = g.getRoots();	
	vector<int>::iterator sit;
	vector<int>::reverse_iterator site;
	
	if(direction == 'd') {
		for (sit = roots.begin(); sit != roots.end(); sit++)
			if (!visited[*sit])
				dfs(g, (*sit), g.getTopoSortList(), visited, direction);
	}
	else if(direction == 'e'){ 
		for (site = roots.rbegin(); site != roots.rend(); site++)
                        if (!visited[*site])
                                dfs(g, (*site), g.getTopoSortList(), visited, direction);
	}

}

void GraphUtil::topological_sort_inv(Graph& g, char direction) {
    g.setContador(0);
        vector<bool> visited(g.num_vertices(), false);
        //vector<int> order1;
        vector<int> order2;
        vector<int> leafs = g.getLeafs();
        vector<int>::iterator sit;
	vector<int>::reverse_iterator site;

        // depth-first-search whole graph
	if(direction == 'd'){
        	for (sit = leafs.begin(); sit != leafs.end(); sit++)
                	if (!visited[*sit])
                        	dfs_inv(g, (*sit), g.getTopoSortList(), visited, direction);
	}
	else if(direction == 'e'){ 
		for (site = leafs.rbegin(); site != leafs.rend(); site++)
                        if (!visited[*site])
                                dfs_inv(g, (*site), g.getTopoSortList(), visited, direction);
	}
}


void GraphUtil::topological_sort(Graph g, vector<int>& tso) {
		vector<bool> visited(g.num_vertices(), false);
	vector<int> order1;
	vector<int> order2;
	vector<int> roots = g.getRoots();
	vector<int>::iterator sit;


	for (sit = roots.begin(); sit != roots.end(); sit++) 
		if (!visited[*sit])
			dfs(g, (*sit), order1, order2, visited);
	
	tso = order2;

}


void GraphUtil::traverse(Graph& tree, int vid, int& pre_post, vector<bool>& visited) {
	visited[vid] = true;
	EdgeList el = tree.out_edges(vid);
	EdgeList::iterator eit;
	int pre_order;
	for (eit = el.begin(); eit != el.end(); eit++) {
		pre_order = pre_post;
		pre_post++;
		if (!visited[*eit])
			traverse(tree, *eit, pre_post, visited);
		tree[*eit].pre_order = pre_order;
		tree[*eit].post_order = pre_post;
		pre_post++;
	}
}

// compute interval label for each node of tree (pre_order, post_order)
void GraphUtil::pre_post_labeling(Graph& tree) {
	vector<int> roots = tree.getRoots();
	vector<int>::iterator sit;
	int pre_post = 0;
	int pre_order = 0;
	vector<bool> visited(tree.num_vertices(), false);

	for (sit = roots.begin(); sit != roots.end(); sit++) {
		pre_order = pre_post;
		pre_post++;
		traverse(tree, *sit, pre_post, visited);
		tree[*sit].pre_order = pre_order;
		tree[*sit].post_order = pre_post;
		pre_post++;
	}
}


// for test
void GraphUtil::genRandomGraph(int n, double c, char* filename) {
	int threshold = int(c*1.00/(n*1.00)*10000);
	Graph g;
	int i, j;
	int rand_num;
	for (i = 0; i < n; i++)
		g.addVertex(i);

	srand(time(NULL));
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			if (i != j) {
				rand_num = rand() % 10000;
				if (rand_num < threshold)
					g.addEdge(i,j);
			}
	}

	ofstream out(filename);
	g.writeGraph(out);
}
