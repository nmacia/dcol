
/*!

\brief This file contains the implementation of the StdWeightedEuclideanFunction methods.

@author   Albert Orriols-Puig and Nuria Macia <br>
          Grup de Recerca en Sistemes Intel.ligents <br>
          La Salle - Universitat Ramon Llull <br>
          C/ Quatre Camins, 2. 08022, Barcelona (Spain) <br>
@date     Created April, 2009 <br>
          Last modified December, 2010

Copyright (C) 2009  Albert Orriols-Puig and Nuria Macia

This file is part of DCoL.

DCoL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DCoL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DCoL.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "StdWeightedEuclideanFunction.h"


StdWeightedEuclideanFunction::StdWeightedEuclideanFunction ( double std ) {

    fourStd = 4 * std;
    if ( Utils::doScreenStatistics ) 
        std::cout << "    > Creating a standard deviation weighted distance function object with 4*std = " << fourStd << std::endl;

    if ( fourStd == 0 ) {
        std::string logMessage = "    > [WARNING DISTANCE METRIC] Using the standard deviation weighted distance function for";
        logMessage += " continuous attributes which have a deviation of 0. \n ";
        logMessage += "        >> This may cause a malfunction. \n ";
        Utils::logWarningError ( logMessage );
    }

} // end StdWeightedEuclideanFunction


StdWeightedEuclideanFunction::~StdWeightedEuclideanFunction () {
} // end StdWeightedEuclideanFunction


float StdWeightedEuclideanFunction::computeDistance ( float att1, float att2 ) {
    return fourStd == 0 ? 0 : ( ( att1 - att2 ) / fourStd );
} // end computeDistance
