
/*!

\brief This file contains the implementation of the DateContainer methods.

The class provides the implementation of methods that create, transform, and delete date strings.

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


#include "DateContainer.h"


DateContainer::DateContainer ( std::string dateFormat ) {

    unsigned int i;

    yearPosition = monthPosition = dayPosition = hourPosition = minutesPosition = secondsPosition = -1;

    for ( i = 0; i < dateFormat.size (); i++ ) {
        if ( i + 3 < dateFormat.size () && 
                dateFormat[i] == 'y' && dateFormat[ i + 1 ] == 'y' &&
                dateFormat[ i + 2 ] == 'y' && dateFormat[ i + 3 ] == 'y' ) {

            yearPosition = i;
        }
        if ( i + 1 < dateFormat.size () && dateFormat[i] == 'M' && dateFormat[ i + 1 ] == 'M' ) {
            monthPosition = i;
        }
        if ( i + 1 < dateFormat.size () && dateFormat[i] == 'd' && dateFormat[ i + 1 ] == 'd' ) {
            dayPosition = i;
        }
        if ( i + 1 < dateFormat.size () && dateFormat[i] == 'H' && dateFormat[ i + 1 ] == 'H' ) {
            hourPosition = i;
        }
        if ( i + 1 < dateFormat.size () && dateFormat[i] == 'm' && dateFormat[ i + 1 ] == 'm' ) {
            minutesPosition = i;
        }
        if ( i + 1 < dateFormat.size () && dateFormat[i] == 's' && dateFormat[ i + 1 ] == 's' ) {
            secondsPosition = i;
        }
    }

} // end DateContainer


float DateContainer::transformToNumeric ( std::string date ) {
    return ( ( float ) transformToLongNumeric ( date ) );
} // end transformToNumeric


long double DateContainer::transformToLongNumeric ( std::string date ) {

    long double numericDate = 0;
    long double aux;

    if ( yearPosition != -1 ) {
        aux = ( date[ yearPosition ]     - '0' ) * 1000 +
              ( date[ yearPosition + 1 ] - '0' ) *  100 +
              ( date[ yearPosition + 2 ] - '0' ) *   10 +
              ( date[ yearPosition + 3 ] - '0' );

        numericDate += aux * 365. * 24. * 3600.;
    }
    if ( monthPosition != -1 ) {
        int monthAdjustment[ 12 ] = {0, -3, -3, -4, -4, -5, -5, -5, -6, -6, -7, -7};
        aux = ( date[ monthPosition ]     - '0' ) * 10 +
              ( date[ monthPosition + 1 ] - '0' );

        numericDate += ( aux * 31. - monthAdjustment[ ( int ) aux ] ) * 24. * 3600.;
    }
    if ( dayPosition != -1 ) {
        aux = ( date[ dayPosition ]     - '0' ) * 10 +
              ( date[ dayPosition + 1 ] - '0' );

        numericDate += aux * 24. * 3600.;
    }
    if ( hourPosition != -1 ) {
        aux = ( date[ hourPosition ]     - '0' ) * 10 +
              ( date[ hourPosition + 1 ] - '0' );

        numericDate += aux * 3600.;
    }
    if ( minutesPosition != -1 ) {
        aux = ( date[ minutesPosition ]     - '0' ) * 10 +
              ( date[ minutesPosition + 1 ] - '0' );

        numericDate += aux * 60.;
    }
    if ( secondsPosition != -1 ) {
        aux = ( date[ secondsPosition ]     - '0' ) * 10 +
              ( date[ secondsPosition + 1 ] - '0' );

        numericDate += aux;
    }

    return numericDate;

} // end transformToLongNumeric


void DateContainer::transformToDate ( std::string date, Date& d ) {

    if ( yearPosition != -1 ) {
        d.year = ( date[ yearPosition ]     - '0' ) * 1000 +
                 ( date[ yearPosition + 1 ] - '0' ) *  100 +
                 ( date[ yearPosition + 2 ] - '0' ) *   10 +
                 ( date[ yearPosition + 3 ] - '0' );
    }
    if ( monthPosition != -1 ) {
        d.month = ( date[ monthPosition ]     - '0' ) * 10 +
                  ( date[ monthPosition + 1 ] - '0' );

    }
    if ( dayPosition != -1 ) {
        d.day =  ( date[ dayPosition ]     - '0' ) * 10 +
                 ( date[ dayPosition + 1 ] - '0' );

    }
    if ( hourPosition != -1 ) {
        d.hour = ( date[ hourPosition ]     - '0' ) * 10 +
                 ( date[ hourPosition + 1 ] - '0' );

    }
    if ( minutesPosition != -1 ) {
        d.minutes = ( date[ minutesPosition ]     - '0' ) * 10 +
                    ( date[ minutesPosition + 1 ] - '0' );

    }
    if ( secondsPosition != -1 ) {
        d.seconds = ( date[ secondsPosition ]     - '0' ) * 10 +
                    ( date[ secondsPosition + 1 ] - '0' );

    }

} // end transformToDate
