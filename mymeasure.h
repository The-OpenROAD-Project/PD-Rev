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
 * mymeasure.h
 * measure memory (KB) and time (seconds).
 *************************************************************************/

#ifndef MYMEASURE_H
#define MYMEASURE_H

#include <ctime>
#include <string>
#include <vector>

using   namespace   std;

class CMeasure {
    public:
        clock_t             begin;
        vector<string>      tasks;
        vector<double>      elapsed_secs;

        // functions
        void process_mem_usage();
        void start_clock();
        void stop_clock(string task);
        void print_clock();
        void printMemoryUsage(void);
};

#endif
