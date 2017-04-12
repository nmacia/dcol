
/*!

\brief This file contains the implementation for the linear SMO according to (Platt, 1998).

This file provides the code to train support vector machines. The code was translated from (Platt, 1998).

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


float ComplexityMeasures::testSMO ( float* w, float b, float** testExamples, int* classOfTestExamples,
                                    int numTestExamples ) {

    int i;
    int numError = 0;

    for ( i = 0; i < numTestExamples; i++ ) {
        if ( ( learnedFunction ( testExamples[i], w, b ) > 0) != ( classOfTestExamples[i] > 0 ) ) {
            numError ++;
        }
    }

    return ( float ) numError / ( float ) numTestExamples;

} // end testSMO


float ComplexityMeasures::getDistanceObjectiveFunction ( float* w, float b, float** testExamples,
                                                         int* classOfTestExamples, int numTestExamples ) {

    int i;
    float dist = 0;

    for ( i = 0; i < numTestExamples; i++ ) {
        dist += fabs ( learnedFunction ( testExamples[i], w, b ) - classOfTestExamples[i] );
    }

    return dist / ( float ) numTestExamples;

} // end getDistanceObjectiveFunction


float ComplexityMeasures::kernelFunction ( int i1, int i2 ) {

    int i;
    float dot = 0.0;

    for ( i = 0; i < numberOfAttributes; i++ ) {
        dot += example[ i1 ][i] * example[ i2 ][i];
    }

    return dot;

} // end kernelFunction


float ComplexityMeasures::learnedFunction ( int k, float* w, float b ) {

    int i;
    float s = 0.0;

    for ( i = 0; i < numberOfAttributes; i++ ) {
        s += w[i] * example[k][i];
    }

    s -= b;
    return s;

} // end learnedFunction


float ComplexityMeasures::learnedFunction ( float* testExample, float* w, float b ) {

    int i;
    float s = 0.0;

    for ( i = 0; i < numberOfAttributes; i++ ) {
        s += w[i] * testExample[i];
    }

    s -= b;
    return s;

} // end learnedFunction


int ComplexityMeasures::takeStep ( int i1, int i2, float& b, float* alpha, float* w, float* errorCache ) {

    int y1, y2, s;
    float alpha1, alpha2; // Old values of alpha1 and alpha2.
    float a1, a2;         // New values of alpha1 and alpha2.
    float E1, E2, L, H, k11, k22, k12, eta, Lobj, Hobj;

    if ( i1 == i2 ) {
        return 0;
    }

    // Look up alpha1, y1, E1, alpha2, y2, E2.
    alpha1 = alpha[i1];

    y1 = classOfExample[ i1 ];

    if ( alpha1 > 0 && alpha1 < C ) {
        E1 = errorCache[ i1 ];
    }
    else {
        E1 = learnedFunction ( i1, w, b ) - y1;
    }

    alpha2 = alpha[ i2 ];
    y2 = classOfExample[ i2 ];

    if ( alpha2 > 0 && alpha2 < C ) {
        E2 = errorCache[ i2 ];
    }
    else {
        E2 = learnedFunction ( i2, w, b ) - y2;
    }

    s = y1 * y2;

    // Compute L, H.
    if ( y1 == y2 ) {
        float gamma = alpha1 + alpha2;

        if ( gamma > C ) {
            L = gamma - C;
            H = C;
        }
        else {
            L = 0;
            H = gamma;
        }
    }
    else { // y1 != y2 .
        float gamma = alpha1 - alpha2;

        if ( gamma > 0 ) {
            L = 0;
            H = C - gamma;
        }
        else {
            L = -gamma;
            H = C;
        }
    }

    if ( L == H ) {
        return 0;
    }

    // Compute eta.
    k11 = kernelFunction ( i1, i1 );
    k12 = kernelFunction ( i1, i2 );
    k22 = kernelFunction ( i2, i2 );

    eta = 2 * k12 - k11 - k22;

    if ( eta < 0 ) {
        a2 = alpha2 + y2 * ( E2 - E1 ) / eta;

        if ( a2 < L ) {
            a2 = L;
        }
        else if ( a2 > H ) {
            a2 = H;
        }
    }
    else {
        // Compute Lobj, Hobj: objective function at a2 = L, a2 = H.
        float c1 = eta / 2;
        float c2 = y2 * ( E1 - E2 ) - eta * alpha2;
        Lobj = c1 * L * L + c2 * L;
        Hobj = c1 * H * H + c2 * H;

        if ( Lobj > Hobj + EPSILON ) {
            a2 = L;
        }
        else if ( Lobj < Hobj - EPSILON ) {
            a2 = H;
        }
        else {
            a2 = alpha2;
        }
    }

    if ( fabs ( a2 - alpha2 ) < EPSILON * ( a2 + alpha2 + EPSILON ) ) {
        return 0;
    }

    a1 = alpha1 - s * ( a2 - alpha2 );

    if ( a1 < 0 ) {
        a2 += s * a1;
        a1 = 0;
    }
    else if ( a1 > C ) {
        float t = a1 - C;
        a2 += s * t;
        a1 = C;
    }

    // Update threshold to reflect change in Lagrange multipliers.
    float delta_b, bnew;

    if ( a1 > 0 && a1 < C ) {
        bnew = b + E1 + y1 * ( a1 - alpha1 ) * k11 + y2 * ( a2 - alpha2 ) * k12;
    }
    else {
        if ( a2 > 0 && a2 < C ) {
            bnew = b + E2 + y1 * ( a1 - alpha1 ) * k12 + y2 * ( a2 - alpha2 ) * k22;
        }
        else {
            float b1, b2;
            b1 = b + E1 + y1 * ( a1 - alpha1 ) * k11 + y2 * ( a2 - alpha2 ) * k12;
            b2 = b + E2 + y1 * ( a1 - alpha1 ) * k12 + y2 * ( a2 - alpha2 ) * k22;
            bnew = ( b1 + b2 ) / 2;
        }
    }

    delta_b = bnew - b;

    b = bnew;

    // Update weight vector to reflect change in a1 and a2.
    float t1 = y1 * ( a1 - alpha1 );
    float t2 = y2 * ( a2 - alpha2 );

    int i;
    for ( i = 0; i < numberOfAttributes; i++ ) {
        w[i] += example[ i1 ][i] * t1 + example[ i2 ][i] * t2;
    }

    // Update error cache using new Lagrange multipliers.
    t1 = y1 * ( a1 - alpha1 );
    t2 = y2 * ( a2 - alpha2 );

    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( 0 < alpha[i] && alpha[i] < C ) {
            errorCache[i] += t1 * kernelFunction ( i1, i ) + t2 * kernelFunction ( i2, i ) - delta_b;
            errorCache[ i1 ] = 0.0;
            errorCache[ i2 ] = 0.0;
        }
    }

    alpha[ i1 ] = a1; // Store a1 in the alpha array.
    alpha[ i2 ] = a2; // Store a2 in the alpha array.

    return 1;
    
} // end takeStep


int ComplexityMeasures::argmaxE1E2 ( int i1, float E1, float& b, float* alpha, float* w, float* errorCache ) {
    
    int k, i2;
    float tmax;

    for ( i2 = -1, tmax = 0, k = 0; k < numberOfExamples; k++ ) {
        if ( alpha[k] > 0 && alpha[k] < C ) {
            float E2, temp;
            E2 = errorCache[k];
            temp = fabs ( E1 - E2 );

            if ( temp > tmax ) {
                tmax = temp;
                i2 = k;
            }
        }
    }

    if ( i2 >= 0 ) {
        if ( takeStep ( i1, i2, b, alpha, w, errorCache ) == 1 ) {
            return 1;
        }
    }

    return 0;

} // end argmaxE1E2


int ComplexityMeasures::iterateNonBoundExamples ( int i1, float& b, float* alpha, float* w, float* errorCache ) {

    int k, k0, i2;

    for ( k0 = ( int ) ( Utils::f_rand () * numberOfExamples ), k = k0; k < numberOfExamples + k0; k++ ) {

        i2 = k % numberOfExamples;

        if ( alpha[i2] > 0 && alpha[i2] < C ) {
            if ( takeStep ( i1, i2, b, alpha, w, errorCache ) ) {
                return 1;
            }
        }
    }

    return 0;

} // end iterateNonBoundExamples


int ComplexityMeasures::iterateEntireTrainingSet ( int i1, float& b, float* alpha, float* w, float* errorCache ) {

    int k, k0, i2;

    for ( k0 = ( int ) ( Utils::f_rand () * numberOfExamples ), k = k0; k < numberOfExamples + k0; k++ ) {
        i2 = k % numberOfExamples;

        if ( takeStep ( i1, i2, b, alpha, w, errorCache ) ) {
            return 1;
        }
    }

    return 0;

} // end iterateEntireTrainingSet


int ComplexityMeasures::examineExample ( int i1, float& b, float* alpha, float* w, float* errorCache ) {

    float y1, alpha1, E1, r1;

    y1 = classOfExample[ i1 ];
    alpha1 = alpha[ i1 ];

    if ( alpha1 > 0 && alpha1 < C ) {
        E1 = errorCache[ i1 ];
    }
    else {
        E1 = learnedFunction ( i1, w, b ) - y1;
    }

    r1 = y1 * E1;

    // Check if example i1 violates KKT condition.
    if ( ( r1 < ( 0 - TOLERANCE ) && alpha1 < C ) || ( r1 > TOLERANCE && alpha1 > 0 ) ) {
        // The current example (i1) has violated the KKT. So, we look for the second example to jointly optimize the two alphas.

        // 1. Try argmax E1 - E2.
        if ( argmaxE1E2 ( i1, E1, b, alpha, w, errorCache ) == 1 ) {
            return  1;
        }

        // 2. Try to iterate through the non-bound examples.
        if ( iterateNonBoundExamples ( i1, b, alpha, w, errorCache ) == 1 ) {
            return  1;
        }

        // 3. Try to iterate through the entire training set.
        if ( iterateEntireTrainingSet ( i1, b, alpha, w, errorCache ) == 1 ) {
            return  1;
        }
    }

    return 0;

} // end examineExample


float* ComplexityMeasures::trainSMO ( float& b ) {

    float* alpha;      // Lagrange multipliers.
    float* w;          // Weight vector.
    float* errorCache;

    int i, k;
    int numChanged = 0;
    int examineAll = 1;
    int iter = 0;

    // If there are unknown values, replace them.
    if ( !areUnknownValuesReplaced ) {
        replaceUnknownValues();
    }

    // If data not normalized, normalize it!
    if ( !areDataNormalized || !areNominalAttributesNormalized || !areContinuousAttributesNormalized ) {

        if ( !areNominalAttributesNormalized && !areContinuousAttributesNormalized ) {

            normalizeAllAttributes ();

            std::string logMessage = "      > [WARNING SVM COMPUTATION] Data need to be normalized to run the SVM. \n ";
            logMessage += "        >> Data will be normalized. \n ";
            Utils::logWarningError ( logMessage );

        }
        else { 

            if ( !areNominalAttributesNormalized && numberOfNominalAttributes > 0 ) {

                normalizeNominalAttributes();

                std::string logMessage = "      > [WARNING SVM COMPUTATION] Data need to be normalized to run the SVM. \n ";
                logMessage += "        >> Data will be normalized. \n ";
                Utils::logWarningError ( logMessage );

            }

            if ( !areContinuousAttributesNormalized && numberOfContinuousAttributes > 0) {
            
                normalizeContinuousAttributes();
            
                std::string logMessage = "      > [WARNING SVM COMPUTATION] Data need to be normalized to run the SVM. \n ";
                logMessage += "        >> Data will be normalized. \n ";
                Utils::logWarningError ( logMessage );

            } 
        }
    }

    alpha = new float [ numberOfExamples ];

    errorCache = new float [ numberOfExamples ];
    w = new float [ numberOfAttributes ];

    if ( Utils::doScreenStatistics ) std::cout << "      > Changing classes to -1, 1 " << std::endl;

    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( classOfExample[i] == 0 ) {
            classOfExample[i] = -1;
        }
    }

    for ( i = 0; i < numberOfExamples; i++ ) {
        alpha[i] = 0;
        // The initial error is 0 - classOfExample[i] since the SVM always predicts zero (w = 0 and b = 0).
        errorCache[i] = 0; // - classOfExample[i];
    }

    for ( i = 0; i < numberOfAttributes; i++ ) {
        w[i] = 0.0;
    }

    if ( Utils::doScreenStatistics ) std::cout << "      > Building the Support Vector Machine [progress line] " << std::endl << "        ";

    int maxIterations = ( numberOfExamples < 25000 ) ? 100000 : 4 * numberOfExamples;

    while ( ( numChanged > 0 || examineAll == 1 ) && iter < maxIterations ) {
        if ( Utils::doScreenStatistics ) { 
            std::cout << "." ;
            std::cout.flush();
        }
        numChanged = 0;

        if ( examineAll == 1 ) {
            for ( k = 0; k < numberOfExamples; k++ ) {
                numChanged += examineExample ( k, b, alpha, w, errorCache );
            }
        }
        else {
            for ( k = 0; k < numberOfExamples; k++ ) {
                if ( alpha[k] > 0 && alpha[k] < C ) {
                    numChanged += examineExample ( k, b, alpha, w, errorCache );
                }
            }
        }

        if ( examineAll == 1 ) {
            examineAll = 0;
        }
        else if ( numChanged == 0 ) {
            examineAll = 1;
        }

        iter ++;
    }

    if ( Utils::doScreenStatistics ) std::cout << std::endl;

    if ( Utils::doScreenStatistics ) std::cout << "      > Changing classes to 0, 1 " << std::endl;

    for ( i = 0; i < numberOfExamples; i++ ) {
        if ( classOfExample[i] == -1 ) {
            classOfExample[i] = 0;
        }
    }

    delete [] alpha;
    delete [] errorCache;

    return w;

} // end trainSMO
