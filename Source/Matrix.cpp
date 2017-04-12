
/*!

\brief This file contains the implementation of the Matrix methods.

The class provides implementation of methods to store, modify, and perform operations between matrices.

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


#include "Matrix.h"

#define NEG_INF (-2147483647 - 1)
#define POS_INF 2147483647


Matrix::Matrix ( int nr, int nc ) {

    int i, j;

    numberOfRows = nr;
    numberOfColumns = nc;

    matrix = new float* [ numberOfRows ];
    for ( i = 0; i < numberOfRows; i++ ) {
        matrix[i] = new float [ numberOfColumns ];
    }

    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = 0; j < numberOfColumns; j++ ) {
            matrix[i][j] = 0;
        }
    }

} // end Matrix


Matrix::Matrix ( const Matrix& m ) {

    int i, j;

    numberOfRows = m.numberOfRows;
    numberOfColumns = m.numberOfColumns;

    matrix = new float* [ numberOfRows ];
    for ( i = 0; i < numberOfRows; i++ ) {
        matrix[i] = new float [ numberOfColumns ];
    }

    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = 0; j < numberOfColumns; j++ ) {
            matrix[i][j] = m.matrix[i][j];
        }
    }

} // end Matrix


Matrix::~Matrix () {

    int i;

    for ( i = 0; i < numberOfRows; i++ ) {
        delete [] matrix[i];
    }
    delete [] matrix;

} // end ~Matrix


float& Matrix::getReference ( int i, int j ) {
        return matrix[i][j];
} // end getReference


float Matrix::getValue ( int i, int j ) {
        return matrix[i][j];
} // end getValue


void Matrix::setValue ( int i, int j, float value ) {
        matrix[i][j] = value;
} // end setValue


Matrix Matrix::computeInverse () {
    
    int i, j;
    
    if ( numberOfRows != numberOfColumns ) {
        std::string logMessage = " > [ERROR INVERSE OF A MATRIX] The matrix is not squared. \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Error computing the inverse of a matrix: The matrix is not squared. " );
    }

    // 1. Declare a matrix with the double number of columns where the inverse will be computed.
    Matrix m ( numberOfRows, numberOfColumns * 2 );

    // 2. Copy the elements of the matrix to the new one.
    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = 0; j < numberOfColumns; j++ ) {
            m.matrix[i][j] = matrix[i][j];
        }
    }

    // 3. Set the diagonal matrix in the matrix compressed in the index numberOfColums, 2 * numberOfColumns.
    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = numberOfColumns; j < 2 * numberOfColumns; j++ ) {
            if ( i == j - numberOfColumns ) {
                m.matrix[i][j] = 1;
            }
            else {
                m.matrix[i][j] = 0;
            }
        }
    }

    // 4. Compute Gauss Jordan.
    m.computeGaussJordan();

    // 5. Copy the inverse matrix.
    Matrix invMatrix ( numberOfRows, numberOfColumns );
    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = 0; j < numberOfColumns; j++ ) {
            invMatrix.matrix[i][j] = m.matrix[i][ j + numberOfColumns ];
        }
    }

    return invMatrix;

} // end computeInverse


void Matrix::computeGaussJordan () {

    int i, j, k, indexSwap;
    float ratio;
    int* index; // Index to store row swaps.

    // Initialize variables.
    k = 0;
    index = new int [ numberOfRows ];
    for ( i = 0; i < numberOfRows; i++ ) {
        index[i] = i;
    }

    //1. We first try to remove all the zeros in the pivot of the squared matrix numberOfRows x numberOfRows.
    //   Remember that the matrix is not squared, but we only consider the square part in this phase.
    for ( i = 0; i < numberOfRows; i++ ) {
        
        // If the pivot is zero, we try to swap with another row.
        indexSwap = -1;
        if ( matrix[i][i] == 0 ) {
            
            for ( j = 0; j < numberOfRows; j++ ) {
                if ( i != j ) {

                    // The best case is when the pivots of the two rows are non-zero.
                    if ( matrix[j][i] != 0 && matrix[i][j] != 0 ) {
                        indexSwap = j;
                    }
                    else if ( matrix[j][i] != 0 && indexSwap == -1 ) {
                        indexSwap = j;
                    }
                }
            }

            if ( indexSwap != -1 ) {
                swapRows ( index, i, indexSwap );
            }
            else {
                std::string logMessage = " > [ERROR GAUSS JORDAN ELIMINATION] Matrix without zeros in diagonal not found. \n ";
                Utils::logWarningError ( logMessage );
                throw std::string ( " Error diagonalizing the matrix: Matrix without zeros in diagonal not found. " );
            }
        }

    }

    // 2. Diagonalize the matrix. Go through each column and row and get a combination that yields 0.
    for ( j = 0; j < numberOfRows; j++ ) { // j is the index of columns, but we go only through the first numberOfRows columns.
        for ( i = 0; i < numberOfRows; i++ ) {
            if ( i != j ) {
                // 2.1. Make matrix[i][j] = 0.
                if ( matrix[i][j] != 0 ) {
                    // 2.1.1. Choose matrix[j][j] to reduce it.
                    ratio = matrix[i][j] / matrix[j][j];
                    
                    // 2.1.2. Decrease the values of each column of row i.
                    for ( k = 0; k < numberOfColumns; k++ ) {
                        matrix[i][k] = matrix[i][k] - ratio * matrix[j][k]; 
                    }

                    // 2.1.3. To make sure that matrix[i][j] = 0, force the zero to avoid any problem with the decimals.
                    matrix[i][j] = 0;

                }
            }
        }
    }

    // 3. Normalize the diagonal.
    for ( i = 0; i < numberOfRows; i++ ) {
        ratio = matrix[i][i];
        for ( j = 0; j < numberOfColumns; j++ ) {
            if ( matrix[i][j] != 0 ) {
                matrix[i][j] /= ratio;
            }
            if ( std::isnan( matrix[i][j] ) ) {
                matrix[i][k] = 0;
            }
            else if ( std::isinf( matrix[i][j] ) < 0 ) {
                matrix[i][k] = NEG_INF;
            }
            else if ( std::isinf( matrix[i][j] ) > 0 ) {
                matrix[i][k] = POS_INF;
            }
        }
    }
    
    // 4. Reswap again.
    resortMatrix ( index );

    // 5. Delete the auxiliary memory.
    delete [] index;

} // end computeGaussJordan


void Matrix::swapRows ( int* index, int i, int j ) {

    float* auxRow;
    int auxInt;

    auxRow = matrix[i];
    matrix[i] = matrix[j];
    matrix[j] = auxRow;

    auxInt = index[i];
    index[i] = index[j];
    index[j] = auxInt;

} // end swapRows


void Matrix::resortMatrix ( int* index ) {

    int i;
    float** aux = new float* [ numberOfRows ];
    
    // Copy to the auxiliary structure resorting.
    for ( i = 0; i < numberOfRows; i++ ) {
        aux[ index[i] ] = matrix[i];
    }

    // Transfer to the original matrix.
    for ( i = 0; i < numberOfRows; i++ ) {
        matrix[i] = aux[i];
    }

    // Delete the auxiliary memory.
    delete [] aux;

} // end resortMatrix


Matrix Matrix::transpose () {

    int i, j;

    // 1. Create a m x n matrix from a n x m matrix.
    Matrix res ( numberOfColumns, numberOfRows );

    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = 0; j < numberOfColumns; j++ ) {
            res.matrix[j][i] = matrix[i][j];
        }
    }

    return res;

} // end transpose


Matrix Matrix::operator - ( const Matrix& m ) {
    
    // 1. Check that both matrices have the same size.
    if ( numberOfRows != m.numberOfRows || numberOfColumns != m.numberOfColumns ) {
        std::string logMessage = " > [ERROR SUBSTRACTING MATRICES] The matrices do not have the same dimensions. \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Error computing the difference between matrices: The matrices do not have the same dimensions. " );
    }

    int i, j;
    Matrix res ( numberOfRows, numberOfColumns );

    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = 0; j < numberOfColumns; j++ ) {
            res.matrix[i][j] = matrix[i][j] - m.matrix[i][j];
        }
    }

    return res;

} // end operator -


Matrix Matrix::operator + ( const Matrix& m ) {
    
    // 1. Check that both matrices have the same size.
    if ( numberOfRows != m.numberOfRows || numberOfColumns != m.numberOfColumns ) {
        std::string logMessage = " > [ERROR ADDING MATRICES] The matrices do not have the same dimensions. \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Error computing the sum of matrices: The matrices do not have the same dimensions. " );
    }

    int i, j;
    Matrix res ( numberOfRows, numberOfColumns );

    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = 0; j < numberOfColumns; j++ ) {
            res.matrix[i][j] = matrix[i][j] + m.matrix[i][j];
        }
    }

    return res;

} // end operator +


Matrix Matrix::operator * ( const Matrix& m ) {
    
    // 1. Check that both matrices have the same size.
    if ( numberOfColumns != m.numberOfRows ) {

        // Convert integers into string;
        std::ostringstream stNumberOfColumns;
        std::ostringstream stNumberOfRows;
        stNumberOfColumns << numberOfColumns;
        stNumberOfRows << m.numberOfRows;

        std::string logMessage = " > [ERROR MULTIPLYING MATRICES] The matrices cannot be multiplied since the first one has ";
        logMessage += stNumberOfColumns.str () + " columns and the second one has " + stNumberOfRows.str () + " rows. \n ";
        Utils::logWarningError ( logMessage );
        throw std::string ( " Error computing the product of matrices: the size of the matrices does not permit their product. " );

    }

    int i, j, k;
    Matrix res ( numberOfRows, m.numberOfColumns );

    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = 0; j < m.numberOfColumns; j++ ) {

            for ( k = 0; k < m.numberOfRows; k++ ) {
                res.matrix[i][j] += matrix[i][k] * m.matrix[k][j];
            }
        }
    }

    return res;

} // end operator *


std::ostream& operator << ( std::ostream& os, Matrix& m ) {
    
    int i, j;

    std::cout.precision ( 5 );
    for ( i = 0; i < m.numberOfRows; i++ ) {
        for ( j = 0; j < m.numberOfColumns; j++ ) {
            std::cout << std::setw( 8 ) << m.matrix[i][j] << "  ";
        }
        std::cout << std::endl;
    }

    return os;

} // end operator <<


Matrix Matrix::multScalar ( float scalar ) {
    
    int i, j;

    Matrix res ( numberOfRows, numberOfColumns );
    
    for ( i = 0; i < numberOfRows; i++ ) {
        for ( j = 0; j < numberOfColumns; j++ ) {
            res.matrix[i][j] =  matrix[i][j] * scalar;
        }
    }

    return res;

} // end multScalar


Matrix Matrix::computePseudoInverse () {

    Matrix* U;
    Matrix* W;
    Matrix* V;

    // Declare matrix U and copy the original matrix.
    U = new Matrix ( *this );    

    // Compute the singular value decomposition.
    U->computeSVD ( W, V );

    // Invert matrix W.
    W->transformDiagonalMatrixToInverse ();

    Matrix res = ( *V ) * ( *W ) * U->transpose ();

    delete W;
    delete V;
    delete U;
    
    return res;

} // end computePseudoInverse


void Matrix::transformDiagonalMatrixToInverse () {

    int i;

    if ( numberOfRows != numberOfColumns ) {
        std::string logMessage = " [ERROR INVERSE TRANSFORMATION]. Inverse not computed since the matrix is not square. \n ";
        Utils::logWarningError ( logMessage );
        return;
    }    

    for ( i = 0; i < numberOfRows; i++ ) {
        if ( matrix[i][i] > 0.00001 ) {
            matrix[i][i] = 1.0 / matrix[i][i];
        }
        else {
            matrix[i][i] = 0;
        }
    }

} // end transformDiagonalMatrixToInverse


// This function does A = U*W*V', where A is the matrix of the object.
// Matrix U will replace the matrix of the object.
void Matrix::computeSVD ( Matrix*& matW, Matrix*& matV ) {

    float* W;  // Diagonal matrix (only storing the diagonal).
    float** V; // Matrix V. 
    int i, j;   // Indeces.

    // Reserve memory for matrices W and V.
    W = new float [ numberOfColumns ];
    V = new float* [ numberOfColumns ];

    for ( i = 0; i < numberOfColumns; i++ ) {
        W[i] = 0;
        V[i] = new float [ numberOfColumns ];
        for ( j = 0; j < numberOfColumns; j++ ) {
            V[i][j] = 0;
        }
    }

    // Calculate the singular values.
    this->decomposeSVD ( W, V );

    // Transform the result to matrices.
    // Create matrix W.
    matW = new Matrix ( numberOfColumns, numberOfColumns );
    for ( i = 0; i < numberOfColumns; i++ ) {
        for ( j = 0; j < numberOfColumns; j++ ) {
            if ( i == j ) {
                matW->matrix[i][j] = W[i];
            }
            else {
                matW->matrix[i][j] = 0;
            }
        }
    }

    delete [] W;

    // Create matrix V.
    matV = new Matrix ();
    matV->numberOfRows    = numberOfColumns;
    matV->numberOfColumns = numberOfColumns;
    matV->matrix = V;

} // end computeSVD


void Matrix::decomposeSVD ( float*& W, float**& V ) {

    // Maximum number of iterations.
    const int maxNumIter = 30;
    const float eps = 0.0000001;

    // Some auxiliary variables.
    int flag, i, its, j, jj, k, l, nm;
    float anorm, c, f, g, h, s, scale, x, y, z;

    // Declare two auxiliary variables for numberOfRows and numberOfColumns.
    int m = numberOfRows;
    int n = numberOfColumns;

    // Auxiliary vector.
    float* rv1;

    // Reserve memory for the all vectors and matrices.
    rv1 = new float [n];
    for ( i = 0; i < n; i++ ) {
        rv1[i] = 0;
    }

    // Householder reduction to bidiagonal form.
    g = scale = anorm = 0.0;
    l = nm = 0;

    for ( i = 0; i < n; i++ ) {
        l = i + 2;
        rv1[i] = scale * g;
        g = s = scale = 0.0;

        // If at least, an element of column i is different from 0.
        if (i < m) {
            for ( k = i; k < m; k++ ) scale += fabs( matrix[k][i] );
            if ( scale != 0.0 ) {

            	for ( k = i; k < m; k++ ) {
            		matrix[k][i] /= scale; // Divide each element of columns i by scale.
            		s += matrix[k][i] * matrix[k][i]; // Add squares of each element in the column.
            	}

            	f = matrix[i][i];
            	g = -SIGN ( sqrt (s), f );
                h = f * g - s;
                matrix[i][i] = f - g;

                for ( j = l - 1; j < n; j++ ) {
                	for ( s = 0.0, k = i; k < m; k++ ) s += matrix[k][i] * matrix[k][j];
                	f = s / h;
                	for ( k = i; k < m; k++ ) matrix[k][j] += f * matrix[k][i];
                }

                for ( k = i; k < m; k++ ) matrix[k][i] *= scale;
            } // end if ( scale ).
        } // end if ( i < m ).

        W[i] = scale * g;
        g = s = scale = 0.0;

        if ( i + 1 <= m && i + 1 != n ) {
            for ( k = l - 1; k < n; k++ ) scale += fabs ( matrix[i][k] );
            if ( scale != 0.0 ) {
                for ( k = l - 1; k < n; k++) {
                    matrix[i][k] /= scale;
                    s += matrix[i][k] * matrix[i][k];
                }

                f = matrix[i][ l - 1 ];
                g = -SIGN ( sqrt ( s ), f );
                h = f * g - s;
                matrix[i][l-1]=f-g;

                for ( k = l - 1; k < n; k++ ) rv1[k] = matrix[i][k] / h;
                for ( j = l - 1; j < m; j++ ) {
                    for ( s = 0.0, k = l - 1; k < n; k++ ) s += matrix[j][k] * matrix[i][k];
                    for ( k = l - 1; k < n; k++ ) matrix[j][k] += s * rv1[k];
                }

                for ( k = l - 1; k < n; k++ ) matrix[i][k] *= scale;
            } // end if ( scale ).
        } // end if ( i + 1 <= m && i + 1 != n ).

        anorm = FMAX ( anorm, ( fabs ( W[i] ) + fabs ( rv1[i] ) ) );
    } // end for.

    // Accumulate of right-hand transformations.
    for ( i = n - 1; i >= 0; i-- ) {
        if ( i < ( n - 1 ) ) {
            if ( g != 0.0 ) {

                // Double division to avoid possible underflow.
                for ( j = l; j < n; j++ ) {
                	V[j][i] = ( matrix[i][j] / matrix [i][l] ) / g;
                }

                for ( j = l; j < n; j++ ) {
                    for ( s = 0.0, k = l; k < n; k++ ) s += matrix[i][k] * V[k][j];
                    for ( k = l; k < n; k++ )          V[k][j] += s * V[k][i];
                }
            }

            for ( j = l; j < n; j++ ) V[i][j] = V[j][i] = 0.0;
        } // end if ( i < ( n -1 ) )

        V[i][i] = 1.0;
        g = rv1[i];
        l = i;
    }

    // Accumulate of left-hand transformations.
    for ( i = IMIN ( m, n ) - 1 ; i >= 0; i-- ) {
        l = i + 1;
        g = W[i];

        for ( j = l; j < n; j++ ) matrix[i][j] = 0.0;

        if ( g != 0.0 ) {
            g = 1.0 / g;
            for ( j = l; j < n; j++ ) {
                for ( s = 0.0, k = l; k < m; k++ ) s += matrix[k][i] * matrix[k][j];
                f = ( s / matrix[i][i] ) * g;
                for ( k = i; k < m; k++ ) matrix[k][j] += f * matrix[k][i];
            }

            for ( j = i; j < m; j++ ) matrix[j][i] *= g;
        }
        else {
        	for ( j = i; j < m; j++ ) matrix[j][i] = 0.0;
        } // end if ( g != 0.0 )

        ++ matrix[i][i];

    }

    // Diagonalization of the bidiagonal form: Loop over singular values and over allowed iterations.
    for ( k = n - 1; k >= 0; k-- ) {
    	for ( its = 0; its < maxNumIter; its ++ ) {
            flag = true;

            for ( l = k; l >= 0; l-- ) { // Test for splitting.
                nm = l - 1;
                if ( l == 0 || fabs ( rv1[l] ) <= ( ( float ) eps * anorm ) ) {
                    flag = false;
                    break;
                }

                if ( fabs ( W[ nm ] ) <= eps * anorm ) break;
            }

            if ( flag ) {
            	c = 0.0; // Cancellation of rv1[l], if l > 0.
            	s = 1.0;
            	for ( i = l; i < k + 1; i++ ) {
                    f = s * rv1[i];
                    rv1[i] = c * rv1[i];
                    if ( fabs ( f ) <= eps * anorm ) break;
                    g = W[i];
                    h = pythag ( f, g );
                    W[i] = h;
                    h = 1.0 / h;
                    c = g * h;
                    s = -f * h;
                    for ( j = 0; j < m; j++ ) {
                    	y = matrix[j][ nm ];
                    	z = matrix[j][i];
                    	matrix[j][ nm ] = y * c + z * s;
                    	matrix[j][i] = z * c - y * s;
                    }
            	}
            }

            z = W[k];

            // In case of convergence, the singular value is made non-negative.
            if ( l == k ) {
            	if ( z < 0.0 ) {
                    W[k] = -z;
                    for ( j = 0; j < n; j++ ) V[j][k] = -V[j][k];
                }
                break;
            }

            if ( its == maxNumIter - 1 ) {
                std::ostringstream stMaxNumIter;
                stMaxNumIter << maxNumIter;
                std::string logMessage = "[WARNING SVD COMPUTATION] No convergence in " + stMaxNumIter.str () + " iterations. Returning an approximation of the value. ";
                Utils::logWarningError ( logMessage );

                // Delete vector rv1.
                delete [] rv1;
                return;
            }

            x = W[l]; // Shift from bottom 2-by-2 minor.
            nm = k - 1;
            y = W[ nm ];
            g = rv1[ nm ];
            h = rv1[k];
            f = ( ( y - z ) * ( y + z ) + ( g - h ) * ( g + h ) ) / ( 2.0 * h * y );
            g = pythag ( f, 1.0 );
            f= ( ( x - z ) * ( x + z ) + h * ( ( y / ( f + SIGN ( g , f ) ) ) - h ) ) / x;

            // Next QR transformation.
            c = s = 1.0;
            for ( j = l; j <= nm ; j++ ) {

            	i = j + 1;
            	g = rv1[i];
            	y = W[i];
            	h = s * g;
            	g = c * g;
            	z = pythag ( f, h );
            	rv1[j] = z;
            	c = f / z;
            	s = h / z;
            	f = x * c + g * s;
            	g = g * c - x * s;
            	h = y * s;
            	y *= c;

            	for ( jj = 0; jj < n; jj ++ ) {
            		x = V[ jj ][j];
            		z = V[ jj ][i];
            		V[ jj ][j] = x * c + z * s;
            		V[ jj ][i] = z * c - x * s;
            	}

            	z = pythag ( f, h );
            	W[j] = z; // Rotation can be arbitrary if z = 0.

            	if ( z ) {
            		z = 1.0 / z;
            		c = f * z;
            		s = h * z;
            	}

            	f = c * g + s * y;
            	x = c * y - s * g;

            	for ( jj = 0; jj < m; jj ++ ) {
            		y = matrix[ jj ][j];
            		z = matrix[ jj ][i];
            		matrix[ jj ][j] = y * c + z * s;
            		matrix[ jj ][i] = z * c - y * s;
            	}

            } // end for ( j = l; j <= nm; j++ ).

            rv1[l] = 0.0;
            rv1[k] = f;
            W[k] = x;

    	} // end for ( its = 0; its < maxNumIter; its ++ ).

    }

    delete [] rv1;

} // end decomposeSVD


// Given the output of decompose, this routine sorts the singular values, and corresponding columns
// of u and v, by decreasing magnitude. Also, signs of corresponding columns are
// flipped so as to maximize the number of positive elements.
void Matrix::reorder ( float*& W, float**& V ) {

    int i, j, k, s, inc = 1;
    float sw;
    float* su;
    float* sv;

    // Declare two auxiliary variables for numberOfRows and numberOfColumns.
    int m = numberOfRows;
    int n = numberOfColumns;

    // Reserve memory for all vectors.
    su = new float [m];
    for ( i = 0; i < m; i++ ) {
        su[i] = 0;
    }

    sv = new float [n];
    for ( i = 0; i < n; i++ ) {
    	sv[i] = 0;
    }

    do {
    	inc *= 3;
    	inc ++;
    } while ( inc <= n ); // Sort. The method is Shell's sort. (The work is negligible as compared to that already done in decompose.)

    do {
    	inc /= 3;

    	for ( i = inc; i < n; i++ ) {

    		sw = W[i];
    		for ( k = 0; k < m; k++ ) su[k] = matrix[k][i];
    		for ( k = 0; k < n; k++ ) sv[k] = V[k][i];
    		j = i;

    		while ( W[ j - inc ] < sw) {

    			W[j] = W[ j - inc ];
    			for ( k = 0; k < m; k++ ) matrix[k][j] = matrix[k][ j - inc ];
    			for ( k = 0; k < n; k++ ) V[k][j] = V[k][ j - inc ];
    			j -= inc;
    			if ( j < inc ) break;

    		}
    		W[j] = sw;
    		for ( k = 0; k < m; k++ ) matrix[k][j] = su[k];
    		for ( k = 0; k < n; k++ ) V[k][j] = sv[k];
    	}
    } while ( inc > 1 );

    for ( k = 0; k < n; k++ ) { // Flip signs.
    	s = 0;
    	for ( i = 0; i < m; i++ ) if ( matrix[i][k] < 0. ) s++;
    	for ( j = 0; j < n; j++ ) if ( V[j][k] < 0. ) s++;
    	if ( s > ( m + n ) / 2 ) {
    		for ( i = 0; i < m; i++ ) matrix[i][k] = - matrix[i][k];
    		for ( j = 0; j < n; j++ ) V[j][k] = - V[j][k];
    	}
    }

} // end reorder


float Matrix::pythag ( float par1, float par2 ) {

    float absPar1, absPar2;

    absPar1 = fabs ( par1 );
    absPar2 = fabs ( par2 );

    if ( absPar1 > absPar2 ) {
        return absPar1 * sqrt ( 1.0 + SQR ( absPar2 / absPar1 ) );
    }
    else {
        return ( absPar2 == 0.0 ? 0.0 : absPar2 * sqrt( 1.0 + SQR ( absPar1 / absPar2 ) ) );
    }

} // end pythag
