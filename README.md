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
```
	tar xvf DCoL-v1.1.tar







```



```




```






| -------------	|-------------| 
| -F 1			| It computes the maximum Fisher's discriminant ratio. | 
| -F 1v			| It computes the directional-vector maximum Fisher's discriminant ratio. |
| -F 2 			|
| -F 3			| It computes the maximum (individual) feature efficiency. | 
| -L 1			| It computes the sum of the error distance of a linear classifier.  
| -L 2			| It computes the training error of a linear classifier. | 
| -N 3			| It computes the leave-one-out error rate of the one-nearest neighbor classifier. | 
| -T 1			| It computes the fraction of maximum covering spheres. |
| -T 2			| It computes the average number of points per dimension. |

| Command		| Distance function routine     | 
| -------------	|-------------| 
| -cM 1			| It implements the Euclidean distance function. |
| -cM 3			| It implements the Euclidean distance function weighted by the standard deviation. |
| -nM 1 (default option)| It implements the overlap distance function. |




| -------------	|-------------| 
| -s num		| It changes the random seed to the specified integer num. If num = 0, the seed is randomly generated using the system time. |

#### Output format
detailed information, option -D must be used.

## References
1. Tin Kam Ho and Mitra Basu. Complexity measures of supervised classification problems. IEEE Transactions on Pattern Analysis and Machine Intelligence, 24(3):289-300, 2002.

2. Tin Kam Ho, Mitra Basu, and Martin Law. Measures of geometrical complexity in classification problems. In Data Complexity in Pattern Recognition, pages 1-23. Springer, 2006.

3. Albert Orriols-Puig, Núria Macià, and Tin Kam Ho. Documentation for the data complexity library in C++. Technical report, La Salle - Universitat Ramon Llull, 2010.

## Comments and suggestions
If you have any comments or find any bug, please send an email to aorriols at gmail dot com or macia dot nuria at gmail dot com

## License
DCoL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. DCoL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.