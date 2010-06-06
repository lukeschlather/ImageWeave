// Luke Schlather
// Friday, April 30 2010
// Licensed under the LGPL
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Loom.h"
using namespace std;
using namespace   cimg_library;

void print(const Configuration& src) {
  cout << endl;
  for(int x=0;x<src.size();++x) {
    for(int y=0;y<src[x].size();++y) {
      cout << src[x][y] << ", ";
    }
    cout << endl;
  }
  
}

void Loom::destroy() {
  multimap<int,char**>::iterator end = weights.end();
 
  for (multimap<int,char**>::iterator img=weights.begin();img!=end;++img) {
    //delete the arrays
  }
}

Loom::Loom(int w, int h,int count)    {

  Configuration config;
  for( int x=0;x<w;++x) {
    config.push_back(vector<int>());
    for( int y=0;y<h;++y) {
      config[x].push_back( rand() % count);
    }
  }
  
}

// uchar Loom::weight(int id, int x, int y) {
//   int cellwidth = library.getWidth();
// int cellheight = library.getHeight();
//     CImg<uchar> *cell = &(library(id));
//   int xcorner = x*cellwidth;
//   int ycorner = y*cellheight;

//   return 0;
// }
