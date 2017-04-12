
#ifndef DISTNODE_H
#define DISTNODE_H


/*!

\brief Maintains an index and a distance.

This class runs the kNN learner, since it permits maintaining the index of the example
and its distance to the heap.

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


class DistNode {

public:

    /** Index of the current node. */
    int index;

    /**  Distance stored in the current node. */
    float dist;

    /**
     * Is the "less than" operator.
     * @param d2 is the DistNode object with which the current object will be compared to.
     * @return A Boolean that indicates which object is smaller.
     * @retval true the current object is less than the object d2.
     * @retval false the current object is greater than or equal to the object d2.
     */
    bool operator < ( DistNode& d2 ) {
        return ( dist < d2.dist );
    } // end operator <

    /**
     * Is the "greater than" operator.
     * @param d2 is the DistNode object with which the current object will be compared to.
     * @return A Boolean that indicates which object is greater.
     * @retval true the current object is greater than the object d2.
     * @retval false the current object is less than or equal to the object d2.
     */
    bool operator > ( DistNode& d2 ) {
        return ( dist > d2.dist );
    } // end operator >

    /**
     * Is the "equal to" operator.
     * @param d2 is the DistNode object with which the current object will be compared to.
     * @return A Boolean that indicates whether the objects are equal.
     * @retval true the current object is equal to the object d2.
     * @retval false the current object is different from the object d2.
     */
    bool operator == ( DistNode& d2 ) {
        return ( dist == d2.dist );
    } // end operator ==

};

#endif
