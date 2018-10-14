
/*!

\brief This file contains the implementation of the InputOptions methods.

The class provides implementation of methods to read the command line parameters, store
them in class attributes, and modify and consult them.

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


#include "InputOptions.h"


InputOptions::InputOptions () {

    F1 = F1v = F2 = F3 = F4 = L1 = L2 = L3 = N1 = N2 = N3 = N4 = T1 = T2 = false;

    /** 
    [NEW COMPLEXITY MEASURE]
    In case of implementing a new complexity measure, please update the following definition by replacing the information in brackets.
    F1 = F1v = F2 = F3 = F4 = L1 = L2 = L3 = N1 = N2 = N3 = N4 = T1 = T2 = [Label of the new measure] = false;
    */

    transformTo2ClassData = false;
    runAllComplexityMeasures = runCrossValidation = false;
    discriminateClasses = false;
    showGPLInfoNoWarr = showGPLInfoRedistribution = false;

    runInMultipleDatasetMode = false;
    replaceUnknownValues     = true;

    inputDatasetName  = "";
    outputDatasetName = "";

    latexOutput = false;
    XMLOutput   = false;

    numberOfComplexityMeasuresToCompute = 0;
    numberOfLabels = 5;
    foldsCV = 10;

    printNormalizedDataset = false;

    typeOfContinuousDistFunction = ExtendedDataset::NORMALIZED_EUCLIDEAN;
    typeOfNominalDistFunction    = ExtendedDataset::OVERLAP_NOMINAL;

} // end InputOptions


InputOptions::~InputOptions () {
} // end ~InputOptions


bool InputOptions::isAnyOptionSelected () {
    return ( F1 || F1v || F2 || F3 || F4 || L1 || L2 || L3 || N1 || N2 || N3 || N4 || T1 || T2 ||
             transformTo2ClassData || runAllComplexityMeasures || runCrossValidation || printNormalizedDataset );

    /** 
    [NEW COMPLEXITY MEASURE]
    In case of implementing a new complexity measure, please update the following definition by replacing the information in brackets.
    return ( F1 || F1v || F2 || F3 || F4 || L1 || L2 || L3 || N1 || N2 || N3 || N4 || T1 || T2 || [Label of the new measure]
    */

} // end isAnyOptionSelected


bool InputOptions::isAnyComplexityMeasureSelected () {
    return ( F1 || F1v || F2 || F3 || F4 || L1 || L2 || L3 || N1 || N2 || N3 || N4 || T1 || T2 );

    /** 
    [NEW COMPLEXITY MEASURE]
    In case of implementing a new complexity measure, please update the following definition by replacing the information in brackets.
    return ( F1 || F1v || F2 || F3 || F4 || L1 || L2 || L3 || N1 || N2 || N3 || N4 || T1 || T2 || [Label of the new measure] );
    */

} // end isAnyComplexityMeasureSelected


bool InputOptions::isIncompatibleOptions () {

    if (  ( transformTo2ClassData || runCrossValidation ) && runInMultipleDatasetMode ) {
        std::string logMessage = "  > [ERROR COMMAND] Incompatible options in command line configuration. \n ";
        logMessage += "    >> -t2class and -cv cannot be run in batch mode (option -B). \n ";
        Utils::logWarningError ( logMessage );
        return true;
    }

    return false;

} // end isIncompatibleOptions


void InputOptions::printCommandLineExample ( const char* invalidOption ) {

    if ( strlen ( invalidOption ) > 0 ) {
        // Convert char* into string.
        std::ostringstream stInvalidOption;
        stInvalidOption << invalidOption;
        std::string logMessage = " \n  > [ERROR COMMAND] Unknown option: " + stInvalidOption.str () + " \n ";
        Utils::logWarningError ( logMessage );
    }

    std::cerr << "    >> The available options are: " << std::endl;
    std::cerr << "        -F [1-->Fisher discriminant|1v-->directional-vector Fisher discriminant|2-->Per-class bounding boxes|3-->Maximum feature efficiency|4-->Collective feature efficiency|No number-->Run all] " << std::endl;
    std::cerr << "        -L [1-->Minimized sum distances LC|2-->Training error LC|3-->Nonlinearity LC|No number-->Run all] " << std::endl;
    std::cerr << "        -N [1-->Fraction points boundary|2-->Intra/inter NN distance|3-->Error rate 1NN|4-->Nonlinearity 1NN|No number-->Run all] " << std::endl;
    std::cerr << "        -T [1-->Fraction covering spheres|2-->#samples/#dimensions|No number-->Run all] " << std::endl;
    std::cerr << "        -A --> Run all the complexity measures " << std::endl;
    std::cerr << "        -d --> Discriminate between classes when running the measures " << std::endl;
    std::cerr << "        -cv --> Create folds according to stratified-cross validation " << std::endl;
    std::cerr << "        -t2class --> Transform m-class data set into m two-class data sets " << std::endl;
    std::cerr << "        -B --> Run with multiple data sets " << std::endl;
    std::cerr << "        -p --> Print output with normalized attributes " << std::endl;
    std::cerr << "        -cM [1-->Euclidean|2-->Normalized Euclidean|3-->Std Euclidean] for continuous values " << std::endl;
    std::cerr << "        -nM [1-->Overlap|2-->VDM|3-->Euclidean|4-->Normalized Euclidean] for nominal values " << std::endl;
    std::cerr << "        -D --> Print debug information " << std::endl;
    std::cerr << "        -nRU --> Do not replace unknown values " << std::endl;
    std::cerr << "        -i input_file --> Input file " << std::endl;
    std::cerr << "        -o output_file --> Output file " << std::endl;
    std::cerr << "        -latex --> Output file in latex format " << std::endl;
    std::cerr << "        -xml --> Output file in XML format " << std::endl;
    std::cerr << "        show w --> Show GPL information about no warranty " << std::endl;
    std::cerr << "        show c --> Show GPL information about redistribution " << std::endl;
    std::cerr << std::endl;

} // end printCommandLineExample


void InputOptions::printUsage () {

    std::cout << "  > USAGE: " << std::endl;
    std::cout << "      ./dcol -i <input_file> -o <output_file> [OPTIONS] " << std::endl << std::endl;

    std::cout << "  > where, " << std::endl;
    std::cout << "     -i <input_file>: input_file is the name of the file with either: " << std::endl;
    std::cout << "                           (1) the input data set (if option -B is not specified) or " << std::endl;
    std::cout << "                           (2) a list of data set file names (if option -B is specified). " << std::endl;
    std::cout << "     -o <output_file>: output_file is the name of the output file where the results will be written. " << std::endl << std::endl;

    std::cout << "  > and [OPTIONS] includes: " << std::endl;
    std::cout << "        -F [1-->Fisher discriminant|1v-->directional-vector Fisher discriminant|2-->Per-class bounding boxes|3-->Maximum feature efficiency|4-->Collective feature efficiency|No number-->Run all] " << std::endl;
    std::cout << "        -L [1-->Minimized sum distances LC|2-->Training error LC|3-->Nonlinearity LC|No number-->Run all] " << std::endl;
    std::cout << "        -N [1-->Fraction points boundary|2-->Intra/inter NN distance|3-->Error rate 1NN|4-->Nonlinearity 1NN|No number-->Run all] " << std::endl;
    std::cout << "        -T [1-->Fraction covering spheres|2-->#samples/#dimensions|No number-->Run all] " << std::endl;
    std::cout << "        -A --> Run all the complexity measures " << std::endl;
    std::cout << "        -d --> Discriminate between classes when running the measures " << std::endl;
    std::cout << "        -cv --> Create folds according to stratified-cross validation " << std::endl;
    std::cout << "        -t2class --> Transform m-class data set into m two-class data sets " << std::endl;
    std::cout << "        -B --> Run with multiple data sets " << std::endl;
    std::cout << "        -p --> Print output with normalized attributes " << std::endl;
    std::cout << "        -cM [1-->Euclidean|2-->Normalized Euclidean|3-->Std Euclidean] for continuous values " << std::endl;
    std::cout << "        -nM [1-->Overlap|2-->VDM|3-->Euclidean|4-->Normalized Euclidean] for nominal values " << std::endl;
    std::cout << "        -D --> Print debug information " << std::endl;
    std::cout << "        -nRU --> Do not replace unknown values " << std::endl;
    std::cout << "        -i input_file --> Input file " << std::endl;
    std::cout << "        -o output_file --> Output file " << std::endl;
    std::cout << "        -latex --> Output file in latex format " << std::endl;
    std::cout << "        -xml --> Output file in XML format " << std::endl;
    std::cout << "        show w --> Show GPL information about no warranty " << std::endl;
    std::cout << "        show c --> Show GPL information about redistribution " << std::endl;

    std::cout << std::endl;

} // end printUsage


void InputOptions::parseInput ( int argc, char **argv ) {

    int i, value;
    bool inputDSetSpecified  = false;
    bool outputDSetSpecified = false;

    if ( argc < 3 ) {
        printUsage ();
        exit ( -1 );
    }

    for ( i = 1; i < argc; i++ ) {
    	if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'F' ) {
    		if ( i != argc - 1 && strlen ( argv[ i + 1 ] ) == 2 && argv[ i + 1 ][0] == '1' && argv[ i + 1 ][1] == 'v' ) {
                if ( !F1v ) numberOfComplexityMeasuresToCompute ++;
                F1v = true;
                i++;
    	    }
    	    else if ( i != argc - 1 && atoi ( argv[ i + 1 ] ) > 0 ) {
                i++;

                switch ( atoi ( argv[i] ) ) {

                    case 1:
                    	if ( !F1 ) numberOfComplexityMeasuresToCompute ++;
                    	F1 = true;
                    	break;

                    case 2:
                        if ( !F2 ) numberOfComplexityMeasuresToCompute ++;
                        F2 = true;
                        break;

                    case 3:
                        if ( ! F3 ) numberOfComplexityMeasuresToCompute ++;
                        F3 = true;
                        break;

                    case 4:
                        if ( !F4 ) numberOfComplexityMeasuresToCompute ++;
                        F4 = true;
                        break;

                    case 0:
                        if ( !F1 )  numberOfComplexityMeasuresToCompute ++;
                        if ( !F1v ) numberOfComplexityMeasuresToCompute ++;
                        if ( !F2 )  numberOfComplexityMeasuresToCompute ++;
                        if ( !F3 )  numberOfComplexityMeasuresToCompute ++;
                        if ( !F4 )  numberOfComplexityMeasuresToCompute ++;
                     	F1 = F1v = F2 = F3 = F4 = true;
                        break;

                    default:
                    	std::string logMessage = " \n  > [WARNING COMMAND] Ignoring not known input parameter: " + std::string ( argv[i] );
                        Utils::logWarningError ( logMessage );
                }
            }
            else {
                if ( !F1 )  numberOfComplexityMeasuresToCompute ++;
                if ( !F1v ) numberOfComplexityMeasuresToCompute ++;
                if ( !F2 )  numberOfComplexityMeasuresToCompute ++;
                if ( !F3 )  numberOfComplexityMeasuresToCompute ++;
                if ( !F4 )  numberOfComplexityMeasuresToCompute ++;
                F1 = F1v = F2 = F3 = F4 = true;
            }
        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'L' ) {

            if ( i != argc - 1 && atoi ( argv[ i + 1 ] ) > 0 ) {
                i++;

                switch ( atoi ( argv[i] ) ) {

                    case 1:
                        if ( !L1  ) numberOfComplexityMeasuresToCompute ++;
                        L1 = true;
                        break;

                    case 2:
                        if ( !L2 ) numberOfComplexityMeasuresToCompute ++;
                        L2 = true;
                        break;

                    case 3:
                        if ( !L3 ) numberOfComplexityMeasuresToCompute ++;
                        L3 = true;
                        break;

                    case 0:
                    	if ( !L1 ) numberOfComplexityMeasuresToCompute ++;
                    	if ( !L2 ) numberOfComplexityMeasuresToCompute ++;
                    	if ( !L3 ) numberOfComplexityMeasuresToCompute ++;
                        L1 = L2 = L3 = true;
                        break;

                    default:
                    	std::string logMessage = " \n  > [WARNING COMMAND] Ignoring not known input parameter: " + std::string ( argv[i] );
                        Utils::logWarningError ( logMessage );
                }
            }
            else {
                if ( !L1 ) numberOfComplexityMeasuresToCompute ++;
                if ( !L2 ) numberOfComplexityMeasuresToCompute ++;
                if ( !L3 ) numberOfComplexityMeasuresToCompute ++;
                L1 = L2 = L3 = true;
            }
        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'N' ) {
            if ( i != argc - 1 && atoi ( argv[ i + 1 ] ) > 0 ) {
                i++;

                switch ( atoi ( argv[i] ) ) {

                    case 1:
                        if ( !N1 ) numberOfComplexityMeasuresToCompute ++;
                        N1 = true;
                        break;

                    case 2:
                        if ( !N2 ) numberOfComplexityMeasuresToCompute ++;
                        N2 = true;
                        break;

                    case 3:
                        if ( !N3 ) numberOfComplexityMeasuresToCompute ++;
                        N3 = true;
                        break;

                    case 4:
                        if ( !N4 ) numberOfComplexityMeasuresToCompute ++;
                        N4 = true;
                        break;

                    case 0:
                    	if ( !N1 ) numberOfComplexityMeasuresToCompute ++;
                    	if ( !N2 ) numberOfComplexityMeasuresToCompute ++;
                    	if ( !N3 ) numberOfComplexityMeasuresToCompute ++;
                    	if ( !N4 ) numberOfComplexityMeasuresToCompute ++;
                    	N1 = N2 = N3 = N4 = true;
                        break;

                    default:
                    	std::string logMessage = " \n  > [WARNING COMMAND] Ignoring not known input parameter: " + std::string ( argv[i] );
                        Utils::logWarningError ( logMessage );
                }
            }
            else {
            	if ( !N1 ) numberOfComplexityMeasuresToCompute ++;
            	if ( !N2 ) numberOfComplexityMeasuresToCompute ++;
            	if ( !N3 ) numberOfComplexityMeasuresToCompute ++;
            	if ( !N4 ) numberOfComplexityMeasuresToCompute ++;
            	N1 = N2 = N3 = N4 = true;
            }
        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'T' ) {

            if ( i != argc - 1 && atoi ( argv[ i + 1 ] ) > 0 ) {
                i++;

                switch ( atoi ( argv[i] ) ) {

                    case 1:
                    	if ( !T1 ) numberOfComplexityMeasuresToCompute ++;
                    	T1 = true;
                        break;

                    case 2:
                    	if ( !T2 ) numberOfComplexityMeasuresToCompute ++;
                    	T2 = true;
                        break;

                    case 0:
                    	if ( !T1 ) numberOfComplexityMeasuresToCompute ++;
                    	if ( !T2 ) numberOfComplexityMeasuresToCompute ++;
                        T1 = T2 = true;
                        break;

                    default:
                    	std::string logMessage = " \n  > [WARNING COMMAND] Ignoring not known input parameter: " + std::string ( argv[i] );
                    	Utils::logWarningError ( logMessage );
                }
            }
            else {
            	if ( !T1 ) numberOfComplexityMeasuresToCompute ++;
            	if ( !T2 ) numberOfComplexityMeasuresToCompute ++;
            	T1 = T2 = true;
            }
        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'd' ) {
            discriminateClasses = true;
        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'A' ) {

            F1 = F1v = F2 = F3 = F4 = L1 = L2 = L3 = N1 = N2 = N3 = N4 = T1 = T2 = true;
            numberOfComplexityMeasuresToCompute = totalNumberOfComplexityMeasures;
            runAllComplexityMeasures = true;

        }
        else if ( strlen ( argv[i] ) == 3 && argv[i][0] == '-' && argv[i][1] ==  'n' && argv[i][2] == 'M' ) {

            i++;
            value = atoi ( argv[i] );

            if ( value < 1 || value > 4 ) {
                printCommandLineExample ( argv[i] );
                exit ( -1 );
            }

            switch ( value ) {

                case 1:
                    typeOfNominalDistFunction = ExtendedDataset::OVERLAP_NOMINAL;
                    break;

                case 2:
                    typeOfNominalDistFunction = ExtendedDataset::VDM_NOMINAL;
                    break;

                case 3:
                    typeOfNominalDistFunction = ExtendedDataset::EUCLIDEAN;
                    break;

                case 4:
                    typeOfNominalDistFunction = ExtendedDataset::NORMALIZED_EUCLIDEAN;
                    break;

            }
        }
        else if ( strlen ( argv[i] ) == 3 && argv[i][0] == '-' && argv[i][1] == 'c' && argv[i][2] == 'M' ) {

            i++;
            value = atoi ( argv[i] );

            if ( value < 1 || value > 3 ) {
                printCommandLineExample ( argv[i] );
                exit ( -1 );
            }

            switch ( value ) {

                case 1:
                    typeOfContinuousDistFunction = ExtendedDataset::EUCLIDEAN;
                    break;

                case 2:
                    typeOfContinuousDistFunction = ExtendedDataset::NORMALIZED_EUCLIDEAN;
                    break;

                case 3:
                    typeOfContinuousDistFunction = ExtendedDataset::STD_WEIGHTED_EUCLIDEAN;
                    break;
            }
        }
        else if ( strlen ( argv[i] ) == 3 && argv[i][0] == '-' && argv[i][1] == 'c' && argv[i][2] == 'v' ) {

            runCrossValidation = true;

            if ( i != argc - 1 && ( atoi ( argv[ i + 1 ] ) != 0 || argv[ i + 1 ][0] == '0' ) ) {
                // The number of folds has been specified.
                i++;

                if ( atoi ( argv[i] ) > 1 ) {
                    foldsCV = atoi ( argv[i] );
                }
                else {
                	std::string logMessage = " \n  > [WARNING COMMAND] The number of folds must be greater than one. \n ";
                	logMessage += "    >> The number of folds is set to the default value 10. ";
                	Utils::logWarningError ( logMessage );
                }
            }
            else {
            	std::string logMessage = " \n  > [WARNING COMMAND] The number of folds for option -cv has not been specified. \n ";
            	logMessage += "    >> The number of folds is set to the default value 10. ";
                Utils::logWarningError ( logMessage );
            }

        }
        else if ( strlen ( argv[i] ) == 8 && argv[i][0] == '-' && argv[i][1] == 't' && argv[i][2] == '2'
                  && argv[i][3] == 'c' && argv[i][4] == 'l' && argv[i][5] == 'a' && argv[i][6] == 's'
                  && argv[i][7] == 's' ) {

            transformTo2ClassData = true;

        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'i' ) {
          
            if ( i != argc - 1 ) {
                i++;
                inputDatasetName = argv[i];
                inputDSetSpecified = true;
            }
            else {
                std::string logMessage = " \n  > [ERROR COMMAND] Input data set name not specified in the -i option. \n\n ";
                Utils::logWarningError ( logMessage );
                exit ( -1 );
            }

        
        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'o' ) {
        
            if ( i != argc - 1 ) {
                i++;
                outputDatasetName = argv[i];
                outputDSetSpecified = true;
            }
            else {
                std::string logMessage = " \n  > [ERROR COMMAND] Output data set name not specified in the -o option. \n\n ";
                Utils::logWarningError ( logMessage );
                exit ( -1 );
            }
        
        }
        else if ( strlen ( argv[i] ) == 6 && strcmp ( argv[i], "-latex" ) == 0 ) {

           latexOutput = true;

        }
        else if ( strlen ( argv[i] ) == 4 && strcmp ( argv[i], "-xml" ) == 0 ) {

            XMLOutput = true;

        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 's' ) {
        
            i++;
            Utils::setSeed ( atoi ( argv[i] ) );
        
        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'B' ) {
        
            runInMultipleDatasetMode = true;
        
        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'p' ) {
        
            printNormalizedDataset = true;
        
        }
        else if ( strlen ( argv[i] ) == 2 && argv[i][0] == '-' && argv[i][1] == 'D' ) {
        
            Utils::doScreenStatistics = true;
        
        }
        else if ( strlen ( argv[i] ) == 4 && argv[i][0] == '-' && argv[i][1] == 'n' &&
                  argv[i][2] == 'R' && argv[i][3] == 'U' ) {

            replaceUnknownValues = false;
        
        }
        else if ( strlen ( argv[i] ) == 4 && argv[i][0] == 's' && argv[i][1] == 'h' && argv[i][2] == 'o' 
                  && argv[i][3] == 'w' ) {

            if ( i != argc - 1 && argv[ i + 1 ][0] == 'w' ) {
                i++;
                showGPLInfoNoWarr = true;
            }
            else if ( i != argc - 1 && argv[ i + 1 ][0] == 'c' ) {
                i++;
                showGPLInfoRedistribution = true;
            }
            else {
                std::string logMessage = " \n  > [ERROR COMMAND] The option 'show' has to be followed by 'c' or 'w'. \n\n ";
                Utils::logWarningError ( logMessage );
                exit ( -1 );
            }
        
        }
        else {
        
            printCommandLineExample ( argv[i] );
            exit ( -1 );
        
        }

    }

    Utils::printGPLInformation ( showGPLInfoNoWarr, showGPLInfoRedistribution );

    if ( !inputDSetSpecified ) {
        if ( !showGPLInfoNoWarr && !showGPLInfoRedistribution ) {
            std::string logMessage = " \n  > [ERROR COMMAND] The name of the file that contains the input data set has not been specified (option -i). \n ";
            logMessage += "    >> PROGRAM ABORTED \n ";
            Utils::logWarningError ( logMessage );
        }
        exit ( -1 );
    }

    if ( !outputDSetSpecified ) {
        if ( !showGPLInfoNoWarr && !showGPLInfoRedistribution ) {
            std::string logMessage = " \n  > [ERROR COMMAND] The name of the output file has not been specified (option -o). \n ";
            logMessage += "    >> PROGRAM ABORTED \n ";
            Utils::logWarningError ( logMessage );
        }
        exit ( -1 );
    }

} // end parseInput
