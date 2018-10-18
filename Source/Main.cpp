
/*!

\brief The main file contains the implementation of the methods that gather the configuration of the run and control it.

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
#include "InputOptions.h"
#include "ResultsContainer.h"

#include <iomanip>

#define WIDTH_RES 12
#define NUM_RESULTS 14
#define NUM_ATT_RESULTS 1
#define DECIMAL_PRECISION 3
#define MIN_PRINTFLOAT 0.0009

#define CF1  0
#define CF1v 1
#define CF2  2
#define CF3  3
#define CF4  4
#define CL1  5
#define CL2  6
#define CL3  7
#define CN1  8
#define CN2  9
#define CN3 10
#define CN4 11
#define CT1 12
#define CT2 13

#define AF3 0


/**
 * Prints the legend to the output file.
 * @param opts is the structure that maintains the input parameters.
 * @param fout is the output file where the legend will be printed.
 */
void printLegend ( InputOptions& opts, std::ofstream& fout ) {

    fout << "TABLE LEGEND" << std::endl;
    if ( opts.getF1 () )  fout << "  F1:  Maximum Fisher's discriminant ratio " << std::endl;
    if ( opts.getF1v () ) fout << "  F1v: Directional-vector maximum Fisher's discriminant ratio " << std::endl;
    if ( opts.getF2 () )  fout << "  F2:  Overlap of the per-class bounding boxes " << std::endl;
    if ( opts.getF3 () )  fout << "  F3:  Maximum (individual) feature efficiency " << std::endl;
    if ( opts.getF4 () )  fout << "  F4:  Collective feature efficiency (sum of each feature efficiency) " << std::endl;
    if ( opts.getL1 () )  fout << "  L1:  Minimized sum of the error distance of a linear classifier (linear SMO) " << std::endl;
    if ( opts.getL2 () )  fout << "  L2:  Training error of a linear classifier (linear SMO) " << std::endl;
    if ( opts.getL3 () )  fout << "  L3:  Nonlinearity of a linear classifier (linear SMO) " << std::endl;
    if ( opts.getN1 () )  fout << "  N1:  Fraction of points on the class boundary " << std::endl;
    if ( opts.getN2 () )  fout << "  N2:  Ratio of average intra/inter class nearest neighbor distance " << std::endl;
    if ( opts.getN3 () )  fout << "  N3:  Leave-one-out error rate of the one-nearest neighbor classifier " << std::endl;
    if ( opts.getN4 () )  fout << "  N4:  Nonlinearity of the one-nearest neighbor classifier " << std::endl;
    if ( opts.getT1 () )  fout << "  T1:  Fraction of maximum covering spheres " << std::endl;
    if ( opts.getT2 () )  fout << "  T2:  Average number of points per dimension " << std::endl;

    /** 
    [NEW COMPLEXITY MEASURE]
    In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
    if ( opts.get[Label of the new measure] () ) fout << " [Label of the new measure]: [Name of the new measure] " << std::endl;
    */

    fout << std::endl << std::endl;
    fout << "DATA SET" << "                     " ;

    if ( opts.getF1 () )  fout << std::setw ( WIDTH_RES ) << "F1";
    if ( opts.getF1v () ) fout << std::setw ( WIDTH_RES ) << "F1v";
    if ( opts.getF2 () )  fout << std::setw ( WIDTH_RES ) << "F2";
    if ( opts.getF3 () )  fout << std::setw ( WIDTH_RES ) << "F3";
    if ( opts.getF4 () )  fout << std::setw ( WIDTH_RES ) << "F4";
    if ( opts.getL1 () )  fout << std::setw ( WIDTH_RES ) << "L1";
    if ( opts.getL2 () )  fout << std::setw ( WIDTH_RES ) << "L2";
    if ( opts.getL3 () )  fout << std::setw ( WIDTH_RES ) << "L3";
    if ( opts.getN1 () )  fout << std::setw ( WIDTH_RES ) << "N1";
    if ( opts.getN2 () )  fout << std::setw ( WIDTH_RES ) << "N2";
    if ( opts.getN3 () )  fout << std::setw ( WIDTH_RES ) << "N3";
    if ( opts.getN4 () )  fout << std::setw ( WIDTH_RES ) << "N4";
    if ( opts.getT1 () )  fout << std::setw ( WIDTH_RES ) << "T1";
    if ( opts.getT2 () )  fout << std::setw ( WIDTH_RES ) << "T2";

    /** 
    [NEW COMPLEXITY MEASURE]
    In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
    if ( opts.get[Label of the new measure] () ) fout << std::setw ( WIDTH_RES ) << "[Label of the new measure]";
    */

    fout << std::endl;
    
} // end printLegend


/**
 * Prints the legend to the screen.
 * @param opts contains the run options.
 */
void printLegendToScreen ( InputOptions& opts ) {
    
    std::cout << "TABLE LEGEND" << std::endl;

    if ( opts.getF1 () )  std::cout << "  F1: Maximum Fisher's discriminant ratio " << std::endl;
    if ( opts.getF1v () ) std::cout << "  F1v: Directional-vector maximum Fisher's discriminant ratio " << std::endl;
    if ( opts.getF2 () )  std::cout << "  F2: Overlap of the per-class bounding boxes " << std::endl;
    if ( opts.getF3 () )  std::cout << "  F3: Maximum (individual) feature efficiency " << std::endl;
    if ( opts.getF4 () )  std::cout << "  F4: Collective feature efficiency (sum of each feature efficiency) " << std::endl;
    if ( opts.getL1 () )  std::cout << "  L1: Minimized sum of the error distance of a linear classifier (linear SMO) " << std::endl;
    if ( opts.getL2 () )  std::cout << "  L2: Training error of a linear classifier (linear SMO) " << std::endl;
    if ( opts.getL3 () )  std::cout << "  L3: Nonlinearity of a linear classifier (linear SMO) " << std::endl;
    if ( opts.getN1 () )  std::cout << "  N1: Fraction of points on the class boundary " << std::endl;
    if ( opts.getN2 () )  std::cout << "  N2: Ratio of average intra/inter class nearest neighbor distance " << std::endl;
    if ( opts.getN3 () )  std::cout << "  N3: Leave-one-out error rate of the one-nearest neighbor classifier " << std::endl;
    if ( opts.getN4 () )  std::cout << "  N4: Nonlinearity of the one-nearest neighbor classifier " << std::endl;
    if ( opts.getT1 () )  std::cout << "  T1: Fraction of maximum covering spheres " << std::endl;
    if ( opts.getT2 () )  std::cout << "  T2: Average number of points per dimension " << std::endl;

    /** 
    [NEW COMPLEXITY MEASURE]
    In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
    if ( opts.get[Label of the new measure] () ) std::cout << " [Label of the new measure]: [Name of the new measure] " << std::endl;
    */

    std::cout << std::endl << std::endl;
    std::cout << "DATA SET" << "                     " ;

    if ( opts.getF1 () )  std::cout << std::setw ( WIDTH_RES ) << "F1";
    if ( opts.getF1v () ) std::cout << std::setw ( WIDTH_RES ) << "F1v";
    if ( opts.getF2 () )  std::cout << std::setw ( WIDTH_RES ) << "F2";
    if ( opts.getF3 () )  std::cout << std::setw ( WIDTH_RES ) << "F3";
    if ( opts.getF4 () )  std::cout << std::setw ( WIDTH_RES ) << "F4";
    if ( opts.getL1 () )  std::cout << std::setw ( WIDTH_RES ) << "L1";
    if ( opts.getL2 () )  std::cout << std::setw ( WIDTH_RES ) << "L2";
    if ( opts.getL3 () )  std::cout << std::setw ( WIDTH_RES ) << "L3";
    if ( opts.getN1 () )  std::cout << std::setw ( WIDTH_RES ) << "N1";
    if ( opts.getN2 () )  std::cout << std::setw ( WIDTH_RES ) << "N2";
    if ( opts.getN3 () )  std::cout << std::setw ( WIDTH_RES ) << "N3";
    if ( opts.getN4 () )  std::cout << std::setw ( WIDTH_RES ) << "N4";
    if ( opts.getT1 () )  std::cout << std::setw ( WIDTH_RES ) << "T1";
    if ( opts.getT2 () )  std::cout << std::setw ( WIDTH_RES ) << "T2";

    /** 
    [NEW COMPLEXITY MEASURE]
    In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
    if ( opts.get[Label of the new measure] () ) std::cout << std::setw ( WIDTH_RES ) << "[Label of the new measure]";
    */

    std::cout << std::endl;
    
} // end printLegendToScreen


/**
 * Runs the complexity measures required by the user for a single data set.
 * @param dSet is the data set.
 * @param opts is the run options.
 * @return A float* with the results.
 */
Results runMeasuresForASingleDataset ( ComplexityMeasures* dSet, InputOptions& opts ) {
   
    float* vectorResults;
    float  measureResult;
    float  measureResultAux;
    float* results = new float [ NUM_RESULTS ];
    float** attResults = new float* [ NUM_ATT_RESULTS ];

    for (int i=0; i < NUM_ATT_RESULTS; ++i) {
        attResults[i] = new float [dSet->getNumberOfAttributes()];
    }

    int    att, i;
    std::string logMessage;

    // For the support vector machine, we need the weights (w) and the offset (b).
    float* w = 0;
    float b = 0;

    // Initialize the results to -1 to indicate that they are not valid.
    for ( i = 0; i < NUM_RESULTS; i++ ) {
        results[i] = -1;
    }

    if ( opts.getF1 () ) {
        std::cout << std::endl << "    > Processing the maximum Fisher's discriminant ratio (F1) " << std::endl;

        results[ CF1 ] = dSet->computeFisher ( att );
        std::cout << "      > Fisher discr. ratio: " << results[ CF1 ] << std::endl;
    }

    if ( opts.getF1v () ) {
        std::cout << std::endl << "    > Processing the directional-vector maximum Fisher's discriminant ratio (F1v) " << std::endl;
        if ( dSet->getNumberOfClasses() != 2 ) {
            logMessage = "";
            logMessage = "      > [ERROR F1v] The directional-vector maximum Fisher's discriminant ratio can only be applied to two-class data sets. \n ";
            Utils::logWarningError ( logMessage );
        }
        else {
            results[ CF1v ] = dSet->computeFisherVectorized ();
            std::cout << "      > Directional-vector maximum Fisher discr. ratio: " << results[ CF1v ] << std::endl;
        }
    }

    if ( opts.getF2 () ) {
        std::cout << std::endl << "    > Processing the overlap of per-class bounding boxes (F2) " << std::endl;
        results[ CF2 ] = dSet->computeVolumeOverlap();
        std::cout << "      > Volume overlap: " << results[ CF2 ] << std::endl;
    }

    if ( opts.getF3 () ) {
        std::cout << std::endl << "    > Processing the efficiency of attributes (F3/F4) " << std::endl;
        vectorResults = dSet->computeMaximumEfficiencyOfAttributes ( att, measureResult );
        measureResultAux = 0;

        for ( i = 0; i < dSet->getNumberOfAttributes (); i++ ) {
            measureResultAux += vectorResults[i] / dSet->getNumberOfExamples ();
            std::cout << "      > Discriminant power of attribute " << i << ": " << ( vectorResults[i] / dSet->getNumberOfExamples() ) << std::endl;
            attResults[ AF3 ][i] = vectorResults[i] / dSet->getNumberOfExamples();
        }

        std::cout <<  "      > Maximum (individual) efficiency of attributes: " << measureResult << std::endl;
        results[ CF3 ] = measureResult;
        attResults[ AF3 ] = vectorResults;
        delete [] vectorResults;

        if ( opts.getF4 () ) {
            std::cout <<  "      > Collective efficiency of attributes: " << measureResultAux << std::endl;
            results[ CF4 ] = measureResultAux;
        }
    }

    if ( opts.getF4 () && !opts.getF3 () ) {
        std::cout << std::endl << "    > Processing the efficiency of attributes (F3/F4) " << std::endl;
        vectorResults = dSet->computeMaximumEfficiencyOfAttributes ( att, measureResult );
        measureResult = 0;

        for ( i = 0; i < dSet->getNumberOfAttributes(); i++ ) {
            std::cout << "      > Discriminant power of attribute " << i << ": " << ( vectorResults[i] / dSet->getNumberOfExamples () ) << std::endl;
            measureResult += vectorResults[i] / dSet->getNumberOfExamples ();
        }

        std::cout <<  "      > Collective efficiency of attributes: " << measureResult << std::endl;
        delete [] vectorResults;
        results[ CF4 ] = measureResult;
    }

    if ( opts.getN1 () ) {
        std::cout << std::endl << "    > Processing the fraction of points on the class boundary (N1) " << std::endl;

        int** spanningTree = dSet->computePrim ();
        results[ CN1 ] = dSet->computeBoundary ( spanningTree );
        std::cout << "      > Fraction of points on the class boundary: " << results[ CN1 ] << std::endl;

        for ( i = 0; i < dSet->getNumberOfExamples () - 1; i++ ) {
            delete [] spanningTree[i];
        }

        delete [] spanningTree;
    }

    if ( opts.getN2 () ) {
        std::cout << std::endl << "    > Processing the ratio of average intra/inter class nearest neighbor distance (N2) " << std::endl;
        results[ CN2 ] = dSet->computeIntraInter ();
        std::cout << "      > Ratio of average intra/inter distance: " << results[ CN2 ] << std::endl;
    }

    if ( opts.getN3 () ) {
        std::cout << std::endl << "    > Processing the leave-one-out error rate of the one-nearest neighbor classifier (N3) " << std::endl;
        results[ CN3 ] = dSet->computeNonLinearityKNNTrain ();
        std::cout << "      > Error rate of the 1NN: " << results[ CN3 ] << std::endl;
    }

    if ( opts.getN4 () ) {
        std::cout << std::endl << "    > Processing the nonlinearity of the one-nearest neighbor classifier (N4) " << std::endl;
        results[ CN4 ] = dSet->computeNonLinearityKNNConvexHull ();
        std::cout << "      > Nonlinearity of the 1NN: " << results[ CN4 ] << std::endl;
    }

    if ( opts.getT1() ) {
        std::cout << std::endl << "    > Processing the fraction of maximum covering spheres (T1) " << std::endl;
        vectorResults = dSet->computeFractMaxCoveringSpheres();
        std::cout << "      > Fraction max. cov. spheres (numSpheres - meanSpheres - stdSpheres): " ;

        results[ CT1 ] = vectorResults[0] / dSet->getNumberOfExamples ();
        std::cout << results[ CT1 ] << " ( " ;
        for ( i = 0; i < 3; i++ ) std::cout << vectorResults[i] << " " ;
        std::cout << ") " << std::endl;
        delete [] vectorResults;
    }

    if ( opts.getT2 () ) {
        std::cout << std::endl << "    > Processing the average number of points per dimension (T2) " << std::endl;
        results[ CT2 ] = dSet->averageNumberOfSamplesPerDimension ();
        std::cout << "      > Number of points per dimension: " << results[ CT2 ] << std::endl;
    }

    if ( ( opts.getL1 () || opts.getL2 () || opts.getL3 () ) && dSet->getNumberOfClasses () == 2 ) {
        std::cout << std::endl << "    > Running SMO to get the linear classifier " << std::endl;
        w = dSet->trainSMO ( b );
    }

    if ( opts.getL1 () ) {
        std::cout << std::endl << "    > Processing the minimized sum of the error distance of a linear classifier (Linear SMO) (L1) " << std::endl;

        if ( dSet->getNumberOfClasses () != 2 ) {
            // Convert integer into string.
            std::ostringstream stNumberOfClasses;
            stNumberOfClasses << dSet->getNumberOfClasses ();
            logMessage = "";
            logMessage = "      > [ERROR L1] The linear discriminant can only be applied to two-class data sets. \n ";
            logMessage += "        >> The current data set '" + dSet->getName () ;
            logMessage += "' has " + stNumberOfClasses.str () + "; thus, the measure will not be applied. \n ";
            Utils::logWarningError ( logMessage );
            results[ CL1 ] = -1;
        }
        else {
            results[ CL1 ] = dSet->computeNonLinearityLCDistance ( w, b );
            std::cout << "      > Error distance of a LC: " << results[ CL1 ] << std::endl;
        }
    }

    if ( opts.getL2 () ) {
        std::cout << std::endl << "    > Processing the training error of a linear classifier (Linear SMO) (L2) " << std::endl;

        if ( dSet->getNumberOfClasses () != 2 ) {
            // Convert integer into string.
            std::ostringstream stNumberOfClasses;
            stNumberOfClasses << dSet->getNumberOfClasses ();
            logMessage = "";
            logMessage = "      > [ERROR L2] The linear discriminant can only be applied to two-class data sets. \n ";
            logMessage += "        >> The current data set '" + dSet->getName () ;
            logMessage += "' has " + stNumberOfClasses.str () + "; thus, the measure will not be applied. \n ";
            Utils::logWarningError ( logMessage );
            results[ CL2 ] = -1;
        }
        else {
            results[ CL2 ] = dSet->computeNonLinearityLCTrain ( w, b );
            std::cout << "      > Training error of a LC: " << results[ CL2 ] << std::endl;
        }
    }

    if ( opts.getL3 () ) {
        std::cout << std::endl << "    > Processing the nonlinearity of a linear classifier (Linear SMO) (L3) " << std::endl;

        if ( dSet->getNumberOfClasses() != 2 ) {
            // Convert integer into string.
            std::ostringstream stNumberOfClasses;
            stNumberOfClasses << dSet->getNumberOfClasses ();
            logMessage = "";
            logMessage = "      > [ERROR L3] The linear discriminant can only be applied to two-class data sets. \n ";
            logMessage += "        >> The current data set '" + dSet->getName ();
            logMessage += "' has " + stNumberOfClasses.str () + "; thus, the measure will not be applied. \n ";
            Utils::logWarningError ( logMessage );
            results[ CL3 ] = -1;
        }
        else {
            results[ CL3 ] = dSet->computeNonLinearityLCConvexHull ( w, b );
            std::cout << "      > Nonlinearity of a LC: " << results[ CL3 ] << std::endl;
        }
    }

    if ( ( opts.getL1 () || opts.getL2 () || opts.getL3 () ) && dSet->getNumberOfClasses () == 2 ) {
        std::cout << std::endl << "    > Destroying the support vector machine " << std::endl;
        delete [] w;
    }

    /** 
    [NEW COMPLEXITY MEASURE]
    In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
    if ( opts.get[Label of the new measure] () ) {
        std::cout << std::endl << "    > Processing the [Name of the new measure] ([Label of the new measure]) " << std::endl;
        results[ [Counter] ] = dSet->compute[Name of the new measure] ();
        std::cout << "      > [Measure]: " << results[ [Counter] ] << std::endl;
    */

    delete dSet;

    std::cout << std::endl << "  > END OF COMPLEXITY MEASURES PROCESSING " << std::endl << std::endl;
    
    return Results(results, attResults, dSet->getNumberOfAttributes ());

} // end runMeasuresForASingleDataset


/**
 * Runs the measures for a data set. If the data set has two classes or the
 * option -t2class (that is, discriminate each pair of classes) has not been selected,
 * the complexity measures are run on the data set.
 * If the option -t2class is selected and the data set has more than two classes,
 * each pair of classes of the data set with m classes are discriminated,
 * resulting in m data sets, and the complexity measures are run for each data set.
 * @param opts contains the run options.
 * @param datasetNames is a vector with the names of the processed data sets [output parameter].
 * @param numberOfDatasets is an integer that returns the number of data sets.
 * @return A float** with the results for each data set.
 */
Results* runMeasures ( InputOptions& opts, std::string*& datasetNames, int& numberOfDatasets ) {

    int i;
    ComplexityMeasures* dSet;
    Results* results;
    std::string sAux;

    // 1. Read the input data set: read attribute information and replace unknown values.
    std::cout << std::endl << std::endl << "  > PROCESSING COMPLEXITY MEASURES ON DATA SET: " << opts.getInputDatasetName () << std::endl;

    dSet = new ComplexityMeasures ( opts.getInputDatasetName (), true, opts.getReplaceUnknownValues (),
                                   opts.getTypeOfContinuousDistFunction (), opts.getTypeOfNominalDistFunction () );

    // 2. Analyze whether we have to discriminate the data set in classes.
    if ( opts.doDiscriminateClasses () && dSet->getNumberOfClasses () > 2 ) {

        // 2.1. Get the number of data sets after discriminating them.
        numberOfDatasets = dSet->getNumberOfClasses();

        sAux = opts.getOutputDatasetName ();
        opts.setOutputDatasetName ( opts.getInputDatasetName () );
        datasetNames = dSet->generate2ClassDatasets ( opts.getOutputDatasetName () );
        opts.setOutputDatasetName ( sAux );
        delete dSet;

        // 2.2. Reserve memory for the table of statistics.
        results = new Results [ numberOfDatasets ];

        // 2.3. Get the input data set name.
        sAux = opts.getInputDatasetName ();

        // 2.4. Get the results of the complexity measures per each data set.
        for ( i = 0; i < numberOfDatasets; i++ ) {

            opts.setInputDatasetName ( datasetNames[i] );
            dSet = new ComplexityMeasures ( opts.getInputDatasetName (), true, opts.getReplaceUnknownValues (),
                                           opts.getTypeOfContinuousDistFunction (), opts.getTypeOfNominalDistFunction () );

            results[i] = runMeasuresForASingleDataset ( dSet, opts );
        }

        // 2.5. Restore the initial data set name.
        opts.setInputDatasetName ( sAux );
    }
    else {
        // 2.1. Reserve memory for one data set.
        numberOfDatasets = 1;
        results = new Results [ numberOfDatasets ];
        datasetNames = new std::string [ numberOfDatasets ];

        // 2.2.  Get the results.
        datasetNames[0] = opts.getInputDatasetName ();
        results[0] = runMeasuresForASingleDataset ( dSet, opts );
    }

    return results;

} // end runMeasures


/**
 * Runs routines that are not complexity measures. These include:
 * cross-validation, transformation into two-class data sets, and printing data sets.
 * @param opts contains the run options.
 */
void runOtherRoutines ( InputOptions& opts ) {

    ComplexityMeasures* dSet;
    std::string* aux;
    
    // Run other functionalities.
    if ( opts.getRunCrossValidation () || opts.getTransformTo2ClassData () || opts.getPrintNormalizedDataset () ) {

        try {
            // 1. Read the input data set: read attribute information and replace unknown values.
            dSet = new ComplexityMeasures ( opts.getInputDatasetName (), true, false,
                                           ExtendedDataset::EUCLIDEAN, opts.getTypeOfNominalDistFunction () );

            // Conditions concerning k-fold cross-validation and partitioning.
            if ( opts.getRunCrossValidation () ) {
                std::cout << std::endl << std::endl << "  > RUNNING CROSS VALIDATION " << std::endl;
                dSet->stratifiedCrossValidation ( opts.getFoldsCV (), opts.getOutputDatasetName () );
                dSet->deleteExamplesPerClass ();
                std::cout << std::endl << "  > END OF RUNNING CROSS VALIDATION " << std::endl << std::endl;
            }

            if ( opts.getTransformTo2ClassData () ) {
                std::cout << std::endl << std::endl <<  "  > TRANSFORMING M-CLASS DATA SET INTO M TWO-CLASS DATA SETS " << std::endl << std::endl;
                aux = dSet->generate2ClassDatasets ( opts.getOutputDatasetName () );
                std::cout << std::endl << "  > END OF TRANSFORMING M-CLASS DATA SET INTO M TWO-CLASS DATA SETS " << std::endl << std::endl;
                delete [] aux;
            }

            if ( opts.getPrintNormalizedDataset () ) {
                std::ofstream fout;
                fout.open ( opts.getOutputDatasetName ().c_str (), std::ofstream::out );
                dSet->print ( fout, true );
                fout.close ();
            }

            delete dSet;
        }
        catch ( std::string error ) {
            std::string logMessage = "\n>> [ERROR DATA SET FORMAT]: The data set " + opts.getInputDatasetName() ;
            logMessage += " does not exist or it is incorrectly formatted. \n ";
            logMessage += " \n > Did you wanted to run in batch mode? If so, add option -B. \n ";
            logMessage += "  > Otherwise, please verify that the data set '" + opts.getInputDatasetName () + "' ";
            logMessage += "follows either the KEEL or the WEKA format. \n ";
            Utils::logWarningError ( logMessage );
        }

    }

} // end runOtherRoutines


/**
 * Prints the results obtained from a run with multiple data sets to a LaTeX-formatted file.
 * @param opts contains the run options.
 * @param results contains the results of the complexity measures for each data set.
 * @see runInMultipleDatasetMode.
 */
void printLatexFile ( InputOptions& opts, ResultsContainer& results ) {

    int i;

    // Show a message to correct the .tex file.
    std::string logMessage = "  > [WARNING LATEX FILE] The table of results may be too wide. \n ";
    logMessage += "    >> Reduce the font size: \\scriptsize or \\tiny. \n ";
    logMessage += "    >> Adjust the value of the left margin to center the table. \n ";
    Utils::logWarningError ( logMessage );

    // Print the complexity measures results to the output file.
    std::ofstream fout;
    fout.open ( opts.getOutputDatasetName ().append ( ".tex" ).c_str (), std::ofstream::out );
    float* dsResults;
    Results resultsOneRun;

    // Latex file header and packages.
    fout << "\\documentclass{article}" << std::endl;
    fout << "\\usepackage{longtable}" << std::endl << std::endl;
    fout << "\\begin{document}" << std::endl << std::endl;
 
    // Table environment. 
    fout << "\\begin{center}" << std::endl;
 
    fout << "\\footnotesize{             \% Font size. Reduce the font size (\\scriptsize or \\tiny), if the table is too wide." << std::endl;

    fout << "\\setlength\\LTleft{-100pt}  \% Left margin. Adjust the value to center the table." << std::endl << std::endl;


    fout << "\\begin{longtable}{l" ;
    for ( i = 0; i < opts.getNumberOfComplexityMeasuresToCompute (); i++ ) {
       fout << "r" ;
    }
    fout << "}" << std::endl << std::endl;

    // Table header. 
    // First header.
    fout << "% First header" << std::endl; 
    fout << "\\caption[Complexity measures]{Summary of the complexity measures computation} \\" << "\\" << std::endl;
    fout << "\\label{tab:CMresults} \\" << "\\" << std::endl;

    fout << "\\hline" << std::endl;
    fout << "\\textbf{Data set name}" ;  
    if ( opts.getF1 () )  fout << " & \\textbf{F1}";
    if ( opts.getF1v () ) fout << " & \\textbf{F1v}";
    if ( opts.getF2 () )  fout << " & \\textbf{F2}";
    if ( opts.getF3 () )  fout << " & \\textbf{F3}";
    if ( opts.getF4 () )  fout << " & \\textbf{F4}";
    if ( opts.getL1 () )  fout << " & \\textbf{L1}";
    if ( opts.getL2 () )  fout << " & \\textbf{L2}";
    if ( opts.getL3 () )  fout << " & \\textbf{L3}";
    if ( opts.getN1 () )  fout << " & \\textbf{N1}";
    if ( opts.getN2 () )  fout << " & \\textbf{N2}";
    if ( opts.getN3 () )  fout << " & \\textbf{N3}";
    if ( opts.getN4 () )  fout << " & \\textbf{N4}";
    if ( opts.getT1 () )  fout << " & \\textbf{T1}";
    if ( opts.getT2 () )  fout << " & \\textbf{T2}";

    /** 
    [NEW COMPLEXITY MEASURE]
    In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
    if ( opts.get[Label of the new measure] () ) fout << " & \\textbf{[Label of the new measure]}";
    **/

    fout << " \\" << "\\" << std::endl;
    fout << "\\hline" << std::endl;
    fout << "\\hline" << std::endl;
    fout << "\\endfirsthead" << std::endl << std::endl;

    // Next headers. 
    fout << "% Next headers" << std::endl; 
    fout << "\\multicolumn{" << opts.getNumberOfComplexityMeasuresToCompute() + 1 << "}{c}{{\\bfseries \\tablename\\ \\thetable{} -- continued from previous page}} \\" << "\\" << std::endl;
    fout << "\\hline" << std::endl;
    fout << "\\textbf{Data set name}" ;  
    if ( opts.getF1 () )  fout << " & \\textbf{F1}";
    if ( opts.getF1v () ) fout << " & \\textbf{F1v}";
    if ( opts.getF2 () )  fout << " & \\textbf{F2}";
    if ( opts.getF3 () )  fout << " & \\textbf{F3}";
    if ( opts.getF4 () )  fout << " & \\textbf{F4}";
    if ( opts.getL1 () )  fout << " & \\textbf{L1}";
    if ( opts.getL2 () )  fout << " & \\textbf{L2}";
    if ( opts.getL3 () )  fout << " & \\textbf{L3}";
    if ( opts.getN1 () )  fout << " & \\textbf{N1}";
    if ( opts.getN2 () )  fout << " & \\textbf{N2}";
    if ( opts.getN3 () )  fout << " & \\textbf{N3}";
    if ( opts.getN4 () )  fout << " & \\textbf{N4}";
    if ( opts.getT1 () )  fout << " & \\textbf{T1}";
    if ( opts.getT2 () )  fout << " & \\textbf{T2}";

    /** 
    [NEW COMPLEXITY MEASURE]
    In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
    if ( opts.get[Label of the new measure] () ) fout << " & \\textbf{[Label of the new measure]}";
    **/

    fout << " \\" << "\\" << std::endl;
    fout << "\\hline" << std::endl;
    fout << "\\hline" << std::endl;
    fout << "\\endhead" << std::endl << std::endl;

    // Table footer.
    // First footer.
    fout << "% First footer" << std::endl; 
    fout << "\\hline" << std::endl;
    fout << "\\multicolumn{" << opts.getNumberOfComplexityMeasuresToCompute() + 1 << "}{r}{{Continued on next page}} \\" << "\\" << std::endl;
    fout << "\\hline" << std::endl;
    fout << "\\endfoot" << std::endl << std::endl;

    // Last footer.
    fout << "% Last footer" << std::endl; 
    fout << "\\hline" << std::endl;
    fout << "\\hline" << std::endl;
    fout << "\\endlastfoot" << std::endl << std::endl;

    for ( i = 0; i < results.getNumberOfDatasets(); i++ ) {
        
        fout << results.getShortDatasetName ( i, 30 ) ;
        resultsOneRun = results.getResult ( i );
        dsResults = resultsOneRun.dsResults;

        if ( opts.getF1 () ) fout << " & " << ( ( dsResults[ CF1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF1 ] < MIN_PRINTFLOAT && dsResults[ CF1 ] != -1 && dsResults[ CF1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF1 ]; 
     
        if ( opts.getF1v () ) fout << " & " << ( ( dsResults[ CF1v ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF1v ] < MIN_PRINTFLOAT && dsResults[ CF1v ] != -1 && dsResults[ CF1v ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF1v ]; 

        if ( opts.getF2 () ) fout << " & " << ( ( dsResults[ CF2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF2 ] < MIN_PRINTFLOAT && dsResults[ CF2 ] != -1 && dsResults[ CF2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF2 ]; 

        if ( opts.getF3 () ) fout << " & " << ( ( dsResults[ CF3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF3 ] < MIN_PRINTFLOAT && dsResults[ CF3 ] != -1 && dsResults[ CF3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF3 ]; 

        if ( opts.getF4 () ) fout << " & " << ( ( dsResults[ CF4 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF4 ] < MIN_PRINTFLOAT && dsResults[ CF4 ] != -1 && dsResults[ CF4 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF4 ]; 

        if ( opts.getL1 () ) fout << " & " << ( ( dsResults[ CL1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CL1 ] < MIN_PRINTFLOAT && dsResults[ CL1 ] != -1 && dsResults[ CL1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL1 ]; 

        if ( opts.getL2 () ) fout << " & " << ( ( dsResults[ CL2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CL2 ] < MIN_PRINTFLOAT && dsResults[ CL2 ] != -1 && dsResults[ CL2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL2 ]; 

        if ( opts.getL3 () ) fout << " & " << ( ( dsResults[ CL3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CL3 ] < MIN_PRINTFLOAT && dsResults[ CL3 ] != -1 && dsResults[ CL3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL3 ]; 

        if ( opts.getN1 () ) fout << " & " << ( ( dsResults[ CN1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CN1 ] < MIN_PRINTFLOAT && dsResults[ CN1 ] != -1 && dsResults[ CN1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN1 ]; 

        if ( opts.getN2 () ) fout << " & " << ( ( dsResults[ CN2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CN2 ] < MIN_PRINTFLOAT && dsResults[ CN2 ] != -1 && dsResults[ CN2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN2 ]; 

        if ( opts.getN3 () ) fout << " & " << ( ( dsResults[ CN3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CN3 ] < MIN_PRINTFLOAT && dsResults[ CN3 ] != -1 && dsResults[ CN3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN3 ]; 

        if ( opts.getN4 () ) fout << " & " << ( ( dsResults[ CN4 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CN4 ] < MIN_PRINTFLOAT && dsResults[ CN4 ] != -1 && dsResults[ CN4 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN4 ]; 

        if ( opts.getT1 () ) fout << " & " << ( ( dsResults[ CT1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CT1 ] < MIN_PRINTFLOAT && dsResults[ CT1 ] != -1 && dsResults[ CT1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CT1 ]; 

        if ( opts.getT2 () ) fout << " & " << ( ( dsResults[ CT2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CT2 ] < MIN_PRINTFLOAT && dsResults[ CT2 ] != -1 && dsResults[ CT2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CT2 ]; 

        /** 
        [NEW COMPLEXITY MEASURE]
        In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
        if ( opts.get[Label of the new measure] () ) fout << " & " << ( ( dsResults[ [Counter] ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ [Counter] ] < MIN_PRINTFLOAT && dsResults[ [Counter] ] != -1 && dsResults[ [Counter] ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ [Counter] ]; 
        **/
 
        fout << " \\" << "\\" << std::endl;
    }

    fout << "\\hline" << std::endl << std::endl;
    fout << "\\end{longtable}" << std::endl << std::endl;
    fout << "}" << std::endl;
    fout << "\\end{center}" << std::endl << std::endl;
    fout << "\\end{document}" << std::endl;

} // end printLatexFile


/**
 * Prints the results obtained from a run with multiple data sets to a XML-formatted file.
 * @param opts contains the run options.
 * @param results contains the results of the complexity measures for each data set.
 * @see runInMultipleDatasetMode.
 */
void printXMLFile ( InputOptions& opts, ResultsContainer& results ) {

    int i;

    // Print the complexity measures results to the XML output file.
    std::ofstream fout;
    fout.open ( opts.getOutputDatasetName ().append ( ".xml" ).c_str (), std::ofstream::out );
    Results resultsOneRun;
    float *dsResults;

    fout << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>" << std::endl; 
    fout << "<ComplexityAnalysis>" << std::endl;

    for ( i = 0; i < results.getNumberOfDatasets (); i++ ) {
        
        fout << "<Dataset>" << std::endl;

        fout << "<Name> " << results.getDatasetName ( i ) << " </Name> " << std::endl;
        resultsOneRun = results.getResult ( i );
        dsResults = resultsOneRun.dsResults;

        if ( opts.getF1 () ) fout << "<F1> " << ( ( dsResults[ CF1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF1 ] < MIN_PRINTFLOAT && dsResults[ CF1 ] != -1 && dsResults[ CF1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF1 ] << " </F1>" << std::endl; 

        if ( opts.getF1v () ) fout << "<F1v> " << ( ( dsResults[ CF1v ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF1v ] < MIN_PRINTFLOAT && dsResults[ CF1v ] != -1 && dsResults[ CF1v ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF1v ] << " </F1v>" << std::endl; 
     
        if ( opts.getF2 () ) fout << "<F2> " << ( ( dsResults[ CF2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF2 ] < MIN_PRINTFLOAT && dsResults[ CF2 ] != -1 && dsResults[ CF2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF2 ] << " </F2>" << std::endl; 

        if ( opts.getF3 () ) fout << "<F3> " << ( ( dsResults[ CF3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF3 ] < MIN_PRINTFLOAT && dsResults[ CF3 ] != -1 && dsResults[ CF3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF3 ] << " </F3>" << std::endl; 

        if ( opts.getF4 () ) fout << "<F4> " << ( ( dsResults[ CF4 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CF4 ] < MIN_PRINTFLOAT && dsResults[ CF4 ] != -1 && dsResults[ CF4 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF4 ] << " </F4>" << std::endl; 

        if ( opts.getL1 () ) fout << "<L1> " << ( ( dsResults[ CL1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CL1 ] < MIN_PRINTFLOAT && dsResults[ CL1 ] != -1 && dsResults[ CL1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL1 ] << " </L1>" << std::endl; 

        if ( opts.getL2 () ) fout << "<L2> " << ( ( dsResults[ CL2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CL2 ] < MIN_PRINTFLOAT && dsResults[ CL2 ] != -1 && dsResults[ CL2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL2 ] << " </L2>" << std::endl; 

        if ( opts.getL3 () ) fout << "<L3> " << ( ( dsResults[ CL3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CL3 ] < MIN_PRINTFLOAT && dsResults[ CL3 ] != -1 && dsResults[ CL3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL3 ] << " </L3>" << std::endl; 

        if ( opts.getN1 () ) fout << "<N1> " << ( ( dsResults[ CN1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CN1 ] < MIN_PRINTFLOAT && dsResults[ CN1 ] != -1 && dsResults[ CN1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN1 ] << " </N1>" << std::endl; 

        if ( opts.getN2 () ) fout << "<N2> " << ( ( dsResults[ CN2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CN2 ] < MIN_PRINTFLOAT && dsResults[ CN2 ] != -1 && dsResults[ CN2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN2 ] << " </N2>" << std::endl; 

        if ( opts.getN3 () ) fout << "<N3> " << ( ( dsResults[ CN3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CN3 ] < MIN_PRINTFLOAT && dsResults[ CN3 ] != -1 && dsResults[ CN3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN3 ] << " </N3>" << std::endl; 

        if ( opts.getN4 () ) fout << "<N4> " << ( ( dsResults[ CN4 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CN4 ] < MIN_PRINTFLOAT && dsResults[ CN4 ] != -1 && dsResults[ CN4 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN4 ] << " </N4>" << std::endl; 

        if ( opts.getT1 () ) fout << "<T1> " << ( ( dsResults[ CT1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CT1 ] < MIN_PRINTFLOAT && dsResults[ CT1 ] != -1 && dsResults[ CT1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CT1 ] << " </T1>" << std::endl; 

        if ( opts.getT2 () ) fout << "<T2> " << ( ( dsResults[ CT2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ CT2 ] < MIN_PRINTFLOAT && dsResults[ CT2 ] != -1 && dsResults[ CT2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CT2 ] << " </T2>" << std::endl; 

        /** 
        [NEW COMPLEXITY MEASURE]
        In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
        if ( opts.get[Label of the new measure] () ) fout << "<[Label of the new measure]> " << ( ( dsResults[ [Counter] ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << ( ( dsResults[ [Counter] ] < MIN_PRINTFLOAT && dsResults[ [Counter] ] != -1 && dsResults[ [Counter] ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ [Counter] ] << " </[Label of the new measure]>" << std::endl; 
        **/
        
        fout << "</Dataset> " << std::endl;

    }

    fout << "</ComplexityAnalysis>" << std::endl;

} // end printXMLFile


/**
 * Prints the results obtained from a run with multiple data sets.
 * @param opts contains the run options.
 * @param results contains the results of the complexity measures for each data set.
 * @see runInMultipleDatasetMode.
 */
void printResultsFromMultipleDatasetRun ( InputOptions& opts, ResultsContainer& results ) {

    int i;

    // Print the complexity measures results to the output file.
    std::ofstream fout;
    fout.open ( opts.getOutputDatasetName ().append ( ".txt" ).c_str (), std::ofstream::out );
    Results resultsOneRun;
    float* dsResults;

    if ( !fout.is_open () || !fout.good () ) {
        std::string logMessage = " \n\n  > [ERROR FILE] The results cannot be printed to the file " + opts.getOutputDatasetName ();
        logMessage += " since the folder does not exist. \n ";
        logMessage += "  > The results are printed to screen: \n ";
        Utils::logWarningError ( logMessage );

        printLegendToScreen ( opts );

        for ( i = 0; i < results.getNumberOfDatasets (); i++ ) {
          
            std::cout << results.getShortDatasetName ( i, 30 ) << std::setw ( 31 - results.getShortDatasetName ( i, 30 ).size () + WIDTH_RES ) ;
            resultsOneRun = results.getResult ( i );
            dsResults = resultsOneRun.dsResults;

            if ( opts.getF1 () ) std::cout << std::fixed << ( ( dsResults[ CF1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF1 ] < MIN_PRINTFLOAT && dsResults[ CF1 ] != -1 && dsResults[ CF1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF1 ]; 

            if ( opts.getF1v () ) std::cout << std::fixed << ( ( dsResults[ CF1v ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF1v ] < MIN_PRINTFLOAT && dsResults[ CF1v ] != -1 && dsResults[ CF1v ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF1v ]; 

            if ( opts.getF2 () ) std::cout << std::fixed << ( ( dsResults[ CF2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF2 ] < MIN_PRINTFLOAT && dsResults[ CF2 ] != -1 && dsResults[ CF2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF2 ]; 

            if ( opts.getF3 () ) std::cout << std::fixed << ( ( dsResults[ CF3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF3 ] < MIN_PRINTFLOAT && dsResults[ CF3 ] != -1 && dsResults[ CF3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF3 ]; 

            if ( opts.getF4 () ) std::cout << std::fixed << ( ( dsResults[ CF4 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF4 ] < MIN_PRINTFLOAT && dsResults[ CF4 ] != -1 && dsResults[ CF4 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF4 ]; 

            if ( opts.getL1 () ) std::cout << std::fixed << ( ( dsResults[ CL1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CL1 ] < MIN_PRINTFLOAT && dsResults[ CL1 ] != -1 && dsResults[ CL1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL1 ]; 

            if ( opts.getL2 () ) std::cout << std::fixed << ( ( dsResults[ CL2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CL2 ] < MIN_PRINTFLOAT && dsResults[ CL2 ] != -1 && dsResults[ CL2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL2 ]; 

            if ( opts.getL3 () ) std::cout << std::fixed << ( ( dsResults[ CL3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CL3 ] < MIN_PRINTFLOAT && dsResults[ CL3 ] != -1 && dsResults[ CL3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL3 ]; 

            if ( opts.getN1 () ) std::cout << std::fixed << ( ( dsResults[ CN1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CN1 ] < MIN_PRINTFLOAT && dsResults[ CN1 ] != -1 && dsResults[ CN1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN1 ]; 

            if ( opts.getN2 () ) std::cout << std::fixed << ( ( dsResults[ CN2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CN2 ] < MIN_PRINTFLOAT && dsResults[ CN2 ] != -1 && dsResults[ CN2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN2 ]; 

            if ( opts.getN3 () ) std::cout << std::fixed << ( ( dsResults[ CN3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CN3 ] < MIN_PRINTFLOAT && dsResults[ CN3 ] != -1 && dsResults[ CN3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN3 ]; 

            if ( opts.getN4 () ) std::cout << std::fixed << ( ( dsResults[ CN4 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CN4 ] < MIN_PRINTFLOAT && dsResults[ CN4 ] != -1 && dsResults[ CN4 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN4 ]; 

            if ( opts.getT1 () ) std::cout << std::fixed << ( ( dsResults[ CT1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CT1 ] < MIN_PRINTFLOAT && dsResults[ CT1 ] != -1 && dsResults[ CT1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CT1 ]; 

            if ( opts.getT2 () ) std::cout << std::fixed << ( ( dsResults[ CT2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CT2 ] < MIN_PRINTFLOAT && dsResults[ CT2 ] != -1 && dsResults[ CT2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CT2 ]; 

            /** 
            [NEW COMPLEXITY MEASURE]
            In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
            if ( opts.get[Label of the new measure] () ) std::cout << std::fixed << ( ( dsResults[ [Counter] ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ [Counter] ] < MIN_PRINTFLOAT && dsResults[ [Counter] ] != -1 && dsResults[ [Counter] ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ [Counter] ]; 
            */

            std::cout << std::endl;
        }
    }
    else {
        printLegend ( opts, fout );

        for ( i = 0; i < results.getNumberOfDatasets (); i++ ) {
    
            fout << results.getShortDatasetName ( i, 30 ) << std::setw ( 31 - results.getShortDatasetName ( i, 30 ).size () + WIDTH_RES ) ;
            resultsOneRun = results.getResult ( i );
            dsResults = resultsOneRun.dsResults;

            if ( opts.getF1 () ) fout << std::fixed << ( ( dsResults[ CF1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF1 ] < MIN_PRINTFLOAT && dsResults[ CF1 ] != -1 && dsResults[ CF1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF1 ]; 

            if ( opts.getF1v () ) fout << std::fixed << ( ( dsResults[ CF1v ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF1v ] < MIN_PRINTFLOAT && dsResults[ CF1v ] != -1 && dsResults[ CF1v ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF1v ]; 

            if ( opts.getF2 () ) fout << std::fixed << ( ( dsResults[ CF2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF2 ] < MIN_PRINTFLOAT && dsResults[ CF2 ] != -1 && dsResults[ CF2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF2 ]; 

            if ( opts.getF3 () ) fout << std::fixed << ( ( dsResults[ CF3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF3 ] < MIN_PRINTFLOAT && dsResults[ CF3 ] != -1 && dsResults[ CF3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF3 ]; 

            if ( opts.getF4 () ) fout << std::fixed << ( ( dsResults[ CF4 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CF4 ] < MIN_PRINTFLOAT && dsResults[ CF4 ] != -1 && dsResults[ CF4 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CF4 ]; 

            if ( opts.getL1 () ) fout << std::fixed << ( ( dsResults[ CL1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CL1 ] < MIN_PRINTFLOAT && dsResults[ CL1 ] != -1 && dsResults[ CL1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL1 ]; 

            if ( opts.getL2 () ) fout << std::fixed << ( ( dsResults[ CL2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CL2 ] < MIN_PRINTFLOAT && dsResults[ CL2 ] != -1 && dsResults[ CL2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL2 ]; 

            if ( opts.getL3 () ) fout << std::fixed << ( ( dsResults[ CL3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CL3 ] < MIN_PRINTFLOAT && dsResults[ CL3 ] != -1 && dsResults[ CL3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CL3 ]; 

            if ( opts.getN1 () ) fout << std::fixed << ( ( dsResults[ CN1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CN1 ] < MIN_PRINTFLOAT && dsResults[ CN1 ] != -1 && dsResults[ CN1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN1 ]; 

            if ( opts.getN2 () ) fout << std::fixed << ( ( dsResults[ CN2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CN2 ] < MIN_PRINTFLOAT && dsResults[ CN2 ] != -1 && dsResults[ CN2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN2 ]; 

            if ( opts.getN3 () ) fout << std::fixed << ( ( dsResults[ CN3 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CN3 ] < MIN_PRINTFLOAT && dsResults[ CN3 ] != -1 && dsResults[ CN3 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN3 ]; 

            if ( opts.getN4 () ) fout << std::fixed << ( ( dsResults[ CN4 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CN4 ] < MIN_PRINTFLOAT && dsResults[ CN4 ] != -1 && dsResults[ CN4 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CN4 ]; 

            if ( opts.getT1 () ) fout << std::fixed << ( ( dsResults[ CT1 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CT1 ] < MIN_PRINTFLOAT && dsResults[ CT1 ] != -1 && dsResults[ CT1 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CT1 ]; 

            if ( opts.getT2 () ) fout << std::fixed << ( ( dsResults[ CT2 ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ CT2 ] < MIN_PRINTFLOAT && dsResults[ CT2 ] != -1 && dsResults[ CT2 ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ CT2 ]; 

            /** 
            [NEW COMPLEXITY MEASURE]
            In case to implement a new complexity measure, please follow the structure indicated below by replacing the information in brackets.
            if ( opts.get[Label of the new measure] () ) fout << std::fixed << ( ( dsResults[ [Counter] ] != -1 ) ? std::setprecision ( DECIMAL_PRECISION ) : std::setprecision ( 0 ) ) << std::setw ( WIDTH_RES ) << ( ( dsResults[ [Counter] ] < MIN_PRINTFLOAT && dsResults[ [Counter] ] != -1 && dsResults[ [Counter] ] != 0. ) ? std::scientific : std::fixed ) << dsResults[ [Counter] ]; 
            */

            fout << std::endl;
        }

        fout.close ();

        if ( opts.getLatexOutput () ) {
            printLatexFile ( opts, results );
        }

        if ( opts.getXMLOutput () ) {
            printXMLFile ( opts, results );
        }

    }

} // end printResultsFromMultipleDatasetRun


/**
 * Runs the complexity measures for all the data sets specified in the input file.
 * @param opts is the input options.
 */
void runInMultipleDatasetMode ( InputOptions& opts ) {

    std::ifstream fBatch, fBatch2;
    std::string line;
    int numberOfDatasetsInFile = 0;
    int i, j;
    Results* resultsOneRun;
    std::string* datasetsOneRun;
    int numberOfDatasetsOneRun = 0;

    // Object to store all the data sets and results after running the measures.
    ResultsContainer allResults;

    // Data sets read from the file.
    Vector<std::string> vectorDatasetNames;

    // 1. Get the number of data sets in the batch file.
    fBatch.open ( opts.getInputDatasetName ().c_str (), std::ifstream::in );

    if ( !fBatch ) {
        std::string logMessage = " \n  > [ERROR FILE] Error opening the input file " + opts.getInputDatasetName () + " \n ";
        Utils::logWarningError ( logMessage );
        exit ( -1 );
    }

    while ( Utils::readLine ( fBatch, line ) ) numberOfDatasetsInFile ++;
    fBatch.close ();

    // 2. Get statistics for each data set.
    fBatch2.open ( opts.getInputDatasetName ().c_str (), std::ifstream::in );

    if ( !fBatch2 ) {
        std::string logMessage = " \n  > [ERROR FILE] Error opening the input file " + opts.getInputDatasetName () + " \n ";
        Utils::logWarningError ( logMessage );
        exit ( -1 );
    }

    for ( i = 0; i < numberOfDatasetsInFile; i++ ) {
        Utils::readLine ( fBatch2, line );
        vectorDatasetNames.addElement ( line );

        opts.setInputDatasetName ( vectorDatasetNames.elementAt ( i ) );

        // Control errors. If one data set cannot be opened, we continue with the next one.
        try {
            resultsOneRun = runMeasures ( opts, datasetsOneRun, numberOfDatasetsOneRun );

            for ( j = 0; j < numberOfDatasetsOneRun; j++ ) {
                allResults.addElement ( datasetsOneRun[j], resultsOneRun[j] );
            }

            // Delete the outer structure of resultsOneRun and datasetNames.
            delete [] resultsOneRun;
            delete [] datasetsOneRun;
        }
        catch ( std::string error ) {
            std::string logMessage = " \n    > [ERROR DATA SET FORMAT]: The data set " + opts.getInputDatasetName() + " does not exist ";
            logMessage += "or it is incorrectly formatted. \n ";
            logMessage += "      >> Please verify that the data set '" + opts.getInputDatasetName() + "' ";
            logMessage += "follows either the KEEL or the WEKA format. \n  ";
            Utils::logWarningError ( logMessage );
        }
    }

    // 3. Close input file.
    fBatch2.close ();

    // 4. Print the results.
    printResultsFromMultipleDatasetRun ( opts, allResults ); 

} // end runInMultipleDatasetMode


/**
 * Runs the application depending on the options specified by the user.
 * @param opts contains the run options.
 */
void run ( InputOptions& opts ) {
    
    std::string* datasetNames;
    int numberOfDatasets;
    int i;
    Results* resultsArray;
    ResultsContainer allResults;

    // Start the logger
    Utils::initLog( opts.getOutputDatasetName () + ".log" );

    if ( opts.getRunInMultipleDatasetMode () ) {
        runInMultipleDatasetMode ( opts );
        return;
    }
    else { // Run measures once.

        // We first compute the complexity measures.
        if ( opts.isAnyComplexityMeasureSelected () ) {
            try {
                resultsArray = runMeasures ( opts, datasetNames, numberOfDatasets );

                for ( i = 0; i < numberOfDatasets; i++ ) {
                    allResults.addElement ( datasetNames[i], resultsArray[i] );
                }

                printResultsFromMultipleDatasetRun ( opts, allResults ); 
            }
            catch ( std::string error ) {
                std::string logMessage = "      >> Did you wanted to run in batch mode? If so, add option -B. \n ";
                logMessage += "     >> Otherwise, please verify that the data set '";
                logMessage += opts.getInputDatasetName() + "' ";
                logMessage += "exists and follows either the KEEL or the WEKA format. \n ";
                Utils::logWarningError ( logMessage );
                exit ( 0 );
            }

            // Delete memory.
            delete [] datasetNames;
            delete [] resultsArray;

        }

        // Then, we run other routines.
        runOtherRoutines ( opts );
    }

    // Close the logger
    Utils::closeLog ();

} // end run


/**
 * Main procedure.
 * @param argc is the number of input arguments.
 * @param argv is a vector of strings with the input arguments.
 * @return An integer, always 1.
 */
int main ( int argc, char** argv ) {

    InputOptions opts;

    // Display the copyright information.
    std::cout << std::endl;
    std::cout << " DCoL  Copyright (C) 2009  Albert Orriols-Puig and Nuria Macia " << std::endl;
    std::cout << " The DCoL comes with ABSOLUTELY NO WARRANTY; for details type `./dcol show w'. " << std::endl;
    std::cout << " This is free software, and you are welcome to redistribute it " << std::endl;
    std::cout << " under certain conditions; type `./dcol show c' for details. " << std::endl;
    std::cout << std::endl;

    opts.parseInput ( argc, argv );

    if ( !opts.isAnyOptionSelected () ) {
        std::string logMessage = " \n  > [ERROR COMMAND] Call syntax is incomplete. \n ";
        logMessage += "    >> You have to select, at least, one of the following options. \n ";
        Utils::logWarningError ( logMessage );
        std::string blank = "";
        opts.printCommandLineExample ( blank.c_str () );
        return 0;
    }

    if ( opts.isIncompatibleOptions () ) {
        return 0;
    }

    // Run depending on the options.
    run ( opts );

    return 1;

} // end main
