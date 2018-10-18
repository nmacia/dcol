
/*!

\brief This file contains the implementation of the StringTokenizer methods.

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


#include "StringTokenizer.h"

StringTokenizer::StringTokenizer ( std::string str, std::string sep = " " ) {

    index = 0;
    count = 0;
    int i = 0, j = 0;
    
    char s_aux[ str.size() ];

    // Discard while having the delimiter.
    while ( i < ( int ) str.size () && ( str[i] == sep[0] || str[i] == '\n' || str[i] == ' ' || str[i] == '\t' ) ) i++;

    while ( i < ( int ) str.size () ) {

        // Get a token.
        j = 0;
        
        while ( i < ( int ) str.size () && str[i] != sep[0] && str[i] != '\t' ) {
            s_aux[j] = str[i];
            i++;
            j++;
        }

        s_aux[j] = '\0';
        elements.addElement ( s_aux );
        count ++;

        // Discard while having the delimiter.
        while ( i < ( int ) str.size () && ( str[i] == sep[0] || str[i] == '\n' || str[i] == ' ' || str[i] == '\t' ) ) i++;
    }

} // end StringTokenizer


StringTokenizer::StringTokenizer ( std::string str, std::string sep = " ", std::string sep2 = " " ) {

    index = 0;
    count = 0;
    int i = 0, j = 0;
    char s_aux[ str.size() ];

    // Discard while having the delimiter.
    while ( i < ( int ) str.size () && ( str[i] == sep[0] || str[i] == sep2[0] || str[i] == '\n' || str[i] == ' ' || str[i] == '\t' ) ) i++;

    while ( i < ( int ) str.size () ) {

        // Get a token.
        j = 0;
        
        while ( i < ( int ) str.size () && str[i] != sep[0] && str[i] != sep2[0] && str[i] != '\t' ) {
            s_aux[j] = str[i];
            i++;
            j++;
        }

        s_aux[j] = '\0';
        elements.addElement ( s_aux );
        count ++;

        // Discard while having the delimiter.
        while ( i < ( int ) str.size () && ( str[i] == sep[0] || str[i] == sep2[0] || str[i] == '\n' || str[i] == ' ' || str[i] == '\t' ) ) i++;
    }
    
} // end StringTokenizer

StringTokenizer::~StringTokenizer () {
} // end ~StringTokenizer

std::string StringTokenizer::getNextToken () {

    index ++;

    if ( index > count ) {
        throw std::string ( " Index out of bounds. " );
    }

    return elements.elementAt ( index -1 );

} // end getNextToken


int StringTokenizer::countTokens () const {
    return count;
} // end countTokens


bool StringTokenizer::hasMoreTokens () const {

    if ( index >= count )  return false;
    return true;

} // end hasMoreTokens
