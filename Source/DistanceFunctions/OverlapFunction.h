
#ifndef OVERLAPDISTANCE_H
#define OVERLAPDISTANCE_H

#include "DistanceFunction.h"


/*!

\brief Implements the overlap distance function for nominal attributes.

This is an implementation of the DistanceFunction abstract class for nominal attributes.
It implements the overlap distance.
This distance sets 0 to the distance of two attributes with the same values and 1 otherwise.


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


class OverlapFunction : public DistanceFunction {

public:

    /**
     * Constructs a OverlapFunction object.
     */
    OverlapFunction ();

    /**
     * Destructs the OverlapFunction object.
     */
    ~OverlapFunction ();

    /**
     * Computes the distance between two attribute values.
     * @param att1 is the value of attribute 1.
     * @param att2 is the value of attribute 2.
     * @return A float with the distance between att1 and att2.
     */
    float computeDistance ( float att1, float att2 );

};

#endif
