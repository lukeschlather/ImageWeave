// Luke Schlather
// Saturday, April 17 2010
// Licensed under the LGPL
#include<iostream>
#include<vector>
#include "ImageSet.h"
#include <boost/filesystem.hpp>
using namespace std;
using namespace cimg_library;
namespace fs = boost::filesystem;

extern double ImageSet::width=24;
extern double ImageSet::height=18;

ImageSet::ImageSet(const char* imagedir) {
  if ( !fs::exists(imagedir ) ) {
    std::cerr << "\n Image directory not found: " << imagedir << std::endl;
    throw 2;
  }
  if ( fs::is_directory( imagedir ) )  {
    fs::directory_iterator end_iter;
    for ( fs::directory_iterator dir_itr( imagedir );
          dir_itr != end_iter;
          ++dir_itr )  {
      try  {
        if ( fs::is_directory( dir_itr->status() ) )  {
          std::cout << dir_itr->path().filename() << " [directory]\n";
        }
        else if ( fs::is_regular_file( dir_itr->status() ) )  {
	  // We have a real file, let's try to read it in.
          std::cout << "Reading" << dir_itr->path().filename() << "\n";
	  CImg<uchar> image(dir_itr->path().external_file_string().c_str());
	  double iwidth = image.width();
	  double iheight = image.height();
	  double aspectRatio = iwidth/iheight;
	  if (aspectRatio > .5 && aspectRatio < 2) {
	    cout << "'good' aspect ratio:" << aspectRatio << endl;
	    bunch.push_back(image.resize(width,height));
	    iwidth=bunch.back().width();
	    iheight=bunch.back().height();
	    cout << iwidth <<"x" << iheight << endl;
	  } else {
	    cout << "Aspect ratio too weird: "<< aspectRatio << ": " << iwidth << "x" << iheight << endl;
	  }
       }
        else {
          std::cout << dir_itr->path().filename() << " [other]\n";
        }
	
      }
      catch ( const std::exception & ex ) {
        std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
      }
    }
      
      
      
    } else  {
    std::cerr << "\n Not an image directory: " << imagedir << std::endl;
      throw 3;
    }
    
}

Image ImageSet::weaveAll(int h, int w) {
  vector< vector <int> > all;
  int i=0;
  for( int x=0;x<w;++x) {
    all.push_back(vector<int>());
    for( int y=0;y<h;++y) {
      all[x].push_back((i++) % bunch.width());
    }
  }
  
  return this->weave(all);
}

Image ImageSet::weave(vector< vector<int> >& matrix) {
  double xdim=matrix.size();
  double ydim=matrix[0].size();
  
  Image ret(xdim*width,ydim*height,1,3,0);
  
  for (int x=0;x<xdim;++x) {
    for (int y=0;y<ydim;++y) {
      for(int a=0;a<width;++a) {
  	for(int b=0;b<height;++b) {
	  CImg<uchar> &src =  bunch[matrix[y][x]];
	  ret( x*width + a,y*height + b,0)= src(a,b,0);
	  ret( x*width + a,y*height + b,1)= src(a,b,1);
	  ret( x*width + a,y*height + b,2)= src(a,b,2);
  	}
      }
    }
  }
  return ret;
}
