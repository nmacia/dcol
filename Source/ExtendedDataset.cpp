
/*!

\brief This file contains the implementation of the ExtendedDataset methods.

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
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DCoL. If not, see <http://www.gnu.org/licenses/>.

*/


#include "ExtendedDataset.h"


float ExtendedDataset::MAXIMUM_EXAMPLES_FOR_PRECALCULATION = 5000;

// Distance functions.
int ExtendedDataset::EUCLIDEAN              = 1;
int ExtendedDataset::NORMALIZED_EUCLIDEAN   = 2;
int ExtendedDataset::STD_WEIGHTED_EUCLIDEAN = 3;
int ExtendedDataset::OVERLAP_NOMINAL        = 4;
int ExtendedDataset::VDM_NOMINAL            = 5;


ExtendedDataset::ExtendedDataset ( std::string fileName, bool readAttInfo, bool repUnknownVal,
                                   int realAttributesFunction, int nominalAttributesFunction ) : 
                                   Dataset ( fileName, readAttInfo, repUnknownVal ) {

    // Initialize the class variables.
    numberOfExamplesPerClass    = 0;
    examplesPerClass       = 0;
    indexExamplesPerClass  = 0;
    distanceFunction       = 0;
    distances              = 0;

    // After normalizing the data, create distance functions.
    instantiateDistanceFunctions ( realAttributesFunction, nominalAttributesFunction );

    if ( Utils::doScreenStatistics ) {
        print ( false );
    }
    
} // end ExtendedDataset


ExtendedDataset::~ExtendedDataset () {

    int i;
    
    if ( Utils::doScreenStatistics ) std::cout << std::endl << std::endl << "    > DESTROYING THE EXTENDED DATASET " << std::endl;

    // Delete the structure that maintains the examples per class (only if built).
    if ( numberOfExamplesPerClass ) delete [] numberOfExamplesPerClass;

    if ( examplesPerClass ) {
        for ( i = 0; i < numberOfClasses; i++ ) delete [] examplesPerClass[i];
        delete [] examplesPerClass;
    }

    if ( indexExamplesPerClass ) {
        for ( i = 0; i < numberOfClasses; i++ ) delete [] indexExamplesPerClass[i];
        delete [] indexExamplesPerClass;
    }

    // Delete the vector of distances if calculated.
    delete [] distances;

    // Delete the distanceFunctions.
    for ( i = 0; i < numberOfAttributes; i++ ) {
        delete distanceFunction[i];
    }

    delete [] distanceFunction;

    if ( Utils::doScreenStatistics ) std::cout << "      > ExtendedDataset destroyed " << std::endl;

} // end ~ExtendedDataset


void ExtendedDataset::instantiateDistanceFunctions ( int realAttributesFunction, int nominalAttributesFunction ) {

    int att;

    if ( Utils::doScreenStatistics ) std::cout << std::endl <<  "    > Declaring a distance object for each input variable " << std::endl;

    // Store the type of distance functions.
    typeOfContinuousDistance = realAttributesFunction;
    typeOfNominalDistance    = nominalAttributesFunction;

    // Reserve memory for a distance metric objects per data set attribute.
    distanceFunction = new DistanceFunction* [ numberOfAttributes ];

    // TO SPEED UP: If we receive NORMALIZED_EUCLIDEAN, we normalize continuous attributes.
    if ( !areContinuousAttributesNormalized && typeOfContinuousDistance == ExtendedDataset::NORMALIZED_EUCLIDEAN ) {
        if ( Utils::doScreenStatistics ) {
            std::cout << "    > Normalizing continuous attributes since NORMALIZED_EUCLIDEAN " ;
            std::cout << "has been selected as distance function for continuous attributes " << std::endl;
        }
        normalizeContinuousAttributes ();
        typeOfContinuousDistance = realAttributesFunction = ExtendedDataset::EUCLIDEAN;
    }

    if ( !areNominalAttributesNormalized && typeOfNominalDistance == ExtendedDataset::NORMALIZED_EUCLIDEAN ) {
        if ( Utils::doScreenStatistics ) {
            std::cout << "  > Normalizing nominal attributes since NORMALIZED_EUCLIDEAN ";
            std::cout << "has been selected as distance function for nominal attributes " << std::endl;
        }
        normalizeNominalAttributes ();
        typeOfNominalDistance = nominalAttributesFunction = ExtendedDataset::EUCLIDEAN;
    }

    for ( att = 0; att < numberOfAttributes; att ++ ) {

        if ( typeOfRepresentation [ att ] == 'N' ) {
            if ( typeOfNominalDistance == ExtendedDataset::EUCLIDEAN ) {
                distanceFunction [ att ] = new EuclideanFunction ();
            }
            else if ( typeOfNominalDistance == ExtendedDataset::NORMALIZED_EUCLIDEAN ) {
                distanceFunction [ att ] = new NormalizedEuclideanFunction ( minAttValue[ att ], maxAttValue[ att ] );
            }
            else if ( typeOfNominalDistance == ExtendedDataset::STD_WEIGHTED_EUCLIDEAN ) {
                distanceFunction [ att ] = new StdWeightedEuclideanFunction ( stdAttValue[ att ] );

                if ( stdAttValue[ att ] < 0.01 ) {
                    std::ostringstream stAtt;
                    stAtt << att;
                    std::string logMessage = "    > [WARNING] The standard deviation of attribute " + stAtt.str () + " is very small: ";
                    stAtt << stdAttValue [ att ];
                    logMessage += stAtt.str () + ". This may cause a malfunction of the ";
                    logMessage += " standard-deviation-weighted Euclidean distance function. \n ";
                    Utils::logWarningError ( logMessage );
                }
            }
            else if ( typeOfNominalDistance == ExtendedDataset::OVERLAP_NOMINAL ) {
                distanceFunction [ att ] = new OverlapFunction ();
            }
            else if ( typeOfNominalDistance == ExtendedDataset::VDM_NOMINAL ) {
                distanceFunction [ att ] = new VDMFunction ( avgNominalValue[ att ], numberOfClasses, ( int ) ( maxAttValue[ att ] - minAttValue[ att ] + 1 ) );
            }
            else {
                std::string logMessage = " > [ERROR DISTANCE METRIC] No distance metric for continuous attributes. \n ";
                Utils::logWarningError ( logMessage );
                exit ( -1 );
            }
        }
        else { // Continuous attribute.
            if ( typeOfContinuousDistance == ExtendedDataset::EUCLIDEAN ) {
                distanceFunction [ att ] = new EuclideanFunction ();
            }
            else if ( typeOfContinuousDistance == ExtendedDataset::NORMALIZED_EUCLIDEAN ) {
                distanceFunction [ att ] = new NormalizedEuclideanFunction ( minAttValue[ att ], maxAttValue[ att ] );
            }
            else if ( typeOfContinuousDistance == ExtendedDataset::STD_WEIGHTED_EUCLIDEAN ) {
                distanceFunction [ att ] = new StdWeightedEuclideanFunction ( stdAttValue[ att ] );
            }
            else if ( typeOfContinuousDistance == ExtendedDataset::OVERLAP_NOMINAL ||
                      typeOfContinuousDistance == ExtendedDataset::VDM_NOMINAL ) {
                std::string logMessage = " > [ERROR DISTANCE METRIC] VDM_NOMINAL metric has been declared for continuous attributes. \n ";
                logMessage += "   >> It will be replaced with NORMALIZED_EUCLIDEAN. \n ";
                Utils::logWarningError ( logMessage );
                distanceFunction [ att ] = new NormalizedEuclideanFunction ( minAttValue[ att ], maxAttValue[ att ] );
            }
            else {
                std::string logMessage = " > [ERROR DISTANCE METRIC] Error in instantiating a metric for a continuous attribute. \n ";
                Utils::logWarningError ( logMessage );
                exit ( -1 );
            }
        }
    }

} // end instantiateDistanceFunctions


void ExtendedDataset::deleteExamplesPerClass () {

    int i;

    if ( numberOfExamplesPerClass ) delete [] numberOfExamplesPerClass;

    if ( examplesPerClass ) {
        for ( i = 0; i < numberOfClasses; i++ ) delete [] examplesPerClass[i];
        delete [] examplesPerClass;
    }

    if ( indexExamplesPerClass ) {
        for ( i = 0; i < numberOfClasses; i++ ) delete [] indexExamplesPerClass[i];
        delete [] indexExamplesPerClass;
    }

    numberOfExamplesPerClass   = 0;
    examplesPerClass      = 0;
    indexExamplesPerClass = 0;
    
} // end deleteExamplesPerClass


float* ExtendedDataset::getDatasetCharacteristics () {

    float* results = new float [11];
    int majClass, minClass;
    int numMissValue = 0, numInstMissValue = 0, numAttMissValue = 0;
    bool hasMissValue;
    bool* attWithMissValue = new bool [ numberOfAttributes ];
    int att, i;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        attWithMissValue[ att ] = false;
    }

    results[0] = numberOfExamples;
    results[1] = numberOfAttributes;
    results[2] = results[3] = results[4] = 0;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        switch ( typeOfRepresentation[ att ] ) {

            case 'R':
                results[2] ++;
                break;

            case 'I':
                results[3] ++;
                break;

            case 'N':
                results[4] ++;
                break;
        }
    }

    results[5] = numberOfClasses;

    // Count missing attributes, examples, and values.
    for ( i = 0; i < numberOfExamples; i++ ) {
        hasMissValue = false;

        for ( att = 0; att < numberOfAttributes; att ++ ) {
            if ( example[i][ att ] == Dataset::UNKNOWN_VALUE ) {
                hasMissValue = true;
                numMissValue ++;
                attWithMissValue[ att ] = true;
            }
        }

        if ( hasMissValue ) numInstMissValue ++;
    }

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        if ( attWithMissValue[ att ] ) numAttMissValue ++;
    }

    delete [] attWithMissValue;

    results[6] = ( float ) numAttMissValue / ( float ) numberOfAttributes;
    results[7] = ( float ) numInstMissValue / ( float ) numberOfExamples;
    results[8] = ( float ) numMissValue / ( float ) ( numberOfAttributes * numberOfExamples );

    // Organize examples per class.
    organizePerClass();

    majClass = minClass = 0;

    for ( i = 1; i < numberOfClasses; i++ ) {
        if ( numberOfExamplesPerClass[i] > numberOfExamplesPerClass[ majClass ] ) {
            majClass = i;
        }

        if ( numberOfExamplesPerClass[i] < numberOfExamplesPerClass[ minClass ] ) {
            minClass = i;
        }
    }

    results[9]  = ( float ) numberOfExamplesPerClass[ majClass ] / ( float ) numberOfExamples;
    results[10] = ( float ) numberOfExamplesPerClass[ minClass ] / ( float ) numberOfExamples;

    return results;
    
} // end getDatasetCharacteristics


float* Dataset::getCurrentExample () {
    return example[ currentExample ];
} // end getCurrentExample


int Dataset::getCurrentClass () {
    return classOfExample[ currentExample ];
} // end getCurrentClass


std::string* ExtendedDataset::generate2ClassDatasets ( std::string baseName ) {

    std::string auxName;
    char auxClass[10];
    std::ofstream fout;
    std::string* newDatasetNames;
    int i;

    if ( numberOfClasses == 2 ) {
        std::string logMessage = "  > [ERROR PARTITIONING] The data set has only two classes. \n ";
        logMessage += "    >> It will not be partitioned. \n ";
        Utils::logWarningError ( logMessage );
        newDatasetNames = new std::string [1];
        newDatasetNames[0] = baseName;
        return newDatasetNames;
    }

    newDatasetNames = new std::string[ numberOfClasses ];

    for ( i = 0; i < numberOfClasses; i++ ) {
        sprintf ( auxClass, "%d", i );
        auxName = baseName + ".2c" + auxClass;

        std::cout << "      > Creating file: " << auxName << std::endl;
        fout.open ( auxName.c_str (), std::ofstream::out );
        printOneClassAgainstOthers ( fout, i );

        fout.close ();

        // Copy the name of the new data set.
        newDatasetNames[i] = auxName;
    }

    return newDatasetNames;

} // end generate2ClassDatasets


//// PRINT DATA

std::ofstream& ExtendedDataset::print ( std::ofstream& fout, bool printNormalizedData = false ) {

    int att, i;

    // If required, normalize the data.
    if ( printNormalizedData ) {
        normalizeAllAttributes ();
    }
    else {
        unNormalizeAllAttributes ();
    }

    // Print the header.
    fout << "@relation " << relationName << std::endl;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        fout << "@attribute " << attName[ att ];

        switch ( typeOfRepresentation[ att ] ) {

            case 'N':
                if ( areNominalAttributesNormalized ) {
                    fout << " real [0, 1]";
                }
                else {
                    fout << " {" << enumAtt[ att ][0];
                    for ( i = 1; i <= ( int ) maxAttValue[ att ]; i++ )	fout << "," << enumAtt[ att ][i];
                    fout << "}" ;
                }
                break;

            case 'I':
                if ( areContinuousAttributesNormalized ) {
                    fout << " real [0, 1]";
                }
                else {
                    fout << " integer " << "["  << ( int ) minAttValue[ att ] << "," <<  ( int ) maxAttValue[ att ] << "]";
                }
                break;

            case 'R':
                if ( areContinuousAttributesNormalized ) {
                    fout << " real [0, 1]";
                }
                else {
                    fout << " real " << "[" << minAttValue[ att ] << "," <<  maxAttValue[ att ] << "]";
                }
                break;
        }

        fout << std::endl;
    }

    fout << "@attribute " << className << " {" << nominalClassValues[0];
    for ( i = 1; i < numberOfClasses; i++ ) fout << "," << nominalClassValues[i];
    fout << "}" << std::endl;
    fout << "@data" << std::endl;

    for ( i = 0; i < numberOfExamples; i++ ) {
        for ( att = 0; att < numberOfAttributes; att ++ ) {
            if ( example[i][ att ] == Dataset::UNKNOWN_VALUE ) {
                fout << "?," ;
            }
            else {
                if ( printNormalizedData ) {
                    fout << example[i][ att ] << ",";
                }
                else {
                    switch ( typeOfRepresentation[ att ] ) {

                        case 'N':
                            if ( areNominalAttributesNormalized ) {
                                fout << example[i][ att ] << ",";
                            }
                            else {
                                fout << enumAtt[ att ][ ( int ) example[i][ att ] ] << ",";
                            }
                            break;

                        case 'I':
                            if ( areContinuousAttributesNormalized ) {
                                fout << example[i][ att ] << ",";
                            }
                            else {
                                fout << ( int ) example[i][ att ] << ",";
                            }
                            break;

                        case 'R':
                            fout << example[i][ att ] << ",";
                            break;
                    }
                }
            }
        }

        fout <<  nominalClassValues[ ( int ) classOfExample[i] ] << std::endl;
    }

    return fout;

} // end print


std::ofstream& ExtendedDataset::printOneClassAgainstOthers ( std::ofstream& fout, int whichClass ) {

    int att, i;

    std::string nameGroupClass = "groupClass";

    fout << "@relation " << relationName << "-class" << whichClass << std::endl;

    for ( att = 0; att < numberOfAttributes; att++ ) {
        fout << "@attribute " << attName[ att ];

        switch ( typeOfRepresentation[ att ] ) {

            case 'N':
                fout << " {" << enumAtt[ att ][0];
                for ( i = 1; i <= ( int ) maxAttValue[ att ]; i++ ) fout << "," << enumAtt[ att ][i];
                fout << "}";
                break;

            case 'I':
                fout << " integer " << "["  << ( int ) minAttValue[ att ] << "," <<  ( int ) maxAttValue[ att ] << "]";
                break;

            case 'R':
                fout << " real " << "[" << minAttValue[ att ] << "," <<  maxAttValue[ att ] << "]";
                break;
        }

        fout << std::endl;
    }

    fout << "@attribute " << className << " {" << nominalClassValues[ whichClass ];
    fout << "," << nameGroupClass  << "}" << std::endl;
    fout << "@data" << std::endl;

    for ( i = 0; i < numberOfExamples; i++ ) {
        for ( att = 0; att < numberOfAttributes; att ++ ) {
            if ( example[i][ att ] == Dataset::UNKNOWN_VALUE ) {
                fout << "?," ;
            }
            else {
                switch ( typeOfRepresentation[ att ] ) {

                    case 'N':
                        fout << enumAtt[ att ][ ( int ) example[i][ att ] ] << ",";
                        break;

                    case 'I':
                        fout << ( int ) example[i][ att ] << ",";
                        break;

                    case 'R':
                        fout << example[i][ att ] << ",";
                        break;
                }
            }
        }

        if ( classOfExample[i] == whichClass ) {
            fout << nominalClassValues[ ( int ) classOfExample[i] ] << std::endl;
        }
        else {
            fout << nameGroupClass << std::endl;
        }
    }

    return fout;

} // end printOneClassAgainstOthers


void ExtendedDataset::printHeader ( std::ofstream& fout ) {

    int att, i;

    fout << "@relation " << relationName << std::endl;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        fout << "@attribute " << attName[ att ];

        switch ( typeOfRepresentation[ att ] ) {

            case 'N':
                fout << " {" << enumAtt[ att ][0];
                for ( i = 1; i <= ( int ) maxAttValue[ att ]; i++ ) fout << "," << enumAtt[ att ][i];
                fout << "}";
                break;

            case 'I':
                fout << " integer " << "["  << ( int ) minAttValue[ att ] << "," <<  ( int ) maxAttValue[ att ] << "]";
                break;

            case 'R':
                fout << " real " << "[" << minAttValue[ att ] << "," <<  maxAttValue[ att ] << "]";
                break;
        }

        fout << std::endl;
    }

    fout << "@attribute " << className << " {" << nominalClassValues[0];
    for ( i = 1; i < numberOfClasses; i++ ) fout << "," << nominalClassValues[i];
    fout << "}" << std::endl;
    fout << "@data" << std::endl;

} // end printHeader


void ExtendedDataset::print ( bool printExamples ) {

    int i, j, k;

    std::cout << std::endl << "    > PRINTING THE ENVIRONMENT STATE " << std::endl << std::endl;
    std::cout << "       Relation name: " << relationName << std::endl;
    std::cout << "       Number of attributes: " <<  numberOfAttributes << std::endl;
    std::cout << "       Number of examples: " << numberOfExamples << std::endl;

    for ( i = 0; i < numberOfAttributes; i++ ) {
        std::cout << "       == Attribute " << i << ": " << attName[i] << std::endl;
        std::cout << "\t\t Type: " << typeOfRepresentation[i] << std::endl;
        std::cout << "\t\t Minimum bound: " << minAttValue[i] << std::endl;
        std::cout << "\t\t Maximum bound: " << maxAttValue[i] << std::endl;

        for ( j = 0; j < numberOfClasses; j++ ) {
            std::cout << "\t\t Class " << j << ": ";

            if ( nominalClassValues ) std::cout << "  " << nominalClassValues[j];

            std::cout << "  \t Avg + std  ( minVal - maxVal ) --> ";
            std::cout << "\t" << std::setw ( 8 ) << avgRealValue[i][j] << " + " << std::setw ( 8 ) << stdRealValue[i][j];
            std::cout << " ("  << std::setw ( 4 ) << minAttValuePerClass[i][j] << " - ";
            std::cout << std::setw ( 4 ) << maxAttValuePerClass[i][j] << " )" << std::endl;

            if ( typeOfRepresentation[i] == 'N' ) { // Print the frequencies.
                std::cout << "\t\t Frequencies per nominal value: ";

                for ( k = 0; k <= ( int ) maxAttValue[i]; k++ ) {
                    std::cout << avgNominalValue[i][j][k] ;
                    if ( k < ( int ) maxAttValue[i] ) std::cout << " - ";
                }

                std::cout << std::endl;
            }
        }

        if ( typeOfRepresentation[i] == 'N' ) {
            std::cout << "\t\t It can take the values: ";

            for ( j = 0; j <= ( int ) maxAttValue[i]; j++ )
                std::cout << enumAtt[i][j] << "; ";

            std::cout << "--" << std::endl;
        }
    }

    std::cout << "       Class name: " << className << std::endl;
    std::cout << "       Number of classes: " << numberOfClasses << std::endl;
    std::cout << "\t\t Class type: " << classType << std::endl;

    if ( classType == 'N' ) {
        std::cout << "\t\t Class values: ";
        for ( i = 0; i < numberOfClasses; i++ ) std::cout << nominalClassValues[i] << "; ";
        std::cout << std::endl;
    }

    // Print examples if required.
    if ( printExamples ) {
        std::cout << std::endl << "       > EXAMPLES " << std::endl << std::endl;
        for ( i = 0; i < numberOfExamples; i++ ) {
            std::cout << "Example " << std::setw ( 2 ) << i << ": ";

            for ( j = 0; j < numberOfAttributes; j++ ) {
                std::cout << " " << std::setw ( 4 ) << example[i][j] << " ";
            }

            std::cout << "class = " << classOfExample[i] << std::endl;
        }
    }

    std::cout << std::endl << "    > END OF PRINTING THE ENVIRONMENT STATE " << std::endl << std::endl;

} // end print


void ExtendedDataset::printExample ( std::ofstream& fout, float* inst, int action ) {

    int att;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        if ( inst[ att ] == Dataset::UNKNOWN_VALUE ) {
            fout << "?," ;
        }
        else {
            switch ( typeOfRepresentation[ att ] ) {

                case 'N':
                    fout << inst[ att ] << ",";
                    break;

                case 'I':
                    fout << ( int ) inst[ att ] << ",";
                    break;

                case 'R':
                    fout << inst[ att ] << ",";
                    break;
            }
        }
    }

    fout << nominalClassValues[ ( int ) action ] << std::endl;

} // end printExample


void ExtendedDataset::printExampleNoNormalized ( std::ofstream& fout, float* inst, int action ) {

	int att;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        if ( inst[ att ] == Dataset::UNKNOWN_VALUE ) {
            fout << "?," ;
        }
        else {
            switch ( typeOfRepresentation[ att ] ) {

                case 'N':
                    fout << enumAtt[ att ][ ( int ) inst[ att ] ].c_str () << ",";
                    break;

                case 'I':
                    if ( areContinuousAttributesNormalized && ( maxAttValue[ att ] - minAttValue[ att ] ) > 0 )
                        fout << ( int ) Utils::roundf ( inst[ att ]* ( maxAttValue[ att ] - minAttValue[ att ] ) + minAttValue[ att ] ) << ",";
                    else
                        fout << ( int ) inst[ att ] << ",";
                    break;

                case 'R':
                    if ( areContinuousAttributesNormalized && ( maxAttValue[ att ] - minAttValue[ att ] ) > 0 )
                        fout << inst[ att ]* ( maxAttValue[ att ] - minAttValue[ att ] ) + minAttValue[ att ] << ",";
                    else
                        fout << inst[ att ] << ",";
                    break;
            }
        }
    }

    fout << nominalClassValues[ ( int ) action ] << std::endl;

} // end printExampleNoNormalized


void ExtendedDataset::printExample ( float* inst, int action ) {

	int att ;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        if ( inst[ att ] == Dataset::UNKNOWN_VALUE ) {
            std::cout << "?," ;
        }
        else {
            switch ( typeOfRepresentation[ att ] ) {

                case 'N':
                    std::cout << ( int ) inst[ att ] ;
                    break;

                case 'I':
                    std::cout << ( int ) inst[ att ] << ",";
                    break;

                case 'R':
                    std::cout << inst[ att ] << ",";
                    break;
            }
        }
    }

    std::cout << nominalClassValues[ ( int ) action ] << std::endl;

} // end printExample


//// FUNCTIONS FOR DATA PARTITION

void ExtendedDataset::organizePerClass () {

    int i;
    int* counterInstPerClass;

    if ( numberOfExamplesPerClass != 0 ) {
        if ( Utils::doScreenStatistics ) std::cout << "      > Examples already organized per class " << std::endl;

        return;
    }

    if ( Utils::doScreenStatistics ) std::cout << "      > Organizing examples per class " << std::endl;

    numberOfExamplesPerClass = new int [ numberOfClasses ];
    counterInstPerClass = new int [ numberOfClasses ];

    for ( i = 0; i < numberOfClasses; i++ ) {
        numberOfExamplesPerClass[i] = 0;
        counterInstPerClass[i] = 0;
    }

    for ( i = 0; i < numberOfExamples; i++ ) {
        numberOfExamplesPerClass[ classOfExample[i] ] ++;
    }

    // Reserve memory to maintain pointers per class.
    examplesPerClass = new float** [ numberOfClasses ];
    indexExamplesPerClass = new int* [ numberOfClasses ];

    for ( i = 0; i < numberOfClasses; i++ ) {
        if ( Utils::doScreenStatistics ) {
            std::cout << "        > Number of examples of class " << i;
            if ( nominalClassValues ) std::cout << " " << nominalClassValues[i];
            std::cout << ": " << numberOfExamplesPerClass[i] << std::endl;
        }
        examplesPerClass[i] = new float* [ numberOfExamplesPerClass[i] ];
        indexExamplesPerClass[i] = new int [ numberOfExamplesPerClass[i] ];
    }

    int whichClass;

    // Group the examples per class.
    for ( i = 0; i < numberOfExamples; i++ ) {
        whichClass = classOfExample[i];
        examplesPerClass [ whichClass ] [ counterInstPerClass[ whichClass ] ] = example[i];
        indexExamplesPerClass [ whichClass ] [ counterInstPerClass[ whichClass ] ] = i;
        counterInstPerClass [ whichClass ] ++;
    }

    delete [] counterInstPerClass;

} // end organizePerClass


void ExtendedDataset::stratifiedCrossValidation ( int k, std::string outName ) {

    int i, j;
    int fold, rnd;
    int* numInstPerClassInFold = new int  [ numberOfClasses ];
    int* residualPerClass      = new int  [ numberOfClasses ];
    bool* exampleSelected      = new bool [ numberOfExamples ];

    // Organize the examples per class.
    organizePerClass();

    std::cout << std::endl << "    > Running " << k << "-fold cross validation: " << std::endl;

    for ( i = 0; i < numberOfClasses; i++ ) {
        numInstPerClassInFold[i] = numberOfExamplesPerClass[i] / k; // Integer division.
        residualPerClass[i] = numberOfExamplesPerClass[i] - numInstPerClassInFold[i] * k;
        std::cout << "      > Residual class " << i << ": " << residualPerClass[i] << std::endl;
    }

    std::ofstream ftra, ftst;

    std::string aux;
    char buff1[10], buff2[10];
    int extra = 0;

    for ( fold = 0; fold < k; fold ++ ) {

        // Reset the used examples.
        for ( i = 0; i < numberOfExamples; i++ ) {
            exampleSelected[i] = false;
        }

        // Open the train and test files.
        sprintf ( buff1, "%d", fold );
        sprintf ( buff2, "%d", k );

        aux = outName + "-" + buff2 + "-" + buff1 + "tst.dat";
        ftst.open ( aux.c_str (), std::ofstream::out );

        if ( ftst ) {

            std::cout << "    > Creating fold " << aux << std::endl;
            printHeader ( ftst );

            // Set the corresponding number of examples per class.
            for ( i = 0; i < numberOfClasses; i++ ) {
                extra = 0;
                if ( fold != k - 1 && residualPerClass[i] > 0 ) {
                    extra = 1 + ( int ) ( residualPerClass[i] / k );
                    residualPerClass[i] -= extra;
                }
                else if ( fold == k - 1 && residualPerClass[i] > 0 ) {
                    extra = ( int ) residualPerClass[i];
                }

                for ( j = 0; j < numInstPerClassInFold[i] + extra; j++ ) {

                    rnd = Utils::i_rand ( 0, numberOfExamplesPerClass[i] - 1 );
                    printExampleNoNormalized ( ftst, examplesPerClass[i][ rnd ], i );

                    exampleSelected [ indexExamplesPerClass[i][ rnd ] ] = true;

                    if ( rnd < numberOfExamplesPerClass[i] - 1 ) {
                        examplesPerClass[i][ rnd ] = examplesPerClass[i][ numberOfExamplesPerClass[i] - 1 ];
                        indexExamplesPerClass[i][ rnd ] = indexExamplesPerClass[i][ numberOfExamplesPerClass[i] - 1 ];
                    }

                    numberOfExamplesPerClass[i] --;
                }
            }

            ftst.close();

            // Print the training data set.
            sprintf ( buff1, "%d", fold );
            sprintf ( buff2, "%d", k );
            aux = outName + "-" + buff2 + "-" + buff1 + "tra.dat";
            ftra.open ( aux.c_str (), std::ofstream::out );

            if ( ftra ) {
                std::cout << "    > Creating fold " << aux << std::endl;
                printHeader ( ftra );

                for ( i = 0; i < numberOfExamples; i++ ) {
                    if ( !exampleSelected[i] ) {
                        printExampleNoNormalized ( ftra, example[i], classOfExample[i] );
                    }
                }

                ftra.close ();
            }
        }
        else {
            std::string logMessage = " > [ERROR FILE] Cannot open the file: " + aux + " \n ";
            Utils::logWarningError ( logMessage );
        }
    }

    // Delete memory.
    delete [] numInstPerClassInFold;
    delete [] residualPerClass;
    delete [] exampleSelected;

} // end stratifiedCrossValidation


void ExtendedDataset::crossValidation ( int k, std::string outName ) {

    int fold, numInstPerFold, rnd, i;
    float residual;
    float partialResidual = 0;
    std::ofstream fout;

    std::cout << " > Applying cross-validation " << std::endl;

    numInstPerFold = numberOfExamples / k;
    residual =  ( ( float ) numberOfExamples ) / ( ( float ) k ) - numInstPerFold;

    std::cout << "   > Number of examples per fold: " << numInstPerFold << std::endl;
    std::cout << "   > Residual: " << residual << std::endl;

    std::string aux;
    int extra = 0;

    for ( fold = 0; fold < k; fold ++ ) {
        aux = ( char ) ( '0' + fold );
        aux = outName + "10-" + aux + "tst.dat";
        fout.open ( aux.c_str (), std::ofstream::out );

        if ( fout ) {
            printHeader ( fout );
            extra = 0;
            partialResidual += residual;

            if ( partialResidual >= 1.0 ) {
                extra = 1;
                partialResidual = 0;
            }

            if ( fold == k - 1 ) {
                extra = numberOfExamples - numInstPerFold;
            }

            std::cout << "   > Creating fold " << aux << " consisting of ";
            std::cout << ( numInstPerFold + extra ) << " examples. " << std::endl;

            for ( i = 0; i < numInstPerFold + extra; i++ ) {
                rnd = Utils::i_rand ( 0, numberOfExamples - 1 );
                printExampleNoNormalized ( fout, example[ rnd ], classOfExample[ rnd ] );

                if ( rnd < numberOfExamples - 1 ) {
                    delete  example[ rnd ];
                    example[ rnd ] = example[ numberOfExamples - 1 ];
                }
                else {
                    delete example[ numberOfExamples - 1 ];
                }

                numberOfExamples --;
            }

            fout.close ();
        }
        else {
            std::string logMessage = " > [ERROR FILE] Cannot open the file: " + aux + " \n ";
            Utils::logWarningError ( logMessage );
        }
    }

} // end crossValidation


//// FUNCTIONS TO CALCULATE DISTANCES

float ExtendedDataset::getDistance ( float* ex1, float* ex2 ) {

    int att;
    float dist = 0;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        if ( ex1[ att ] == Dataset::UNKNOWN_VALUE || ex2[ att ] == Dataset::UNKNOWN_VALUE ) {
            dist += 1;
        }
        else {
            dist += pow ( ( double ) distanceFunction[ att ]->computeDistance ( ex1[ att ], ex2[ att ] ), ( double ) 2 );
        }
    }

    return pow ( ( double ) dist, ( double ) 0.5 );

} // end getDistance


float ExtendedDataset::getDistance ( int ex1, int ex2 ) {

    int att;
    float dist = 0;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        if ( example[ ex1 ][ att ] == Dataset::UNKNOWN_VALUE || example[ ex2 ][ att ] == Dataset::UNKNOWN_VALUE ) {
            dist += 1;
        }
        else {
            dist += pow ( ( double ) distanceFunction[ att ]->computeDistance ( example[ ex1 ][ att ], example[ ex2 ][ att ] ), ( double ) 2 );
        }
    }

    return pow ( ( double ) dist, ( double ) 0.5 );

} // end getDistance


float ExtendedDataset::getApproximateDistance ( float* ex1, float* ex2 ) {

    int att;
    float dist = 0;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        if ( ex1[ att ] == Dataset::UNKNOWN_VALUE || ex2[ att ] == Dataset::UNKNOWN_VALUE ) {
            dist += 1;
        }
        else {
            dist += pow ( ( double ) distanceFunction[ att ]->computeDistance ( ex1[ att ], ex2[ att ] ), ( double ) 2 );
        }
    }

    return dist;

} // end getApproximateDistance


float ExtendedDataset::getApproximateDistance ( int ex1, int ex2 ) {

    int att;
    float dist = 0;

    for ( att = 0; att < numberOfAttributes; att ++ ) {
        if ( example[ ex1 ][ att ] == Dataset::UNKNOWN_VALUE || example[ ex2 ][ att ] == Dataset::UNKNOWN_VALUE ) {
            dist += 1;
        }
        else {
            dist += pow ( ( double ) distanceFunction[ att ]->computeDistance ( example[ ex1 ][ att ], example[ ex2 ][ att ] ), ( double ) 2 );
        }
    }

    return dist;

} // end getApproximateDistance


int ExtendedDataset::INDEX ( int i, int j ) {

    int max = Utils::max ( i, j );
    int min = Utils::min ( i, j );

    return  ( min*numberOfExamples + max - ( ( min + 1 ) * ( min + 2 ) / 2 ) );

} // end INDEX
