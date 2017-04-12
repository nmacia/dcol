## Description

The data complexity library (DCoL) is a library implemented in C++ that provides the implementation of a set of measures designed to characterize the apparent complexity of data sets for supervised learning, which were originally proposed by Ho and Basu (2002). In particular, the code supplies the following measures:

**Measures of overlaps in the feature values from different classes.** 

* The maximum Fisher's discriminant ratio (F1).
* The directional-vector maximum Fisher's discriminant ratio (F1v).
* The overlap of the per-class bounding boxes (F2).
* The maximum (individual) feature efficiency (F3).
* The collective feature efficiency (F4).

**Measures of class separability.** 

* The leave-one-out error rate of the one-nearest neighbor classifier (L1).
* The minimized sum of the error distance of a linear classifier (L2).
* The fraction of points on the class boundary (N1).
* The ratio of average intra/inter class nearest neighbor distance (N2).
* The training error of a linear classifier (N3).

**Measures of geometry, topology, and density of manifolds.** 

* The nonlinearity of a linear classifier (L3).
* The fraction of maximum covering spheres (T1).
* The average number of points per dimension (T2).

The implementation of these complexity measures is based on the descriptions provided by Ho and Basu (2002) and Ho et al. (2006). However, some of them have been revised and updated from its initial definition (Orriols-Puig et al., 2010). The majority of these measures were initially designed for two-class data sets and were only applied to problems with continuous attributes; nominal or categorical attributes were numerically coded and treated as continuous. The latter restriction was because most of the complexity measures rely on distance functions between attributes. In our implementation, all the measures, except for those based on a linear discriminant and on the directional-vector Fisher's discriminant, have been extended to deal with m-class data sets (m > 2), following the guidelines suggested by Ho et al. (2006). Furthermore, the most relevant distance functions for continuous and nominal attributes have been implemented. In this way, we enable our library to deal with m-class data sets that contain nominal and/or continuous attributes.

The library also offers two other functionalities:

* Partition of the data set by means of stratified k-fold cross-validation.
* Conversion of an m-class data set (m > 2) to m two-class data sets. Each new data set discriminates one of the classes against the others.


## Install

### System requeriments
The only requirement to run the DCoL is to have a C++ compiler installed in your computer. The code was implemented on a Unix/Linux 32-bit architecture, and g++4 was used. Nonetheless, it does not use third party libraries for compatibility across platforms, so that the code can be compiled and run in any platform that has a C++ compiler. The instructions given in the description below correspond to Unix/Linux commands. The same process can be followed with the equivalent instructions for Windows and Mac operating systems.

### How to compile the code
After downloading the software release, the sources can be extracted typing the following commands:        
```	gunzip DCoL-v1.1.tar.gz
	tar xvf DCoL-v1.1.tar```
For Windows operating system, please use any available unzip software. This command will create five folders: (1) Data, (2) Source, (3) Test, (4) Help, and (5) Documentation. 
**Data** contains data set examples belonging to the UCI repository. Specifically, there are four two-class data sets—h-s, pim, tao, and wbcd—, and three m-class data sets (m > 2)— bal, iris, and thy. 
**Source** has the source files of the library (including the folder DistanceFunctions which has the implementation ofthe distance functions) and a Makefile. 
**Test** contains a folder named Datasets which includes some data sets used for the testing and the script to replicate the experiments reported in the Appendix B. 
**Help** has the on-line documentation browser (in HTML) and the off-line reference manual (in LATEX) from the source files. 
**Documentation** contains a user and programmer manual.To compile the code in a Unix/Linux environment, go to the Source folder and call 

```	make
``` 
This will generate a set of object files plus the application file, which is named dcol. Temporal and object files can be removed with the command

```	make clean
```
 For Windows and Mac systems, please follow the compilation procedure of the chosen compiler.Note: gettimeofday() is not a native Windows function. So, please comment out lines 64-69 in file Utils.cpp and uncomment lines 72-73.
### How to run the codeAfter compiling the code, the application can be run with the command:

``` 	./dcol
 ``` 
 The input format, command line options, and output format are specified below.
#### Input formatThe application requires an input file which must contain either (1) the data set for runs with a single data set or (2) a list of paths to data set files for runs with multiple data sets. The input data sets can follow either the WEKA format (Witten and Frank, 2005) or the KEEL format (Alcalá-Fdez et al., 2008) which is an extension of the WEKA format. Note that the application does not accept string attributes.
#### Command line syntaxThe syntax to call the application is```      ./dcol -i <input file> -o <output file> [OPTIONS]```Two parameters are required to run the application:1. -i \<input file\>: It specifies the name of the file with either (1) the input data set in KEEL or WEKA format for runs with a single data set (i.e., option -B is not specified) or (2) a list of data set paths and names for runs with multiple data sets (if option -B is specified).2. -o \<output file\>: It sets the name of the output file where the results of the application will be written.
In addition, a set of optional parameters can be specified (i.e., [OPTIONS]). In the following, we show the syntax for (1) the options to select which complexity measures are run, (2) the options to select distance functions for continuous attributes and nominal attributes, and (3) other options, which allow for other functionalities such as transformation of m-class data sets (m > 2) into m two-class data sets and stratified cross-validation.
###### Complexity measures options| Command		| Complexity measure routine     | 
| -------------	|-------------| 
| -F 1			| It computes the maximum Fisher's discriminant ratio. | 
| -F 1v			| It computes the directional-vector maximum Fisher's discriminant ratio. |
| -F 2 			|
| -F 3			| It computes the maximum (individual) feature efficiency. | | -F 4			| It computes the collective feature efficiency. |
| -L 1			| It computes the sum of the error distance of a linear classifier.  
| -L 2			| It computes the training error of a linear classifier. | | -L 3			| It computes the nonlinearity of a linear classifier. || -N 1			| It computes the fraction of points on the class boundary. || -N 2			| It computes the ratio of average intra/inter class nearest neighbor distance. |
| -N 3			| It computes the leave-one-out error rate of the one-nearest neighbor classifier. | | -N 4			| It computes the nonlinearity of the one-nearest neighbor classifier. |
| -T 1			| It computes the fraction of maximum covering spheres. |
| -T 2			| It computes the average number of points per dimension. || -A			| This option runs all the complexity measures.| -d			| It splits m-class data sets into m two-class data sets and applies the complexity measures to these two-class data sets. If the input data set has two classes, the activation of this option has no effect on the run. |
###### Distance function optionsWe extended the code to be able to efficiently deal with continuous and nominal/categorical attributes. 
| Command		| Distance function routine     | 
| -------------	|-------------| 
| -cM 1			| It implements the Euclidean distance function. || -cM 2	(default option) | It implements the normalized Euclidean distance function. |
| -cM 3			| It implements the Euclidean distance function weighted by the standard deviation. |
| -nM 1 (default option)| It implements the overlap distance function. || -nM 2 		| It implements the VDM distance function (Stanfill and Waltz, 1986). | 
The application also permits mapping the nominal values into integers and applying an Eu- clidean distance (option -nM 3) or a normalized Euclidean distance (option -nM 4).
###### Other options
The application also provides other functionalities, which are detailed as follows.
| Command		| Functionality    | 
| -------------	|-------------| | -t2class		| This option requires that the input data set have m classes (m > 2). It generates m new data sets (one for each class), which consist of examples of one class against examples of all the other classes. Therefore, it creates two-class data sets in which each concept is discriminated against the rest of the domain. The name of the output file is determined by option -o.| -cv numFolds		| This option runs a stratified cross-validation with numFolds folds (where numFolds has to be greater than 1). The name of the output file is determined by option -o. If numFolds is not specified or incorrectly set (i.e., numFolds < 2), it is set to 10 by default.|
| -s num		| It changes the random seed to the specified integer num. If num = 0, the seed is randomly generated using the system time. || -D			| It prints debug information to screen while running the application. 	|| -B			| It permits running the complexity measures for multiple data sets. The file specified with option -i is not considered as a data set, but as a file that contains the paths to many data sets (one path in each line). Thus, the application is run with the specified options for each one of the data sets.| -nRU			| It indicates that the missing values do not have to be replaced. If this option is not specified, the library automatically replaces (1) missing values of continuous attributes with the average value of the attribute for the class of the example and (2) missing values of nominal attributes with the median value of the attribute for the class of the example. |

#### Output formatThe output depends on the type of run.* For runs concerning complexity measures, the application writes an output file which specifies the value for each measure. In the next section there are several examples of output files.* For runs concerning options -t2class and -cv, different files are created with the corre- sponding output data sets. The root name of the output file is the one specified with option -o.By default the system saves the computation of the complexity measures in a .txt format file. However, there are two more formats available:1. -latex: It saves the results in a .tex format file. By compiling this file with a LATEX compiler, the resulting document shows the output in a table view.2. -xml: It saves the output in a .xml format file.In any case, information of the process will be displayed on the screen. For more 
detailed information, option -D must be used.Finally, warnings and errors will be reported in file <output file>.log where <output file> is the output file name specified by option -o.

## References
1. Tin Kam Ho and Mitra Basu. Complexity measures of supervised classification problems. IEEE Transactions on Pattern Analysis and Machine Intelligence, 24(3):289-300, 2002.

2. Tin Kam Ho, Mitra Basu, and Martin Law. Measures of geometrical complexity in classification problems. In Data Complexity in Pattern Recognition, pages 1-23. Springer, 2006.

3. Albert Orriols-Puig, Núria Macià, and Tin Kam Ho. Documentation for the data complexity library in C++. Technical report, La Salle - Universitat Ramon Llull, 2010.

## Comments and suggestions
If you have any comments or find any bug, please send an email to aorriols at gmail dot com or macia dot nuria at gmail dot com

## License
DCoL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. DCoL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
