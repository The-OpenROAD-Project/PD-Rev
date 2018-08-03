/**************************************************************************
 * Copyright(c) 2018 Regents of the University of California
 *              Kwangsoo Han, Andrew B. Kahng and Sriram Venkatesh
 * Contact      kwhan@ucsd.edu, abk@cs.ucsd.edu, srvenkat@ucsd.edu
 * Affiliation: Computer Science and Engineering Department, UC San Diego,
 *              La Jolla, CA 92093-0404, USA
 *
 *************************************************************************/

#include "argument.h"
#include "mystring.h"
#include "mymeasure.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <omp.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>

using   namespace   std;

int     main(int argc, char* argv[]){
	CArgument          argument;
	CMeasure           measure;

	cout << "------------------------------------------------------"<<endl;
	cout << "  UCSD Prim-Dijkstra Revisited                        "<<endl; 
	cout << "  coded by Kwangsoo Han and Sriram Venkatesh          "<<endl;
	cout << "------------------------------------------------------"<<endl;

	bool para = true;
	para = argument.argument(argc, argv);
	if (!para) {
		return 0;
	}

	class number {
		public:
			int val;
	};

	vector <Graph *> my_graphs;
	unsigned num_nets = argument.num_nets;
	
  // Read input files
	ifstream inputFile;
	inputFile.open(argument.input_file.c_str(), fstream::in);
	string line, l;
	vector<unsigned> x,y;
	unsigned tmp_x = 0, tmp_y = 0;
  unsigned tmp_t = 0;
	string tmp;
	while (getline(inputFile, line)) {
		istringstream ss(line);
		vector<string> values;
		while (ss) {
			string next;
			if (!getline(ss, next, ' ')) break;
			values.push_back(next);
		}
		if (values.size() == 3 && values[0] == "Net") {
      if (argument.verbose > 3) {
        cout << values[0] << " " << values[1] << " --- " << values[2] << endl;
      }
			unsigned term_cnt = 0;
			while(1) {
				getline(inputFile, l);
				istringstream ss_l(l);
				if(l.empty()) { break; }
				ss_l >> tmp_t >> tmp_x >> tmp_y;
        if (argument.verbose > 3) {
          cout << tmp_x << " " << tmp_y << endl;
        }
        bool flag = false;
        for (unsigned i = 0; i < x.size(); ++i)
        {
          if(tmp_x == x[i] && tmp_y == y[i])
          {
            flag = true;
          }
        }
        if (flag)
            continue;
				x.push_back(tmp_x);
				y.push_back(tmp_y);
				term_cnt++;
			}
			my_graphs.push_back(new Graph(term_cnt, argument.verbose, argument.alpha1, argument.alpha2,
                                    argument.alpha3, argument.alpha4, argument.root_idx, 
                                    argument.beta, argument.margin, argument.seed, 
                                    argument.distance, x, y));
			x.clear(); y.clear();
		}
	}
	inputFile.close();
	
  num_nets = my_graphs.size();

	measure.start_clock();
	omp_set_num_threads(4);
	unsigned net_num = argument.net_num ; 
  bool runOneNet = argument.runOneNet;
  cout << "\nGenerating nearest neighbor graph..." << endl;
  #pragma omp parallel for
	for (unsigned i = 0; i < num_nets; ++i) {
		// Guibas-Stolfi algorithm for computing nearest NE (north-east) neighbors
	  if (i == net_num || !runOneNet) {
	  	my_graphs[i]-> buildNearestNeighborsForSPT(my_graphs[i]->num_terminals);
	  }
	}

  if (argument.verbose > 2) {
    double totAvgNN = 0.0;
    for (unsigned i = 0; i < num_nets; ++i) {
      totAvgNN += my_graphs[i]->avgNN;
    }
    cout << "AvgNN = " << totAvgNN / num_nets << endl;
  }

  cout << "\nFinished generating nearest neighbor graph..." << endl;
	measure.stop_clock("Graph generation");

  //****************PD*********************
	measure.start_clock();
	cout << "\nRunning PD algorithm... alpha = "
       << argument.alpha1 << endl;
  #pragma omp parallel for
	for (unsigned i = 0; i < num_nets; ++i) {
		// binary-heap based implementation of Prim-Dijkstra
	  if (i == net_num || !runOneNet) {
	  	my_graphs[i]-> run_PD_brute_force(argument.alpha1);
	  }
	}
	for (unsigned i = 0; i < num_nets; ++i) {
    cout << "  Net " << i
         << " WL = " << my_graphs[i]->pd_wl
         << " PL = " << my_graphs[i]->pd_pl 
         << endl;
  }
	cout << "Finished running P-D algorithm..." << endl;
	measure.stop_clock("Prim-Dijkstra");
  //****************PD*********************

  if (argument.verbose > 0) {
    ofstream ofs;
    ofs.open("PD.txt", fstream::out);
    for (unsigned i = 0; i < num_nets; ++i) {
      ofs << "\nNet n" << i << " " << my_graphs[i]->nodes.size() << endl;
      my_graphs[i]->print_tree_v2(ofs);
      ofs << endl;
    }
  }

  //****************PDBU*********************
	if (argument.pdbu_yes == 1) {
	  measure.start_clock();
	  cout << "\nRunning PD-II... alpha = " 
         << argument.alpha2 << endl;
    #pragma omp parallel for
		for (unsigned i = 0; i < num_nets; ++i) {
		  if (i == net_num || !runOneNet) {
		  	my_graphs[i]-> PDBU_new_NN();
		  }
	  }
   	for (unsigned i = 0; i < num_nets; ++i) {
      cout << "  Net " << i 
           << " WL = " << my_graphs[i]->pdbu_wl
           << " PL = " << my_graphs[i]->pdbu_pl 
           << " DC = " << my_graphs[i]->pdbu_dc 
           << endl;
    }
	  cout << "Finished running PD-II..." << endl;
	  measure.stop_clock("PD-II");

    if (argument.verbose > 0) {
      ofstream ofs;
      ofs.open("PD_II.txt", fstream::out);
      for (unsigned i = 0; i < num_nets; ++i) {
        ofs << "\nNet n" << i << " " << my_graphs[i]->nodes.size() << endl;
        my_graphs[i]->print_tree_v2(ofs);
        ofs << endl;
      }
    }
  }
  //****************PD-II*********************

  //****************HVW*********************
  if (argument.hvw_yes == 1) {
	  measure.start_clock();
	  cout << "\nRunning Steiner algorithm..." << endl;
    #pragma omp parallel for
	  for (unsigned i = 0; i < num_nets; ++i) {
	    if (i == net_num || !runOneNet) {
	    	my_graphs[i]-> doSteiner_HoVW();
	    }
	  }
    for (unsigned i = 0; i < num_nets; ++i) {
      cout << "  Net " << i 
           << " WL = " << my_graphs[i]->st_wl
           << " PL = " << my_graphs[i]->st_pl 
           << " DC = " << my_graphs[i]->st_dc 
           << endl;
    }
	  cout << "Finished Steiner algorithm..." << endl;
	  measure.stop_clock("HVW Steinerization");
  //****************HVW*********************

    if (argument.verbose > 0) {
      ofstream ofs;
      ofs.open("HVW.txt", fstream::out);
      for (unsigned i = 0; i < num_nets; ++i) {
        ofs << "\nNet n" << i << " " << my_graphs[i]->nodes.size() << endl;
        my_graphs[i]->print_tree_v2(ofs);
        ofs << endl;
      }
    }

  //****************DAS*********************
	  measure.start_clock();
    cout << "\nRunning DAS algorithm..." << endl;
    #pragma omp parallel for
	  for (unsigned i = 0; i < num_nets; ++i) {
	    if (i == net_num || !runOneNet) {
	    	my_graphs[i]-> fix_max_dc();
	    }
	  }
    for (unsigned i = 0; i < num_nets; ++i) {
      cout << "  Net " << i 
           << " WL = " << my_graphs[i]->daf_wl
           << " PL = " << my_graphs[i]->daf_pl 
           << " DC = " << my_graphs[i]->daf_dc 
           << endl;
    }
    cout << "Finished DAS algorithm..." << endl;
	  measure.stop_clock("DAS");
  //****************DAS*********************

    if (argument.verbose > 0) {
      ofstream ofs;
      ofs.open("DAS.txt", fstream::out);
      for (unsigned i = 0; i < num_nets; ++i) {
        ofs << "\nNet n" << i << " " << my_graphs[i]->nodes.size() << endl;
        my_graphs[i]->print_tree_v2(ofs);
        ofs << endl;
      }
    }
  }
	
	measure.print_clock();
	measure.printMemoryUsage();

	return 1;
}
