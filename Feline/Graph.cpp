/*
Author: Rene Rodrigues Veloso

Inspired on Hilmi Yildirim's code.
(https://code.google.com/p/grail/)

For research purposes.
There is no additional support offered, nor are the author(s)
or their institutions liable under any circumstances.
*/
#include "Graph.h"
#include <math.h>


Graph::Graph() {
	graph = GRA();
	vl = VertexList();
}
Graph::Graph(int size) {
	vsize = size;
	vl = VertexList(size);
	graph = GRA(size, In_OutList());
}

Graph::Graph(GRA& g, VertexList& vlist) {
	vsize = vlist.size();
	graph = g;
	vl = vlist;
}

Graph::Graph(istream& in) {
	readGraph(in);
}

Graph::~Graph() {}

void Graph::printGraph() {
	writeGraph(cout);
}

void Graph::clear() {
	vsize = 0;
	graph.clear();
	vl.clear();
}

int Graph::getContador() {
    return contador;
}

void Graph::setContador(int c){
    contador = c;
}

void Graph::strTrimRight(string& str) {
	string whitespaces(" \t");
	int index = str.find_last_not_of(whitespaces);
	if (index != string::npos)
		str.erase(index+1);
	else
		str.clear();
}

void Graph::readGraph(istream& in) {
	string buf;
	getline(in, buf);

	strTrimRight(buf);
	if (buf != "graph_for_greach") {
		cout << "BAD FILE FORMAT!" << endl;
		//exit(0);
	}

	int n;
	getline(in, buf);
	istringstream(buf) >> n;
	cout<<" initializing..."<<endl;
	vsize = n;

	index = Index(n);
	indexInv = Index(n);
	vl = VertexList(n);
	graph = GRA(n, In_OutList());

	for (int i = 0; i < n; i++)
		addVertex(i);

	cout <<"reading graph..."<<endl;
	esize=0;

	string sub;
	int idx;
	int sid = 0;
	int tid = 0;
	while (getline(in, buf)) {
		strTrimRight(buf);
		idx = buf.find(":");
			sub = buf.substr(0, buf.find(":"));
			istringstream(sub) >> sid;
		buf.erase(0, idx+2);
		while (buf.find(" ") != string::npos) {
			sub = buf.substr(0, buf.find(" "));
			istringstream(sub) >> tid;
			buf.erase(0, buf.find(" ")+1);
			if(sid == tid)
				cout << "Self-edge " << sid << endl;
			if(tid < 0 || tid > n)
				cout << "Wrong tid " << tid << endl;

			addEdge(sid, tid);
			esize++;
		}
	}

	NegativeCut = PositiveCut = ConstantCut = 0;
}

void Graph::writeGraph(ostream& out) {
	cout << "Graph size = " << graph.size() << endl;
	out << "graph_for_greach" << endl;
	out << vl.size() << endl;

	GRA::iterator git;
	EdgeList el;
	EdgeList::iterator eit;
	VertexList::iterator vit;
	int i = 0;
	for (i = 0; i < vl.size(); i++) {
		out << i << ": ";
		el = graph[i].outList;
		for (eit = el.begin(); eit != el.end(); eit++)
			out << (*eit) << " ";
		out << "#" << endl;
	}
/*
	cout << "** In List for graph **" << endl;
	for (i = 0; i < vl.size(); i++) {
		out << i << ": ";
		el = graph[i].inList;
		for (eit = el.begin(); eit != el.end(); eit++)
			out << (*eit) << " ";
		out << "#" << endl;
	}
*/
}

void Graph::writeGraph_forInterval(ostream& out) {
        out << vl.size() << " " << esize << endl;

        GRA::iterator git;
        EdgeList el;
        EdgeList::iterator eit;
        VertexList::iterator vit;
        int i = 0;
        for (i = 0; i < vl.size(); i++) {
                //out << i << ": ";
                el = graph[i].outList;
                for (eit = el.begin(); eit != el.end(); eit++){
                        if(eit < el.end()-1) out << (*eit) + 1 << " ";
			else if(eit == el.end()-1) out << (*eit) + 1;
		}
                out << endl;
        }
/*
        cout << "** In List for graph **" << endl;
        for (i = 0; i < vl.size(); i++) {
                out << i << ": ";
                el = graph[i].inList;
                for (eit = el.begin(); eit != el.end(); eit++)
                        out << (*eit) << " ";
                out << "#" << endl;
        }
*/
}

void Graph::writeGraph_inv(ostream& out) {
        cout << "Graph size = " << graph.size() << endl;
        out << "graph_for_greach" << endl;
        out << vl.size() << endl;

        GRA::iterator git;
        EdgeList el;
        EdgeList::iterator eit;
        VertexList::iterator vit;
        int i = 0;
	
        for (i = 0; i < vl.size(); i++) {
                out << i << ": ";
                el = graph[i].inList;
                for (eit = el.begin(); eit != el.end(); eit++)
                        out << (*eit) << " ";
                out << "#" << endl;
        }
/*
        cout << "** In List for graph **" << endl;
        for (i = 0; i < vl.size(); i++) {
                out << i << ": ";
                el = graph[i].inList;
                for (eit = el.begin(); eit != el.end(); eit++)
                        out << (*eit) << " ";
                out << "#" << endl;
        }
*/
}


void Graph::writeGraph_levels(ostream& out, int level) {
//        cout << "Graph size = " << graph.size() << endl;
  //      out << "graph_for_greach" << endl;
    //    out << vl.size() << endl;

	int cont=0;

        GRA::iterator git;
        EdgeList el;
        EdgeList::iterator eit;
        VertexList::iterator vit;
        int i = 0;

	cont=0;
	int j;

	for (i = 0; i < vl.size(); i++) {
                if(vl[i].top_level <= level){                        
			vl[i].id = cont;
			
			cont++;
		}
	}


	cout << "Graph size = " << cont << endl;
        out << "graph_for_greach" << endl;
        out << cont << endl;
        for (i = 0; i < vl.size(); i++) {
		if(vl[i].top_level <= level){
                	out << vl[i].id << ": ";
               	 	el = graph[i].outList;
             		for (eit = el.begin(); eit != el.end(); eit++)
                        			out << vl[(*eit)].id << " ";
                	out << "#" << endl;
		}
        }
	
/*
        cout << "** In List for graph **" << endl;
        for (i = 0; i < vl.size(); i++) {
                out << i << ": ";
                el = graph[i].inList;
                for (eit = el.begin(); eit != el.end(); eit++)
                        out << (*eit) << " ";
                out << "#" << endl;
        }
*/
}

void Graph::writeIndex(ostream& out) {
	out <<"#size: "<<index.size() << endl;
	out <<"#ID: level X Y { level X Y (inv)} "<<endl;
	
	int i,d;
	for(i=0; i<index.size(); i++) {
	  if(graph[i].inList.size() > 0 || graph[i].outList.size() > 0){ 
		out << i << ": " << index[i].level;
		out << " " <<index[i].coord.x << " " << index[i].coord.y;
		out << " { "<<indexInv[i].level<<" "<<indexInv[i].coord.x<<" "<<indexInv[i].coord.y<<" } ";
                out << " [ "<<vl[i].middle<<","<<vl[i].post<<" ] "; 
		out << endl;
	  }
	}

}

void Graph::addVertex(int vid) {
	int i;

	if (vid >= vl.size()) {
		int size = vl.size();
		for (int i = 0; i < (vid-size+1); i++) {
			graph.push_back(In_OutList());
			vl.push_back(Vertex(vid+i));
			index.push_back(IndexTable(vid+i));
		}
		vsize = vl.size();
	}

	Vertex v;
	v.id = vid;
	v.top_level = -1;
	v.visited = false;
	vl[vid] = v;

	IndexTable c;
	c.id = vid;
	c.level = -1;
	c.coord.x = c.coord.y = -1;
	index[vid] = c;

	IndexTable ivt;
	ivt.id = vid;
	ivt.level = -1;
	ivt.coord.x = ivt.coord.y = -1;
	indexInv[vid] = ivt;

	EdgeList il = EdgeList();
	EdgeList ol = EdgeList();
	In_OutList oil = In_OutList();
	oil.inList = il;
	oil.outList = ol;
	graph[vid] = oil;
}

void Graph::addVertexLl(Vertex& v) {
//	ll[v.top_level].listv.push_back(v.id);

}

void Graph::addEdge(int sid, int tid) {
	if (sid >= vl.size())
		addVertex(sid);
	if (tid >= vl.size())
		addVertex(tid);
	// update edge list
	graph[tid].inList.push_back(sid);
	graph[sid].outList.push_back(tid);
}

int Graph::num_vertices() {
	return vl.size();
}

int Graph::num_edges() {
	EdgeList el;
	GRA::iterator git;
	int num = 0;
	for (git = graph.begin(); git != graph.end(); git++) {
		el = git->outList;
		num += el.size();
	}
	return num;
}

// return out edges of specified vertex
EdgeList& Graph::out_edges(int src) {
	return graph[src].outList;
}

// return in edges of specified vertex
EdgeList& Graph::in_edges(int trg) {
	return graph[trg].inList;
}

int Graph::out_degree(int src) {
	return graph[src].outList.size();
}

int Graph::in_degree(int trg) {
	return graph[trg].inList.size();
}

// get roots of graph (root is zero in_degree vertex)
vector<int> Graph::getRoots() {
	vector<int> roots;
	GRA::iterator git;
	int i = 0;
	for (git = graph.begin(), i = 0; git != graph.end(); git++, i++) {
		if (git->inList.size() == 0 && git->outList.size() > 0)
			roots.push_back(i);
	}

	return roots;
}

// get leafs of graph (leaf is zero out_degree vertex)
vector<int> Graph::getLeafs() {
        vector<int> leafs;
        GRA::iterator git;
        int i = 0;
        for (git = graph.begin(), i = 0; git != graph.end(); git++, i++) {
                if (git->outList.size() == 0 && git->inList.size() > 0){
                        leafs.push_back(i);
		        }
        }

        return leafs;
}

//get branch nodes
vector<int> Graph::getBranches() {
        vector<int> branches;
        GRA::iterator git;
        int i = 0;

        for (git = graph.begin(), i = 0; git != graph.end(); git++, i++) {
                if (git->outList.size() > 0 && git->inList.size() > 0){
                        branches.push_back(i);
                }
        }
        return branches;
}

vector<int> Graph::getAloneVertices() {
        vector<int> branches;
        GRA::iterator git;
        int i = 0;

        for (git = graph.begin(), i = 0; git != graph.end(); git++, i++) {
                if (git->outList.size() == 0 && git->inList.size() == 0){
                        branches.push_back(i);
                }
        }
        return branches;
}

void Graph::printAvgInOutDegreDist() {
        vector<float> inDegreeDist;
	    vector<float> outDegreeDist;
        GRA::iterator git;
        int i = 0, maxIn=0, maxOut=0, avgOut=0, avgIn=0;

        for (git = graph.begin(), i = 0; git != graph.end(); git++, i++) {
                if (git->outList.size() > maxOut)  
			maxOut = git->outList.size();
		avgOut+=git->outList.size();
		avgIn+=git->inList.size();
		if(  git->inList.size() > maxIn )
			maxIn =  git->inList.size();
        }

	for(i=0; i<=maxOut; i++) outDegreeDist.push_back(0);
	for(i=0; i<=maxIn; i++) inDegreeDist.push_back(0);

	for (git = graph.begin(), i = 0; git != graph.end(); git++, i++) {
		outDegreeDist[git->outList.size()]++;
		inDegreeDist[git->inList.size()]++;
	}
    ofstream outIn("inDist.txt");

	for(i=0; i<=maxIn; i++)
		outIn << i+1 << " " << inDegreeDist[i]<<endl;///vl.size() << endl;
	
    outIn.close();
    ofstream outOut("outDist.txt");

        for(i=0; i<=maxOut; i++)
                outOut << i+1 << " " << outDegreeDist[i]<<endl;///vl.size() << endl;
     outOut.close();

     return;
}

// check whether the edge from src to trg is in the graph
bool Graph::hasEdge(int src, int trg) {
	EdgeList el = graph[src].outList;
	EdgeList::iterator ei;
	for (ei = el.begin(); ei != el.end(); ei++)
		if ((*ei) == trg)
			return true;
	return false;

}

// return vertex list of graph
VertexList& Graph::vertices() {
	return vl;
}

Index& Graph::getIndex() {
	return index;
}

Index& Graph::getIndexInv() {
        return indexInv;
}


void Graph::initializeVisited(int maxid){
	int i;
	visited = new int[maxid];
	for(i = 0 ; i< maxid; i++){
		visited[i]=-1;
	}
}

int Graph::getVisited(int pos){
	return visited[pos];
}

void Graph::setQueryCnt(int cnt){
	QueryCnt = cnt;
}
		
int Graph::getQueryCnt(){
	return QueryCnt;
}

LevelList& Graph::getLevelList() {
    return ll;
}

TopoSortList& Graph::getTopoSortList() {
    return ts;
}

void Graph::setTopoSortList(vector<int>& tso) {
    ts = tso;
}

int Graph::getMaxLevels() {
    return maxlevels;
}

int Graph::getNegativeCut() {
    return NegativeCut;
}

void Graph::setNegativeCut(int n) {
    NegativeCut = n;
}

int Graph::getConstantCut() {
    return ConstantCut;
}

int Graph::getPositiveCut() {
    return PositiveCut;
}

int Graph::getHopsTotal() {
    return hopsTotal;
} 


void Graph::setMaxLevels(int max) {
    maxlevels = max;
}

Graph& Graph::operator=(const Graph& g) {
	if (this != &g) {
		graph = g.graph;
		vl = g.vl;
		vsize = g.vsize;
	}
	return *this;
}

// get a specified vertex property
Vertex& Graph::operator[](const int vid) {
	return vl[vid];
}

const double Graph::actualgap(const int vid){
		return vl[vid].mingap;
//	return vl[vid].mingap - vl[vid].tcs;
}

const double Graph::tcs(const int vid){
	return vl[vid].tcs;
}
Graph::Graph(hash_map<int,vector<int> >& inlist, hash_map<int,vector<int> >& outlist) {
	vsize = inlist.size();
	vl = VertexList(vsize);
	graph = GRA(vsize, In_OutList());
	for (int i = 0; i < vsize; i++)
		addVertex(i);
	hash_map<int,vector<int> >::iterator hit, hit1;
	hash_map<int,int> indexmap;
	vector<int> vec;
	vector<int>::iterator vit;
	int k;
	for (hit = inlist.begin(), k = 0; hit != inlist.end(); hit++,k++) {
		indexmap[hit->first] = k;
	}
	for (hit = inlist.begin(), hit1 = outlist.begin(), k = 0; hit != inlist.end(); hit++, hit1++, k++) {
		vec = hit->second;
		for (vit = vec.begin(); vit != vec.end(); vit++)
			graph[k].inList.push_back(indexmap[*vit]);
		vec = hit1->second;
		for (vit = vec.begin(); vit != vec.end(); vit++)
			graph[k].outList.push_back(indexmap[*vit]);
	}
}

void Graph::extract(hash_map<int,vector<int> >& inlist, hash_map<int,vector<int> >& outlist) {
	for (int i = 0; i < vl.size(); i++) {
		inlist[i] = graph[i].inList;
		outlist[i] = graph[i].outList;
	}
//	printMap(inlist,outlist);
}

// for test
void Graph::printMap(hash_map<int,vector<int> >& inlist, hash_map<int,vector<int> >& outlist) {
	cout << "==============================================" << endl;
	hash_map<int, vector<int> >::iterator hit;
	vector<int>::iterator vit;
	for (hit = outlist.begin(); hit != outlist.end(); hit++) {
		cout << hit->first << ": ";
		vector<int> vec = hit->second;
		for (vit = vec.begin(); vit != vec.end(); vit++)
			cout << *vit << " ";
		cout << "#" << endl;
	}
	cout << "In List for graph" << endl;
	for (hit = inlist.begin(); hit != inlist.end(); hit++) {
		cout << hit->first << ": ";
		vector<int> vec = hit->second;
		for (vit = vec.begin(); vit != vec.end(); vit++)
			cout << *vit << " ";
		cout << "#" << endl;
	}
	cout << "================================================" << endl;
}

void Graph::printLevelList() {
//	LevelList::iterator lit;
//	vector<int>::iterator vit;
//	for(lit = ll.begin(); lit != ll.end(); lit++){
//		cout << "Nivel "<<(*lit).level<< ": ";
//		for(vit = (*lit).listv.begin(); vit != (*lit).listv.end(); vit++)
//			cout << (*vit)<< "["<<index[(*vit)].x<<","<<index[(*vit)].y<<"] ";
  //      cout << endl;
//	}
}


bool Graph::contains(int src,int trg){
	int i;

    if(index[src].coord.x > index[trg].coord.x){
    	return false;
	}
	if(index[src].coord.y > index[trg].coord.y){
		return false;
	}

	return true;
}

bool Graph::contains_inv(int src,int trg){
        int i;
        if(index[src].coord.x > index[trg].coord.x  ||   indexInv[trg].coord.x > indexInv[src].coord.x){
             return false;
		}
        if(index[src].coord.y > index[trg].coord.y  ||   indexInv[trg].coord.y > indexInv[src].coord.y){
             return false;
		}
        return true;
}

int Graph::contains_pc_inv(int src,int trg){
        int i;

        if(index[src].coord.x > index[trg].coord.x  ||   indexInv[trg].coord.x > indexInv[src].coord.x)
               return -1;
        if(index[src].coord.y > index[trg].coord.y  ||   indexInv[trg].coord.y > indexInv[src].coord.y)
               return -1;
        if(vl[src].middle <= vl[trg].middle && vl[src].post >= vl[trg].post)
               return 1;
 
        return 0;
}


int Graph::contains_pc(int src,int trg){
        int i;

       	if(index[src].coord.x > index[trg].coord.x)
               	return -1;
       	if(index[src].coord.y > index[trg].coord.y)
               	return -1;
		if(vl[src].middle <= vl[trg].middle && vl[src].post >= vl[trg].post)
			return 1;

        return 0;
}

int Graph::calcPruneTree(int src, int trg, int& vertices, int& edges, vector<bool>& visited){
	visited[src] = true;
	EdgeList el = out_edges(src);
	EdgeList::iterator eit;
	int count=0;

	vertices++;

     for (eit = el.begin(); eit != el.end(); eit++) 
        if( contains (*eit, trg) )
        		edges++;

	for (eit = el.begin(); eit != el.end(); eit++)
                if( (!visited[*eit]) && contains (*eit, trg) )
                        calcPruneTree(*eit, trg, vertices, edges, visited);
                        
        
	return 0;
}

void Graph::calcPrune(int src, int trg) {
	int vertices = 0, edges = 0;
	vector<bool> visited(num_vertices(), false);


	calcPruneTree(src,trg,vertices,edges,visited);

	cout << "#vertices: "<<vertices<<"  #edges: "<<edges<<endl;

}


//*******************************************Algorithm 4
bool Graph::go_for_reach(int src, int trg, bool LEVEL_FILTER) {
#ifdef DEBUG
	TotalCall++;
#endif
	
	if(src==trg)
		return true;

	if(LEVEL_FILTER)
		if(index[src].level >= index[trg].level)
	                return false;


	visited[src] = QueryCnt;
	EdgeList el = out_edges(src);
    EdgeList::iterator eit;

	for (eit = el.begin(); eit != el.end(); eit++) {
		if(visited[*eit]!=QueryCnt && contains(*eit,trg)){
			if(go_for_reach(*eit,trg,LEVEL_FILTER)){
				return true;
			}
		}
	}
	return false;
}

bool Graph::go_for_reach_inv(int src, int trg, bool LEVEL_FILTER) {
#ifdef DEBUG
        TotalCall++;
#endif

        if(src==trg)
                return true;

    	if(LEVEL_FILTER)
	    	if(index[src].level >= index[trg].level || indexInv[trg].level >= indexInv[src].level)
                	return false;


        visited[src] = QueryCnt;
        EdgeList el = out_edges(src);
        EdgeList::iterator eit;

        for (eit = el.begin(); eit != el.end(); eit++) {
                if(visited[*eit]!=QueryCnt && contains_inv(*eit,trg)){
                        if(go_for_reach_inv(*eit,trg,LEVEL_FILTER)){
                                return true;
                        }
                }
        }
        return false;
}

bool Graph::go_for_reach_pc_inv(int src, int trg, bool LEVEL_FILTER, int hop) {
        int res, vertice;
	
        if(src==trg)
                return true;

	if(LEVEL_FILTER)
        	if(index[src].level >= index[trg].level || indexInv[trg].level >= indexInv[src].level)
                	return false;

        visited[src] = QueryCnt;
        EdgeList el = out_edges(src);
        EdgeList::iterator eit;

        EdgeList el2 = out_edges(src);
        EdgeList::iterator eit2;

	if(hop > hopsTotal) hopsTotal = hop;


        for (eit = el.begin(); eit != el.end(); eit++) {
                if(visited[*eit]!=QueryCnt){
                        res = contains_pc_inv(*eit,trg);
                        switch(res){
                                case 1 :
                                        return true;
                                case 0 : if (go_for_reach_pc_inv(*eit,trg,LEVEL_FILTER, hop+1))
                                        return true;
                                        break;
                                case -1 :  //NegativeCut++; 
                                        break;
                        }
                }
        }

        return false;
}

bool Graph::go_for_reach_pc(int src, int trg, bool LEVEL_FILTER, int hop) {
	int res;
	
	if(src==trg)
		return true;
	
	if(LEVEL_FILTER)
		if(index[src].level >= index[trg].level){
			return false;
		}

	visited[src] = QueryCnt;
	EdgeList el = out_edges(src);
   	EdgeList::iterator eit;   

	 if(hop > hopsTotal) hopsTotal = hop;

	for (eit = el.begin(); eit != el.end(); eit++) {
		if(visited[*eit]!=QueryCnt){
			res = contains_pc(*eit,trg);
			switch(res){
				case 1 : 	
					return true;
				case 0 : if (go_for_reach_pc(*eit,trg,LEVEL_FILTER, hop+1))
					return true; 
					break;
				case -1 :  // NegativeCut++; 
					break;
			}
		}
	}

	return false;
}

bool Graph::reach(int src,int trg, bool LEVEL_FILTER){
	if(src == trg){
		return true;
	}

	if(LEVEL_FILTER)
		if(index[src].level >= index[trg].level)
                	return false;

	if(!contains(src,trg))						// if it does not contain reject
		return false;
										
	visited[src]=++QueryCnt;


	return go_for_reach(src,trg,LEVEL_FILTER);
}

bool Graph::reach_inv(int src,int trg, bool LEVEL_FILTER){
        if(src == trg){
                return true;
        }


	if(LEVEL_FILTER)
		if(index[src].level >= index[trg].level || indexInv[trg].level >= indexInv[src].level)
                	return false;
	

        if(!contains_inv(src,trg))                                          // if it does not contain reject
                return false;

                                                                                
        visited[src]=++QueryCnt;


        return go_for_reach_inv(src,trg,LEVEL_FILTER);
}

bool Graph::reach_pc_inv(int src,int trg, bool LEVEL_FILTER){

        if(src == trg){
		ConstantCut++;
                return true;
        }

	if(LEVEL_FILTER)
        	if(index[src].level >= index[trg].level || indexInv[trg].level >= indexInv[src].level){
			NegativeCut++;
			ConstantCut++;
            		return false;
		}

        int res = contains_pc_inv(src,trg);
        if(res){                                                // if it does not contain reject
                switch(res){
                        case -1 :	 NegativeCut++; 
					ConstantCut++;
                                                                return false;
                        case 1 :
					ConstantCut++;
					PositiveCut++;
                                                                return true;
                }
        }
        //CurrentDepth = 0;

	hopsTotal=0;

        visited[src]=++QueryCnt;
        return go_for_reach_pc_inv(src,trg,LEVEL_FILTER,0);
}


bool Graph::reach_pc(int src,int trg, bool LEVEL_FILTER){

	if(src == trg){
		ConstantCut++;
		return true;
	}

	if(LEVEL_FILTER)
		if(index[src].level >= index[trg].level){
			ConstantCut++;
			NegativeCut++;
    	    		return false;
		}
	int res = contains_pc(src,trg);
	if(res){						
		switch(res){
			case -1 : 	 NegativeCut++; 
					 ConstantCut++; 
								return false; 
			case 1 : 
					ConstantCut++;
					PositiveCut++;
								return true;
		}
	}
	//CurrentDepth = 0;

	hopsTotal = 0;
	
	visited[src]=++QueryCnt;
	return go_for_reach_pc(src,trg,LEVEL_FILTER,0);
}
