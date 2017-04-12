
/*!

\brief This file contains the implementation of the Date methods.

The class provides the implementation of methods that initialize and compare Date objects.

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


#include "Date.h"


Date::Date () {
    year = month = day = hour = minutes = seconds = 0;
} // end Date


Date::Date ( int _year, int _month, int _day, int _hour, int _minutes, int _seconds ) {
    year    = _year;
    month   = _month;
    day     = _day;
    hour    = _hour;
    minutes = _minutes;
    seconds = _seconds;
} // end Date


void Date::init ( Date& d ) {
    year    = d.year;
    month   = d.month;
    day     = d.day;
    hour    = d.hour;
    minutes = d.minutes;
    seconds = d.seconds;
} // end init


void Date::init ( int _year, int _month, int _day, int _hour, int _minutes, int _seconds ) {
    year    = _year;
    month   = _month;
    day     = _day;
    hour    = _hour;
    minutes = _minutes;
    seconds = _seconds;
} // end init


bool Date::operator < ( Date& d2 ) {

    if ( year != d2.year ) return ( year < d2.year );
    else if ( month   != d2.month   ) return ( month   < d2.month   );
    else if ( day     != d2.day     ) return ( day     < d2.day     );
    else if ( hour    != d2.hour    ) return ( day     < d2.hour    );
    else if ( minutes != d2.minutes ) return ( minutes < d2.minutes );
    else if ( seconds != d2.seconds ) return ( seconds < d2.seconds );

    return false;

} // end operator <


bool Date::operator > ( Date& d2 ) {

    if ( year != d2.year ) return ( year > d2.year );
    else if ( month   != d2.month   ) return ( month   > d2.month   );
    else if ( day     != d2.day     ) return ( day     > d2.day     );
    else if ( hour    != d2.hour    ) return ( day     > d2.hour    );
    else if ( minutes != d2.minutes ) return ( minutes > d2.minutes );
    else if ( seconds != d2.seconds ) return ( seconds > d2.seconds );

    return false;

} // end operator >


bool Date::operator <= ( Date& d2 ) {

    if ( year != d2.year ) return ( year <= d2.year );
    else if ( month   != d2.month   ) return ( month   <= d2.month   );
    else if ( day     != d2.day     ) return ( day     <= d2.day     );
    else if ( hour    != d2.hour    ) return ( day     <= d2.hour    );
    else if ( minutes != d2.minutes ) return ( minutes <= d2.minutes );
    else if ( seconds != d2.seconds ) return ( seconds <= d2.seconds );

    return true;

} // end operator <=


bool Date::operator >= ( Date& d2 ) {

    if ( year != d2.year ) return ( year >= d2.year );
    else if ( month   != d2.month   ) return ( month   >= d2.month   );
    else if ( day     != d2.day     ) return ( day     >= d2.day     );
    else if ( hour    != d2.hour    ) return ( day     >= d2.hour    );
    else if ( minutes != d2.minutes ) return ( minutes >= d2.minutes );
    else if ( seconds != d2.seconds ) return ( seconds >= d2.seconds );

    return true;

} // end operator >=


bool Date::operator == ( Date& d2 ) {

    if ( year != d2.year ) return false;
    else if ( month   != d2.month   ) return false;
    else if ( day     != d2.day     ) return false;
    else if ( hour    != d2.hour    ) return false;
    else if ( minutes != d2.minutes ) return false;
    else if ( seconds != d2.seconds ) return false;

    return true;

} // end operator ==


bool Date::operator != ( Date& d2 ) {

    if ( year != d2.year ) return true;
    else if ( month   != d2.month   ) return true;
    else if ( day     != d2.day     ) return true;
    else if ( hour    != d2.hour    ) return true;
    else if ( minutes != d2.minutes ) return true;
    else if ( seconds != d2.seconds ) return true;

    return false;

} // end operator !=


Date& Date::operator = ( Date& d ) {

    year    = d.year;
    month   = d.month;
    day     = d.day;
    hour    = d.hour;
    minutes = d.minutes;
    seconds = d.seconds;

    return *this;

} // end operator =


std::ostream& operator << ( std::ostream& os, Date& d ) {

    os << " Year-Month-Day: " << d.year << "-" << d.month << "-" << d.day << " \t ";
    os << " hour:minutes:seconds: " << d.hour << ":" << d.minutes << ":" << d.seconds;

    return os;

} // end operator << 
