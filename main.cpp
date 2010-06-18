// Luke Schlather
// Monday, April 12 2010
// Licensed under the GPLv3
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <unistd.h>
#include <string>
#include "LJFS_Utils.h"
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
  
  int threshold = 40;
  double pct = .75;
  double iterations =1000;
  double popcount = 480;
  ImageSet::setWidth(24);
  ImageSet::setHeight(18);
  float scale = 1;

  string directory("/home/project");
  bool recurse=0;
  string moldPath("/home/project/Pittsburgh_November_035.JPG");
  string outputDirectory("/home/project/output/");
  string usage("ImageWeave -d <directory> \n -h : Print this message. \n -x <width> : set x dimension for image cells  \n -y <height> : set y dimension for image cells \n");

  char c;
  while ((c =getopt(argc,argv,"hd:s:x:y:t:p:m:i:c:o:R"))!=-1) {
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
    case 'o':
      outputDirectory=string(optarg);
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
    case 'R':
      recurse=1;
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


  ImageSet bunch = ImageSet(directory.c_str(),recurse);

  cout << "Read in " << bunch.count() << " images." << endl;
  
  //  CImgDisplay library(  bunch.weaveAll(10,10), "Image Library");
  //bunch.sort(threshold,pct);
  //  CImgDisplay blank(mold);

  // string testset[10] = {
  //   moldPath,
  //   "/home/pictures/Facebook/Travis Hartman/28694_10150183527095147_586850146_12291633_1995_n.jpg",
  //   "/home/pictures/Facebook/Travis Hartman/28533_10150193005605147_586850146_12513724_3583264_n.jpg",
  //   "/home/pictures/2009/PitStop Lindy Hop/Pittsburgh_November_088.JPG",
  //   "/home/pictures/2009/PitStop Lindy Hop/Pittsburgh_November_104.JPG",
  //   "/home/pictures/2009/PitStop Lindy Hop/Pittsburgh_November_070.JPG",
  //   "/home/pictures/Facebook/Travis Hartman/32533_10150201850665147_586850146_12770126_6931221_n.jpg",
  //   "/home/pictures/2009/PitStop Lindy Hop/Pittsburgh_November_036.JPG",
  //   "/home/pictures/Facebook/Travis Hartman/32533_10150201850840147_586850146_12770146_1439247_n.jpg",
  //   "/home/pictures/Facebook/Travis Hartman/26702_10150210832665147_586850146_13044142_3554681_n.jpg",
  // };


  // for (int i=0;i<10;++i) {
  //   moldPath=testset[i];
     CImg<uchar> mold(moldPath.c_str());
    for (threshold=0;threshold<=100;threshold+=10) {
      vector< vector<int> > tapestry = bunch.bruteForce(mold,threshold,pct);
      CImg<uchar> output= bunch.weave(tapestry);
      
      
      fs::path outdir(outputDirectory);
      fs::path moldfile(moldPath);
      string name = moldfile.filename();
      stringstream outfilename(outputDirectory);
      outfilename << printCurrentTime() << "_" << name << "-t" << threshold << "_" << bunch.count() << "_images.ppm";
      cout << "Writing " << outfilename.str() << endl;
      output.save((outputDirectory + outfilename.str()).c_str());
    }
  // }

  //  CImgDisplay result(output);
  // while ( !result.is_closed() ) {
  //   result.wait();
    
  // }
  return 0;
}

