
#ifndef EXTENDEDDATASET_H
#define EXTENDEDDATASET_H

#include "Dataset.h"

#include <cmath>


/*!

\brief Extends from the Dataset class and introduces new functionalities.

The implementation of the methods of data set is done in a single .cpp file: ExtendedDataset.cpp
This class includes the following groups of methods:
	<ol>
		<li> Methods for organizing the examples per class and applying partition procedures. </li>
		<li> Methods for printing the data set in different formats. </li>
	</ol>

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


class ExtendedDataset : public Dataset {

protected:

    /** Type of distance function used for continuous attributes. */
    int typeOfContinuousDistance;

    /** Type of distance function used for nominal attributes. */
    int typeOfNominalDistance;

public:

    /** Maximum examples for precalculation. */
    static float MAXIMUM_EXAMPLES_FOR_PRECALCULATION;


//// DISTANCE FUNCTIONS

    /** Option to select the Euclidean distance function for continuous attributes. */
    static int EUCLIDEAN;

    /** Option to select the normalized Euclidean distance function for continuous attributes. */
    static int NORMALIZED_EUCLIDEAN;

    /** Option to select the Euclidean distance function weighted by standard deviation for continuous attributes. */
    static int STD_WEIGHTED_EUCLIDEAN;

    /** Option to select the overlap distance function for nominal attributes. */
    static int OVERLAP_NOMINAL;

    /** Option to select the VDM distance function for nominal attributes. */
    static int VDM_NOMINAL;

protected:

//// ATTRIBUTES THAT MAINTAIN ALL THE EXAMPLES ORGANIZED PER CLASS

    /** Number of examples per each class. */
    int* numberOfExamplesPerClass;

    /** Examples organized per class (index examplesPerClass[class][example]). */
    float*** examplesPerClass;

    /** Corresponding position of the example in the example vector (index indexExamplesPerClass[class][example]). */
    int** indexExamplesPerClass;


//// ATTRIBUTES TO CALCULATE AND STORE DISTANCES

    /** Distance functions for each attribute. */
    DistanceFunction** distanceFunction;

    /** Distances between each pair of examples. */
    float* distances;

public:

    /**
     * Constructs a new ExtendedDataset object by opening the file and processing the examples in there.
     * @param fileName is the name of the KEEL/WEKA formatted input file.
     * @param readAttInfo indicates whether the information of the attributes has to be read.
     * @param repUnknownVal indicates whether the unknown values need to be replaced.
     * @param realAttributesFunction indicates the type of distance function for continuous attributes.
     * @param nominalAttributesFunction indicates the type of distance function for nominal attributes.
     */
    ExtendedDataset ( std::string fileName, bool readAttInfo, bool repUnknownVal = true,
                      int realAttributesFunction = 1, int nominalAttributesFunction = 1 );

    /**
     * Destructs the ExtendedDataset object.
     */
    ~ExtendedDataset ();

    /**
     * Deletes the structures that maintain the examples organized per class.
     */
    void deleteExamplesPerClass ();

    /**
     * Returns a float* with the characteristics of the data set:
     * number of examples, number of attributes, number of continuous attributes, number of integer attributes, 
     * number of nominal attributes, number of classes,
     * % missing attributes, % missing examples, % missing values, % examples of the majority class, and 
     * % examples of the minority class.
     * @return A float* with the aforementioned information.
     */
    float* getDatasetCharacteristics ();


//// FUNCTIONS FOR DATA PARTITION

    /**
     * Organizes the examples per class. It is needed before calling the stratified cross-validation method.
     */
    void organizePerClass ();

    /**
     * Splits the data set into k stratified sets following the k-fold cross-validation approach.
     * @param k is the number of folds.
     * @param outName is the output file name.
     */
    void stratifiedCrossValidation ( int k, std::string outName );

    /**
     * Splits the data set into k sets following the k-fold cross-validation approach.
     * @param k is the number of folds.
     * @param outName is the output file name.
     */
    void crossValidation ( int k, std::string outName );

    /**
     * Generates m two-class data sets from an m-class data set.
     * @param baseName is the base name of the data sets that will be generated.
     */
    std::string* generate2ClassDatasets ( std::string baseName );


//// PRINT EXAMPLES

    /**
     * Prints the data set to the standard output in KEEL (super set of WEKA) format.
     * @param printExamples indicates whether the examples have to be printed.
     * @see printOneClassAgainstOthers.
     */
    void print ( bool printExamples = false );

    /**
     * Prints the data set to the file referenced by fout in KEEL format.
     * @param fout is the output file.
     * @param printNormalizedData indicates whether data have to be printed normalized.
     * @return An ofstream as a reference to the file.
     */
    std::ofstream& print ( std::ofstream& fout, bool printNormalizedData );

    /**
     * Prints a two-class data set where all the classes except for whichClass are grouped in a class label groupClass.
     * @param fout is the output file.
     * @param whichClass is the class that will be printed against the groupClass.
     * @return An ofstream as a reference to the file.
     * @see generate2ClassDatasets.
     */
    std::ofstream& printOneClassAgainstOthers ( std::ofstream& fout, int whichClass );

    /**
     * Instantiates the distance functions.
     * @param realAttributesFunction is the type of distance function for continuous attributes.
     * @param nominalAttributesFunction is the type of distance function for nominal attriubtes.
     */
    void instantiateDistanceFunctions ( int realAttributesFunction, int nominalAttributesFunction );

protected:

//// PRINT DATA

    /**
     * Prints a single example to the output file.
     * @param fout is the output file where the example will be printed.
     * @param inst is the example to be printed.
     * @param action is the class of the example.
     */
    void printExample ( std::ofstream &fout, float* inst, int action );

    /**
     * Prints a single example (no normalized) to the output file.
     * @param fout is the output file where the example will be printed.
     * @param inst is the example to be printed.
     * @param action is the class of the example.
     */
    void printExampleNoNormalized ( std::ofstream &fout, float* inst, int action );

    /**
     * Prints a single example to the standard output.
     * @param inst is the example to be printed.
     * @param action is the class of the example.
     */
    void printExample ( float* inst, int action );

    /**
     * Prints the header of the data set.
     * @param fout is the output file where the header will be printed.
     */
    void printHeader ( std::ofstream& fout );


//// DISTANCE COMPUTATION

    /**
     * Calculates the index in the distance array.
     * @param i is the index of the first example.
     * @param j is the index of the second example.
     * @return An integer with the distance between i and j.
     */
    int INDEX ( int i, int j );

    /**
     * Returns the distance between two examples.
     * @param i is the position of the first example.
     * @param j is the position of the second example.
     * @return A float with the distance between i and j.
     */
    float getDistance ( int i, int j );

    /**
     * Returns the distance between two examples.
     * @param ex1 is the first example.
     * @param ex2 is the second example.
     * @return A float with the approximate distance between ex1 and ex2.
     */
    float getDistance ( float* ex1, float* ex2 );

    /**
     * Returns the distance but without applying the last square root.
     * Thus, this is not the exact distance but enables to speed up the code.
     * @param i is the position of the first example.
     * @param j is the position of the second example.
     * @return A float with the approximate distance between i and j.
     */
    float getApproximateDistance ( int i, int j );

    /**
     * Returns the distance but without applying the last square root.
     * Thus, this is not the exact distance but enables to speed up the code.
     * @param ex1 is the first example.
     * @param ex2 is the second example.
     * @return A float with the approximate distance between ex1 and ex2.
     */
    float getApproximateDistance ( float* ex1, float* ex2 );

};

#endif
