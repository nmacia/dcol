
######################################################################
##
## Makefile for DCoL in C++.
##
## @author  Albert Orriols-Puig and Nuria Macia 
##          Grup de Recerca en Sistemes Intel.ligents
##          La Salle - Universitat Ramon Llull 
##          C/ Quatre Camins, 2. 08022, Barcelona (Spain)
## @date    Created April, 2009
##          Last modified December, 2010
##
## Copyright (C) 2009  Albert Orriols-Puig and Nuria Macia
##
## This file is part of DCoL.
##
## DCoL is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## DCoL is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with DCoL.  If not, see <http://www.gnu.org/licenses/>.
######################################################################

# MACROS
OBJS =  EuclideanFunction.o\
	NormalizedEuclideanFunction.o\
	StdWeightedEuclideanFunction.o\
	OverlapFunction.o\
	VDMFunction.o\
	Date.o\
        DateContainer.o\
	StringTokenizer.o\
	Utils.o\
	Matrix.o\
	Dataset.o\
	ExtendedDataset.o\
	ComplexityMeasures.o\
	Statistics.o\
	SMO.o\
	InputOptions.o\
	Main.o

# TESTING FLAGS
# CFLAGS = -ansi -Wall -Wextra -c -g -Wno-deprecated -pg
# OFLAGS = -lm -g -Wno-deprecated -pg -o
# P_FLAGS = -c -Wno-deprecated 

# DEBUGING FLAGS
# CFLAGS = -ansi -Wall -Wextra -c -ggdb -Wno-deprecated 
# OFLAGS = -lm -ggdb -Wno-deprecated -o
# P_FLAGS = -c -Wno-deprecated 

# MEMORY PROFILING FLAGS
# CFLAGS = -g -ansi -Wall -Wextra -c -Wno-deprecated -fno-inline
# OFLAGS = -g -fno-inline -lm -O3 -Wno-deprecated -o
# P_FLAGS = -g -c -Wno-deprecated

# TIME PROFILING FLAGS
# CFLAGS = -ansi -Wall -Wextra -c -g -pg -Wno-deprecated
# OFLAGS = -lm -g -pg -Wno-deprecated -o 
# P_FLAGS = -c -g -pg -Wno-deprecated 
### For time profiling run ./dcol. This would generate gmon.out
### Then, run gprof dcol. This will generate a tree.

# RUNNING TAGS
 CFLAGS = -ansi -Wall -Wextra -O3 -c -Wno-deprecated 
 OFLAGS = -lm -Wno-deprecated -O3 -o
 P_FLAGS = -c -Wno-deprecated 

all: total

#### Compiling Distance functions 
NormalizedEuclideanFunction.o: DistanceFunctions/NormalizedEuclideanFunction.cpp  DistanceFunctions/NormalizedEuclideanFunction.h DistanceFunctions/DistanceFunction.h
	g++ ${CFLAGS} DistanceFunctions/NormalizedEuclideanFunction.cpp

StdWeightedEuclideanFunction.o: DistanceFunctions/StdWeightedEuclideanFunction.cpp DistanceFunctions/StdWeightedEuclideanFunction.h DistanceFunctions/DistanceFunction.h
	g++ ${CFLAGS} DistanceFunctions/StdWeightedEuclideanFunction.cpp

EuclideanFunction.o: DistanceFunctions/EuclideanFunction.cpp  DistanceFunctions/EuclideanFunction.h DistanceFunctions/DistanceFunction.h
	g++ ${CFLAGS} DistanceFunctions/EuclideanFunction.cpp

OverlapFunction.o: DistanceFunctions/OverlapFunction.cpp DistanceFunctions/OverlapFunction.h DistanceFunctions/DistanceFunction.h
	g++ ${CFLAGS} DistanceFunctions/OverlapFunction.cpp

VDMFunction.o: DistanceFunctions/VDMFunction.cpp DistanceFunctions/VDMFunction.h DistanceFunctions/DistanceFunction.h
	g++ ${CFLAGS} DistanceFunctions/VDMFunction.cpp

#### Compiling Date class
Date.o: Date.h Date.cpp
	g++ ${CFLAGS} Date.cpp

#### Compiling DateContainer class
DateContainer.o: DateContainer.h DateContainer.cpp Date.h
	g++ ${CFLAGS} DateContainer.cpp

#### Compiling StringTokenizer class
StringTokenizer.o: StringTokenizer.h StringTokenizer.cpp
	g++ ${CFLAGS} StringTokenizer.cpp

#### Compiling Utils class
Utils.o: Utils.h Utils.cpp
	g++ ${CFLAGS} Utils.cpp

#### Compiling Matrix class
Matrix.o: Matrix.h Matrix.cpp
	g++ ${CFLAGS} Matrix.cpp

#### Compiling Dataset class
Dataset.o: Dataset.cpp Dataset.h Utils.h DateContainer.h
	g++ ${CFLAGS} Dataset.cpp

#### Compiling Statistics
Statistics.o: Dataset.h Statistics.cpp Utils.h
	g++ ${CFLAGS} Statistics.cpp

#### Compiling ExtendedDataset class 
ExtendedDataset.o: Dataset.h Dataset.cpp ExtendedDataset.h ExtendedDataset.cpp
	g++ ${CFLAGS} ExtendedDataset.cpp

#### Compiling ComplexityMeasures class
ComplexityMeasures.o: ExtendedDataset.h ExtendedDataset.cpp ComplexityMeasures.h ComplexityMeasures.cpp
	g++ ${CFLAGS} ComplexityMeasures.cpp

#### Compiling SMO
SMO.o: Dataset.h Dataset.cpp ComplexityMeasures.h SMO.cpp 
	g++ ${CFLAGS} SMO.cpp

#### Compiling InputOptions class
InputOptions.o: InputOptions.cpp InputOptions.h 
	g++ ${CFLAGS} InputOptions.cpp

#### Compiling Main
Main.o: Main.cpp  Dataset.h StringTokenizer.h Utils.h ResultsContainer.h
	g++ ${CFLAGS} Main.cpp


# Linking the objects
total: $(OBJS)
	g++ $(OBJS) $(OFLAGS) dcol 
	#cp -f ../*.o ./

prof: $(OBJS)
	g++ $(OBJS) $(OFLAGS) dcol 


# To clean the compiled files
clean:
	rm -f *.o dcol core* *~  DistanceFunctions/*~  DistanceFunctions/*.bak *.bak

# END OF MAKEFILE
