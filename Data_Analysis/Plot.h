//
//  Plot.h
//  Tunneling_Analysis
//
//  Created by Henrik Quanz on 09.08.12.
//  Copyright (c) 2012 HQ Productions. All rights reserved.
//

#ifndef __Tunneling_Analysis__Plot__
#define __Tunneling_Analysis__Plot__
#define STEP 10

#include <iostream>
#include <fstream>
#include <vector>
    
#ifdef HAVE_LIBNCURSES
#include <ncurses.h>
#endif



struct xy {
    double x;
    double y;
    xy(){
        x = 0.0;
        y = 0.0;
    }
};
    
    typedef std::vector<xy> xyVector;
    typedef xyVector::const_iterator xyIt;

class Plot {
    
    xyVector data;
    
    
public:
    Plot();
    Plot(xyVector);
    Plot(double** data, int);
    Plot(const Plot&);
    ~Plot();
    void drawPlot();
    void displayData();
    void readDataFromFile(std::fstream&);
#ifdef HAVE_LIBNCURSES2
    void displayPlot();
#endif
    
    //operators
    Plot& operator=(const Plot&);
    
};

#endif /* defined(__Tunneling_Analysis__Plot__) */
