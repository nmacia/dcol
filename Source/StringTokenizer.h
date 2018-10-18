
#ifndef STRINGTOKENIZER_H
#define STRINGTOKENIZER_H

#include "Vector.h"

#include <cstring>
#include <iostream>


/*!

\brief Implements a string tokenizer class to break strings into tokens.

This class is similar to the Java one and splits a string into tokens separated by the specified delimiters.

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
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DCoL. If not, see <http://www.gnu.org/licenses/>.

*/


class StringTokenizer {

private:

    /** Vector of tokens. */
    Vector<std::string> elements;

    /** Index of the token to be consulted. */
    int index;

    /** Counter of tokens. */
    int count;

public:

    /**
     * Constructs a new StringTokenizer object for the specified string. The characters in the separator argument 
     * are the delimiters that separate tokens and will not be treated as tokens.
     * @param str is the string to be parsed.
     * @param separator is the delimiter character.
     */
    StringTokenizer ( std::string str, std::string separator );

    /**
     * Constructs a StringTokenizer object for the specified string. The characters in the separator and sep2 
     * argument are the delimiters that separate tokens and will not be treated as tokens.
     * @param str is the string to be parsed.
     * @param separator is the first type of delimiter. 
     * @param sep2 is the second type of delimiter.
     */
    StringTokenizer ( std::string str, std::string separator, std::string sep2 );

    /**
     * Destructs the StringTokenizer object.
     */
    ~StringTokenizer ();

    /**
     * Returns the next token.
     * @return A string with the next token.
     */
    std::string getNextToken ();

    /**
     * Counts the number of tokens that a string contains. 
     * @return An integer with the number of tokens contained in the parsed string.
     */
    int countTokens () const;

    /**
     * Indicates whether there are more tokens.
     * @return A Boolean that indicates whether there are more tokens. 
     * @retval true there are more tokens in the string.
     * @retval false there are no more tokens in the string. 
     */
    bool hasMoreTokens () const;

};

#endif
