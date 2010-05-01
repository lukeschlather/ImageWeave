// Luke Schlather
// Monday, April 12 2010
// Licensed under the LGPL
#include <boost/filesystem.hpp>
#include <iostream>
#include <unistd.h>
#include <string>

#include "ImageSet.h"
#include "Loom.h"

namespace fs = boost::filesystem;
using namespace cimg_library;
using namespace std;
int main(int argc, char** argv) {
  
  // if (argc < 2) {
  //   cout << usage << endl;
  //   exit(0);
  // } // no file

  ImageSet::setWidth(24);
  ImageSet::setHeight(18);
  char c;
  string usage("ImageWeave <directory> \n -h : Print this message. \n -x <width> : set x dimension for image cells  \n -y <height> : set y dimension for image cells \n");
  while ((c =getopt(argc,argv,"hx:y:"))!=-1) {
    switch(c) {
    case 'h':
      cout << usage << endl;
      exit(0);
      break;
    case 'x':
      ImageSet::setWidth(atof(optarg));
      break;
    case 'y':
      ImageSet::setHeight(atof(optarg));
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


  ImageSet bunch = ImageSet("/home/project/");
  cout << "Read in " << bunch.count() << " images." << endl;
  
  CImgDisplay library(  bunch.weaveAll(10,10), "Image Library");
  while ( !library.is_closed() ) {
    library.wait();
    if (library.button() && library.mouse_y()>=0) {
      const int y = library.mouse_y();
    }
  }
  return 0;
}

