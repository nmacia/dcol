
#ifndef _HEAP_H
#define _HEAP_H

#include "Utils.h"

#include <iostream>
#include <cstdlib>

/*!

\brief Implements a max-heap.

This class implements a max-heap and is used by the kNN algorithm.

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


template <class Elem> class HeapTree {

private:

    /** Elements of the heap. */
    Elem* data;

    /** Number of elements in the heap. */
    int numberOfElements;

    /** Maximum number of elements permitted in the heap. */
    int maxSize;

public:

    /**
     * Constructs a new HeapTree object of maxSize elements (100 by default).
     * @param mSize is the size of the tree.
     */
    HeapTree ( int mSize );

    /**
     * Copy constructor.
     * @param otherTree is the tree to be copied to the new object.
     */
    HeapTree ( const HeapTree<Elem> &otherTree );

    /**
     * Destructs the HeapTree object.
     */
    ~HeapTree ();

    /**
     * Adds one element to the heap.
     * @param item is the element to be inserted.
     * @return A Boolean that indicates whether the insertion is successful.
     * @retval true the insertion was successful.
     * @retval false the element was not inserted.
     */
    bool add ( const Elem& item );

    /**
     * Returns and removes the first element of the heap.
     * @return An element which is the biggest element in the heap.
     */
    Elem remove ();

    /**
     * Removes all the elements of the heap.
     */
    void removeAll ();

    /**
     * Returns the number of elements in the heap.
     * @return An integer with the number of elements in the heap.
     */
    inline int getNumberOfElements ();

    /**
     * Returns the element at the position specified by the input parameter.
     * @param i is the position to be consulted.
     * @return The element placed in the position i.
     */
    inline Elem& getElementAt ( int i );

    /**
     * Returns the element at the first position.
     * @return The element placed in the first position of the heap.
     */
    inline Elem& getFirst ();

    /**
     * Prints the heap.
     */
    void print ();

private:

    void shiftUp ( int Node );
    void shiftDown ( int Node );
    inline int parentOf ( int Node );
    inline int leftChildOf ( int Node );

};


//// IMPLEMENTATION OF HEAPTREE

template <class Elem> 
HeapTree<Elem>::HeapTree ( int mSize ) {

    maxSize = mSize;
    data = new Elem [ maxSize ];
    numberOfElements = 0;

} // end HeapTree


template <class Elem>
HeapTree<Elem>::HeapTree ( const HeapTree<Elem>& otherTree ) {

    int i;
    maxSize = otherTree.maxSize;
    data = new Elem [ maxSize ];
    numberOfElements = otherTree.numberOfElements;

    for ( i = 0; i < otherTree.numberOfElements; i++ ) {
        data[i] = otherTree.data[i];
    }
    
} // end HeapTree


template <class Elem>
HeapTree<Elem>::~HeapTree () {

    if ( data ) { 
        delete [] data;
    }

} // end ~HeapTree


template <class Elem>
bool HeapTree<Elem>::add ( const Elem& item ) {

    if ( numberOfElements >= maxSize ) {
        return false;
    }

    data[ numberOfElements ] = item;
    shiftUp ( numberOfElements ++ );
    
    return true;
    
} // end add


template <class Elem>
Elem HeapTree<Elem>::remove () {

    Elem aux;

    if ( numberOfElements > 0 ) {
        aux = data[0];
        data[0] = data[ -- numberOfElements ];  // Replace with the last element.
        shiftDown ( 0 );
    }

    return aux;

} // end remove


template <class Elem>
void HeapTree<Elem>::removeAll () {
    numberOfElements = 0;
} // end removeAll


template <class Elem>
inline int HeapTree<Elem>::getNumberOfElements () {
    return numberOfElements;
} // end getNumberOfElements


template <class Elem>
inline Elem& HeapTree<Elem>::getElementAt ( int i ) {

    if ( i >= numberOfElements ) {
        std::string logMessage = "      > [ERROR N4] Not enough elements in the heap. \n ";
        Utils::logWarningError ( logMessage );
        exit ( 0 );
    }

    return data[i];
    
} // end getElementAt


template <class Elem>
inline Elem& HeapTree<Elem>::getFirst() {

    if ( numberOfElements == 0 ) {
        std::string logMessage = "      > [ERROR N4] No elements in the heap. \n ";
        Utils::logWarningError ( logMessage );
        exit ( 0 );
    }

    return data[0];
    
} // end getFirst


template <class Elem>
void HeapTree<Elem>::shiftUp ( int currentNode ) {

    int parent = parentOf ( currentNode );
    Elem item = data[ currentNode ];

    while ( currentNode > 0 ) {
        if ( data[ parent ] < item ) {
            data[ currentNode ] = data[ parent ];
            currentNode = parent;
            parent = parentOf ( currentNode );
        }
        else {
            break;
        }
    }

    data[ currentNode ] = item;

} // end shiftUp


template <class Elem>
void HeapTree<Elem>::shiftDown ( int currentNode ) {

    int child = leftChildOf ( currentNode );
    Elem item = data[ currentNode ];

    while ( child < numberOfElements ) {

        if ( child < ( numberOfElements - 1 ) )
            if ( data[ child ] < data[ child + 1 ] )
                child ++;

        if ( item < data[ child ] ) {
            data[ currentNode ] = data[ child ];
            currentNode       	= child;
            child         		= leftChildOf ( currentNode );
        }
        else {
            break;
        }
    }

    data[ currentNode ] = item;

} // end shiftDown


template <class Elem>
inline int HeapTree<Elem>::parentOf ( int currentNode ) {
    return ( currentNode - 1 ) / 2;
} // end parentOf


template <class Elem>
inline int HeapTree<Elem>::leftChildOf ( int currentNode ) {
    return ( currentNode * 2 ) + 1;
} // end leftChildOf


template <class Elem>
void HeapTree<Elem>::print () {
    
    for ( int i = 0; i < numberOfElements; i++ ) {
        std::cout << data[i] << "  " ;
    }
    std::cout << std::endl;

} // end print

#endif
