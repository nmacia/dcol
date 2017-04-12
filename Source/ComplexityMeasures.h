
#ifndef COMPLEXITYMEASURES_H
#define COMPLEXITYMEASURES_H

#include "ExtendedDataset.h"
#include "DistanceFunctions/NormalizedEuclideanFunction.h"
#include "DistanceFunctions/StdWeightedEuclideanFunction.h"
#include "DistanceFunctions/EuclideanFunction.h"
#include "DistanceFunctions/OverlapFunction.h"
#include "DistanceFunctions/VDMFunction.h"

#include <cmath>


/*!

\brief Extends from the ExtendedDataset class and implements all the complexity measures proposed by Ho and Basu (2002) and some new measures.

In summary, this class implements the following measures:
    <br>
    <ol>
        <li> Measures of overlaps in the feature values from different classes
            <ol>
                <li> Ratio of the maximum Fisher's discriminant (F1) </li>
                <li> Directional-vector maximum Fisher's discriminant ratio (F1v) </li>
                <li> Overlap of the per-class bounding boxes (F2) </li>
                <li> Maximum individual feature efficiency (F3) </li>
                <li> Collective feature efficiency (F4) </li>
            </ol>
        </li>

        <li> Measures of class separability
            <ol>
                <li> Minimized sum of the error distance of a linear classifier (L1) </li>
                <li> Training error of a linear classifier (L2) </li>
                <li> Fraction of points on the class boundary (N1) </li>
                <li> Ratio of average intra/inter class nearest neighbor distance (N2) </li>
                <li> Leave-one-out error rate of the one-nearest neighbor classifier (N3) </li>
            </ol>
        </li>

        <li> Measures of geometry, topology, and density of manifolds
            <ol>
                <li> Nonlinearity of a linear classifier (L3) </li>
                <li> Nonlinearity of the one-nearest neighbor classifier (N4) </li>
                <li> Fraction of maximum covering spheres (T1) </li>
                <li> Average number of points per dimension (T2) </li>
            </ol>
        </li>
    </ol>

    <br>
    The implementation of the methods is organized into two .cpp files:
    <ol>
        <li> ComplexityFunctions.cpp, which implements all the functions of the complexity measures. </li>
        <li> SMO.cpp, which implements the sequential minimal optimization to train
            support vector machines (Platt, 1998). This is used as a linear classifier. </li>
    </ol>
</p>

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


class ComplexityMeasures : public ExtendedDataset {

private:

    /** Indicates the proportion of examples that are generated in the convex hull
        to compute measures L3 and N4. */
    static const int PROPORTION_EXAMPLES_CONVEX_HULL = 2;

    /** Maximum deviation in computing the fraction of maximum covering spheres. */
    static const float EPSILON_SPHERES = 0.55;

    /** Parameter C for training the support vector machine. */
    static const float C = 0.05;

    /** Parameter TOLERANCE for training the support vector machine. */
    static const float TOLERANCE = 0.001;

    /** Parameter EPSILON for training the support vector machine. */
    static const float EPSILON = 0.001;

public:

    /**
     * Constructs a new ComplexityMeasures object. It calls the constructor of the class Dataset.
     * @param fileName is the name of the KEEL/WEKA formatted input file.
     * @param readAttInfo indicates whether the information of the attributes has to be read.
     * @param repUnknownVal indicates whether the unknown values have to be replaced.
     * @param realAttributesFunction indicates the type of distance function for continuous attributes.
     * @param nominalAttributesFunction indicates the type of distance function for nominal attributes.
     */
    ComplexityMeasures ( std::string fileName, bool readAttInfo, bool repUnknownVal = true,
                        int realAttributesFunction = 1, int nominalAttributesFunction = 1 ) 
                        : ExtendedDataset ( fileName,
                                            readAttInfo, repUnknownVal,
                                            realAttributesFunction, nominalAttributesFunction ) {}

    /**
     * Destructs the ComplexityMeasures object.
    */
    ~ComplexityMeasures () {}


//// MEASURES OF CLASS SEPARABILITY

    /**
     * Computes the fraction of points on the class boundary (N1). 
     * This measure provides an estimate of the length of the class boundary.
     * @param spanTree is the minimum spanning tree calculated by Prim's algorithm.
     * @return A float with the value of the measure N1.
     * @see computePrim.
     */
    float computeBoundary ( int** spanTree );

    /**
     * Runs the Prim's algorithm on the complete graph represented by the distances between examples.
     * @return An integer* with N-1 pairs of nearest neighbors, being N the number of examples.
     * @see computeBoundary.
     */
    int** computePrim ();

    /**
     * Computes the ratio of average intra/inter class nearest neighbor distance (N2).
     * This measure calculates, for each example, the distance to its nearest neighbor
     * of the same class and of another class. Then, the ratio between
     * the intra-class distance and the inter-class distance is returned.
     * @return A float with the value of the measure N2.
     */
    float computeIntraInter ();


//// MEASURES OF GEOMETRY, TOPOLOGY, AND DENSITY OF MANYFOLDS

    /**
     * Computes the leave-one-out error rate of the one-nearest neighbor classifier (N3).
     * @param k is the number of neighbors to be used (default 1) for the kNN algorithm.
     * @return A float with the value of the measure N3.
     * @see createExamplesByInterpolation.
     */
    float computeNonLinearityKNNTrain ( int k = 1 );

    /**
     * Computes the nonlinearity of a linear classifier (L3).
     * This measure uses the kNN learning algorithm on an artificially generated
     * data set on the convex hull between classes.
     * @param k is the number of neighbors to be used (default 1) for the kNN algorithm.
     * @return A float with the value of the measure L3.
     * @see createExamplesByInterpolation.
     */
    float computeNonLinearityKNNConvexHull ( int k = 1 );

    /**
     * Computes the sum of the error distance of a linear classifier (L1).
     * @param w is a vector of weights that represent the SVM.
     * @param b is the offset of the SVM.
     * @return A float with the value of the measure L1.
     * @see createExamplesByInterpolation.
     */
    float computeNonLinearityLCDistance ( float* w, float b );

    /**
     * Computes the training error of a linear classifier (L2).
     * @param w is a vector of weights that represent the SVM.
     * @param b is the offset of the SVM.
     * @return A float with the value of the measure L2.
     * @see createExamplesByInterpolation.
     */
    float computeNonLinearityLCTrain ( float* w, float b );

    /**
     * Computes the nonlinearity of the one-nearest neighbor classifier (N4).
     * The error rate is measured on examples artificially generated on the
     * convex hull between classes.
     * @param w is a vector of weights that represent the SVM.
     * @param b is the offset of the SVM.
     * @return A float with the value of the measure N4.
     * @see createExamplesByInterpolation.
     */
    float computeNonLinearityLCConvexHull ( float* w, float b );

    /**
     * Trains a support vector machine by means of the sequential minimal
     * optimization algorithm (Platt, 1998).
     * @param b is a reference to the offset of the SVM.
     * @return A float* with a vector of weights that define the linear kernel.
     */
    float* trainSMO ( float& b );

    /**
     * Computes the average number of samples per dimension (T2).
     * @return A float with the value of the measure T2.
     */
    float averageNumberOfSamplesPerDimension ();

    /**
     * Computes the fraction of maximum covering spheres (T1).
     * @return A float with the value of the measure T1. 
     */
    float* computeFractMaxCoveringSpheres ();


//// MEASURES OF OVERLAPS IN THE FEATURE VALUES FROM DIFFERENT CLASSES

    /**
     * Computes the Fisher's discriminant ratio (F1) according to its directional-vector definition.
     * @param whichAttribute indicates the attribute that maximally discriminates.
     * @return A float with the value of the measure F1.
     */
    float computeFisher ( int& whichAttribute );

    /**
     * Computes the Fisher's discriminant ratio projected over the vector with the best direction (F1v).
     * @return A float with the measure applied to the most discriminant feature.
     */
    float computeFisherVectorized ();

    /**
     * Computes the overlap of the per-class bounding boxes for data sets with
     * any number of classes. We extended the initial definition of the measure
     * provided in (Ho and Basu, 2002) for two-class data sets to m-class data sets
     * as recommended in (Ho, Basu, and Law, 2006).
     * @return A float with the measure applied to the most discriminant feature.
     */
    float computeVolumeOverlap ();

    /**
     * Computes the maximum efficiency of all the attributes (F3,F4).
     * It computes the discriminatory capabilities of each attribute.
     * @param mostDiscrAtt contains the position of the most discriminative attribute is returned (it is passed by reference).
     * @param discPowerOfTheBest contains the discriminant power of the most
     *                           discriminative attribute is returned (it is passed by reference).
     * @return A float* with the value of the measure F3 and F4.
     */
    float* computeMaximumEfficiencyOfAttributes ( int& mostDiscrAtt, float& discPowerOfTheBest );

protected:

    /**
     * Computes the Fisher's discriminant ratio for two-class problems (F1).
     * @param whichAttribute indicates the attribute that maximally discriminates.
     * @return A float with the value of the measure F1.
     */
    float computeFisher2Class ( int& whichAttribute );

    /**
     * Computes the Fisher's discriminant ratio for m-class problems (m>2) (F1).
     * @param whichAttribute indicates the attribute that maximally discriminates.
     * @return A float with the value of the measure F1.
     */
    float computeFisherMClass ( int& whichAttribute );

private:

    int getNearestNeighborOfExample ( int example, float& minDist );
    void getNearestNeighborIntraAndInterClass ( int example, int& neigIntra, float& minDistIntra,
            int& neigInter, float& minDistInter );

    int selectMinNotTreated ( int* neig, float* edge );
    void searchNearestNeighborsOfAnotherClass ( int** neigh, float** distNeigh,
            float& globalMinDist, bool& overlappedExamples );

    void calculateAdherenceSubsets ( int** adherenceOrder, int* maxAdherenceOrder, float** distNeigh,
                                     bool overlappedExamples, float epsilon );

    void eliminateAdherenceSetsIncluded ( int** adherenceOrder, int* maxAdherenceOrder, float epsilon );
    float *getStatisticsFractMaxCoveringSpheres ( int** adherenceOrder, int* maxAdherenceOrder );
    void createExamplesByInterpolation ( float**& testExamples, int*& classOfTestExamples,
                                         int numExamplesPerClass, bool isSMO );

    float runKNN ( int k, float** testExamples, int* classOfTestExamples, int numberOfTestExamples, bool isTrain = false );

    float testSMO ( float* w, float b, float** testExamples, int* classOfTestExamples, int numTestExamples );
    float getDistanceObjectiveFunction ( float* w, float b, float** testExamples,
                                         int* classOfTestExamples, int numTestExamples );

    float kernelFunction ( int i1, int i2 );
    float learnedFunction ( int k, float* w, float b );
    float learnedFunction ( float* testEx, float* w, float b );
    int takeStep ( int i1, int i2, float& b, float* alpha, float* w, float* errorCache );

    int argmaxE1E2 ( int i1, float E1, float& b, float* alpha, float* w, float* errorCache );
    int iterateNonBoundExamples ( int i1, float& b, float* alpha, float* w, float* errorCache );
    int iterateEntireTrainingSet ( int i1, float& b, float* alpha, float* w, float* errorCache );
    int examineExample ( int i1, float& b, float* alpha, float* w, float* errorCache );

    bool getDiscriminativePowerOfAttributes ( float* discPower, int* order, int numAttRemain, float** minValue,
            float** maxValue, bool* pointDisc );

};

#endif
