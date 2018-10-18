
#ifndef DATASET_H
#define DATASET_H

#include "DateContainer.h"
#include "DistNode.h"
#include "Heap.h"
#include "Matrix.h"
#include "StringTokenizer.h"
#include "Utils.h"
#include "Vector.h"

#include "DistanceFunctions/EuclideanFunction.h"
#include "DistanceFunctions/NormalizedEuclideanFunction.h"
#include "DistanceFunctions/StdWeightedEuclideanFunction.h"
#include "DistanceFunctions/OverlapFunction.h"
#include "DistanceFunctions/VDMFunction.h"

#include <cmath>
#include <sstream>


/*!

\brief Implements methods to read, manipulate, and print data.

This class reads the information from the data set (in KEEL/WEKA format) and provides methods to deal with data.

The implementation of the methods is organized into two .cpp files:
	<ol>
		<li> Dataset.cpp, which contains the methods to read, write, and deal with the data. </li>
		<li> Statistics.cpp, which contains the methods to make statistics. </li>
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
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DCoL. If not, see <http://www.gnu.org/licenses/>.

*/


class Dataset {

public:

    /** Representation of the unknown value in an example. */
    static char UNKNOWN_VALUE;

protected:

//// REPRESENTATION OF THE EXAMPLES IN THE DATA SET

    /** Data set examples. */
    float** example;

    /** Class or label of each example. */
    int* classOfExample;

    /** Index of the current example. */
    int currentExample;

    /** Number of input attributes. The class attribute is not considered. */
    int numberOfAttributes;

    /** Number of examples. */
    int numberOfExamples;

    /** Shows whether the examples are sampled sequentially. */
    int sequentialExamples;

    /** Number of different classes. */
    int numberOfClasses;


//// DATA SET INFORMATION

    /** Name of the data set. */
    std::string relationName;

    /** Array with the name of the attributes. */
    std::string* attName;

    /** Array with the type of each input attribute. */
    char* typeOfRepresentation;

    /** Number of continuous attributes. */
    int numberOfContinuousAttributes;

    /** Number of nominal attributes. */
    int numberOfNominalAttributes;

    /** Array with the number of values that a nominal attribute can take. */
    int* nominalAttrNumValues;

    /** Array with the possible values of each nominal attribute. */
    std::string** enumAtt;

    /** Minimum value for each attribute. */
    float* minAttValue;

    /** Maximum value for each attribute. */
    float* maxAttValue;

    /** Average attribute value. */
    float* avgAttValue;

    /** Number of values for the attribute. */
    int* numberValuedAtt;

    /** Standard deviation per attribute. */
    float* stdAttValue;

    /** Minimum value for each attribute and class (index minAttValuePerClass[attribute][class]). */
    float** minAttValuePerClass;

    /** Maximum value for each attribute and class (index maxAttValuePerClass[attribute][class]). */
    float** maxAttValuePerClass;

    /** Name of the class attribute. */
    std::string className;

    /** Position of the class in the list of attributes. */
    int classPosition;

    /** Type of the class. */
    char classType;

    /** Possible values that the class can take if it is nominal. */
    std::string* nominalClassValues;

    /** Minimum value of the class. */
    int minClassValue;

    /** Maximum value of the class. */
    int maxClassValue;


//// ATTRIBUTES TO STORE THE AVERAGE AND THE STANDARD DEVIATION OF EACH ATTRIBUTE PER CLASS

    /** Frequency of the values for nominal attributes (index avgNominalValue[attribute][class][value]). */
    float*** avgNominalValue;

    /** Mean values per attribute and class (continuous attributes) or median values
        per attribute and class (nominal attributes) (index avgRealValue[attriherbute][class]). */
    float** avgRealValue;

    /** Standard deviation per attribute and class (index stdRealValue[attribute][class]). */
    float** stdRealValue;

    /** Counter of number of values per each attribute and class (index avgAttCounter[attribute][class]). */
    float** avgAttCounter;

    /** Indicates whether data have been normalized. */
    bool areDataNormalized;

    /** Indicates whether the real and integer attributes have been normalized. */
    bool areContinuousAttributesNormalized;

    /** Indicates whether the nominal attributes have been normalized. */
    bool areNominalAttributesNormalized;

    /** Indicates whether unknown values have been replaced. */
    bool areUnknownValuesReplaced;

    /** Contains the date format in case of having date attributes. */
    DateContainer** dateFormat;

    /** Contains the dates read initially. Later, they are transformed to ordinal values. */
    Date** dateContent;

public:
    
    /**
     * Constructs a new Dataset object: opens the file and the examples in there.
     * @param fileName is the name of the KEEL/WEKA formatted input file.
     * @param readAttInfo indicates whether the information of the attributes has to be read.
     * @param repUnknownVal indicates whether the unknown values need to be replaced.
     */
    Dataset ( std::string fileName, bool readAttInfo, bool repUnknownVal = true );

    /**
     * Destructs the Dataset object.
     */
    ~Dataset ();


//// GETTING EXAMPLES, ATTRIBUTES, AND GENERAL INFORMATION OF THE DATA

    /**
     * Returns a randomly chosen example.
     * @param wClass is the class of the example (it is passed by reference).
     * @return A float* with the example.
     */
    float* getRandomExample ( int& wClass );

    /**
     * Returns the current example.
     * @return A float* with the current example.
     */
    float* getCurrentExample ();

    /**
     * Returns the class of the current example.
     * @return An integer with the class of the current example.
     */
    int getCurrentClass ();

    /**
     * Indicates that the examples will be explored sequentially.
     */
    void beginSequentialExamples ();

    /**
     * Returns the next example (sequential exploration).
     * @param wClass is a reference where the class of the example is returned.
     * @return A float* with the next example.
     */
    float* getNextExample ( int& wClass );

    /**
     * Returns the number of examples of the data set.
     * @return An integer with the number of examples.
     */
    int getNumberOfExamples ();

    /**
     * Returns the number of attributes of the data set.
     * @return An integer with the number of attributes.
     */
    int getNumberOfAttributes ();

    /**
     * Returns the number of classes of the data set.
     * @return An integer with the number of classes.
     */
    int getNumberOfClasses ();

    /**
     * Computes the initial statistics on the data.
     */
    void makeInitialStatistics ();

    /**
     * Returns the name of the data set.
     * @return A string with the relationship name.
     */
    std::string getName () {
        return relationName;
    };

    /**
     * Returns the class of the ith example.
     * @return An integer with the class value of the ith example.
     */
    int getClassOfExample ( int i ) {
        return classOfExample[i];
    }

protected:

//// INITIALIZATION FUNCTIONS

    /**
     * Initializes the configuration parameters.
     * @param readAttInfo indicates whether the information of the attributes has to be read.
     */
    void initConfigParameters ( bool readAttInfo );

    /**
     * Initializes the attributes.
     */
    void initAttributes();

    /**
     * Initializes several internal parameters.
     * @param readAttInfo indicates whether the information of the attributes has to be read.
     */
    void initParameters ( bool readAttInfo );

    /**
     * Reserves memory for statistics.
     */
    void reserveMemoryForStatistics ();


//// GET DATA FROM FILE

    /**
     * Reads the data from file.
     * @param fin is the input file.
     * @param readAttInfo indicates whether the information of the attributes has to be read.
     */
    void readData ( std::ifstream& fin, bool readAttInfo );

    /**
     * Returns the basic information from the file.
     * @param fileName is the name of the file that contains the input data set.
     * @param readAttInfo indicates whether the information of the attributes has to be read.
     */
    void getBasicInfo ( std::string& fileName, bool readAttInfo );

    /**
     * Returns the basic information from the file.
     * @param fileName is the name of the file that contains the input data set.
     * @param readAttInfo indicates whether the information of the attributes has to be read.
     * @param _attNum is the number of attributes (it is passed by reference).
     * @param _exNum is the number of examples (it is passed by reference).
     */
    void getBasicInfo ( std::string& fileName, bool readAttInfo, int& _attNum, int& _exNum );

    /**
     * Reads the information of the data attributes.
     * @param fin is the input file.
     * @param readAttInfo indicates whether the information of the attributes has to be read.
     * @param readStatistics indicates whether statistics need to be taken after reading the data.
     */
    void readAttributes ( std::ifstream& fin, bool readAttInfo, bool readStatistics = false );

    /**
     * Reads and stores the information of the class.
     * @param s is the class that has been read.
     */
    void readClassInformation ( std::string& s );

    /**
     * Reads and stores the information of a single attribute.
     * @param s is the attribute that has been read.
     * @param st is the tokenized string.
     * @param attCount is the number of the current attribute.
     */
    void readAttributeInformation ( std::string& s, StringTokenizer& st, int attCount );

    /**
     * Get attribute names
     */
    std::string* getAttrNames ();

    /**
     * Transforms the string into lower case.
     * @param s is the string that will be transformed into lower case.
     */
    std::string toLowerCase ( std::string s );

    /**
     * Transforms all date attributes in the data set into integer attributes.
     */
    void transformDateAttributesToNumeric ();

    /**
     * Transforms the date attribute specified in the input parameter into an integer format.
     * @param whichAttribute is the attribute that will be transformed into an integer.
     */
    void transformDateAttributeToNumeric ( int whichAttribute );

    /**
     * Transforms the value passed as parameter to an ordinal value.
     * @param value is the value to be transformed.
     * @param convertionArray is the array that containes the sorted float values.
     * @param arrayLength is the length of the convertionArray array.
     * @return A float with the converted value.
     */
    float convertToOrdinal ( Date value, Date* convertionArray, int arrayLength );

//// PROCESS THE DATA

    /**
     * Normalizes continuous attributes.
     */
    void normalizeContinuousAttributes ();

    /**
     * Normalizes nominal attributes.
     */
    void normalizeNominalAttributes ();

    /**
     * Normalizes all the attributes of the data set.
     */
    void normalizeAllAttributes ();

    /**
     * Unnormalizes all the attributes of the data set.
     */
    void unNormalizeAllAttributes ();

    /**
     * Replaces unknown values.
     */
    void replaceUnknownValues ();

    /**
     * Transforms the nominal value into its corresponding integer.
     * @param attName is the name of the attribute.
     * @param vect is the vector with all the possible nominal values for a given attribute.
     * @param max is the maximum size of the vector.
     * @param s contains the categorical value that has to be transformed into integer.
     */
    int getIntegerFromVector ( std::string attName, std::string* vect, int max, std::string s );

    /**
     * Calculates the average values for each attribute.
     */
    void calculateAverages ();

    /**
     * Calculates the deviations for each attribute.
     */
    void calculateDeviations ();

    /**
     * Removes unnecessary examples.
     */
    void removeAllZeroExamples ();

    /**
     * Returns an n x 1 matrix with the means of the class passed as parameter.
     * @param wClass is the class.
     * @return A matrix with the means per class.
     */
    Matrix* getMeanVectorOfClass ( int wClass );

    /**
     * Returns an n x n matrix with the covariances of the class passed as parameter.
     * @param wClass is the class.
     * @return A matrix with the covariances per class.
     */
    Matrix* getCovarianceMatrixOfClass ( int wClass );

};

#endif
