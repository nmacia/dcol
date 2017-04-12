
#ifndef UTILS_H
#define UTILS_H

#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sys/time.h>

#include "Date.h"

#define s_time struct timeval


/*!

\brief Implements utilities.

This class implements several utilities such as sorting procedures, comparing procedures, 
and random seed initializers.

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


class Utils {

public:

    /** File for warning and error logging*/
    static std::ofstream fLog;

    /** Indicates whether screen statistics must be written. */
    static bool doScreenStatistics;

    /** Identifies continuous attributes. */
    static const char REAL_ATTRIBUTE;

    /** Identifies integer attributes. */
    static const char INTEGER_ATTRIBUTE;

    /** Identifies nominal attributes. */
    static const char NOMINAL_ATTRIBUTE;

    /**
    * Sets the seed of the random number generator.
    * @param seed is the seed.
    */
    static void setSeed ( long seed );

    /**
     * Generates a new random seed by taking the time of the system.
     */
    static void generateNewRandomSeed ();

    /**
     * Returns a continuous random number ranging between 0 and 1.
     * @return A float with the random number.
     */
    static float f_rand ();

    /**
     * Returns an integer random number ranging between lowV and upV.
     * @param lowV is the lower limit of the range.
     * @param upV is the upper limit of the range.
     * @return An integer with the random number.
     */
    static int i_rand ( int lowV, int upV );

    /**
     * Returns a continuous random number ranging between lowV and upV.
     * @param lowV is the lower limit of the range.
     * @param upV is the upper limit of the range.
     * @return A float with the random number.
     */
    static float f_round ( float lowV, int upV );

    /**
     * Reads one line from file.
     * @param fin is the input file.
     * @param line is a string to store the line that will be read (passed by reference).
     * @return A Boolean that indicates whether it is end of file.
     * @retval true end of file.
     * @retval false not end of file yet.
     */
    static bool readLine ( std::ifstream& fin, std::string& line );

    /**
     * Checks whether a line is a comment.
     * @param line is the line to be checked.
     * @return A Boolean that indicates whether the line is a comment.
     * @retval true the line is a comment.
     * @retval false the line contains useful data.
     */
    static bool isAComment ( std::string& line );

    /**
     * Trims a string.
     * @param line is the string to be trimmed.
     * @return A string with the trimmed string.
     */
    static std::string trim ( std::string& line );

    /**
     * Removes the blank spaces at the end of the string.
     * @param line is the string to be processed.
     * @return A string without blank spaces at the end.
     */
    static std::string removeFinalSpaces ( std::string& line );

    /**
     * Prints the configuration parameters to file.
     * @param fileName is the name of the file where the parameters will be printed.
     */
    static void printParamsToFile ( std::string fileName );


//// ASCENDENT ORDERING (the smallest, the first)

    /**
     * Implements the quicksort procedure for arrays of floats.
     * @param vector is the array to be sorted.
     * @param order is an auxiliary vector that needs to be sorted with the vector one.
     * @param i is the initial index from where the vector has to be sorted.
     * @param j is the final index until which the vector has to be sorted.
     */
    static void quickSort ( float* vector, int* order, int i, int j );

    /**
     * Implements an auxiliary function for quickSort for arrays of floats.
     * @param vector is the array to be sorted.
     * @param order is an auxiliary vector that needs to be sorted with the vector one.
     * @param inf is the initial index from where the vector has to be sorted.
     * @param sup is the final index until which the vector has to be sorted.
     */
    static int partition ( float* vector, int* order, int inf, int sup );

    /**
     * Implements the quicksort procedure for arrays of Dates.
     * @param vector is the array to be sorted.
     * @param order is an auxiliary vector that needs to be sorted with the vector one.
     * @param i is the initial index from where the vector has to be sorted.
     * @param j is the final index until which the vector has to be sorted.
     */
    static void quickSort ( Date* vector, int* order, int i, int j );

    /**
     * Implements an auxiliary function for quickSort for arrays of Dates.
     * @param vector is the array to be sorted.
     * @param order is an auxiliary vector that needs to be sorted with the vector one.
     * @param inf is the initial index from where the vector has to be sorted.
     * @param sup is the final index until which the vector has to be sorted.
     */
    static int partition ( Date* vector, int* order, int inf, int sup );

    /**
     * Prints information about the GPL license.
     * @param printWarrantyInfo is a Boolean that indicates whether warranty information has to be printed.
     * @param printRedistributionInfo is a Boolean that indicates whether redistribution information has to be printed.
     */
    static void printGPLInformation ( bool printWarrantyInfo, bool printRedistributionInfo );


//// COMPARISONS

    /**
     * Returns the minimum value between the arguments.
     * @param v1 is the first integer.
     * @param v2 is the second integer.
     * @return An integer with the minimum value between v1 and v2.
     */
    static int min ( int v1, int v2 ) {
        return ( v1 < v2 ? v1 : v2 );
    } // end min

    /**
     * Returns the maximum value between the arguments.
     * @param v1 is the first integer.
     * @param v2 is the second integer.
     * @return An integer with the maximum value between v1 and v2.
     */
    static int max ( int v1, int v2 ) {
        return ( v1 < v2 ? v2 : v1 );
    } // end max

    /**
     * Returns the minimum value between the arguments.
     * @param v1 is the first float.
     * @param v2 is the second float.
     * @return A float with the minimum value between v1 and v2.
     */
    static float min ( float v1, float v2 ) {
        return ( v1 < v2 ? v1 : v2 );
    } // end min

    /**
     * Returns the maximum value between the arguments.
     * @param v1 is the first float.
     * @param v2 is the second float.
     * @return A float with the maximum value between v1 and v2.
     */
    static float max ( float v1, float v2 ) {
        return ( v1 < v2 ? v2 : v1 );
    } // end max

    /**
     * Returns whether the passed string contains only numbers.
     * @param line is the input string.
     * @return A Boolean that indicates whether the passed string contains only numbers.
     * @retval true the passed string contains only numbers.
     * @retval false the passed string contains characters and may contain numbers as well.
     */
    static bool isNumeric ( const char* line );

    /**
     * Rounds the float to an integer. 
     * @param num is the float number to be rounded.
     * @return An integer with the rounded number.
     */
    static int roundf ( float num ) {
        return ( int ) floor ( num + 0.5 );
    } // end roundf

    /** Initializes the file for warning and error logging.
     * @param fileName is the name of the file.
     */
    static void initLog ( std::string fileName ) {

    	Utils::fLog.open ( fileName.c_str (), std::ofstream::out );

    	if ( !Utils::fLog ) {
    		std::cerr << " > [WARNING LOG] Log file for storing warnings and errors cannot be opened. " << std::endl;
    	}

    }

    /**
     * Writes the warning/error string to the log file.
     * @param logMessage is the string to be written.
     */
    static void logWarningError ( std::string logMessage ) {

    	std::cerr << logMessage << std::endl;

    	if ( fLog ) {
    	    fLog << logMessage << std::endl;
    	}
  
    }

    /** 
     * Closes the log file.
     */
    static void closeLog () {
        fLog.close();
    }

};

#endif
