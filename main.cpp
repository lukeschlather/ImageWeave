// Luke Schlather
// Monday, April 12 2010
// Licensed under the LGPL
#include <boost/filesystem.hpp>
#include "ImageSet.h"
#include <iostream>
#include <unistd.h>
#include <string>
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
  CImg<uchar> image("/home/project/Pittsburgh_November_013.JPG"), visu(500,400,1,3,0);
  std::cout << image.width() << " " << image.height() << std::endl;
  image = image.resize(image.width()/2,image.height()/2);
  const uchar red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
  image.blur(2.5);
  CImgDisplay main_disp(image,"Click a point"), draw_disp(visu,"Intensity profile"),library(bunch.get_list(), "Image Library");
  while (!main_disp.is_closed() && !draw_disp.is_closed()) {
    main_disp.wait();
    if (main_disp.button() && main_disp.mouse_y()>=0) {
      const int y = main_disp.mouse_y();
      visu.fill(0).draw_graph(image.get_crop(0,y,0,0,image.width()-1,y,0,0),red,1,1,0,255,0);
      visu.draw_graph(image.get_crop(0,y,0,1,image.width()-1,y,0,1),green,1,1,0,255,0);
      visu.draw_graph(image.get_crop(0,y,0,2,image.width()-1,y,0,2),blue,1,1,0,255,0).display(draw_disp);
      
    }
  }
  return 0;
}

