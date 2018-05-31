/* This file is part of make_projections.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 laster any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 make_projections 1.0.0 
 Copyright (c) Henrik Quanz
*/

#include "Plot.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <memory>
#include <new>
#include "help/HelpFunctions.h"

using namespace std;


Plot::Plot(){
}

Plot::Plot(xyVector data){
    this->data = data;
}

Plot::Plot(double** data, int numberOfPoints){

    for (int i = 0; i < numberOfPoints; i++) {
        xy temp;
        temp.x = data[0][i];
        temp.y = data[1][i];
        this->data.push_back(temp);
    }
}

Plot::Plot(const Plot& src){
    data = src.data;
}

Plot::~Plot(){
    data.clear();
}

void Plot::readDataFromFile(fstream& file){
    if (!file) {
        cerr << "Your file is not valid. Abort!" << endl;
        exit(EXIT_FAILURE);
    }
    
    char* ctemp = new char[200];
    for (int i = 0; file.getline(ctemp, 200) && file; i++) {
        xy* xytemp = new xy;
        string stemp = ctemp;
        stringstream iss(stemp);
        do {
            iss >> xytemp->x >> xytemp->y;
        } while (!iss);
        data.push_back(*xytemp);
        delete xytemp;
    }
    delete[] ctemp;
    return;
}

void Plot::displayData(){
    for (xyIt pos = data.begin(); pos < data.end(); pos++) {
        cout << setprecision(6) << fixed << pos->x << " " <<  pos->y << endl;
    }
    return;
}

Plot& Plot::operator=(const Plot& src){
    if (this != &src) {
        data = src.data;
    }
    return *this;
}

#ifdef HAVE_LIBNCURSES2
    void Plot::displayPlot(){
        CDKSCREEN *cdkscreen         = 0;
        CDKGRAPH *graph              = 0;
        CDKLABEL *pausep             = 0;
        WINDOW *cursesWin            = 0;
        const char *title            = 0;
        const char *xtitle           = 0;
        const char *ytitle           = 0;
        const char *graphChars       = 0;
        const char *mesg[2];
        int values[100];
        int count = 0;
        
        cursesWin = initscr ();
        cdkscreen = initCDKScreen (cursesWin);
        for (xyIt pos = data.begin(); pos < data.end(); pos++) {
            values[count++] = static_cast<int>(pos->y * 100);
            values[count++] = static_cast<int>(pos->x * 100);
        }
        
        count	= 10;
        title	= "<C>Test Graph";
        xtitle	= "<C>X AXIS TITLE";
        ytitle	= "<C>Y AXIS TITLE";
        graphChars	= "0123456789";
        
        mesg[0] = "Press any key when done viewing the graph.";
        
        graph = newCDKGraph (cdkscreen, CENTER, CENTER, 0, 0, title, xtitle, ytitle);
        
        /* Is the graph null? */
        if (graph == 0)
        {
            /* Shut down CDK. */
            destroyCDKScreen (cdkscreen);
            endCDK ();
            
            printf ("Cannot make the graph widget. Is the window too small?\n");
            exit(EXIT_FAILURE);
        }
        
        /* Create the label widget. */
        pausep = newCDKLabel (cdkscreen, CENTER, BOTTOM,
                              (CDK_CSTRING2) mesg, 1,
                              TRUE, FALSE);
        if (pausep == 0)
        {
            /* Shut down CDK. */
            destroyCDKGraph (graph);
            destroyCDKScreen (cdkscreen);
            endCDK ();
            
            printf ("Cannot make the label widget. Is the window too small?\n");
            exit(EXIT_FAILURE);
        }
        
        /* Set the graph values. */
        setCDKGraph (graph, values, count, graphChars, FALSE, vPLOT);
        
        /* Draw the screen. */
        refreshCDKScreen (cdkscreen);
        drawCDKGraph (graph, FALSE);
        drawCDKLabel (pausep, TRUE);
        
        /* Pause until the user says so... */
        waitCDKLabel (pausep, 0);
        
        /* Clean up. */
        destroyCDKGraph (graph);
        destroyCDKLabel (pausep);
        destroyCDKScreen (cdkscreen);
        endCDK ();
        exit(EXIT_SUCCESS);

    }
#endif

