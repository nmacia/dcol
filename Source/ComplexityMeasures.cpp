
/*!

\brief This file contains the implementation of the ComplexityMeasures methods.

The implementation corresponds to the measures described in (Ho and Basu, 2002).
Here we provide a brief description with the original name of the measure in the paper.
    <ol>
        <li> Measures of overlaps in feature values from different classes
            <ol>
                <li> computeFisher: It calculates the maximum Fisher's discriminant ratio (F1). </li>
                <li> computeFisherVectorized: It calculates the directional-vector maximum Fisher's discriminant ratio (F1v). </li>
                <li> computeVolumeOverlap: It calculates the overlap of the per-class bounding boxes (F2). </li>
                <li> computeMaximumEfficiencyOfAttributes: It calculates the maximum (individual) feature efficiency (F3) and the collective feature efficiency (F4). </li>
            </ol>
        </li>
        <li> Measures of class separability
            <ol>
                <li> computeNonLinearityLCDistance: It calculates the minimized sum of the error distance of a linear classifier (L1). </li>
                <li> computeNonLinearityLCDTrain: It calculates the training error of a linear classifier (L2). </li>
                <li> computeBoundary: It calculates the fraction of points on the class boundary (N1). </li>
                <li> computeIntraInter: It calculates the ratio of average intra/inter class nearest neighbor distance (N2). </li>
                <li> computeNonLinearityKNNTrain: It calculates the leave-one-out error rate of the one-nearest neighbor classifier (N3). </li>
             </ol>
        </li>

        <li> Measures of geometry, topology, and density of manifolds
            <ol>
                <li> computeNonLinearityLCDConvexHull: It calculates the nonlinearity of a linear classifier (L3). </li>
                <li> computeNonLinearityKNNConvexHull: It calculates the nonlinearity of the one-nearest neighbor classifier (N4). </li>
                <li> computeFractMaxCoveringSpheres: It calculates the fraction of maximum covering spheres (T1). </li>
                <li> computeAverageNumberOfPointsPerDimension: It calculates the average number of points per dimension (T2). </li>
            </ol>
        </li>
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


#include "ComplexityMeasures.h"


//// MEASURES OF CLASS SEPARABILITY 

float ComplexityMeasures::computeBoundary ( int** spanTree ) {

    int i;
    int different = 0;
    int* node;

    node = new int [ numberOfExamples ];

    for ( i = 0; i < numberOfExamples; i++ ) {
        node[i] = -1;
    }

    // 1. Store the nodes of the spanning tree with different class.
    for ( i = 0; i < numberOfExamples - 1; i++ ) {
        if ( classOfExample[ spanTree[i][0] ] != classOfExample[ spanTree[i][1] ] ) {
            node[ spanTree[i][0] ] = 0;
            node[ spanTree[i][1] ] = 0;
        }
    }

    // 2. Compute the number of nodes of the spanning tree with different class.
    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( node[i] == 0 ) different ++;
    }

    std::cout << "      > Number of nodes on the class boundary: " << different << std::endl;
    std::cout << "      > Total number of nodes: " << numberOfExamples << std::endl;

    delete [] node;

    return ( float ) different / ( float ) numberOfExamples;

} // end computeBoundary


float ComplexityMeasures::computeIntraInter () {

    int i;
    int neigIntra, neigInter;
    float minDistIntra, minDistInter;
    float intraClass = 0, interClass = 0;

    for ( i = 0; i < numberOfExamples; i++ ) {
        getNearestNeighborIntraAndInterClass ( i, neigIntra, minDistIntra, neigInter, minDistInter );
        intraClass += minDistIntra;
        interClass += minDistInter;
    }

    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Intra class distance: " << intraClass << std::endl;
        std::cout << "      > Inter class distance: " << interClass << std::endl;
    }

    if ( interClass == 0 ) {
    	std::string logMessage =  "      > [WARNING N2] The inter class distance is zero. \n ";
        logMessage += "        >> This may indicate either that there are not examples of one of the classes or that there is noise in the training data set so that there are repeated examples labeled differently. \n ";
        logMessage += "        >> The result of the measure will be infinite. \n ";
        Utils::logWarningError ( logMessage );
    }

    return intraClass / interClass;

} // end computeIntraInter


int ComplexityMeasures::getNearestNeighborOfExample ( int example, float& minDist ) {

    int i;
    int neig = -1;
    minDist = FLT_MAX;

    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( i != example && getDistance ( example, i ) < minDist ) {
            neig = i;
            minDist = getDistance ( example, i );
        }
    }

    return neig;

} // end getNearestNeighborOfExample


void ComplexityMeasures::getNearestNeighborIntraAndInterClass ( int iExample, int& neigIntra, float& minDistIntra,
        int& neigInter, float& minDistInter ) {

    int i;
    float distAux = 0;
    neigInter = -1;
    neigIntra = -1;
    minDistInter = FLT_MAX;
    minDistIntra = FLT_MAX;

    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( i != iExample ) {
            distAux = getDistance ( iExample, i );

            if ( classOfExample[i] == classOfExample[ iExample ] && distAux < minDistIntra ) {
                neigIntra = i;
                minDistIntra = distAux;
            }
            else if ( classOfExample[i] != classOfExample[ iExample ] && distAux < minDistInter ) {
                neigInter = i;
                minDistInter = distAux;
            }
        }
    }

    if ( neigInter == -1 ) {
        minDistInter = 0;
    }
    if ( neigIntra == -1 ) {
        minDistIntra = 0;
    }
    
} // end getNearestNeighborIntraAndInterClass


int** ComplexityMeasures::computePrim () {

    int currentNode;
    int i, j;
    int spanTreeIndex = 0;

    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Computing the minimum spanning tree [progress line] [x/100]: " << std::endl << "        ";
    }

    // 1. Spanning tree: pairs of examples.
    int** spanTree = new int* [ numberOfExamples - 1 ];
    for ( i = 0; i < numberOfExamples - 1; i++ ) {
        spanTree[i] = new int [2];
    }

    // 2. Structures to maintain the neighbor closer and the edge to this neighbor.
    int*   neig   = new int   [ numberOfExamples ];
    float* edge   = new float [ numberOfExamples ];

    // 3. Choose a vertex, which will be the seed of the spanning tree: example 0.
    currentNode = 0;

    // 4. Initialize the structures considering that we have a complete GRAPH (all nodes connected).
    neig[ currentNode ] = -1; // Indicates that the node has been processed.
    edge[ currentNode ] = 0;

    for ( i = 1; i < numberOfExamples; i++ ) {
        neig[i] = currentNode;
        edge[i] = getApproximateDistance ( currentNode, i );
    }

    // 5. Create the minimum spanning tree (MST).
    for ( i = 1; i < numberOfExamples; i++ ) {

        if ( Utils::doScreenStatistics ) {
            if ( numberOfExamples > 100 ) {
                if ( ( i - 1 ) % ( int ) ( numberOfExamples / 100 ) == 0 ) {
                    std::cout << ".";
                    if ( i % ( ( int ) ( numberOfExamples * 20 / 100 ) ) == 0 ) std::cout << std::endl << "        ";
                    std::cout.flush ();
                }
            }
        }

        // 5.1. Select the vertex, not treated yet, with minimum distance.
        currentNode = selectMinNotTreated ( neig, edge );

        // 5.2. Add this vertex to the spanning tree.
        spanTree [ spanTreeIndex ][0] = currentNode;
        spanTree [ spanTreeIndex ][1] = neig[ currentNode ];
        spanTreeIndex ++;

        // 5.3. Check the vertex as processed.
        neig [ currentNode ] = -1;

        // 5.4. Recalculate the distances of nearest neighbors.
        for ( j = 0; j < numberOfExamples; j++ ) {
            if ( neig[j] != -1 && edge[j] > getApproximateDistance ( currentNode, j ) ) {
                neig[j] = currentNode;
                edge[j] = getApproximateDistance ( currentNode, j );
            }
        }
    }

    if ( Utils::doScreenStatistics ) std::cout << std::endl;

    // 6. Delete temporal memory.
    delete [] neig;
    delete [] edge;

    return spanTree;

} // end computePrim


int ComplexityMeasures::selectMinNotTreated ( int* neig, float* edge ) {

    int i;
    int min = -1;
    float distMin = FLT_MAX;

    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( ( neig[i] != -1 ) && ( edge[i] < distMin ) ) {
            distMin = edge[i];
            min = i;
        }
    }

    return min;

} // end selectMinNotTreated


//// MEASURES OF GEOMETRY, TOPOLOGY, AND DENSITY OF MANYFOLDS 

float ComplexityMeasures::computeNonLinearityLCDistance ( float* w, float b ) {

    int i;
    float result;

    if ( numberOfClasses != 2 ) {
        std::string logMessage = " \n  > [ERROR L1] Nonlinearity of the linear classifier can be applied to only two-class data sets. \n ";
        Utils::logWarningError ( logMessage );
        exit ( -1 );
    }

    // 1. Change the class 0 to -1.
    if ( Utils::doScreenStatistics ) { 
        std::cout << "      > Changing classes to -1, 1 " << std::endl;
    }

    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( classOfExample[i] == 0 ) {
            classOfExample[i] = -1;
        }
    }

    // 2. Get the sum of distances to the objective function for each training example.
    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Testing SVM with the training examples " << std::endl;
    }

    result = getDistanceObjectiveFunction ( w, b, example, classOfExample, numberOfExamples );

    // 3. Revert the process. Change the class -1 to 0.
    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( classOfExample[i] == -1 ) {
            classOfExample[i] = 0;
        }
    }

    return result;

} // end computeNonLinearityLCDistance


float ComplexityMeasures::computeNonLinearityLCTrain ( float* w, float b ) {

    int i;
    float result;

    if ( numberOfClasses != 2 ) {
        std::string logMessage = " \n  > [ERROR L2] Nonlinearity of the linear classifier can be applied to only two-class data sets. \n ";
        Utils::logWarningError ( logMessage );
        exit ( -1 );
    }

    // 1. Change the class 0 to -1
    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Changing classes to -1, 1 " << std::endl;
    }

    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( classOfExample[i] == 0 ) {
            classOfExample[i] = -1;
        }
    }

    // 2. Test SMO with the train examples.
    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Testing SVM with the training examples " << std::endl;
    }

    result = testSMO ( w, b, example, classOfExample, numberOfExamples );

    // 3. Revert the process. Change the class -1 to 0.
    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( classOfExample[i] == -1 ) {
            classOfExample[i] = 0;
        }
    }

    return result;

} // end computeNonLinearityLCTrain


float ComplexityMeasures::computeNonLinearityLCConvexHull ( float* w, float b ) {

    int numInstToGeneratePerClass;
    float result;
    float** testExamples;
    int *classOfTestExamples;
    int i, cClass;

    if ( numberOfClasses != 2 ) {
        std::string logMessage = " \n  > [ERROR L3] Nonlinearity of the linear classifier can be applied to only two-class data sets. \n ";
        Utils::logWarningError ( logMessage );
        exit ( -1 );
    }

    // 0. Check that all classes have examples.
    organizePerClass ();

    for ( cClass = 0; cClass < numberOfClasses; cClass ++ ) {
        if ( numberOfExamplesPerClass[ cClass ] < 1 ) {
            // Convert integer into string.
            std::ostringstream stCClass;
            stCClass << cClass;
            std::string logMessage = "      > [ERROR L3] Error in computing the nonlinearity of the linear classifier. \n ";
            logMessage += "        >> Class " + stCClass.str () + " has 0 examples. \n ";
            Utils::logWarningError ( logMessage );
            result = -1;
            return result;
        }
    }

    // 1. Create new examples by means of interpolation.
    numInstToGeneratePerClass = PROPORTION_EXAMPLES_CONVEX_HULL * numberOfExamples / numberOfClasses;

    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Generating " << numInstToGeneratePerClass << " examples by means of interpolation " << std::endl;
    }

    createExamplesByInterpolation ( testExamples, classOfTestExamples, numInstToGeneratePerClass, true );

    // 2. Change the class 0 to -1.
    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Changing classes to -1, 1 " << std::endl;
    }

    for ( i = 0; i < numInstToGeneratePerClass * numberOfClasses; i++ ) {
        if ( classOfTestExamples[i] == 0 ) {
            classOfTestExamples[i] = -1;
        }
    }

    // 3. Test SMO with the new interpolated examples.
    if ( Utils::doScreenStatistics )  {
        std::cout << "      > Testing SVM with the test examples " << std::endl;
    }

    result = testSMO ( w, b, testExamples, classOfTestExamples, numInstToGeneratePerClass * numberOfClasses );

    // 4. Uncomment if the process needs to be reverted (just if new complexity measures are applied to the data). Change the class -1 to 0.
    //for ( i = 0; i < numberOfTestExamples; i++ ) {
    //    if ( classOfTestExample[i] == -1 ) {
    //        classOfTestExample[i] = 0;
    //    }
    //}

    // 5. Delete memory.
    for ( i = 0; i < numInstToGeneratePerClass * numberOfClasses; i++ ) {
        delete [] testExamples[i];
    }

    delete [] testExamples;
    delete [] classOfTestExamples;
    
    return result;

} // end computeNonLinearityLCConvexHull


float ComplexityMeasures::computeNonLinearityKNNTrain ( int k ) {

    // 1. Run the KNN algorithm on the train set.
    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Testing the 1NN with the training examples " << std::endl;
    }
    return  runKNN ( k, example, classOfExample, numberOfExamples, true );

} // end computeNonLinearityKNNTrain


float ComplexityMeasures::computeNonLinearityKNNConvexHull ( int k ) {

    float **testExamples;
    int *classOfTestExamples;
    float result;
    int inst, cClass;

    int numInstToGeneratePerClass = PROPORTION_EXAMPLES_CONVEX_HULL * numberOfExamples / numberOfClasses;

    // 0. Check that all classes have examples.
    organizePerClass ();

    for ( cClass = 0; cClass < numberOfClasses; cClass ++ ) {
        if ( numberOfExamplesPerClass[ cClass ] < 1 ) {
            // Convert integer into string.
            std::ostringstream stCClass;
            stCClass << cClass;
            std::string logMessage = " \n      > [ERROR N4] Error in computing the nonlinearity of the KNN classifier. \n ";
            logMessage += "        >> Class " + stCClass.str () + " has 0 examples. \n ";
            Utils::logWarningError ( logMessage );
            result = -1;
            return result;
        }
    }

    // 1. Create the convex hull.
    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Generating " << numInstToGeneratePerClass << " examples by means of interpolation " << std::endl;
    }

    createExamplesByInterpolation ( testExamples, classOfTestExamples, numInstToGeneratePerClass, false );

    // 2. Run the KNN algorithm on the interpolated data.
    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Testing the 1NN with the test examples " << std::endl;
    }

    result = runKNN ( k, testExamples, classOfTestExamples, numInstToGeneratePerClass * numberOfClasses, false );

    // 3. Delete memory.
    for ( inst = 0; inst < numInstToGeneratePerClass*numberOfClasses; inst ++ ) {
        delete [] testExamples[ inst ];
    }

    delete [] testExamples;
    delete [] classOfTestExamples;

    return result;
    
} // end computeNonLinearityKNNConvexHull


float ComplexityMeasures::runKNN ( int k, float** testExamples, int* classOfTestExamples,
                                  int numberOfTestExamples, bool isTrain ) {

    int i, j;
    DistNode dNode;
    float max;
    int majClass;

    int numCorrect = 0, numTotal = 0, numTies = 0;
    bool thereIsTie;

    HeapTree<DistNode>* neighbors = new HeapTree<DistNode> ( k );
    int* votesPerClass = new int [ numberOfClasses ];

    for ( i = 0; i < numberOfTestExamples; i++ ) {

        neighbors->removeAll ();

        for ( j = 0; j < numberOfExamples; j++ ) {

            if ( !isTrain || i != j ) {
                dNode.dist = getApproximateDistance ( testExamples[i], example[j] );
                dNode.index = j;

                if ( neighbors->getNumberOfElements () == k ) {
                    if ( dNode.dist < ( neighbors->getFirst () ).dist ) {
                        neighbors->remove ();
                        neighbors->add ( dNode );
                    }
                }
                else {
                    neighbors->add ( dNode );
                }
            }
        }

        // Get the majority class. Recollect votes per class.
        for ( j = 0; j < numberOfClasses; j++ ) {
            votesPerClass[j] = 0;
        }

        // Get the majority class among the k neighbors.
        for ( j = 0; j < neighbors->getNumberOfElements(); j++ ) {
            dNode = neighbors->getElementAt ( j );
            votesPerClass[ classOfExample[ dNode.index ] ] ++;
        }

        max = votesPerClass[0];

        majClass = 0;
        thereIsTie = false;

        for ( j = 1; j < numberOfClasses; j++ ) {
            if ( votesPerClass[j] > max ) {
                max = votesPerClass[j];
                majClass = j;
                thereIsTie = false;
            }
            else if ( votesPerClass[j] ==  max ) {
                thereIsTie = true;
            }
        }

        dNode = neighbors->getFirst ();

        // Count the number of correct classifications.
        if ( !thereIsTie && classOfTestExamples[i] == majClass ) {
            numCorrect ++;
        }
        else if ( thereIsTie ) {
            numTies ++;
        }

        numTotal ++;
    }

    delete neighbors;
    delete [] votesPerClass;

    if ( Utils::doScreenStatistics ) {
        std::cout << "        > Number of incorrect classifications: " << ( numTotal - numCorrect ) << std::endl;
        std::cout << "        > Number of classifications: " << numTotal << std::endl;
    }

    return 1. - ( ( ( float ) numCorrect ) / ( ( float ) numTotal ) );

} // end runKNN


void ComplexityMeasures::createExamplesByInterpolation ( float**& testExamples, int*& classOfTestExamples,
                                                         int numExamplesTestPerClass, bool isSMO ) {
 
    int i, j, cClass, inst = 0, ex1, ex2;
    float rnd;

    // 1. Allocate memory for the examples.
    testExamples = new float* [ numExamplesTestPerClass * numberOfClasses ];
    classOfTestExamples = new int [ numExamplesTestPerClass * numberOfClasses ];

    // 2. Organize the examples of the training data set per class.
    if ( numberOfExamplesPerClass != 0 ) { 
        organizePerClass ();
    }

    // 3. Generate the test examples.
    for ( cClass = 0; cClass < numberOfClasses; cClass ++ ) {

        if ( Utils::doScreenStatistics ) {
            std::cout << "        > Generating " << numExamplesTestPerClass ;
            std::cout << " examples of class " << cClass << std::endl;
        }

        for ( i = 0; i < numExamplesTestPerClass; i++ ) {

            // 3.1. Allocate memory for one example.
            testExamples[inst] = new float [ numberOfAttributes ];

            // 3.2. Select two examples of the class cClass.
            do {
                ex1 = Utils::i_rand ( 0, numberOfExamplesPerClass[ cClass ] - 1 );
                ex2 = Utils::i_rand ( 0, numberOfExamplesPerClass[ cClass ] - 1 );
            }
            while ( ex1 == ex2 && numberOfExamplesPerClass[ cClass ] > 1 );

            // 3.3. Get the absolute position of example 1 and 2.
            ex1 = indexExamplesPerClass[ cClass ][ ex1 ];
            ex2 = indexExamplesPerClass[ cClass ][ ex2 ];

            // 3.4. Create a new example from these two ones.
            for ( j = 0; j < numberOfAttributes; j++ ) {

                if ( example[ ex1 ][j] == Dataset::UNKNOWN_VALUE && example[ ex2 ][j] == Dataset::UNKNOWN_VALUE ) {
                    testExamples[ inst ][j] = Dataset::UNKNOWN_VALUE;
                }
                else if ( example[ ex1 ][j] == Dataset::UNKNOWN_VALUE ) {
                    testExamples[ inst ][j] = ( Utils::f_rand( ) < 0.5 )? example[ ex2 ][j]:Dataset::UNKNOWN_VALUE;
                }
                else if ( example[ ex2 ][j] == Dataset::UNKNOWN_VALUE ) {
                    testExamples[ inst ][j] = ( Utils::f_rand () < 0.5 )? example[ ex1 ][j]:Dataset::UNKNOWN_VALUE;
                }
                else {
                    rnd = Utils::f_rand ();
                    testExamples[ inst ][j] = example[ ex1 ][j] * rnd + example[ ex2 ][j] * ( 1 - rnd );
                }

            }

            // 3.5. Set the class.
            if ( isSMO && cClass == 0 ) {
                classOfTestExamples [ inst ] = -1;
            }
            else {
                classOfTestExamples [ inst ] = cClass;
            }

            // 3.6. Increment the index inst.
            inst ++;
        }
    }

} // end createExamplesByInterpolation


float* ComplexityMeasures::computeFractMaxCoveringSpheres () {

    // 0. Declare temporal variables.
    int i;
    int** neigh;                     // Nearest neighbor to each example.
    float** distNeigh;               // Distance to the nearest neighbor.
    float globalMinDist;             // Global minimum distance between pairs of examples of the same class.
    bool overlappedExamples = false; // Indicates whether there exist two examples of different class laying
                                     // in the exact same point of the feature space.
    float epsilon;                   // Maximum separation.

    int** adherenceOrder;
    int*  maxAdherenceOrder;

    // 1. Organize examples per class.
    organizePerClass ();

    // 2. Initialize variables that contain the neighbors and distances to them.
    neigh             = new int*   [ numberOfClasses ];
    distNeigh         = new float* [ numberOfClasses ];
    adherenceOrder    = new int*   [ numberOfClasses ];
    maxAdherenceOrder = new int    [ numberOfClasses ];

    for ( i = 0; i < numberOfClasses; i++ ) {
        neigh[i]          = new int   [ numberOfExamplesPerClass[i] ];
        distNeigh[i]      = new float [ numberOfExamplesPerClass[i] ];
        adherenceOrder[i] = new int   [ numberOfExamplesPerClass[i] ];
    }

    // 3. Search the nearest neighbors for each example.
    searchNearestNeighborsOfAnotherClass ( neigh, distNeigh, globalMinDist, overlappedExamples );

    // 4. Define the maximum separation permitted, epsilon.
    epsilon = ( ComplexityMeasures::EPSILON_SPHERES * globalMinDist );

    // 5. Search for the adherence subsets.
    calculateAdherenceSubsets ( adherenceOrder, maxAdherenceOrder, distNeigh, overlappedExamples, epsilon );

    // 6. Eliminate adherence subsets strictly included in another.
    eliminateAdherenceSetsIncluded ( adherenceOrder, maxAdherenceOrder, epsilon );

    // 7. Get statistics for the fraction of maximum covering spheres measure.
    float* valuesReturn = getStatisticsFractMaxCoveringSpheres ( adherenceOrder, maxAdherenceOrder );

    // 8. Delete all the auxiliar variables.
    for ( i = 0; i < numberOfClasses; i++ ) {
        delete [] neigh[i];
        delete [] distNeigh[i];
        delete [] adherenceOrder[i];
    }

    delete [] neigh;
    delete [] distNeigh;
    delete [] adherenceOrder;
    delete [] maxAdherenceOrder;

    return valuesReturn;

} // end computeFractMaxCoveringSpheres


void ComplexityMeasures::searchNearestNeighborsOfAnotherClass ( int** neigh, float** distNeigh,
        float& globalMinDist, bool& overlappedExamples ) {

    int cClass, oClass; // Current class (cClass) and opposite class (oClass).
    int i, j;
    float dist;

    globalMinDist = FLT_MAX;

    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Searching the nearest neighbors of another class " << std::endl;
    }

    for ( cClass = 0; cClass < numberOfClasses; cClass ++ ) {
        // Initialize to the farther possible distance.
        for ( i = 0; i < numberOfExamplesPerClass[ cClass ]; i++ ) {
            distNeigh[ cClass ][i] = FLT_MAX;
        }

        for ( oClass = 0; oClass < numberOfClasses; oClass++ ) {
            if ( oClass != cClass ) {
                for ( i = 0; i < numberOfExamplesPerClass[ cClass ]; i++ ) {

                    for ( j = 0; j < numberOfExamplesPerClass[ oClass ]; j++ ) {
                        dist = getDistance ( indexExamplesPerClass[ cClass ][i], indexExamplesPerClass[oClass][j] );

                        if ( dist < distNeigh[ cClass ][i] ) {
                            neigh[ cClass ][i]     = indexExamplesPerClass[ oClass ][j];
                            distNeigh[ cClass ][i] = dist;
                        }
                    }

                    // Update the minimum distance between pairs of examples of different classes.
                    if ( distNeigh[ cClass ][i] == 0 ) {
                        overlappedExamples = true;
                    }
                    else if ( globalMinDist > distNeigh[ cClass ][i] ) {
                        globalMinDist = distNeigh[ cClass ][i];
                    }
                }
            }
        }
    }
    
} // end searchNearestNeighborsOfAnotherClass


void ComplexityMeasures::calculateAdherenceSubsets ( int** adherenceOrder, int* maxAdherenceOrder,
        float** distNeigh, bool overlappedExamples, float epsilon ) {

    int cClass, i;

    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Calculating adherence subsets " << std::endl;
    }

    for ( cClass = 0; cClass < numberOfClasses; cClass ++ ) {
        maxAdherenceOrder[ cClass ] = 0;

        for ( i = 0; i < numberOfExamplesPerClass[ cClass ]; i++ ) {
            // If we find two overlapped cases, the adherence order of the example is zero.
            if ( overlappedExamples && distNeigh[ cClass ][i] == 0. ) {
                adherenceOrder[ cClass ][i] = 0;
            }
            else { // The nearest neighbor is not laying just in the same position.
                adherenceOrder[ cClass ][i] = ( int ) ( distNeigh[ cClass ][i] / epsilon ) - 1;
            }

            // Compute the maximum order per class.
            if ( adherenceOrder[ cClass ][i] > maxAdherenceOrder[ cClass ] ) {
                maxAdherenceOrder[ cClass ] = adherenceOrder[ cClass ][i];
            }
        }
    }
    
} // end calculateAdherenceSubsets


void ComplexityMeasures::eliminateAdherenceSetsIncluded ( int** adherenceOrder, int* maxAdherenceOrder,
                                                          float epsilon ) {

    int cClass, i, j;
    int maximum, nextMaximum;
    float difOfOrder, dist;

    if ( Utils::doScreenStatistics ) {
        std::cout << "      > Eliminating adherence subsets that are included in others " << std::endl;
    }

    for ( cClass = 0; cClass < numberOfClasses; cClass ++ ) {
        maximum = maxAdherenceOrder[ cClass ];

        // While we have a maximum adherence subset.
        while ( maximum >= 0 ) {

            // Search for all the sets with adherence order = maximum, and try to subsume the others to it.
            for ( i = 0; i < numberOfExamplesPerClass[ cClass ]; i++ ) {

                if ( adherenceOrder[ cClass ][i] == maximum ) { // Example really far from the boundary.

                    // Eliminate the sets that are stricly included in this set.
                    for ( j = 0; j < numberOfExamplesPerClass[ cClass ]; j++ ) {
                        difOfOrder = ( float ) ( adherenceOrder[ cClass ][i] - adherenceOrder[ cClass ][j] ) * epsilon;
                        dist = getDistance ( indexExamplesPerClass[ cClass ][i], indexExamplesPerClass[ cClass ][j] );

                        if ( dist < difOfOrder ) { // The adherence subset j is completely included in i.
                            // Remove adherence set since it is included in i.
                            adherenceOrder[ cClass ][j] = -1;
                        }
                    }
                }
            }

            // Search for the following maximum adherence set.
            nextMaximum = -1;

            for ( i = 0; i < numberOfExamplesPerClass[ cClass ]; i++ ) {
                if ( adherenceOrder[ cClass ][i] != -1 && adherenceOrder[ cClass ][i] < maximum
                        && adherenceOrder[ cClass ][i] > nextMaximum ) {

                    nextMaximum = adherenceOrder[ cClass ][i];
                }
            }

            // Set to maximum the next maximum adherence set order.
            maximum = nextMaximum;
        }
    }
    
} // end eliminateAdherenceSetsIncluded


float* ComplexityMeasures::getStatisticsFractMaxCoveringSpheres ( int** adherenceOrder, int* maxAdherenceOrder ) {

    int cClass, i;
    float sum = 0, sumsqr = 0, numOrders = 0;
    float* stats = new float [5];

    for ( cClass = 0; cClass < numberOfClasses; cClass ++ ) {
        for ( i = 0; i < numberOfExamplesPerClass[ cClass ]; i++ ) {
            if ( adherenceOrder[ cClass ][i] >= 0 ) {
                sum += ( float ) ( adherenceOrder[ cClass ][i] );
                sumsqr += ( float ) ( adherenceOrder[ cClass ][i] * adherenceOrder[ cClass ][i] );
                numOrders ++;
            }
        }
    }

    // 0. Number of adherence orders.
    stats[0] = numOrders;

    // 1. Mean of the order of the adherence sets.
    stats[1] = sum / numOrders;

    // 2. Standard deviation of the order of the adherence sets.
    stats[2] = sqrt ( ( sumsqr - sum * sum / numOrders ) / ( numOrders - 1 ) );

    // 3. Maximum order of class 0.
    stats[3] = maxAdherenceOrder [0];

    // 4. Maximum order of class 1.
    stats[4] = maxAdherenceOrder [1];

    return stats;
    
} // end getStatisticsFractMaxCoveringSpheres


//// MEASURES OF OVERLAPS IN FEATURE VALUES FROM DIFFERENT CLASSES

float ComplexityMeasures::computeFisher ( int& whichAttribute ) {

    if ( numberOfClasses == 2 ) {
        return computeFisher2Class ( whichAttribute );
    }
    else {
        // Recommend to apply the measure to two-class problems.
        if ( numberOfClasses != 2 ) {
            std::ostringstream stNumberOfClasses;
    	    stNumberOfClasses << numberOfClasses;
            std::string logMessage = "      > [WARNING F1] Applying the maximum Fisher discriminant ratio to a " + stNumberOfClasses.str () + "-class data set. \n ";
    	    logMessage += "        >> It is strongly recommended to activate option -d to consider the effect of each individual class. \n ";
    	    Utils::logWarningError ( logMessage );
    	}

        return computeFisherMClass ( whichAttribute );
    }

} // end computeFisher


float ComplexityMeasures::computeFisher2Class ( int& whichAttribute ) {

    int i;
    float fisherValue = -1;
    float tmp;
    bool atLeastOne = false;

    if ( numberOfClasses != 2 ) {
        std::string logMessage = " \n  > [ERROR F1] Fisher's discriminant can be applied to only two-class data sets. \n ";
        Utils::logWarningError ( logMessage );
        exit ( -1 );
    }

    whichAttribute = -1;

    for ( i = 0; i < numberOfAttributes; i++ ) {

        if ( avgRealValue[i][0] - avgRealValue[i][1] != 0  &&
                stdRealValue[i][0]  + stdRealValue[i][1] != 0 ) {
            atLeastOne = true;

            tmp = pow ( avgRealValue[i][0] - avgRealValue[i][1], 2 ) /
                  ( pow ( stdRealValue[i][0], 2 ) + pow ( stdRealValue[i][1], 2 ) );

            if ( fisherValue < tmp ) {
                fisherValue = tmp;
                whichAttribute = i;
            }
        }
    }

    if ( !atLeastOne ) {
        std::string logMessage = "      > [ERROR F1] All the instances are equal except for the class. \n ";
        logMessage += "        >> Do not consider the results obtained by Fisher Discriminant. \n ";
        Utils::logWarningError ( logMessage );
    }

    return fisherValue;

} // end computeFisher2Class


float ComplexityMeasures::computeFisherMClass ( int& whichAttribute ) {

    int c1, c2, att;
    float f, den, maxF1 = -1;
    bool atLeastOne = false;

    whichAttribute = -1;

    // Organize instances per class. If they are already organized nothing is done.
    organizePerClass ();

    for ( att = 0; att < numberOfAttributes; att++ ) {
        // 1. Calculate the numerator.
        f = den = 0;

        for ( c1 = 0; c1 < numberOfClasses; c1++ ) {
            den += ( ( float ) numberOfExamplesPerClass[ c1 ] / ( float ) numberOfExamples ) * pow ( stdRealValue[ att ][c1], 2 );

            for ( c2 = c1 + 1; c2 < numberOfClasses; c2++ ) {

                if ( c1 != c2 ) {
                    f += ( ( float ) numberOfExamplesPerClass[c1] / ( float ) numberOfExamples ) *
                         ( ( float ) numberOfExamplesPerClass[c2] / ( float ) numberOfExamples ) *
                         pow ( avgRealValue[att][c1] - avgRealValue[att][c2], 2 );
                }
            }
        }

        if ( f != 0 && den != 0 ) {
            atLeastOne = true;
            f = f / den;

            if ( f > maxF1 ) {
                maxF1 = f;
                whichAttribute = att;
            }
        }
    }

    if ( !atLeastOne ) {
        std::string logMessage = "      > [ERROR F1] All the instances are equal except for the class. \n ";
        logMessage += "        >> Do not consider the results obtained by Fisher Discriminant. \n ";
        Utils::logWarningError ( logMessage );
    }

    return maxF1;

} // end computeFisherMClass


float ComplexityMeasures::computeFisherVectorized () {

    Matrix *mu1, *mu2, *cov1, *cov2;
    float prop1, prop2;

    float fisherValue = -FLT_MAX;

    // 1. Check whether the number of classes is 2. If not, fail.
    if ( numberOfClasses != 2 ) {
        std::string logMessage = " \n  > [ERROR F1] Fisher's discriminant ratio can be applied to only two-class data sets. \n ";
        Utils::logWarningError ( logMessage );
        exit ( -1 );
    }

    // 2. Check that all classes have examples.
    organizePerClass ();

    // 3. Reserve memory for the matrices of means and covariances.
    mu1  = getMeanVectorOfClass ( 0 );
    mu2  = getMeanVectorOfClass ( 1 );
    cov1 = getCovarianceMatrixOfClass ( 0 );    
    cov2 = getCovarianceMatrixOfClass ( 1 );    

    // 4. Get the proportion of examples per class.
    prop1 = ( ( float ) numberOfExamplesPerClass[0] ) / ( ( float ) ( numberOfExamplesPerClass[0] + numberOfExamplesPerClass[1] ) );
    prop2 = ( ( float ) numberOfExamplesPerClass[1] ) / ( ( float ) ( numberOfExamplesPerClass[0] + numberOfExamplesPerClass[1] ) );

    // 5. Compute the direction vector d.
    try  {
        Matrix diffMu = ( *mu1 ) - ( *mu2 );
        Matrix sumCov = cov1->multScalar ( prop1 ) + cov2->multScalar ( prop2 );

        Matrix invSumCov = sumCov.computePseudoInverse();
        Matrix d         = invSumCov * diffMu;

        // 6. Compute Fisher.
        Matrix dT     = d.transpose ();
        // Matrix nom = (dT * diffMu) * (dT * diffMu).transpose();
        Matrix denom  = ( dT * sumCov ) * d;

        // 7. Get the value of Fisher --> By simplyfing the formulas we do not need it.
        // fisherValue = nom.getValue ( 0, 0 ) / denom.getValue ( 0, 0 );

        // 8. Delete memory.
        delete mu1;
        delete mu2;
        delete cov1;
        delete cov2;

        if ( std::isnan ( fisherValue ) || std::isnan ( denom.getValue ( 0, 0 ) ) ) {
            return 0;
        }
        else {
            return denom.getValue ( 0, 0 );
        }
    } catch ( const char* error ) {
        return -1;
    }

} // end computeFisherVectorized


float ComplexityMeasures::computeVolumeOverlap () {

    int att, c1, c2;
    float minMax, maxMin, maxMax, minMin;
    float twoClassF2, F2 = 0;

    for ( c1 = 0; c1 < numberOfClasses; c1 ++ ) {
        for ( c2 = c1 + 1; c2 < numberOfClasses; c2 ++ ) {

            twoClassF2 = 1;

            for ( att = 0; att < numberOfAttributes; att ++ ) {
               
                if ( maxAttValuePerClass[ att ][ c1 ] == -FLT_MAX ) {
                    std::string logMessage = "      > [ERROR F2] There are no examples that predict class ";
                    if ( classType == 'N' ) {
                        logMessage += nominalClassValues[ c1 ] + ". \n ";
                    }
                    else {
                        // Convert integer into string.
                        std::ostringstream stC1;
                        stC1 << c1;
                        logMessage += stC1.str () +  ". \n ";
                    }
                    logMessage += "        >> We disregard this class. The result will be inconsistent. \n ";
                    Utils::logWarningError ( logMessage );
                }
                else if ( maxAttValuePerClass[ att ][ c2 ] == -FLT_MAX ) {
                    std::string logMessage = "      > [ERROR F2] There are no examples that predict class ";
                    if ( classType == 'N' ) {
                        logMessage += nominalClassValues[ c2 ] + ". \n ";
                    }
                    else {
                        // Convert integer into string.
                        std::ostringstream stC2;
                        stC2 << c2;
                        logMessage += stC2.str () + ". \n ";
                    }
                    logMessage += "        >> We disregard this class. The result will be inconsistent. \n ";
                    Utils::logWarningError ( logMessage );
                }
                else {
                    minMax = Utils::min ( maxAttValuePerClass[ att ][ c1 ], maxAttValuePerClass[ att ][ c2 ] );
                    maxMin = Utils::max ( minAttValuePerClass[ att ][ c1 ], minAttValuePerClass[ att ][ c2 ] );
                    maxMax = Utils::max ( maxAttValuePerClass[ att ][ c1 ], maxAttValuePerClass[ att ][ c2 ] );
                    minMin = Utils::min ( minAttValuePerClass[ att ][ c1 ], minAttValuePerClass[ att ][ c2 ] );

                    if ( maxMax != minMin ) {
                        twoClassF2 *= ( minMax - maxMin ) / ( maxMax - minMin );
                    }
                    else { // Division by zero.
                        // Convert integer into string.
                    	std::ostringstream stAtt;
                    	stAtt << att;
                    	std::string logMessage = "      > [WARNING F2] Attribute " + stAtt.str () + ": constant for classes ";
                        if ( classType == 'N' ) { 
                            logMessage += nominalClassValues[ c1 ] + " and ";
                            logMessage += nominalClassValues[ c2 ] + ". \n ";
                            Utils::logWarningError( logMessage );
                        }
                        else {
                            // Convert integer into string.
                            std::ostringstream stC1;
                            std::ostringstream stC2;
                            stC1 << c1;
                            stC2 << c2;
                            logMessage += stC1.str () + " and ";
                            logMessage += stC2.str() + ". \n ";
                            Utils::logWarningError( logMessage );
                        }
                    }
                }
            }

            F2 += fabs ( twoClassF2 ); // Be aware. This is the union!! May be other operators (not +) are better.
        }
    }

    return F2;
    
} // end computeVolumeOverlap


float* ComplexityMeasures::computeMaximumEfficiencyOfAttributes ( int& mostDiscrAtt, float& discPowerOfTheBest ) {

    int i, j;
    int bestAtt;
    bool finish = false;
    float overlapMin, overlapMax;
    float* discPower, *initialDiscPower;  // Both variables store the discriminative power of each attribute.
    float* cumulDiscPower;                // Cumulative discriminant power (considering more discriminant variables).
    int* order;                           // Maintain the order of the attributes.
    bool* pointDisc;                      // Indicate whether the point has been discriminated.
    int numAttRemain, numExRemain;        // Number of attributes and examples that remain to process.
    float **minValue, **maxValue;         // Minimum and maximum value per attribute and class.

    mostDiscrAtt = 0;
    discPowerOfTheBest = 0;

    // Recommend to apply the measure to two-class problems.
    if ( numberOfClasses != 2 ) {
    	std::ostringstream stNumberOfClasses;
        stNumberOfClasses << numberOfClasses;

    	std::string logMessage = "      > [WARNING F3] Applying Maximum efficiency to a " + stNumberOfClasses.str () + "-class data set. \n ";
        logMessage += "        >> The meaning of the result provided by this measure may not reflect the efficiency for each class " ;
        logMessage += "since averages are provided. \n ";
        logMessage += "        >> It is strongly recommended to activate option -d to consider the effect of each individual class. \n ";
        Utils::logWarningError ( logMessage );
    }

    // 0. Organize examples per class.
    organizePerClass ();

    // 1. Initialize variables.
    numAttRemain     = numberOfAttributes;
    numExRemain      = numberOfExamples;
    initialDiscPower = new float  [ numberOfAttributes ];
    discPower        = new float  [ numberOfAttributes ];
    cumulDiscPower   = new float  [ numberOfAttributes ];
    order            = new int    [ numberOfAttributes ];
    minValue         = new float* [ numberOfAttributes ];
    maxValue         = new float* [ numberOfAttributes ];

    for ( i = 0; i < numberOfAttributes; i++ ) {
        discPower[i]        = 0;
        initialDiscPower[i] = 0;
        cumulDiscPower[i]   = 0;
        order[i]            = i;
        
        minValue[i] = new float [ numberOfClasses ];
        maxValue[i] = new float [ numberOfClasses ];

        for ( j = 0; j < numberOfClasses; j++ ) {
            minValue[i][j] = FLT_MAX;
            maxValue[i][j] = -FLT_MAX;
        }
    }

    pointDisc = new bool [ numberOfExamples ];

    for ( i = 0; i < numberOfExamples;  i++ ) {
        pointDisc[i] = false;
    }

    while ( !finish ) {

        // 2.1. Get the discriminative power of each of the remaining attributes by analyzing the
        //      number of overlapped points between the two classes for each attribute.
        //      We finish if we find an attribute that completely discriminates the classes.
        finish = getDiscriminativePowerOfAttributes ( discPower, order, numAttRemain, minValue, maxValue, pointDisc );

        // 2.2. If we are in the first iteration, we store the initial discriminatory power of each attribute.
        if ( numAttRemain == numberOfAttributes ) {
            for ( i = 0; i < numberOfAttributes; i++ ) {
                initialDiscPower[i] = discPower[i];
            }
        }

        // 2.3. Sort the attributes depending on their discriminatory power.
        Utils::quickSort ( discPower, order, 0, numAttRemain - 1 );

        // 2.4. Update the cumulative disciminatory power of the best attribute.
        cumulDiscPower[ order[ numAttRemain - 1 ] ] = discPower[ numAttRemain - 1 ];

        // 2.5. Store the most discriminative attribute if it is the first iteration.
        if ( numAttRemain == numberOfAttributes ) {
            // The most discriminative attribute is the last in the vector.
            mostDiscrAtt = order[ numAttRemain - 1 ];
            discPowerOfTheBest = discPower [ numAttRemain - 1 ] / numberOfExamples;
        }

        // 2.6. Use the first attribute. So, one less attribute remaining.
        numAttRemain --;

        // 2.7. Reset the discriminatory power of unused variables.
        for ( i = 0; i < numAttRemain; i++ ) {
            discPower [i] = 0;
        }

        // 2.8. Check the number of examples that we can discriminate according to the
        //      most discriminative attribute.
        bestAtt = order[ numAttRemain ];

        overlapMin = Utils::max ( minValue[ bestAtt ][0], minValue[ bestAtt ][1] );
        overlapMax = Utils::min ( maxValue[ bestAtt ][0], maxValue[ bestAtt ][1] );

        for ( i = 0; i < numberOfExamples; i++ ) {
            if ( !pointDisc[i] &&
                    ( example[i][ bestAtt ] < overlapMin ||
                      example[i][ bestAtt ] > overlapMax ) ) {
                pointDisc[i] = true;
                numExRemain --;
            }
        }

        if ( numExRemain == 0 || numAttRemain == 0 ) finish = true;
    }

    // Delete memory.
    for ( i = 0; i < numberOfAttributes; i++ ) {
        delete [] minValue[i];
        delete [] maxValue[i];
    }

    delete [] discPower;
    delete [] initialDiscPower;
    delete [] pointDisc;
    delete [] order;
    delete [] minValue;
    delete [] maxValue;

    return cumulDiscPower;
    
} // end computeMaximumEfficiencyOfAttributes


bool ComplexityMeasures::getDiscriminativePowerOfAttributes ( float* discPower, int* order,
        int numAttRemain, float** minValue, float** maxValue, bool* pointDisc ) {

    int i, j, att;
    float overlapMin, overlapMax;
    bool finish = false;

    for ( j = 0; j < numAttRemain; j++ ) {

        att = order[j];

        for ( i = 0; i < numberOfClasses; i++ ) {
            minValue[ att ][i] = FLT_MAX;
            maxValue[ att ][i] = -FLT_MAX;
        }

        for ( i = 0; i < numberOfExamples; i++ ) {

            if ( !pointDisc[i] && example[i][ att ] != Dataset::UNKNOWN_VALUE ) { // If the point has not been discriminated already.
                if ( example[i][ att ] < minValue[ att ][ classOfExample[i] ] ) {
                    minValue[ att ][ classOfExample[i] ] = example[i][ att ];
                }

                if ( example[i][ att ] > maxValue[ att ][ classOfExample[i] ] ) {
                    maxValue[ att ][ classOfExample[i] ] = example[i][ att ];
                }
            }

        }

        // Get the maximum of the minimums, and the minimum of the maximums.
        overlapMin = minValue[ att ][0];
        overlapMax = maxValue[ att ][0];

        for ( i = 1; i < numberOfClasses; i++ ) {
            if ( minValue[ att ][i] > overlapMin ) {
                overlapMin = minValue[ att ][i];
            }
            if ( maxValue[ att ][i] < overlapMax ) {
                overlapMax = maxValue[ att ][i];
            }
        }

        if ( overlapMin > overlapMax ) { // The attribute completely discriminates all the examples per class.
            discPower[j] = numberOfExamples;

            // Substract all the examples that have been discriminated.
            for ( i = 0; i < numberOfAttributes; i++ ) {
                if ( i != j ) { 
                    discPower[j] -= discPower[i];
                }
            }
            finish = true;
        }
        else { // Count the number of examples that are truly discriminated by the attribute.

            for ( i = 0; i < numberOfExamples; i++ ) {
                if ( !pointDisc[i] &&
                        ( example[i][ att ] < overlapMin ||
                          example[i][ att ] > overlapMax ||
                          example[i][ att ] == Dataset::UNKNOWN_VALUE ) ) {
                    discPower[j] ++;
                }
            }
        }
    }

    return finish;
    
} // end getDiscriminativePowerOfAttributes


float ComplexityMeasures::averageNumberOfSamplesPerDimension () {
    return ( ( float ) numberOfExamples ) / ( ( float ) numberOfAttributes );
} // end averageNumberOfSamplesPerDimension
