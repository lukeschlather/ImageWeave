// Luke Schlather
// Monday, April 12 2010
// Licensed under the GPLv3
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
  double pct = .75;
  double iterations =1000;
  double popcount = 480;
  ImageSet::setWidth(24);
  ImageSet::setHeight(18);
  float scale = 1;
  char c;
  string directory("/home/project");
  string moldPath("/home/project/Pittsburgh_November_035.JPG");
  string usage("ImageWeave -d <directory> \n -h : Print this message. \n -x <width> : set x dimension for image cells  \n -y <height> : set y dimension for image cells \n");

  while ((c =getopt(argc,argv,"hd:s:x:y:t:p:m:i:c:"))!=-1) {
    switch(c) {
    case 'h':
      cout << usage << endl;
      exit(0);
      break;
    case 'x':
      ImageSet::setWidth(atof(optarg));
      break;
    case 'm':
      moldPath=string(optarg);
      break;
    case 'd':
      directory = string(optarg);
      break;
    case 'y':
      ImageSet::setHeight(atof(optarg));
      break;
    case 't':
      threshold=atoi(optarg);
      break;
    case 'c':
      pct = atof(optarg);
      break;
    case 'p':
      popcount= atof(optarg);
      break;
    case 'i':
      iterations= atof(optarg);
      break;
    case 's':
      scale = atof(optarg);
      ImageSet::setWidth(24*scale);
      ImageSet::setHeight(18*scale);
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


  ImageSet bunch = ImageSet(directory.c_str());

  cout << "Read in " << bunch.count() << " images." << endl;
  
  CImgDisplay library(  bunch.weaveAll(10,10), "Image Library");
  //bunch.sort(threshold,pct);
  CImg<uchar> mold(moldPath.c_str());
  CImgDisplay blank(mold);

  vector< vector<int> > tapestry = bunch.bruteForce(mold,threshold,pct);
  CImgDisplay result(bunch.weave(tapestry));
  while ( !library.is_closed() ) {
    library.wait();
    
  }
  return 0;
}

