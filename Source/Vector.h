
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "Utils.h"

#include <iostream>
#include <cstdlib>

typedef int UINT;


/*!

\brief Implements a Vector class like Java class Vector.

This class is the class Vector with templates.

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


template <class Etype> class Vector {

protected:

    /** Size of the vector. */
    UINT m_size;

    /** Capacity of the vector. */
    UINT m_capacity;

    /** Minimum increment when the vector runs out of memory. */
    UINT m_increment;

    /** Vector where the elements are stored. */
    Etype** m_pData;

public:

    /**
     * Constructs a new Vector object.
     * @param initialCapacity is the initial capacity of the vector.
     * @param capacityIncrement is the amount by which the capacity is increased when the vector overflows.
     */
    Vector ( UINT initialCapacity = 10, UINT capacityIncrement = 10 );

    /**
     * Copy constructor.
     * @param rhv is the Vector object that will be copied to the new one.
     */
    Vector ( const Vector& rhv );

    /** Destructs a Vector object. */
    virtual ~Vector ();

    /**
     * Returns the capacity of the vector.
     * @return An integer with the capacity of the vector.
     */
    UINT capacity () const;

    /**
     * Returns whether the Vector contains the passed element.
     * @param elem is the element that will be searched for.
     * @return A Boolean that indicates whether the element has been found.
     * @retval true the element has been found.
     * @retval false the element has not been found.
     */
    bool contains ( const Etype& elem ) const;

    /**
     * Returns the first element of the vector.
     * @return The first element of the vector. 
     */
    const Etype& firstElement () const;

    /**
     * Returns the index of a given element.
     * @param elem is the element that will be searched for.
     * @return An integer that indicates the position where the element has been found.
     * @retval -1 the element has not been found.
     */
    int indexOf ( const Etype& elem ) const;

    /**
     * Returns whether the Vector is empty or not.
     * @return A Boolean that inidicates whether the vector is empty.
     * @retval true the vector is empty.
     * @retval false the vector contains elements.
     */
    bool isEmpty () const;

    /**
     * Returns the last element.
     * @return The last element.
     */
    const Etype& lastElement () const;

    /**
     * Returns the last index where a given element appears.
     * @param elem is the element that will be searched for.
     * @return An integer with the last position where the element has been found.
    */
    int lastIndexOf ( const Etype& elem ) const;

    /**
     * Returns the number of elements in the vector.
     * @return An integer with the number of elements in the vector.
     */
    UINT size () const;

    /**
     * Copies the array into the vector.
     * @param array is the array to be copied.
     */
    void copyInto ( Etype* array ) const;

    /**
     * Adds an element in the vector.
     * @param obj is the element to be added.
     */
    void addElement ( const Etype& obj );

    /**
     * Ensures capacity by increasing it if necessary.
     * @param minCapacity is the minimum capacity.
     */
    void ensureCapacity ( UINT minCapacity );

    /**
     * Removes all the elements from the vector.
     */
    void removeAllElements ();

    /**
     * Removes the given element.
     * @param obj is the element to be removed.
     * @return A Boolean that indicates whether the element has been removed.
     * @retval true the element has been removed successfully.
     * @retval false the element has not been removed.
     */
    bool removeElement ( const Etype& obj );

    /**
     * Sets the size of the vector.
     * @param newSize is the new size of the vector.
     */
    void setSize ( UINT newSize );

    /**
     * Trims the vector to the actual size.
     */
    void trimToSize ();

    /**
     * Returns the element at the indicated position.
     * @param index is the position to be consulted.
     * @return The corresponding element.
     */
    Etype& elementAt ( UINT index ) const;

    /**
     * Inserts an element into a specified position.
     * @param obj is the object to be inserted.
     * @param index is the position where the object will be inserted.
     */
    void insertElementAt ( const Etype& obj, UINT index );

    /**
     * Removes an element from the specified position.
     * @param index is the position that will be removed.
     */
    void removeElementAt ( UINT index );

    /**
     * Removes an element from one position without deleting the element.
     * @param index is the position of the element that will be removed.
     */
    void removeElementAtWithoutDestroyingElement ( UINT index );

    /**
     * Sets the element at a specified position.
     * @param obj is the object to be inserted.
     * @param index is the position where the object will be inserted.
     */
    void setElementAt ( const Etype& obj, UINT index );

    /**
     * Overloads the operator [].
     * @param index is the position of the consulted element.
     * @return The consulted element.
     */
    const Etype& operator [] ( UINT index ) const;

    /**
     * Overloads the operator [].
     * @param index is the position of the consulted element.
     * @return The consulted element.
     */
    Etype& operator [] ( UINT index );

    /**
     * Overloads the operator ==.
     * @param rhv is the vector with which the current vector will be compared.
     * @return A Boolean that indicates whether the two elements are equal.
     * @retval true the current vector is equal to rhv.
     * @retval false the current vector is different from rhv.
     */
    bool operator == ( const Vector& rhv );

protected:

    /**
     * Returns the minimum value between the two arguments.
     * @param left is the first argument.
     * @param right is the second argument.
     * @return An integer with the minimum value between left and right.
     */
    int min ( UINT left, UINT right ) const;

    /**
     * Checks whether the index is correct.
     * @param index is the index to be checked.
     */
    void verifyIndex ( UINT index ) const;

};


//// IMPLEMENTATION OF VECTOR

template <class Etype>
Vector<Etype>::Vector ( UINT initialCapacity, UINT capacityIncrement ) {

    m_size = 0;
    m_capacity = initialCapacity;
    m_pData = new Etype* [ m_capacity ];
    m_increment = capacityIncrement;

} // end Vector


template <class Etype>
Vector<Etype>::Vector ( const Vector<Etype>& rhv ) {

    UINT i;

    m_size = rhv.m_size;
    m_capacity = rhv.m_capacity;
    m_pData = new Etype* [ m_capacity ];
    m_increment = rhv.m_increment;

    for ( i = 0; i < m_size; i++ ) {
        m_pData[i] = new Etype ( * ( rhv.m_pData[i] ) );
    }

} // end Vector


template <class Etype>
Vector<Etype>::~Vector () {

    removeAllElements();
    delete [] m_pData;

} // end ~Vector


template <class Etype>
UINT Vector<Etype>::capacity () const {
    return m_capacity;
} // end capacity


template <class Etype>
bool Vector<Etype>::contains ( const Etype& elem ) const {

    UINT i;
    
    for ( i = 0; i < m_size; i++ ) {
        if ( *m_pData[i] == elem )
            return true;
    }

    return false;

} // end contains


template <class Etype>
void Vector<Etype>::copyInto ( Etype* array ) const {

    UINT i;

    for ( i = 0; i < m_size; i++ )
        array[i] = *m_pData[i];

} // end copyInto


template <class Etype>
Etype & Vector<Etype>::elementAt ( UINT index ) const {

    verifyIndex ( index );
    return *m_pData[ index ];

} // end elementAt


template <class Etype>
const Etype & Vector<Etype>::firstElement () const {

    if ( m_size == 0 ) {
        // Throw "Empty Vector Exception".
        std::string logMessage = " > [ERROR VECTOR] Method firstElement() has been called on empty Vector. \n ";
        Utils::logWarningError ( logMessage );
        exit ( 1 );
    }

    return *m_pData[0];
    
} // end firstElement


template <class Etype>
int Vector<Etype>::indexOf ( const Etype& elem ) const {

    UINT i;

    for ( i = 0; i < m_size; i++ ) {
        if ( *m_pData[i] == elem )
            return i;
    }

    return -1;

} // end indexOf


template <class Etype>
bool Vector<Etype>::isEmpty () const {
    return m_size == 0;
} // end isEmpty


template <class Etype>
const Etype & Vector<Etype>::lastElement () const {

    if ( m_size == 0 ) {
        // Throw "Empty Vector Exception".
        std::cerr << " lastElement() called on empty Vector " << std::endl;
        exit ( 1 );
    }

    return *m_pData[ m_size - 1 ];
    
} // end lastElement


template <class Etype>
int Vector<Etype>::lastIndexOf ( const Etype& elem ) const {
    
    // Check for empty vector.
    if ( m_size == 0 )
        return -1;

    UINT i = m_size;

    do {
        i -= 1;
        if ( *m_pData[i] == elem )
            return i;
    }
    
    while ( i != 0 );
    return -1;
    
} // end lastIndexOf


template <class Etype>
UINT Vector<Etype>::size () const {
    return m_size;
} // end size


template <class Etype>
void Vector<Etype>::addElement ( const Etype& obj ) {

    if ( m_size == m_capacity )
        ensureCapacity ( m_capacity + m_increment );

    m_pData[ m_size ++ ] = new Etype ( obj );

} // end addElement


template <class Etype>
void Vector<Etype>::ensureCapacity ( UINT minCapacity ) {

    if ( minCapacity > m_capacity ) {
        UINT i;
        m_capacity = minCapacity;

        Etype** temp = new Etype* [ m_capacity ];

        // Copy all the elements over upto newsize.
        for ( i = 0; i < m_size; i++ )
            temp[i] = m_pData[i];

        delete [] m_pData;
        m_pData = temp;
    }
    
} // end ensureCapacity


template <class Etype>
void Vector<Etype>::insertElementAt ( const Etype& obj, UINT index ) {

    UINT i;

    verifyIndex ( index );    // This will throw if true.

    if ( m_size == m_capacity )
        ensureCapacity ( m_capacity + m_increment );

    Etype* newItem = new Etype ( obj );    // Pointer to new item.
    Etype* tmp;                            // Temp to hold item to be moved over.

    for ( i = index; i <= m_size; i++ ) {
        tmp = m_pData[i];
        m_pData[i] = newItem;

        if ( i != m_size )
            newItem = tmp;
        else
            break;
    }

    m_size ++;
    
} // end insertElementAt


template <class Etype>
void Vector<Etype>::removeAllElements () {

    UINT i;

    // Avoid memory leak.
    for ( i = 0; i < m_size; i++ )
        delete m_pData[i];

    m_size = 0;
    
} // end removeAllElements


template <class Etype>
bool Vector<Etype>::removeElement ( const Etype& obj ) {

    UINT i;

    for ( i = 0; i < m_size; i++ ) {
        if ( *m_pData[i] == obj ) {
            removeElementAt ( i );
            return true;
        }
    }

    return false;

} // end removeElement


template <class Etype>
void Vector<Etype>::removeElementAt ( UINT index ) {

    UINT i;

    verifyIndex ( index );
    delete m_pData[ index ];
    for ( i = index + 1; i < m_size; i++ )
        m_pData[ i - 1 ] = m_pData[i];

    m_size--;
    
} // end removeElementAt


template <class Etype>
void Vector<Etype>::removeElementAtWithoutDestroyingElement ( UINT index ) {

    UINT i;

    verifyIndex ( index );

    for ( i = index + 1; i < m_size; i++ )
        m_pData[ i - 1 ] = m_pData[i];

    m_size--;

} // end removeElementAtWithoutDestroyingElement


template <class Etype>
void Vector<Etype>::setElementAt ( const Etype& obj, UINT index ) {

    verifyIndex ( index );
    *m_pData[ index ] = obj;

} // end setElementAt


template <class Etype>
void Vector<Etype>::setSize ( UINT newSize ) {

    UINT i;

    if ( newSize > m_capacity )
        ensureCapacity ( newSize );
    else if ( newSize < m_size ) {
        for ( i = newSize; i < m_size; i++ )
            delete m_pData[i];

        m_size = newSize;
    }
    
} // end setSize


template <class Etype>
void Vector<Etype>::trimToSize () {

    if ( m_size != m_capacity ) {
        Etype** temp = new Etype* [ m_size ];
        UINT i;

        for ( i = 0; i < m_size; i++ )
            temp[i] = m_pData[i];

        delete [] m_pData;
        m_pData = temp;
        m_capacity = m_size;
    }

} // end trimToSize


template <class Etype>
int Vector<Etype>::min ( UINT left, UINT right ) const {
    return left < right ? left : right;
} // end min


template <class Etype>
void Vector<Etype>::verifyIndex ( UINT index ) const {
    
    if ( index >= m_capacity ) {
        std::cerr << "Index out of bounds";
        exit ( 1 );
    
    }

} // end verifyIndex


template <class Etype>
bool Vector<Etype>::operator == ( const Vector<Etype>& rhv ) {

    UINT i;

    if ( m_size != rhv.m_size ) return false;

    for ( i = 0; i < m_size; i++ ) {
        if ( *m_pData[i] != * ( rhv.m_pData[i] ) ) return false;
    }

    return true;

} // end operator ==


template <class Etype>
const Etype & Vector<Etype>::operator [] ( UINT index ) const {
    return elementAt ( index );
} // end operator []


template <class Etype>
Etype & Vector<Etype>::operator [] ( UINT index ) {

    verifyIndex ( index );
    return *m_pData[ index ];

} // end operator []

#endif
