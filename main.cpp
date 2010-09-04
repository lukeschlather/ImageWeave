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
  
  int threshold;
  double pct = .75;
  double iterations =1000;
  double popcount = 480;
  ImageSet::setWidth(24);
  ImageSet::setHeight(18);
  float scale = 1;



  string directory("/home/project");
  bool recurse=0;
  vector<string> moldPaths;
  string outputDirectory("/home/project/output/");
  string usage("ImageWeave -d <directory> \n -h : Print this message. \n -x <width> : set x dimension for image cells  \n -y <height> : set y dimension for image cells \n");

  bool battery=false; // run a battery of tests

  char c;
  while ((c =getopt(argc,argv,"hd:s:x:y:t:p:m:i:c:o:Rb"))!=-1) {
    switch(c) {
    case 'h':
      cout << usage << endl;
      exit(0);
      break;
    case 'x':
      ImageSet::setWidth(atof(optarg));
      break;
    case 'b':
      battery=true;
      break;
    case 'm':
      moldPaths.push_back(string(optarg));
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

  ImageSet bunch = ImageSet(directory.c_str(),recurse);

  cout << "Read in " << bunch.count() << " images." << endl;
  for (unsigned int i=0;i<moldPaths.size();++i) {
    string moldPath=moldPaths[i];
    CImg<uchar> mold(moldPath.c_str());
    int maxThreshold;
    int step=10;
    if(battery) {
      maxThreshold=threshold;
      threshold=threshold/10;
      step=threshold;
    }
    else { 
      maxThreshold=threshold; 
      step=100;
    }
    for (;threshold<=maxThreshold;threshold+=step) {
      cout << "step: " << step << endl;
      CImg<uchar> mold(moldPath.c_str());
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
  } 

  
  return 0;
}

