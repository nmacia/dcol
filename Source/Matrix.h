
#ifndef MATRIX_H 
#define MATRIX_H 

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Utils.h"


/*! 

\brief Implements methods to store, transform, and operate with matrices.

This class enables the user to create matrices and apply certain operations.  

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


class Matrix{

private:

    /** Number of rows of the matrix. */
    int numberOfRows;

    /** Number of columns of the matrix. */    
    int numberOfColumns;

    /** Matrix. */
    float** matrix;
    
public:
    
    /**
     * Constructs a new Matrix object.
     */
    Matrix () {}

    /**
     * Constructs a new Matrix object.
     * @param nr is the number of rows. 
     * @param nc is the number of columns.
     */
    Matrix ( int nr, int nc );

    /**
     * Copy constructor.
     * @param m is the matrix to be copied. 
     */
    Matrix ( const Matrix& m );
    
    /**
     * Destructs the Matrix object.
     */
    ~Matrix ();

    /**
     * Returns a reference to the position indicated by the indeces.
     * @param i is the row index.
     * @param j is the column index.
     * @return A float reference of the position indicated by the indeces.
     */
    float& getReference ( int i, int j );

    /**
     * Returns the value of the position indicated by the indeces.
     * @param i is the row index.
     * @param j is the column index.
     * @return A float with the position indicated by the indeces.
     */
    float getValue ( int i, int j );

    /**
     * Sets the value to the specified position of the matrix.
     * @param i is the row index.
     * @param j is the column index.
     * @param value is the value that has to be set.
     */
    void setValue ( int i, int j, float value );

    /**
     * Computes the inverse of the matrix with the Gauss Jordan procedure.
     * @return A Matrix with the inverse of the original matrix.
     */
    Matrix computeInverse ();

    /**
     * Computes the Gauss Jordan elimination on the matrix.
     */
    void computeGaussJordan ();

    /**
     * Swaps the two rows passed as argument.
     * @param index is the vector where the rows will be swapped.
     * @param i is the first row.
     * @param j is the second row.
     */
    void swapRows ( int* index, int i, int j );

    /**
     * Resorts the matrix rows according to the indeces passed as argument.
     * @param index contains the indeces to resort.
     */
    void resortMatrix ( int *index ) ;

    /**
     * Returns the transposed matrix. 
     * @return A Matrix with the transposed matrix.
     */
    Matrix transpose ();

    /**
     * Computes the difference of matrices. 
     * @param m is the second matrix of the difference.
     * @return A Matrix with the new matrix.
     */
    Matrix operator - ( const Matrix& m );

    /**
     * Computes the sum of matrices. 
     * @param m is the second matrix of the sum.
     * @return A Matrix with the new matrix.
     */
    Matrix operator + ( const Matrix& m );

    /**
     * Computes the product of matrices. 
     * @param m is the second matrix of the product.
     * @return A Matrix with the new matrix.
     */
    Matrix operator * ( const Matrix& m );

    /**
     * Overloads the standard output.
     * @param os is a reference to the standard output.
     * @param m is a reference to the matrix that has to be printed.
     * @return A reference to ostream.
     */
    friend std::ostream& operator << ( std::ostream& os, Matrix& m );

    /**
     * Multiplies the matrix by a scalar number.
     * @param scalar is the scalar that multiplies the matrix.
     * @return A Matrix having multiplied the original one with the scalar number.
     */
    Matrix multScalar ( float scalar );

    /**
     * Computes the pseudo inverse of the matrix. The object of the class is modified.
     * @return A Matrix with the pseudo inverse matrix.
     * @see computeSVD.
     */
    Matrix computePseudoInverse ();

    /**
     * Transforms a diagonal square matrix into its inverse by replacing each element of the diagonal
     * with its reciprocal. The matrix is overwritten.
     */
    void transformDiagonalMatrixToInverse ();

    /**
     * Computes the singular value decomposition matrix = U·W·V' where (U is mxn, W is nxn, and V is nxn). 
     * The matrix U replaces matrix (the class attribute) on output.
     * @param matW is the diagonal matrix W of the decomposition.
     * @param matV is the matrix V of size nxn. Note that it is not the transpose matrix.
     * @see calculateSVD.
     */
    void computeSVD ( Matrix*& matW, Matrix*& matV );

    /**
     * Computes the singular value decomposition matrix = U*W*V' where (U is mxn, W is nxn, and V is nxn).
     * The matrix U replaces matrix (the class attribute) on output.
     * The diagnoal matrix W is output as a vector of size n.
     * The matrix V (not the transpose) is computed.
     * @param W is a vector of size n that represents the diagonal of matrix W [output parameter].
     * @param V is the matrix V of size nxn. Note that it is not the transpose matrix.
     */
    void decomposeSVD( float*& W, float**& V );

    /**
     * The routine sorts the singular values, and the corresponding values of U and V, by decreasing magnitude.
     * In addition, in order to maximize the number of positive elements, signs of corresponding columns are flipped.
     */
    void reorder( float*& W, float**& V );

    /**
     * Computes (par1^2 + par2^2)^0.5 without destructive underflow or overflow.
     * @param par1 is the first parameter.
     * @param par2 is the second parameter.
     * @return A float with (par1^2 + par2^2)^0.5 without destructive underflow or overflow.
     */
    float pythag ( float par1, float par2 );

    /**
     * Computes the square of a float.
     * @param x is the argument.
     * @return A float with x^2.
    */
    float SQR ( float x ) {
        return ( x == 0 ? 0.0 : x * x );
    }

    /**
     * Returns the first parameter as positive if the second one is positive. 
     * Otherwise, returns the negative value of the second parameter. 
     * @param par1 is the first parameter.
     * @param par2 is the second parameter.
     * @return A float with the positive or negative value of the first parameter 
     *           according to the sign of the second parameter.
    */
    float SIGN ( float par1, float par2 ) {
        return ( par2 >= 0.0 ? fabs ( par1 ) : -fabs ( par1 ) );
    }

    /**
     * Returns the maximum of the two parameters.
     * @param par1 is the first parameter.
     * @param par2 is the second parameter.
     * @return A float with the maximum value between par1 and par2.
    */
    float FMAX ( float par1, float par2 ) {
        return ( par1 > par2 ? par1 : par2 );
    }

    /**
     * Returns the minimum of the two parameters.
     * @param par1 is the first parameter.
     * @param par2 is the second parameter.
     * @return An integer with the maximum value between par1 and par2.
    */
    int IMIN ( int par1, int par2 ) {
        return ( par1 < par2 ? par1 : par2 );
    }

};

#endif
