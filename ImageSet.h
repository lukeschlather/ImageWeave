#ifndef _IMAGELOADER_
#define _IMAGELOADER_
#include "CImg.h"

#include<vector>

typedef unsigned char uchar;
typedef cimg_library::CImg<uchar> Image;
// Luke Schlather
// Tuesday, April 13 2010
// Licensed under the LGPL, see LICENSE.txt for the full text. 
class ImageSet { 
  static double width;
  static double height;
  cimg_library::CImgList<uchar> bunch; 

public:
  // Reads all files in a directory, determines which ones are images,
  // and scales them to width by height, adding them to the bunch CImgList.
  // Error codes:
  // 2: imagedir does not exist.
  // 3: imagedir is not a directory.
  ImageSet(const char* imagedir);
  
  static ImageSet LoadSet(const char* setfile);
  static ImageSet WriteSet(const char* setfile);
  
  cimg_library::CImgList<uchar> & get_list() {return bunch;}
  static void setWidth(const int& wid) { width = wid; }
  static void setHeight(const int& hei) { height=hei; }

  int count() {
    return bunch.width();
  }
  
  Image operator[] (const int & id);

  // Takes a two-dimensional array of image ids, and outputs a single
  // image built of those images. **Expects a rectangle.**
  Image weave(std::vector< std::vector<int> >& matrix);  

  Image weaveAll(int x, int y);
};

#endif // _IMAGELOADER_
