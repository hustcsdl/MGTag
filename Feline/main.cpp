/*
Author: Rene Rodrigues Veloso

Inspired on Hilmi Yildirim's code.
(https://code.google.com/p/grail/)

For research purposes.
There is no additional support offered, nor are the author(s)
or their institutions liable under any circumstances.
*/
#include "Graph.h"
#include "GraphUtil.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <cstring>

#include "TripleBit.h"
#include "URITable.h"

#include "utils.h"

int dims=1;

	char* filename = NULL;
	char* testfilename = NULL;
	bool debug = false;
	bool PC_FILTER = true;
	bool INV_FILTER = false;
	bool indexPrint = false;
	bool LEVEL_LIST = true;
	float labeling_time, query_time, query_timepart,exceptionlist_time;
	int spread=1;  // reserved for future

	bool QUERY = false;
	int u=-1, v=-1;
	bool LEVEL_FILTER = false;
	bool countNodes = false;
	char fileCountNodes[100];

static void usage() {
	cout << "\nUsage:\n"
		" feline [-h] <filename> -test <testfilename> <dictionary path> <string vertex pair file>\n"
		"Description:\n"
		"	-h			Print the help message.\n"
		"  <filename> is the name of the input graph in gra format.\n"
		"	-test		Set the queryfile which contains a line of <source> <target> <reachability> for each query. \n"
		//"	-pc		    Set the positive cut filter. \n"
		"	-inv		Set the inversion filter (B-Feline).\n"
		"	-index		Save the index to index.out. \n"
		//"	-llist		Print the number of vertice per level. \n"
		//"	-query 	<u> <v>	Get the status of a query u->v.\n"
		//"	-lf		Level filter\n"
		<< endl;
}


static void parse_args(int argc, char *argv[]){
	if (argc == 1) {
		usage();
		exit(0);
	}

 	int i = 1;

	strcpy(fileCountNodes, "countNodes.out");

	while (i < argc) {
		if (strcmp("-h", argv[i]) == 0) {
			usage();
			exit(0);
		}else if (strcmp("-test", argv[i]) == 0) {
			i++;
			testfilename = argv[i++];
		}else if (strcmp("-pc", argv[i]) == 0) {
			i++;
			PC_FILTER = true;
		}
		else if (strcmp("-inv", argv[i]) == 0) {
                        i++;
                        INV_FILTER = true;
                }
		//else if (strcmp("-dim", argv[i]) == 0) {
		//	i++;
		//	dims = atoi(argv[i++]);
		//}
		else if(strcmp("-index", argv[i]) == 0) {
			i++;
			indexPrint = true;
		}
		//else if(strcmp("-query", argv[i]) == 0) {
        //                i++;
        //                QUERY = true;
		//	u = atoi(argv[i++]);
		//	v = atoi(argv[i++]);
        //        }
		//else if(strcmp("-llist", argv[i]) == 0) {
        //                i++;
        //                LEVEL_LIST = true;
        //        }
		else if(strcmp("-lf", argv[i]) == 0) {

                        i++;
                        LEVEL_FILTER = true;
                }
		else {
			filename = argv[i++];
		}
	}
	if(!testfilename){
		cout << "Please provide a test file : -test <testfilename> " << endl;
		exit(0);
	}
}




int main(int argc, char* argv[]) {
	if(argc != 5) {
		fprintf(stderr, "Usage: %s <primary graph> <index flag> <dictionary path> <vertex_string_pair file>\n", argv[0]);
		return -1;
	}
	struct timeval start_time, end_time;
	gettimeofday(&start_time, NULL);
	//parse_args(argc,argv);

	/*
		Read Graph from the input file
	*/
	ifstream infile(string(argv[1]).c_str());
	if (!infile) {
		cout << "Error: Cannot open " << string(argv[1]).c_str() << endl;
		return -1;
	}

	Graph g(infile);
	cout << "#vertex size:" << g.num_vertices() << "\t#edges size:" << g.num_edges() << endl;
	//cout << "#roots: " << g.getRoots().size() <<endl;
	//cout << "#leafs: " << g.getLeafs().size() <<endl;
	//cout << "#branches: " << g.getBranches().size() <<endl;
	//cout << "#alone vertices: " << g.getAloneVertices().size() <<endl;	

// ofstream outInv("graph.inv");
// g.writeGraph_inv(outInv);
// outInv.close();

// ofstream outInt("graph.interval");
// g.writeGraph_forInterval(outInt);
// outInt.close();
//return 0;

	int s, t, i, t_max, s_max, reachable_max;
	int left = 0;
	int gsize = g.num_vertices();

	bool r;
	struct timeval after_time, before_time, after_timepart, before_timepart;

	float query_max=0, avg_query=0;;

	// prepare queries
	srand48(time(NULL));
	cout << "preparing queries..." << endl;
	vector<int> src;
	vector<int> trg;
	vector<int> labels;
	vector<int>::iterator sit, tit, lit;
	int success=0,fail=0;
	int label;
	
	URITable *uriTable = URITable::load(argv[3]);
	unsigned vertex1, vertex2;
	char vertex1_str[2048], vertex2_str[2048];
	Status status1, status2;
	freopen(string(argv[4]).c_str(), "r", stdin);
	while(scanf("%s %s", vertex1_str, vertex2_str) != EOF){
		status1 = uriTable->getIdByURI(vertex1_str, vertex1);
		status2 = uriTable->getIdByURI(vertex2_str, vertex2);
		if(status1 == URI_FOUND && status2 == URI_FOUND){
			src.push_back(vertex1);
			trg.push_back(vertex2);
			labels.push_back(random() % 2);
		}
	}
	fclose(stdin);

	cout << "queries are ready" << endl;
	gettimeofday(&end_time, NULL);
	float prepare_time = (end_time.tv_sec - start_time.tv_sec)*1000.0 + (end_time.tv_usec - start_time.tv_usec)*1.0/1000.0;
	cout<<"prepare time elapse: "<<prepare_time<<" ms"<<endl;

	gettimeofday(&before_time, NULL);

    //start indexing
    	GraphUtil::topo_leveler(g, PC_FILTER, false, LEVEL_LIST, spread);
	
	if(INV_FILTER)  // for B-Feline
		GraphUtil::topo_leveler(g, PC_FILTER, true, LEVEL_LIST, spread);
	
	g.initializeVisited(g.num_vertices());
	g.setQueryCnt(0);

	if(atoi(argv[2])){	
		//ofstream outGRAPH("inverted.gra");
                //g.writeGraph_inv(outGRAPH);
                //outGRAPH.close();

		ofstream outIndex("index.out");
        	g.writeIndex(outIndex);
        	outIndex.close();
	}

	gettimeofday(&after_time, NULL);

	labeling_time = (after_time.tv_sec - before_time.tv_sec)*1000.0 +
		(after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
	cout << "#construction time:" << labeling_time << " (ms)" << endl;
	cout << dims <<" dims" << endl;

	// process queries
	cout << "process queries..." << endl;
	//gettimeofday(&before_time, NULL);

	int seenpositive = 0;

	int source, target, contain, cont=0;
	int reachable = 0, nonreachable =0, falsep_count=0;
	int nc_count=0;
	int avgHops = 0;

	for (sit = src.begin(), tit = trg.begin(), lit = labels.begin();sit != src.end(); ++sit, ++tit, ++lit) {

		s = *sit;
 	    t = *tit;
		if(PC_FILTER && INV_FILTER==false){
			r = g.reach_pc(s,t,LEVEL_FILTER);
			avgHops += g.getHopsTotal();
		}
		else if(PC_FILTER == false && INV_FILTER)
			r = g.reach_inv(s,t,LEVEL_FILTER);
		else if(PC_FILTER && INV_FILTER){ 	
			r = g.reach_pc_inv(s,t,LEVEL_FILTER);
			avgHops += g.getHopsTotal();
		}
		else r = g.reach(s,t,LEVEL_FILTER);

		if(r==true) {
                    reachable++;
                	if(*lit == 0) {
                	    fail++;
                	} else success++;
        }
       	else {
                	nonreachable++;

                	if(*lit == 1) {
                        	fail++;
                	}
                	else   success++;
         }
    }

	/*gettimeofday(&after_time, NULL);
	query_time = (after_time.tv_sec - before_time.tv_sec)*1000.0 +
		(after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
	cout << "#total query running time:" << query_time << " (ms)" << endl;*/

	cout << "Success Rate " << success << "/" << success+fail << endl;
	//cout << "ConstantCuts: "<< g.getConstantCut() << endl;
	//cout << "-Positives: "<< g.getPositiveCut() << endl;
	//cout << "-Negatives: "<< g.getNegativeCut() << endl;
	
	cout << "COMPARA: "<< labeling_time <<endl;
	
	delete uriTable;
	gettimeofday(&end_time, NULL);
	float total_time = (end_time.tv_sec - start_time.tv_sec)*1000.0 + (end_time.tv_usec - start_time.tv_usec)*1.0/1000.0;
	cout<<"total time elapse: "<<total_time<<" ms, total query time elapse: "<<(total_time - labeling_time)<<" ms"<<endl;
    return 0;
}
