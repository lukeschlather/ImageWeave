#ifndef _LOOM_
#define _LOOM_
// Luke Schlather
// Friday, April 30 2010
// Licensed under the LGPL

#include<vector>
#include<map>
typedef unsigned char uchar;
#include "ImageSet.h"


typedef std::vector <std::vector <int> > Configuration;



void print(const Configuration& src);


class Loom { 
  
  Configuration config;
  std::multimap<int,char**> weights;
  //  ImageSet library;
  cimg_library::CImg<uchar> pattern;

  int cellularWidth;
  int cellularHeight;

  void destroy();
  //  void clearWeights();
 public:
  Loom(int x, int y, int count);
  ~Loom() { destroy();}


  Configuration& getConfiguration() { return config;}
  
  // Distance/Weight/Quality function. Checks weights table to see if
  // it's already been computed for that cell position against the
  // given id, and if not, computes the weight and places it in the
  // table.
  uchar weight(int id, int x, int y);  

};


#endif // _LOOM_
