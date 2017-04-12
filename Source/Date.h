
#ifndef DATE_H 
#define DATE_H 

#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>


/*! 

\brief Implements methods to initialize and compare dates.

This class contains the information of a date.


@author   Albert Orriols-Puig and Nuria Macia <br>
          Grup de Recerca en Sistemes Intel.ligents <br>
          La Salle - Universitat Ramon Llull <br>
          C/ Quatre Camins, 2. 08022, Barcelona (Spain) <br>
@date     Created June, 2010 <br>
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


class Date{

friend class DateContainer;

private:

    /** Year. */
    int year;

    /** Month. */
    int month;

    /** Day. */
    int day;

    /** Hour. */
    int hour;

    /** Minutes. */
    int minutes;

    /** Seconds. */
    int seconds;

public:

    /**
     * Constructs a new Date object.
     */
    Date ();

    /**
     * Constructs a new Date object with the passed values.
     * @param _year is the year.
     * @param _month is the month.
     * @param _day is the day.
     * @param _hour is the hour.
     * @param _minutes is the minutes.
     * @param _seconds is the seconds.
     */
    Date ( int _year, int _month, int _day, int _hour, int _minutes, int _seconds );

    /**
     * Destructs the Date object.
     */
    ~Date () {}

    /**
     * Initializes a Date object with the passed values.
     * @param _year is the year.
     * @param _month is the month.
     * @param _day is the day.
     * @param _hour is the hour.
     * @param _minutes is the minutes.
     * @param _seconds is the seconds.
     */
    void init ( int _year, int _month, int _day, int _hour, int _minutes, int _seconds );

    /**
     * Initializes a Date object with the object.
     * @param d is the date to be copied to the class object.
     */
    void init ( Date& d );

   /**
     * Is the "less than" operator.
     * @param d2 is the Date object with which the current object will be compared to.
     * @return A Boolean that indicates which object is smaller.
     * @retval true the current object is less than the object d2.
     * @retval false the current object is greater than or equal to the object d2.
     */
    bool operator < ( Date& d2 );

    /**
     * Is the "greater than" operator.
     * @param d2 is the Date object with which the current object will be compared to.
     * @return A Boolean that indicates which object is greater.
     * @retval true the current object is greater than the object d2.
     * @retval false the current object is less than or equal to the object d2.
     */
    bool operator > ( Date& d2 );

   /**
     * Is the "less than or equal to" operator.
     * @param d2 is the Date object with which the current object will be compared to.
     * @return A Boolean that indicates which object is smaller or equal.
     * @retval true the current object is less than or equal to the object d2.
     * @retval false the current object is greater than the object d2.
     */
    bool operator <= ( Date& d2 );

    /**
     * Is the "greater than or equal to" operator.
     * @param d2 is the Date object with which the current object will be compared to.
     * @return A Boolean that indicates which object is greater or equal.
     * @retval true the current object is greater than or equal to the object d2.
     * @retval false the current object is less than the object d2.
     */
    bool operator >= ( Date& d2 );

    /**
     * Is the "equal to" operator.
     * @param d2 is the Date object with which the current object will be compared to.
     * @return A Boolean that indicates whether the objects are equal.
     * @retval true the current object is equal to the object d2.
     * @retval false the current object is different from the object d2.
     */
    bool operator == ( Date& d2 );

    /**
     * Is the "different from" operator.
     * @param d2 is the Date object with which the current object will be compared to.
     * @return A Boolean that indicates whether the objects are different.
     * @retval true the current object is different from the object d2.
     * @retval false the current object is equal to the object d2.
     */
    bool operator != ( Date& d2 );

    /**
     * Is the "copy" operator.
     * @param d2 is the Date object that will be copied to the current object. 
     * @return A Date& that references the copied object. 
     */
    Date& operator = ( Date& d2 );

    /**
     * Overloads the standard output.
     * @param os is a reference to the standard output.
     * @param d is a reference to the date that has to be printed.
     * @return A reference to ostream.
     */
    friend std::ostream& operator << ( std::ostream& os, Date& d );

};

#endif
