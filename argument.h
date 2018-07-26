/**************************************************************************
 * Copyright(c) 2018 Regents of the University of California
 *              Kwangsoo Han, Andrew B. Kahng and Sriram Venkatesh
 * Contact      kwhan@ucsd.edu, abk@cs.ucsd.edu, srvenkat@ucsd.edu
 * Affiliation: Computer Science and Engineering Department, UC San Diego,
 *              La Jolla, CA 92093-0404, USA
 *
 *************************************************************************/

/**************************************************************************
 * UCSD Prim-Dijkstra Revisited
 * argument.h
 *************************************************************************/

#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <vector>

using   namespace   std;

class   CArgument{
    public:
        unsigned         num_nets;
        unsigned         net_num;
        bool             runOneNet;
        unsigned         num_terminals;
        unsigned         verbose;
        float            alpha1;
        float            alpha2;
        float            alpha3;
        float            alpha4;
        float            margin;
        unsigned         seed;
	string		 input_file;
        unsigned         root_idx;
        unsigned         hvw_yes;
        unsigned         pdbu_yes;
        float            beta;
        unsigned         distance;

        // functions
        CArgument();
        bool        argument(int argc, char* argv[]);
        void        help();
};

#endif
