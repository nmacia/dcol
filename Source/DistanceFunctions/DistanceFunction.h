
#ifndef DISTANCEFUNCTION_H
#define DISTANCEFUNCTION_H

#include "../Utils.h"

#include <cmath>
#include <iostream>


/*!

\brief Abstract class that defines the distance function classes behavior.

This is an abstract class that provides the basic methods of distance functions for individual attributes.
Five types of distance functions have been implemented:

<ol>
	<li> EuclideanFunction: Euclidean distance for continuous attributes. </li>
	<li> NormalizedEuclideanFunction: Normalized Euclidean distance for continuous attributes. </li>
	<li> StdWeightedEuclideanFunction: Standard-deviation weighted Euclidean distance for continuous attributes. </li>
	<li> OverlapFunction: Distance for nominal attributes that defines distance 0 for equally valued attributes and distance 1 otherwise. </li>
	<li> VDMFunction: Distance between nominal attributes by means of the frequency of each value in the data set. </li>
</ol>

For further details on these distance functions see (Randall & Martinez, 1997).

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


class DistanceFunction {

public:

    /**
     * Destructs the DistanceFunction.
     */
    virtual ~DistanceFunction () {};

    /**
     * Computes the distance between two attribute values.
     * @param att1 is the value of attribute 1.
     * @param att2 is the value of attribute 2.
     * @return A float with the distance between att1 and att2.
     */
    virtual float computeDistance ( float att1, float att2 ) = 0;

};

#endif
