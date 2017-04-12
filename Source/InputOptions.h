
#ifndef INPUTOPTIONS_H
#define INPUTOPTIONS_H

#include "ComplexityMeasures.h"
#include "Utils.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>


/*!

\brief Parses and stores the command line options.

This class parses and maintains the command line parameters for the configuration.

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


class InputOptions {

private:

    /** Name of the input data set. */
    std::string inputDatasetName;

    /** Name of the output data set. */
    std::string outputDatasetName;

    /** Indicates whether the output file has to be in Latex format. */
    bool latexOutput;

    /** Indicates whether the output file has to be in XML format. */
    bool XMLOutput;

    /** Number of labels. */
    int numberOfLabels;

    /** Indicates whether the data set has to be transformed into two-class data sets. */
    bool transformTo2ClassData;

    /** Indicates whether cross validation has to be run. */
    bool runCrossValidation;

    /** Number of folds of the cross validation. */
    int foldsCV;

    /** Indicates whether batch runs have to be run. */
    bool runInMultipleDatasetMode;

    /** Indicates whether all the complexity measures have to be run. */
    bool runAllComplexityMeasures;

    /** Flags for each complexity measure.
       [NEW COMPLEXITY MEASURE]
       In case to implement a new complexity measure, please add a new flag. 
       */
    bool F1, F1v, F2, F3, F4, L1, L2, L3, N1, N2, N3, N4, T1, T2;

    /** Number of complexity measures to compute. */
    int numberOfComplexityMeasuresToCompute;

    /** Total number of complexity measures that could be computed if the user selects them all.
       [NEW COMPLEXITY MEASURE]
       In case to implement a new complexity measure, please increment the totalNumberOfComplexityMeasures constant
       */
    static const int totalNumberOfComplexityMeasures = 14;

    /** Indicates whether classes have to be discriminated before running the complexity measures. */
    bool discriminateClasses;

    /** Indicates whether the data set has to be printed. */
    bool printNormalizedDataset;

    /** Indicates the type of distance measure for continuous attributes. */
    int typeOfContinuousDistFunction;

    /** Indicates the type of distance measure for nominal attributes. */
    int typeOfNominalDistFunction;

    /** Indicates whether the unknown values have to be replaced. */
    bool replaceUnknownValues;

    /** Indicates whether the information of the ABSOLUTELY NO WARRANTY of the GPL software has to be shown */
    bool showGPLInfoNoWarr;

    /** Indicates whether the information of redistribution of the GPL software has to be shown */
    bool showGPLInfoRedistribution;

public:

    /**
     * Constructs a new InputOptions object.
     */
    InputOptions ();

    /**
     * Destructs the InputOptions object.
     */
    ~InputOptions ();

    /**
     * Parses the input options specified from the command line.
     * @param argc is the argument counter.
     * @param argv contains the command line arguments.
     */
    void parseInput ( int argc, char **argv );

    /**
     * Indicates whether at least one option has been selected.
     * @return A Boolean that indicates whether any option has been selected.
     * @retval true at least one option has been selected.
     * @retval false no options have been selected.
     */
    bool isAnyOptionSelected ();

    /**
     * Checks whether any complexity measure has been selected.
     * @return A Boolean that indicates whether any complexity measure is selected.
     * @retval true at least one complexity measure has been selected.
     * @retval false no complexity measures have been selected.
     */
    bool isAnyComplexityMeasureSelected ();

    /**
     * Checks whether there are incompatible options selected.
     * In case of incompatibility among options, it generates an error message to duly inform the user.
     * @return A Boolean that indicates whether any option is incompatible.
     * @retval true selected options are incompatible. 
     * @retval false selected options are well-defined.
     */
    bool isIncompatibleOptions ();

    /**
     * Shows a list of all the possible configuration parameters.
     * @param invalidOption is a string with the invalid option(s) introduced by the user.
     */
    void printCommandLineExample ( const char *invalidOption );

    /**
     * Prints the application usage mode.
     */
    void printUsage ();

    /**
     * Returns the name of the input data set.
     * @return A string with the name of the input data set.
     */
    std::string getInputDatasetName () {
        return inputDatasetName;
    } // end getInputDatasetName

    /**
     * Returns the name of the output data set.
     * @return A string with the name of the output data set.
     */
    std::string getOutputDatasetName () {
        return outputDatasetName;
    } // end getOutputDatasetName

    /**
     * Returns whether the user has selected the option of saving the results in a latex file. 
     * @return A Boolean that indicates whether the user has selected the option of saving the results in a latex file. 
     * @retval true latex output file has been selected.
     * @retval false latex output file has not been selected.
     */
    bool getLatexOutput () {
        return latexOutput;
    } // end getLatexOutput

    /**
     * Returns whether the user has selected the option of saving the results in a XML file. 
     * @return A Boolean that indicates whether the user has selected the option of saving the results in a XML file. 
     * @retval true XML output file has been selected.
     * @retval false XML output file has not been selected.
     */
    bool getXMLOutput () {
        return XMLOutput;
    } // end getXMLOutput

    /**
     * Returns whether the user has selected the option of transforming an m-class data set into m 
     * different two-class data sets.
     * @return A Boolean that indicates whether the user has selected the option of transforming the 
     * data set into different two-class data sets.
     * @retval true transforming option has been selected.
     * @retval false transforming option has not been selected.
     */
    bool getTransformTo2ClassData () {
        return transformTo2ClassData;
    } // end getTransformTo2ClassData

    /**
     * Returns whether the user has selected the option of running cross-validation.
     * @return A Boolean that indicates whether the user has selected the option of running cross-validation.
     * @retval true cross-validation option has been selected.
     * @retval false cross-validation option has not been selected.
     */
    bool getRunCrossValidation () {
        return runCrossValidation;
    } // end getRunCrossValidation

    /**
     * Returns the number of folds in cross-validation.
     * @return An integer with the number of folds in cross-validation.
     */
    int getFoldsCV () {
        return foldsCV;
    } // end getFoldsCV

    /**
     * Returns whether the user has selected the option of running the measures in batch mode.
     * @return A Boolean that indicates whether the user has selected the option of running the measures in batch mode.
     * @retval true batch mode has been selected.
     * @retval false batch mode has not been selected.
     */
    bool getRunInMultipleDatasetMode () {
        return runInMultipleDatasetMode;
    } // end getRunInMultipleDatasetMode

    /**
     * Returns whether the user has selected the option of running all the complexity measures.
     * @return A Boolean that indicates whether the user has selected the option of running all the complexity measures.
     * @retval true option of running all the complexity measures has been selected.
     * @retval false specific complexity measures have been selected to be run.
     */
    bool getRunAllComplexityMeasures () {
        return runAllComplexityMeasures;
    } // end getRunAllComplexityMeasures

    /**
     * Returns whether the user has selected to run the maximum Fisher's discriminant ratio (measure F1).
     * @return A Boolean that indicates whether the user has selected to run F1.
     * @retval true measure F1 has been selected to be run.
     * @retval false measure F1 has not been selected to be run.
     */
    bool getF1 () {
        return F1;
    } // end getF1

    /**
     * Returns whether the user has selected to run the vector-directed version of
     * the maximum Fisher's discriminant ratio (measure F1v).
     * @return A Boolean that indicates whether the user has selected to run F1v.
     * @retval true measure F1v has been selected to be run.
     * @retval false measure F1v has not been selected to be run.
     */
    bool getF1v () {
        return F1v;
    } // end getF1v

    /**
     * Returns whether the user has selected to run the overlap of per-class bounding boxes (measure F2).
     * @return A Boolean that indicates whether the user has selected to run F2.
     * @retval true measure F2 has been selected to be run.
     * @retval false measure F2 has not been selected to be run.
     */
    bool getF2 () {
        return F2;
    } // end getF2

    /**
     * Returns whether the user has selected to run the maximum (individual) feature efficiency (measure F3).
     * @return A Boolean that indicates whether the user has selected to run F3.
     * @retval true measure F3 has been selected to be run.
     * @retval false measure F3 has not been selected to be run.
     */
    bool getF3 () {
        return F3;
    } // end getF3

    /**
     * Returns whether the user has selected to run the collective feature efficiency (measure F4).
     * @return A Boolean that indicates whether the user has selected to run F4.
     * @retval true measure F4 has been selected to be run.
     * @retval false measure F4 has not been selected to be run.
     */
    bool getF4 () {
        return F4;
    } // end getF4

    /**
     * Returns whether the user has selected to run the minimized sum of the error 
     * distance of a linear classifier (measure L1).
     * @return A Boolean that indicates whether the user has selected to run L1.
     * @retval true measure L1 has been selected to be run.
     * @retval false measure L1 has not been selected to be run.
     */
    bool getL1 () {
        return L1;
    } // end getL1

    /**
     * Returns whether the user has selected to run the training error of a linear classifier 
     * (measure L2).
     * @return A Boolean that indicates whether the user has selected to run L2.
     * @retval true measure L2 has been selected to be run.
     * @retval false measure L2 has not been selected to be run.
     */
    bool getL2 () {
        return L2;
    } // end getL2

    /**
     * Returns whether the user has selected to run the nonlinearity of a linear classifier (measure L3).
     * @return A Boolean that indicates whether the user has selected to run L3.
     * @retval true measure L3 has been selected to be run.
     * @retval false measure L3 has not been selected to be run.
     */
    bool getL3 () {
        return L3;
    } // end getL3

    /**
     * Returns whether the user has selected to run the fraction of points on the class boundary (measure N1).
     * @return A Boolean that indicates whether the user has selected to run N1.
     * @retval true measure N1 has been selected to be run.
     * @retval false measure N1 has not been selected to be run.
     */
    bool getN1 () {
        return N1;
    } // end getN1

    /**
     * Returns whether the user has selected to run the ratio of average intra/inter 
     * class nearest neighbor distance (measure N2).
     * @return A Boolean that indicates whether the user has selected to run N2.
     * @retval true measure N2 has been selected to be run.
     * @retval false measure N2 has not been selected to be run.
     */
    bool getN2 () {
        return N2;
    } // end getN2

    /**
     * Returns whether the user has selected to run the leave-one-out error rate of the 
     * one-nearest neighbor classifier (measure N3).
     * @return A Boolean that indicates whether the user has selected to run N3.
     * @retval true measure N3 has been selected to be run.
     * @retval false measure N3 has not been selected to be run.
     */
    bool getN3 () {
        return N3;
    } // end getN3

    /**
     * Returns whether the user has selected to run the nonlinearity of the one-nearest neighbor classifier (measure N4).
     * @return A Boolean that indicates whether the user has selected to run N4.
     * @retval true measure N4 has been selected to be run.
     * @retval false measure N4 has not been selected to be run.
     */
    bool getN4 () {
        return N4;
    } // end getN4

    /**
     * Returns whether the user has selected to run the fraction of maximum covering spheres (measure T1).
     * @return A Boolean that indicates whether the user has selected to run T1.
     * @retval true measure T1 has been selected to be run.
     * @retval false measure T1 has not been selected to be run.
     */
    bool getT1 () {
        return T1;
    } // end getT1

    /**
     * Returns whether the user has selected to run the average number of points per dimension (measure T2).
     * @return A Boolean that indicates whether the user has selected to run T2.
     * @retval true measure T2 has been selected to be run.
     * @retval false measure T2 has not been selected to be run.
     */
    bool getT2 () {
        return T2;
    } // end getT2

    /**
     * Returns the number of complexity measures to compute.
     * @return An interger with the number of complexity measures to compute.
     */
    int getNumberOfComplexityMeasuresToCompute () {
        return numberOfComplexityMeasuresToCompute;
    } // end getNumberOfComplexityMeasuresToCompute 

    /**
     * Returns whether the classes need to be discriminated when applying the complexity measures.
     * @return A Boolean that indicates whether the classes have to be discriminated for multiple class data sets.
     * @retval true classes have to be discriminated for multiple class data sets.
     * @retval false classes have not to be discriminated for multiple class data sets.
     */
    bool doDiscriminateClasses () {
        return discriminateClasses;
    } // end doDiscriminateClasses

    /**
     * Returns whether the data set needs to be printed.
     * @return A Boolean that indicates whether the user has selected the option of printing the data set.
     * @retval true the option of printing the data set has been selected.
     * @retval false the option of printing the data set has not been selected.
     */
    bool getPrintNormalizedDataset () {
        return printNormalizedDataset;
    } // end getPrintNormalizedDataset

    /**
     * Returns the type of distance function for continuous attributes.
     * @return An integer with the type of distance function for continuous attributes.
     * @retval 1 Euclidean distance function.
     * @retval 2 normalized Euclidean distance function (default option).
     * @retval 3 Euclidean distance function weighted by the standard deviation.
     */
    int getTypeOfContinuousDistFunction () {
        return typeOfContinuousDistFunction;
    } // end getTypeOfContinuousDistFunction

    /**
     * Returns the type of distance function for nominal attributes.
     * @return An integer with the type of distance function for nominal attributes.
     * @retval 1 overlap distance function (default option).
     * @retval 2 VDM distance function.
     * @retval 3 Euclidean distance function.
     * @retval 4 normalized Euclidean distance function.
     */
    int getTypeOfNominalDistFunction () {
        return typeOfNominalDistFunction;
    } // end getTypeOfNominalDistFunction

    /**
     * Returns whether the unknown values have to be replaced.
     * @return A Boolean that indicates whether the unknown values have to be replaced.
     * @retval true unknown values have to be replaced.
     * @retval false unknown values do not have to be replaced.
     */
    bool getReplaceUnknownValues () {
        return replaceUnknownValues;
    } // end getReplaceUnknownValues

    /**
     * Returns whether the GPL information about ABSOLUTELY NO WARRANTY has to be shown.
     * @return A Boolean that indicates whether the GPL information about ABSOLUTELY NO WARRANTY has to be shown.
     * @retval true GPL information about ABSOLUTELY NO WARRANTY has to be shown.
     * @retval false GPL information about ABSOLUTELY NO WARRANTY must not be shown.
     */
    bool getShowGPLInfoNoWarr () {
        return showGPLInfoNoWarr;
    } // end getShowGPLInfoNoWarr

    /**
     * Returns whether the GPL information about redistribution has to be shown.
     * @return A Boolean that indicates whether the GPL information about redistribution has to be shown.
     * @retval true GPL information about redistribution has to be shown.
     * @retval false GPL information about redistribution must not be shown.
     */
    bool getShowGPLInfoRedistribution () {
        return showGPLInfoRedistribution;
    } // end getShowGPLInfoNoWarr

    /**
     * Sets the name of the input data set.
     * @param datasetName is the name of the input data set.
     */
    void setInputDatasetName ( std::string datasetName ) {
        inputDatasetName = datasetName;
    } // end setInputDatasetName

    /**
     * Sets the name of the output data set.
     * @param datasetName is the name of the output data set.
     */
    void setOutputDatasetName ( std::string datasetName ) {
        outputDatasetName = datasetName;
    } // end setOutputDatasetName

};

#endif
