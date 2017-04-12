
/*!

\brief This file contains the implementation of the Utils methods.

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


#include "Utils.h"


/** To trace comments. */
bool Utils::doScreenStatistics = false;

/** Types of attributes. */
const char Utils::REAL_ATTRIBUTE = 'R';
const char Utils::INTEGER_ATTRIBUTE = 'I';
const char Utils::NOMINAL_ATTRIBUTE = 'N';
std::ofstream Utils::fLog;


void Utils::setSeed ( long seed ) {
    
    if ( seed == 0 ) {
        generateNewRandomSeed ();
    }
    else {
        srand ( seed );
    }
    
} // end setSeed


void Utils::generateNewRandomSeed () {

    int i, j;
    char buffer [30];

    // For Linux and MacOS X.
    struct timeval tv;
    time_t curtime;
    gettimeofday ( &tv, NULL );
    curtime = tv.tv_sec;
    strftime ( buffer, 30, "%T", localtime ( &curtime ) );
    sprintf ( buffer, "%s%ld", buffer, ( long int ) tv.tv_usec );

    // For Ms Windows.
    // srand ( time ( NULL ) );
    // sprintf ( buffer, "%d", rand () );

    j = 0;

    for ( i = 0; i < ( int ) strlen ( buffer ); i++ ) {
        if ( buffer[i] >= '0' && buffer[i] <= '9' ) {
            buffer[j] = buffer[i];
            j++;
        }
    }

    buffer[j] = 0;
    buffer[0] = buffer[1] = buffer[2] = '0';

    std::cout << " > New random seed: " << atol ( buffer ) << std::endl;
    srand ( atol ( buffer ) );

} // end generateNewRandomSeed


float Utils::f_rand () {
    return ( float ) rand () / ( ( float ) RAND_MAX + 1 );
} // end f_rand


int Utils::i_rand ( int lowV, int upV ) {
    return lowV + ( int ) ( ( float ) ( upV - lowV + 1 ) * f_rand () );
} // end i_rand


bool Utils::readLine ( std::ifstream& fin, std::string& line ) {

    // Read an entire line.
    getline ( fin, line, '\n' );

    while ( !fin.eof () && ( line.size () == 0 || isAComment ( line ) ) ) {
        // Read an entire line.
        getline ( fin, line, '\n' ); 
    }

    if ( fin.eof () ) return false;

    if ( line[line.length () - 1] == '\r') line.resize( line.length() - 1 );

    return true;
    
} // end readLine


bool Utils::isAComment ( std::string& line ) {

    int i = 0;
    bool isAComment = false;

    while ( i < ( int ) line.size () ) {
        if ( line.at ( i ) == '%' ) {
            isAComment = true;
            break;
        }
        if ( line.at ( i ) != ' ' ) {
            break;
        }
        i++;
    }

    return isAComment;

} // end isAComment


std::string Utils::trim ( std::string& line ) {

    int size = line.size ();

    // Search for the initial spaces.
    int i = 0;
    while ( i < ( int ) line.size () && ( line[i] == ' ' ||
                                         line[i] == '\n' || line[i] == '\t' || line[i] == '\r' ||
                                         line[i] == ( char ) 13 ) ) {
        i++;
        size --;
    }

    // It is the size minus 1 because we count from 0 to size - 1.
    int e = line.size () - 1;
    while ( e >= 0 && ( line[e] == ' ' ||
                        line[e] == '\n' || line[e] == '\t' || line[i] == '\r' ||
                        line[i] == ( char ) 13 ) ) {
        e--;
        size --;
    }

    return line.substr ( i, size - 1 );

} // end trim


std::string Utils::removeFinalSpaces ( std::string& line ) {
    
    std::string out;
    int j = line.size () - 1;

    while ( j >= 0 && line[j] == ' ' ) {
        j--;
    }

    if ( j < ( int ) line.size () - 1 ) {
        return line.substr ( 0, j + 1 );
    }
    else {
        return line;
    }
    
} // end removeFinalSpaces


void Utils::quickSort ( float* vector, int* order, int inf, int sup ) {

    int pivot;

    if ( inf < sup ) {
        // Divide and conquer.
        pivot = partition ( vector, order, inf, sup );
        quickSort ( vector, order, inf, pivot - 1 );
        quickSort ( vector, order, pivot + 1, sup );
    }

} // end quickSort


int Utils::partition ( float* vector, int* order, int inf, int sup ) {

    float tempF;
    int   tempI;
    int   pivotPosition    = inf;
    int   lastSmallerValue = inf;
    int   firstUnknown     = inf + 1;

    for ( ; firstUnknown <= sup; firstUnknown ++ ) {
        if ( vector[ firstUnknown ] < vector[ pivotPosition ] ) {
            lastSmallerValue ++;

            tempF = vector[ firstUnknown ];
            vector[ firstUnknown ] = vector[ lastSmallerValue ];
            vector[ lastSmallerValue ] = tempF;

            if ( order ) {
                tempI = order[ firstUnknown ];
                order[ firstUnknown ] = order[ lastSmallerValue ];
                order[ lastSmallerValue ] = tempI;
            }
        }
    }

    tempF = vector[ inf ];
    vector[ inf ] = vector[ lastSmallerValue ];
    vector[ lastSmallerValue ] = tempF;

    if ( order ) {
        tempI = order[ inf ];
        order[ inf ] = order[ lastSmallerValue ];
        order[ lastSmallerValue ] = tempI;
    }

    return lastSmallerValue;
    
} // end partition


void Utils::quickSort ( Date* vector, int* order, int inf, int sup ) {

    int pivot;

    if ( inf < sup ) {
        // Divide and conquer.
        pivot = partition ( vector, order, inf, sup );
        quickSort ( vector, order, inf, pivot - 1 );
        quickSort ( vector, order, pivot + 1, sup );
    }

} // end quickSort


int Utils::partition ( Date* vector, int* order, int inf, int sup ) {

    Date  tempF;
    int   tempI;
    int   pivotPosition    = inf;
    int   lastSmallerValue = inf;
    int   firstUnknown     = inf + 1;

    for ( ; firstUnknown <= sup; firstUnknown ++ ) {
        if ( vector[ firstUnknown ] < vector[ pivotPosition ] ) {
            lastSmallerValue ++;

            tempF = vector[ firstUnknown ];
            vector[ firstUnknown ] = vector[ lastSmallerValue ];
            vector[ lastSmallerValue ] = tempF;

            if ( order ) {
                tempI = order[ firstUnknown ];
                order[ firstUnknown ] = order[ lastSmallerValue ];
                order[ lastSmallerValue ] = tempI;
            }
        }
    }

    tempF = vector[ inf ];
    vector[ inf ] = vector[ lastSmallerValue ];
    vector[ lastSmallerValue ] = tempF;

    if ( order ) {
        tempI = order[ inf ];
        order[ inf ] = order[ lastSmallerValue ];
        order[ lastSmallerValue ] = tempI;
    }

    return lastSmallerValue;
    
} // end partition


void Utils::printGPLInformation ( bool printWarrantyInfo, bool printRedistributionInfo ) {

    if ( printRedistributionInfo ) {
        std::cout << "THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY ";
        std::cout << "APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT ";
        std::cout << "HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY ";
        std::cout << "OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, ";
        std::cout << "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR ";
        std::cout << "PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM ";
        std::cout << "IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ";
        std::cout << "ALL NECESSARY SERVICING, REPAIR OR CORRECTION." << std::endl << std::endl;
    }

    if ( printWarrantyInfo ) {
        std::ifstream fin;
        std::string line;

        fin.open ( "COPYING" );
        if ( fin ) {
            getline ( fin, line, '\n' ); 
            while ( !fin.eof () ) {
                if ( line.find ( "END OF TERMS AND CONDITIONS" ) != std::string::npos ) {
                    std::cout << line << std::endl;
                    break;
                }
                std::cout << line << std::endl;
                getline ( fin, line, '\n' ); 
            }
            fin.close ();
            std::cout << std::endl;
        }
    }

} // end printGPLInformation


bool Utils::isNumeric ( const char* line ) {

    int i;

    for ( i = 0; i < ( int ) strlen ( line ); i++ ) {
        if ( ( line[i] < '0' || line[i] > '9' ) && line[i] != ' ' && line[i] != '.' && line[i] != '-'  ) {
          
             // Treat scientific notation (e.g., 4.7E+10, 4.7e-10). 
             if ( ( line[ i - 1 ] >= '0' && line [ i - 1 ] <= '9' ) 
                && ( line[i] == 'e' || line[i] == 'E' )
                && ( line[ i + 1 ] == '+' || line[ i + 1 ] == '-' )
                && ( line[ i + 2 ] >= '0' && line [ i + 2 ] <= '9' ) ) {
                i = i + 2;
             }
             else { // It is not numeric.
                 return false;
             } 
        }
    }

    return true;

} // end isNumeric
