
#ifndef DATECONTAINER_H 
#define DATECONTAINER_H 

#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "Date.h"


/*! 

\brief Implements method to create, transform, and delete date strings.

This class reads dates from strings formatted in a prefixed way.


@author   Albert Orriols-Puig and Nuria Macia <br>
          Grup de Recerca en Sistemes Intel.ligents <br>
          La Salle - Universitat Ramon Llull <br>
          C/ Quatre Camins, 2. 08022, Barcelona (Spain) <br>
@date     Created June, 2010 <br>
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


class DateContainer{

private:

    /** Position of the string where the year begins. */
    int yearPosition;

    /** Position of the string where the month begins. */
    int monthPosition;

    /** Position of the string where the day begins. */
    int dayPosition;

    /** Position of the string where the hour begins. */
    int hourPosition;

    /** Position of the string where the minute begins. */
    int minutesPosition;

    /** Position of the string where the second begins. */
    int secondsPosition;

public:

    /**
     * Constructs a new DateContainer object.
     * @param dateFormat specifies the format of the date.
     */
    DateContainer ( std::string dateFormat );

    /**
     * Destructs the DateContainer object.
     */
    ~DateContainer (){}

    /**
     * Returns a numeric value that corresponds to the date passed as argument.
     * @param date is the date to be transformed.
     * @return A float with the transformed date.
     */
    float transformToNumeric ( std::string date );

    /**
     * Returns a numeric value that corresponds to the date passed as argument.
     * @param date is the date to be transformed.
     * @return A long double with the transformed date.
     */
    long double transformToLongNumeric ( std::string date );

    /**
     * Returns a Date object that corresponds to the date passed as argument.
     * @param date is the date to be transformed.
     * @param d is the object where the date will be written.
     */
    void transformToDate ( std::string date, Date& d );

};

#endif
