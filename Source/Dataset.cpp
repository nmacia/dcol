
/*!

\brief This file contains the implementation of the Dataset methods.

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


#include "Dataset.h"


char Dataset::UNKNOWN_VALUE = '?';


Dataset::Dataset ( std::string fileName, bool readAttInfo, bool repUnknownVal ) {

    std::ifstream fin;
    std::string line;

    // Initialize the attributes of the environment.
    initAttributes ();

    // Initialize two variables to count the number of attributes and examples.
    numberOfAttributes = 0;
    numberOfExamples   = 0;

    // Get basic information.
    if ( Utils::doScreenStatistics ) std::cout << std::endl << "    > Getting basic information from the database: " << fileName << std::endl;
    getBasicInfo ( fileName, readAttInfo, numberOfAttributes, numberOfExamples );

    // Initialize parameters.
    if ( Utils::doScreenStatistics ) std::cout << "    > Initializing parameters " << std::endl;
    initParameters ( readAttInfo );

    // The file is opened.
    fin.open ( fileName.c_str (), std::ifstream::in );

    // Read the name and type of each attribute.
    if ( Utils::doScreenStatistics ) std::cout << std::endl << "    > Reading attributes " << std::endl;
    readAttributes ( fin, readAttInfo );

    // All data are read.
    if ( Utils::doScreenStatistics ) std::cout << std::endl << "    > Reading data " << std::endl;
    readData ( fin, readAttInfo );

    // Transform date attributes into real-valued attributes
    if ( Utils::doScreenStatistics ) std::cout << std::endl << "    > Transforming dates into numeric " << std::endl;
    transformDateAttributesToNumeric ();

    // Make statistics.
    if ( readAttInfo ) {
        if ( Utils::doScreenStatistics ) std::cout << std::endl << "    > Making statistics " << std::endl;
        makeInitialStatistics ();
    }

    // Replace missing values.
    if ( repUnknownVal ) {
        if ( Utils::doScreenStatistics ) std::cout << std::endl << "    > Replacing missing values " << std::endl;
        replaceUnknownValues ();
    }

    // The file is closed.
    fin.close ();

} // end Dataset


void Dataset::initAttributes () {

    currentExample       = 0;
    sequentialExamples   = false;
    example              = 0;
    classOfExample       = 0;
    attName              = 0;
    typeOfRepresentation = 0;
    nominalAttrNumValues = 0;
    enumAtt              = 0;
    minAttValue          = 0;
    maxAttValue          = 0;
    avgAttValue          = 0;
    stdAttValue          = 0;
    numberValuedAtt      = 0;
    minAttValuePerClass  = 0;
    maxAttValuePerClass  = 0;
    nominalClassValues   = 0;
    avgNominalValue      = 0;
    avgRealValue         = 0;
    stdRealValue         = 0;
    avgAttCounter        = 0;
    dateFormat           = 0;
    dateContent          = 0;

    numberOfContinuousAttributes = 0;
    numberOfNominalAttributes    = 0;

    areDataNormalized                 = false;
    areNominalAttributesNormalized    = false;
    areContinuousAttributesNormalized = false;
    areUnknownValuesReplaced          = false;

} // end initAttributes


void Dataset::initParameters ( bool readAttInfo ) {

    int i;

    classOfExample  = new int    [ numberOfExamples ];
    example         = new float* [ numberOfExamples ];

    for ( i = 0; i < numberOfExamples; i++ ) {
        example[i] = new float [ numberOfAttributes ];
    }

    if ( readAttInfo ) {
        initConfigParameters ( readAttInfo );
    }
    
} // end initParameters


Dataset::~Dataset () {

    int i, j;

    if ( Utils::doScreenStatistics ) {
        std::cout << std::endl << std::endl << "    > DESTROYING THE DATASET" << std::endl;
    }

    // Delete the examples and the classes.
    for ( i = 0; i < numberOfExamples; i++ ) {
        delete [] example[i];
    }

    delete [] example;
    delete [] classOfExample;

    // Delete the additional information about the attributes.
    delete [] attName;
    delete [] typeOfRepresentation;
    delete [] nominalAttrNumValues;
    delete [] minAttValue;
    delete [] maxAttValue;
    delete [] avgAttValue;
    delete [] stdAttValue;
    delete [] numberValuedAtt;
    delete [] dateFormat;
    delete [] dateContent;

    for ( i = 0; i < numberOfAttributes; i++ ) {
        if ( enumAtt[i] ) delete [] enumAtt[i];
        if ( minAttValuePerClass[i] ) delete [] minAttValuePerClass[i];
        if ( maxAttValuePerClass[i] ) delete [] maxAttValuePerClass[i];
    }

    delete [] minAttValuePerClass;
    delete [] maxAttValuePerClass;
    delete [] enumAtt;
    delete [] nominalClassValues;

    // Delete attributes that maintain statistical information.
    if ( avgNominalValue ) {
        for ( i = 0; i < numberOfAttributes; i++ ) {
            if ( avgNominalValue[i] ) {
                for ( j = 0; j < numberOfClasses; j++ ) {
                    if ( avgNominalValue[i][j] ) {
                        delete [] avgNominalValue[i][j];
                    }
                }
                delete [] avgNominalValue[i];
            }
        }
        delete [] avgNominalValue;
    }

    if ( avgRealValue && avgAttCounter && stdRealValue ) {
       
        for ( i = 0; i < numberOfAttributes; i++ ) {
            delete [] avgRealValue[i];
            delete [] avgAttCounter[i];
            delete [] stdRealValue[i];
        }
       
        delete [] stdRealValue;
        delete [] avgRealValue;
        delete [] avgAttCounter;
 
    }

    if ( Utils::doScreenStatistics ) {
       std::cout << "      > Dataset destroyed" << std::endl;
    }

} // end ~Dataset


float* Dataset::getRandomExample ( int& wClass ) {

    currentExample = ( int ) ( Utils::f_rand () * ( float ) numberOfExamples );
    wClass = classOfExample[ currentExample ];
    return example[ currentExample ];

} // end getRandomExample 


void Dataset::beginSequentialExamples () {

    sequentialExamples = true;
    currentExample = numberOfExamples - 1;

} // end beginSequentialExamples


float* Dataset::getNextExample ( int& wClass ) {

    currentExample = ( currentExample + 1 ) % numberOfExamples;
    wClass = classOfExample[ currentExample ];
    return example[ currentExample ];

} // end getNextExample


int Dataset::getNumberOfExamples () {
    return numberOfExamples;
} // end getNumberOfExamples


int Dataset::getNumberOfAttributes () {
    return numberOfAttributes;
} // end getNumberOfAttributes


int Dataset::getNumberOfClasses () {
    return numberOfClasses;
} // end getNumberOfClasses


std::string* Dataset::getAttrNames () {
    return attName;
}

//// READ DATA

void Dataset::readData ( std::ifstream& fin, bool readAttInfo ) {
    
    int i = 0;
    int j = 0;
    int attN = 0;
    std::string line;

    for ( i = 0; i < numberOfExamples; i++ ) {
        Utils::readLine ( fin, line );
        StringTokenizer st ( line, "," );

        // Control errors on data reading.
        if ( st.countTokens () != numberOfAttributes + 1 ) {

            // Convert integer into string.
            std::ostringstream stNumberOfAttributes;
            std::ostringstream stCountTokens;
            stNumberOfAttributes << numberOfAttributes + 1;
            stCountTokens << st.countTokens ();

            std::string logMessage = " \n    > [ERROR DATA SET FORMAT] The number of attributes should be ";
            logMessage += stNumberOfAttributes.str () + " and is " + stCountTokens.str () + ": " + line + " \n ";
            logMessage += " and is " + stCountTokens.str () + ": " + line + " \n ";
            logMessage += "      >> The run is not continued. \n ";
            Utils::logWarningError ( logMessage );
            throw std::string ( " Error reading the data: The number of attributes of the examples are different. " );

        }

        // Intepret the data.
        attN = 0;

        for ( j = 0; j < st.countTokens (); j++ ) {
            if ( j == classPosition ) { // Read the class value for that example.
                switch ( classType ) {

                    case 'N':
                        classOfExample[i] = getIntegerFromVector ( className, nominalClassValues,
                                            maxClassValue + 1, st.getNextToken () );
                        break;

                    case 'I':
                        classOfExample[i] = atoi ( st.getNextToken ().c_str () ) - minClassValue;
                        break;
                }
            }
            else {
                line = st.getNextToken();

                if ( line == "<null>" || line == "<NULL>" || line == "?" ) example[i][ attN ] = Dataset::UNKNOWN_VALUE;
                else {
                    switch ( typeOfRepresentation[ attN ] ) {

                        case 'N':
                            example[i][ attN ] = getIntegerFromVector ( ( j > classPosition ? attName[ j - 1 ] : attName[j] ),
                                               enumAtt[ attN ], ( int ) maxAttValue[ attN ] + 1, line );
                            break;

                        case 'I':
                            if ( !Utils::isNumeric ( line.c_str () ) ) {

                            	std::string logMessage = " \n    > [WARNING DATA SET FORMAT] Attribute " + attName[ attN ];
                            	logMessage += " should be numeric and it is not: " + line + " \n ";
                            	logMessage += "      >> We consider 0. \n ";
                                Utils::logWarningError ( logMessage );

                            }

                            example[i][ attN ] = atof ( line.c_str () );

                            if ( readAttInfo ) {
                                if ( example[i][ attN ] < minAttValue[ attN ] ) minAttValue[ attN ] = example[i][ attN ];
                                if ( example[i][ attN ] > maxAttValue[ attN ] ) maxAttValue[ attN ] = example[i][ attN ];
                            }
                            else { // If doing test and reading an "impossible" value, set to most feasible one.
                                if ( example[i][ attN ] > maxAttValue[ attN ] ) example[i][ attN ] = maxAttValue[ attN ];
                                if ( example[i][ attN ] < minAttValue[ attN ] ) example[i][ attN ] = minAttValue[ attN ];
                            }
                            break;

                        case 'R':
                            if ( !Utils::isNumeric ( line.c_str () ) ) {

                            	std::string logMessage = " \n    > [WARNING DATA SET FORMAT] Attribute " + attName[ attN ];
                            	logMessage += " should be numeric and it is not: " + line + " \n ";
                            	logMessage += "      >> We consider 0. \n ";
                                Utils::logWarningError ( logMessage );
                            }

                            example[i][ attN ] = atof ( line.c_str () );

                            if ( readAttInfo ) {
                                if ( example[i][ attN ] < minAttValue[ attN ] ) minAttValue[ attN ] = example[i][ attN ];
                                if ( example[i][ attN ] > maxAttValue[ attN ] ) maxAttValue[ attN ] = example[i][ attN ];
                            }
                            else { // If doing test and reading an "impossible" value, set to most feasible one.
                                if ( example[i][ attN ] > maxAttValue[ attN ] ) example[i][ attN ] = maxAttValue[ attN ];
                                if ( example[i][ attN ] < minAttValue[ attN ] ) example[i][ attN ] = minAttValue[ attN ];
                            }
                            break;

                        case 'D':
                            dateFormat[ attN ]->transformToDate ( line.c_str (), dateContent[ attN ][i] );
                            std::cout << "      > Date read: " << dateContent[ attN ][i] << std::endl;
                    }
                }

                attN ++;
            }
        }
    }
    
} // end readData


void Dataset::replaceUnknownValues () {

    int i, att;

    for ( i = 0; i < numberOfExamples; i++ ) {
        for ( att = 0; att < numberOfAttributes; att ++ ) {
            if ( example[i][ att ] == Dataset::UNKNOWN_VALUE ) {
                // Replace the value for the average (for integer of real representation)
                // or the most used value for nominal representation.
                if ( typeOfRepresentation[ att ] == 'R' || typeOfRepresentation[ att ] == 'I' ) {
                    example[i][ att ] = avgRealValue[ att ][ classOfExample[i] ];
                }
                else {
                    example[i][ att ] = round ( avgRealValue[ att ][ classOfExample[i] ] );
                }
            }
        }
    }

    areUnknownValuesReplaced = true;

} // end replaceUnknownValues


int Dataset::getIntegerFromVector ( std::string attName, std::string* vect, int max, std::string s ) {

    int i = 0;

    while ( i < max ) {
        if ( vect[i] == s ) break;
        i++;
    }

    if ( i == max ) {
        std::string logMessage = " \n    > [ERROR DATA SET FORMAT] Nominal value '" + s + "' not found in the definition of the attribute '";
        logMessage += attName + "'. \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Error reading the data: Nominal value '" + s + "' for attribute '" + attName + "' not found. " );
    }

    return i;

} // end getIntegerFromVector


void Dataset::normalizeContinuousAttributes () {

    int i, att;

    if ( areContinuousAttributesNormalized || areDataNormalized ) {
        std::cout << "        > The continuous attributes have been previously normalized " << std::endl;
        return;
    }

    for ( i = 0; i < numberOfExamples; i++ ) {
        for ( att = 0; att < numberOfAttributes; att ++ ) {
            if ( example[i][ att ] != Dataset::UNKNOWN_VALUE ) {
                if ( typeOfRepresentation[ att ] == 'I' || typeOfRepresentation[ att ] == 'R' ) {
                    if ( maxAttValue[ att ] - minAttValue[ att ] > 0 )
                        example[i][ att ] = ( example[i][ att ] - minAttValue[ att ] ) / ( maxAttValue[ att ] - minAttValue[ att ] );
                }
            }
        }
    }

    areContinuousAttributesNormalized = true;

    if ( areNominalAttributesNormalized ) {
        areDataNormalized = true;
    }

} // end normalizeContinuousAttributes


void Dataset::normalizeNominalAttributes () {

    int i, att;

    if ( areNominalAttributesNormalized || areDataNormalized ) {
        std::cout << "        > The nominal attributes have been previously normalized " << std::endl;
        return;
    }

    for ( i = 0; i < numberOfExamples; i++ ) {
        for ( att = 0; att < numberOfAttributes; att ++ ) {
            if ( example[i][ att ] != Dataset::UNKNOWN_VALUE ) {
                if ( typeOfRepresentation[ att ] == 'N' ) {
                    if ( maxAttValue[ att ] - minAttValue[ att ] > 0 )
                        example[i][ att ] = ( example[i][ att ] - minAttValue[ att ] ) / ( maxAttValue[ att ] - minAttValue[ att ] );
                }
            }
        }
    }

    areNominalAttributesNormalized = true;

    if ( areContinuousAttributesNormalized ) {
        areDataNormalized = true;
    }

} // end normalizeNominalAttributes


void Dataset::normalizeAllAttributes () {

    int i, att ;

    if ( areDataNormalized ) {
        std::cout << "        > The data have been previously normalized " << std::endl;
        return;
    }

    for ( i = 0; i < numberOfExamples; i++ ) {
        for ( att = 0; att < numberOfAttributes; att ++ ) {
            if ( example[i][ att ] != Dataset::UNKNOWN_VALUE ) {

                if ( typeOfRepresentation[ att ] == 'N' && !areNominalAttributesNormalized ) {
                    if ( maxAttValue[ att ] - minAttValue[ att ] > 0 )
                        example[i][ att ] = ( example[i][ att ] - minAttValue[ att ] ) / ( maxAttValue[ att ] - minAttValue[ att ] );
                }
                else if ( (typeOfRepresentation[ att ] == 'I' || typeOfRepresentation[ att ] == 'R')
                            && !areContinuousAttributesNormalized ) {
                    if ( maxAttValue[ att ] - minAttValue[ att ] > 0 )
                        example[i][ att ] = ( example[i][ att ] - minAttValue[ att ] ) / ( maxAttValue[ att ] - minAttValue[ att ] );
                }
            }
        }
    }

    areDataNormalized                 = true;
    areContinuousAttributesNormalized = true;
    areNominalAttributesNormalized    = true;

} // end normalizeAllAttributes


void Dataset::unNormalizeAllAttributes () {

    int i, att;

    for ( i = 0; i < numberOfExamples; i++ ) {
        for ( att = 0; att < numberOfAttributes; att ++ ) {
            if ( example[i][ att ] != Dataset::UNKNOWN_VALUE ) {

                if ( typeOfRepresentation[ att ] == 'N' && areNominalAttributesNormalized ) {
                    if ( maxAttValue[ att ] - minAttValue[ att ] > 0 )
                        example[i][ att ] = Utils::roundf ( example[i][ att ] * ( maxAttValue[ att ] - minAttValue[ att ] ) + minAttValue[ att ] );
                }
                else if ( (typeOfRepresentation[ att ] == 'I' || typeOfRepresentation[ att ] == 'R')
                            && areContinuousAttributesNormalized ) {
                    if ( maxAttValue[ att ] - minAttValue[ att ] > 0 ) {
                        example[i][ att ] = example[i][ att ] * ( maxAttValue[ att ] - minAttValue[ att ] ) + minAttValue[ att ];
                        if ( typeOfRepresentation[ att ] == 'I' ) {
                            example[i][ att ] = Utils::roundf ( example[i][ att ] );
                        }
                    }
                }
            }
        }
    }

    areDataNormalized                 = false;
    areContinuousAttributesNormalized = false;
    areNominalAttributesNormalized    = false;

} // end unNormalizeAllAttributes


/** INITIALIZES THE CONFIGURATION PARAMETERS **/
void Dataset::initConfigParameters ( bool readAttInfo ) {

    int i;

    if ( readAttInfo ) {
        typeOfRepresentation = new char    [ numberOfAttributes ];
        nominalAttrNumValues = new int     [ numberOfAttributes ];
        minAttValue          = new float   [ numberOfAttributes ];
        maxAttValue          = new float   [ numberOfAttributes ];
        attName              = new std::string    [ numberOfAttributes ];
        enumAtt              = new std::string*   [ numberOfAttributes ];
        dateFormat           = new DateContainer* [ numberOfAttributes ];
        dateContent          = new Date*   [ numberOfAttributes ];

        for ( i = 0; i < numberOfAttributes; i++ ) {
            enumAtt[i] = NULL;
        }
    }

} // end initConfigParameters


/** GETS THE BASIC INFORMATION FROM THE FILE **/
void Dataset::getBasicInfo ( std::string& fileName, bool readAttInfo ) {

    bool dataStarted = false;
    bool outputRead = false;
    bool relationDefined = false;
    int numOfDefinedAttributes = 0;
    std::ifstream fin;
    std::string line, lastAttribute;
    std::string logMessage;
 
    // Open the file.
    fin.open ( fileName.c_str (), std::ifstream::in );

    if ( !fin ) {
        logMessage = "";
        logMessage = " \n    > [ERROR FILE] File not found: " + fileName + " \n ";
        logMessage += "      >> Check the path or filename. \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Error opening the data set. " );
    }

    // Make a loop until the end of file counting the number of attributes and the number of examples.
    while ( Utils::readLine ( fin, line ) ) {
        if ( toLowerCase ( line ).find ( "@relation" ) != std::string::npos ) {
            relationDefined = true;
        }
        else if ( toLowerCase ( line ).find ( "@attribute" ) != std::string::npos ) {
            numOfDefinedAttributes ++;
            numberOfAttributes ++;
            if ( readAttInfo ) {
                StringTokenizer st ( line, "\"", "{" );
                st.getNextToken();
                lastAttribute = st.getNextToken();
            }
        }
        else if ( toLowerCase ( line ).find ( "@data" ) != std::string::npos ) {
            dataStarted = true;
        }
        else if ( toLowerCase ( line ).find ( "@outputs" ) != std::string::npos ) {
            StringTokenizer st ( line, " ", "," );
            st.getNextToken (); // The first token is "@outputs".

            if ( readAttInfo ) {
                className = st.getNextToken ();
                if ( line.find ( "," ) !=  std::string::npos ) {
                    logMessage = "";
                    logMessage = " \n    > [ERROR DATA SET FORMAT] @outputs contains more than one attribute: ";
                    logMessage += line + " \n ";
                    Utils::logWarningError ( logMessage );
                    throw std::string ( " Error reading the data: @outputs contains more than one attribute. " );
                }
            }

            // Decrement in one the number of attributes because one of them is the class.
            numberOfAttributes --;

            outputRead = true;
        }
        else if ( dataStarted ) numberOfExamples ++;
        else if ( toLowerCase ( line ).find ( "@inputs" ) == std::string::npos ) {
            logMessage = "";
            logMessage = " \n    > [WARNING DATA SET FORMAT] The following HEADER line is not recognized: " + line + " \n ";
            logMessage += "      >> This line will be ignored. \n ";
            Utils::logWarningError ( logMessage );
        }
    }

    if ( !outputRead ) {
        if ( readAttInfo ) className = lastAttribute;
        numberOfAttributes --;
    }

    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Number of attributes: " << numberOfAttributes << std::endl;
        std::cout << "      > Number of examples: " << numberOfExamples << std::endl << std::endl;
    }

    // The file is closed.
    fin.close();

    // Control errors.
    if ( !relationDefined ) {
        logMessage = "";
    	logMessage = " \n    > [WARNING DATA SET FORMAT] The tag '@relation' is missing in the data set definition. \n ";
        logMessage += "      >> No name will be considered for the data set. \n ";
    	Utils::logWarningError ( logMessage );
    }

    if ( !dataStarted ) {
        logMessage = "";
        logMessage = " \n    > [ERROR DATA SET FORMAT] The tag '@data' is missing in the data set definition. \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Data set malformatted: the tag '@data' is missing in the data set definition. " );
    }

    if ( numOfDefinedAttributes < 2 ) {
        std::ostringstream stNumOfDefinedAttributes;
        stNumOfDefinedAttributes << numOfDefinedAttributes;
        logMessage = "";
        logMessage = " \n    > [ERROR DATA SET FORMAT] The data set only defines " + stNumOfDefinedAttributes.str ();
        logMessage += " attributes. \n ";
        logMessage += "      >> At least, 2 attributes (one input attribute and one output attribute) ";
        logMessage += "have to be defined. \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Data set malformatted: less than two attributes are defined. " );
    }

    if ( numberOfExamples <= 0 ) {
        logMessage = "";
        logMessage = " \n    > [ERROR DATA SET FORMAT] No examples are defined. \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Data set malformatted: no examples are defined. " );
    }

} // end getBasicInfo


void Dataset::getBasicInfo ( std::string& fileName, bool readAttInfo, int& _attNum, int& _exNum ) {
    
    getBasicInfo ( fileName, readAttInfo );
    _attNum = numberOfAttributes;
    _exNum = numberOfExamples;

} // end getBasicInfo


void Dataset::readAttributes ( std::ifstream& fin, bool readAttInfo, bool readStatistics ) {

    int i, j;
    int attCount = 0;
    std::string line, s_aux;

    while ( Utils::readLine ( fin, line ) ) { // It will go out when finding "@data" token.

        if ( toLowerCase ( line ).find ( "@relation" ) != std::string::npos ) {
            StringTokenizer st1 ( line, " " );
            st1.getNextToken ();
            if ( readAttInfo ) relationName = st1.getNextToken ();

        }
        else if ( toLowerCase ( line ).find ( "@attribute" ) != std::string::npos && readAttInfo ) {

            StringTokenizer st2 ( line, "\"", "{" );

            // Do not mind about the first token (is @attribute).
            st2.getNextToken ();

            // The next token is the attribute name.
            s_aux = st2.getNextToken();

            if ( readAttInfo ) {
                if ( s_aux == className ) {
                    // Store the position in the file where the class is situated.
                    classPosition = attCount;

                    // Store the information about the class.
                    readClassInformation ( line );
                }
                else {
                    // Store the name of the attribute.
                    attName[ attCount ] = s_aux;

                    // Distinguish nominal, integer, or real representation.
                    readAttributeInformation ( line, st2, attCount );

                    // Increment the number of tokens.
                    attCount ++;
                }
            }
        }
        else if ( toLowerCase ( line ).find ( "@statistics" ) != std::string::npos && readStatistics ) {
            std::cout << "  > Reading statistics information. " << std::endl;

            for ( i = 0; i < attCount; i++ ) {
                Utils::readLine ( fin, line );
                StringTokenizer st3 ( line, " " );

                for ( j = 0; j < numberOfClasses; j++ ) {
                    avgRealValue[i][j] = atof ( st3.getNextToken ().c_str () );
                }
            }
        }
        else if ( toLowerCase ( line ).find ( "@data" ) != std::string::npos ) break;
    }

} // end readAttributes


void Dataset::readClassInformation ( std::string& s ) {

    int j;

    if ( toLowerCase ( s ).find ( "real" ) != std::string::npos ) {
        std::string logMessage = " \n    > [ERROR DATA SET FORMAT] The output attribute is real-valued and ";
        logMessage += "should be either integer or nominal: " + s + " \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Error reading the data: The output attribute is real-valued. " );
    }

    if ( toLowerCase ( s ).find ( "integer" ) != std::string::npos ) { // The attribute class type is an integer.
        if ( s.find ( "[" ) == std::string::npos ) exit ( -1 ); // The interval of classes are needed.

        // Set class type to integer.
        classType = 'I';

        // Get the boundaries.
        std::string bound  = s.substr ( s.find_first_of ( "[" ) + 1, s.find_first_of ( "]" ) - s.find_first_of ( "[" ) - 1 );
        std::string min    = bound.substr ( 0,  bound.find_first_of ( "," ) ).c_str ();
        minClassValue = atoi ( min.c_str () );
        bound         = bound.substr ( bound.find_first_of ( "," ) + 1, bound.find_first_of ( "," ) - bound.size () );

        StringTokenizer stBound ( bound, " " );
        std::string max = stBound.getNextToken ().c_str ();
        maxClassValue = atoi ( max.c_str () );

        // Fix the number of actions of the system.
        numberOfClasses = maxClassValue - minClassValue + 1;
    }
    else if ( s.find ( "{" ) != std::string::npos ) { // The attribute class type is a nominal.

        // Set the class type to nominal.
        classType = 'N';
        s = s.substr ( s.find_first_of ( "{" ) + 1, s.find_first_of ( "}" ) - s.find_first_of ( "{" ) - 1 );

        StringTokenizer st_n ( s, "," );
        numberOfClasses = st_n.countTokens ();

        minClassValue = 0;
        maxClassValue = numberOfClasses - 1;

        nominalClassValues = new std::string[ numberOfClasses ];

        for ( j = 0; j < numberOfClasses; j++ ) {
            nominalClassValues[j] = st_n.getNextToken ();
        }
    }
    else {
        std::string logMessage = " \n    > [ERROR DATA SET FORMAT] Attribute incorrectly defined: " + s + " \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Attribute malformatted: " + s + ". " );
    }

} // end readClassInformation


void Dataset::readAttributeInformation ( std::string& s, StringTokenizer& st, int attCount ) {

    int j;

    if ( s.find ( "{" ) == std::string::npos &&
        (toLowerCase ( s ).find ( "real" ) != std::string::npos ||
         toLowerCase ( s ).find ( "integer" ) != std::string::npos || 
         toLowerCase ( s ).find ( "numeric" ) != std::string::npos)) {
        // The next token is the type. We supose that it only can be a character, integer, or real.
        std::string type = toLowerCase ( st.getNextToken () );

        if ( type == "integer" ) typeOfRepresentation[ attCount ] = 'I';
        else                     typeOfRepresentation[ attCount ] = 'R';

        numberOfContinuousAttributes ++;

        if ( Utils::doScreenStatistics )
            std::cout << attName[attCount] << " --> " << typeOfRepresentation[ attCount ] << std::endl;
        // Get the attributes boundaries if they exist.
        if ( s.find ( "[" ) == std::string::npos ) { // They do not exist.
            minAttValue[ attCount ] = FLT_MAX;
            maxAttValue[ attCount ] = -FLT_MAX;
        }
        else { // The boundaries are specified.

            std::string bound       = s.substr ( s.find_first_of ( "[" ) + 1, s.find_first_of ( "]" ) - s.find_first_of ( "[" ) - 1 );
            std::string min         = bound.substr ( 0,  bound.find_first_of ( "," ) ).c_str ();
            minAttValue[ attCount ] = atof ( min.c_str () );

            bound = bound.substr ( bound.find_first_of ( "," ) + 1, bound.find_first_of ( "," ) - bound.size () );
            StringTokenizer stBound ( bound, " " );
            std::string max = stBound.getNextToken ().c_str ();
            maxAttValue[ attCount ] = atof ( max.c_str () );
        }
    }
    else if ( toLowerCase ( s ).find ( "date" ) != std::string::npos ) {

        // Get the string that specifies the date format.
        int position;
        std::string dateString;
        position = toLowerCase ( s ).find ( " date " ) + 5;
        while ( ( unsigned int ) position < s.size () && s[ position ] == ' ' ) position ++;
        dateString = s.substr ( position, s.size () );

        // Create a date reader.
        dateFormat[ attCount ]  = new DateContainer ( dateString );
        dateContent[ attCount ] = new Date [ numberOfExamples ]; 

        typeOfRepresentation[ attCount ] = 'D';
        numberOfContinuousAttributes ++;

        if ( Utils::doScreenStatistics ) 
            std::cout << attName[attCount] << " --> " << typeOfRepresentation[ attCount ] << std::endl;
    }
    else if ( s.find ( "{" ) != std::string::npos ) { // It is a nominal.
    
        typeOfRepresentation[ attCount ] = 'N';
        numberOfNominalAttributes ++;
        
        // Control format error.
        if ( s.find ( "}" ) == std::string::npos ) {
            std::string logMessage = " \n    > [ERROR DATA SET FORMAT] Attribute incorrectly defined: " + s + " \n ";
            Utils::logWarningError ( logMessage );
            throw std::string ( " Attribute malformatted: " + s + ". " );
        }

        if ( Utils::doScreenStatistics )
            std::cout << attName[attCount] << " --> " << typeOfRepresentation[ attCount ] << std::endl;

        s = s.substr ( s.find_first_of ( "{" ) + 1, s.find_first_of ( "}" ) - s.find_first_of ( "{" ) - 1 );

        StringTokenizer st_n ( s, "," );
        minAttValue[ attCount ] = 0.0;
        maxAttValue[ attCount ] = st_n.countTokens () - 1;
        nominalAttrNumValues[ attCount ] = st_n.countTokens ();

        // Reserve memory to store all the nominal identifiers.
        enumAtt[ attCount ] = new std::string [ ( int ) maxAttValue[ attCount ] + 1 ];

        j = 0;

        while ( st_n.hasMoreTokens () ) {
            enumAtt[ attCount ][j] = st_n.getNextToken ();
            j++;
        }
    }
    else if ( toLowerCase ( s ).find ( "string" ) != std::string::npos ) { 
        std::string logMessage = " \n    > [ERROR DATA SET FORMAT] Type of attribute not accepted: " + s + " \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Attribute malformatted: " + s + ". " );
    }
    else { 
        std::string logMessage = " \n    > [ERROR DATA SET FORMAT] Attribute incorrectly defined: " + s + " \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Attribute malformatted: " + s + ". " );
    }

} // end readAttributeInformation


std::string Dataset::toLowerCase ( std::string s ) {

    int i;

    for ( i = 0; i < ( int ) s.length (); i++ ) {
        if ( s[i] >= 'A' && s[i] <= 'Z' ) {
            s[i] = 'a' + ( s[i] - 'A' );
        }
    }

    return s;

} // end toLowerCase


void Dataset::transformDateAttributesToNumeric () {

    int att;

    for ( att = 0; att < numberOfAttributes; att ++  ) {
        if ( typeOfRepresentation[ att ] == 'D' ) {
            transformDateAttributeToNumeric ( att );
        }
    }

} // end transformDateAttributesToNumeric


void Dataset::transformDateAttributeToNumeric ( int whichAttribute ) {

    int i, j, arrayLength;
    Date* values = new Date [ numberOfExamples ];
    int* index  = NULL;

    for ( i = 0; i < numberOfExamples; i++ ) {
        values[i] = dateContent[ whichAttribute ][i];
    }

    Utils::quickSort ( values, index, 0, numberOfExamples - 1 );

    // Remove repeated values from the array.
    i = 0; // Indexes the last not repeated value.
    j = 1;

    while ( j < numberOfExamples ) {
        if ( values[j] != values[i] ) {
            values[ i + 1 ].init ( values[j] );
            i++;
        }
        j++;
    }

    arrayLength = i + 1;

    // Number of different values in the array.
    minAttValue [ whichAttribute ] = 0;
    maxAttValue [ whichAttribute ] = i;

    // Replace the values for its order.
    for ( i = 0; i < numberOfExamples; i++ ) {
        example[i][ whichAttribute ] = convertToOrdinal ( dateContent[ whichAttribute ][i], values, arrayLength );
		std::cout << "        > Value for example " << i << ": " << example[i][ whichAttribute ] << std::endl;
    }

    // Delete Reserved memory.
    delete [] values;

    // Delete the date format and the date content.
    delete    dateFormat [ whichAttribute ];
    delete [] dateContent[ whichAttribute ];

    typeOfRepresentation[ whichAttribute ] = 'I';

} // end transformDateAttributeToNumeric


float Dataset::convertToOrdinal ( Date value, Date* convertionArray, int arrayLength ) {

    int i;

    for ( i = 0; i < arrayLength; i++ ) {
        if ( convertionArray[i] >= value ) {
            return i;
        }
    }

    return arrayLength - 1;

} // end convertToOrdinal


void Dataset::removeAllZeroExamples () {

    int i, att;

    i = 0;

    while ( i < numberOfExamples ) {
        for ( att = 0; att < numberOfAttributes; att ++ ) {
            if ( example[i][ att ] !=  0 ) {
                break;
            }
            else if ( att == numberOfAttributes - 1 ) {
                // Remove example.
                delete example[i];
                example[i] = example[ numberOfExamples - 1 ];
                numberOfExamples --;
                i--;
                std::cout << "  > EXAMPLE " << i << " REMOVED " << std::endl;
            }
        }

        i++;
    }

} // end removeAllZeroExamples


Matrix* Dataset::getMeanVectorOfClass( int wClass ) {

    Matrix* m = new Matrix ( numberOfAttributes, 1 );
    int i, att, counter;

    // Compute the average again in case that the data set has been normalized.
    for ( att = 0; att < numberOfAttributes; att ++ ) {
        counter = 0;
        for ( i = 0; i < numberOfExamples; i++ ) {
            if ( classOfExample[i] == wClass && example[i][ att ] != Dataset::UNKNOWN_VALUE ) {
                m->setValue ( att, 0, m->getValue ( att, 0 ) + example[i][ att ] );
                counter ++;
            }
        }

        if ( counter > 0 ) {
            m->setValue ( att, 0, m->getValue ( att, 0 ) / counter );
        }
    }

    return m;
    
} // end getMeanVectorOfClass


Matrix* Dataset::getCovarianceMatrixOfClass ( int wClass ) {
    
    int i, att, k, counter;
    Matrix* m = new Matrix ( numberOfAttributes, numberOfAttributes );

    for ( i = 0; i < numberOfAttributes; i++ ) {
        for ( att = 0; att < numberOfAttributes; att ++ ) {

            counter = 0;
            for ( k = 0; k < numberOfExamples; k++ ) {
                if ( classOfExample[k] == wClass ) {

                    if ( example[k][i] != Dataset::UNKNOWN_VALUE ) {
                        m->setValue ( i, att, m->getValue ( i, att ) + ( example[k][i] - avgRealValue[i][ wClass ] ) * ( example[k][ att ] - avgRealValue[ att ][ wClass ] ) );
                        counter ++;
                    }
                }
            }

            if ( counter > 1 ) {
                m->setValue ( i, att, m->getValue ( i, att ) / ( counter - 1 ) );
            }
            else {
                m->setValue ( i, att, 0 );
            }

        }
    }

    return m;
    
} // end getCovarianceMatrixOfClass
