// Luke Schlather
// Monday, April 12 2010
// Licensed under the LGPL
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <unistd.h>
#include <string>

#include "ImageSet.h"
#include "Loom.h"

namespace fs = boost::filesystem;
using namespace cimg_library;
using namespace std;


void strike(CImg<uchar> & img,int x, int y, int c, int len) {
  while (--len) {
    for (int i=0;i<20;++i) {
      img(x+i,y+i,c)= 255;
    }
    ++x, ++y;
  }
}

int main(int argc, char** argv) {
  
  // if (argc < 2) {
  //   cout << usage << endl;
  //   exit(0);
  // } // no file
  int threshold = 40;
  double pct = 60;
  string usage("Similarity -d <directory> \n -h : Print this message. \n -x <width> : set x dimension for image cells  \n -y <height> : set y dimension for image cells \n");

  char c;
  while ((c =getopt(argc,argv,"hd:s:x:y:t:p:m:i:c:"))!=-1) {
    switch(c) {
    case 'h':
      cout << usage << endl;
      exit(0);
      break;
    case 't':
      threshold=atoi(optarg);
      break;
    case 'c':
      pct = atof(optarg);
      break;
    default:
      cout << "Unrecognized option: '" << c <<"'" << endl;
      cout << usage << endl;
    }
  } 
  // if(optind<argc) {
  // }
  // else {
  //   cout << "No image specified" << endl;
  //   cout << usage << endl;
  //   exit(0);
  // } 
  string strone(argv[optind]);
  string strtwo(argv[optind+1]);

  ImageSet bunch;
  bunch.setThreshold(threshold);
  
  CImg<uchar> one(strone.c_str());
  //  CImgDisplay first(one,strtwo.c_str());
  CImg<uchar> two(strtwo.c_str());
  //  CImgDisplay second(two,strtwo.c_str());
  cout << "Similarity:" << bunch.percentMatch(one,two) << endl;
  cout << "Similarity:" << bunch.percentMatch(two,one) << endl;
  

  // while ( !first.is_closed() && !second.is_closed() ) {
  //   second.wait();
  // }
  return 0;
}

