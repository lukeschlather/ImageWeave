#ifndef _IMAGELOADER_
#define _IMAGELOADER_
#include "CImg.h"

#include<vector>
#include<map>
#include<unistd.h>
typedef unsigned char uchar;
typedef cimg_library::CImg<uchar> Image;


class Node;

class Node : public std::pair<int, std::vector<Node> > {
  
};

// Luke Schlather
// Tuesday, April 13 2010
// Licensed under the LGPL, see LICENSE.txt for the full text. 
class ImageSet {
private:
  static double width;
  static double height;
  cimg_library::CImgList<uchar> bunch; 
  uchar threshold;
protected:
  ImageSet& copy(const ImageSet& src);
public:
  // Reads all files in a directory, determines which ones are images,
  // and scales them to width by height, adding them to the bunch CImgList.
  // Error codes:
  // 2: imagedir does not exist.
  // 3: imagedir is not a directory.
  ImageSet(const char* imagedir);
  ImageSet(const ImageSet& src);




  static ImageSet LoadSet(const char* setfile);
  static ImageSet WriteSet(const char* setfile);
  
  cimg_library::CImgList<uchar> & get_list() {return bunch;}
  static void setWidth(const int& wid) { width = wid; }
  static void setHeight(const int& hei) { height=hei; }
  
  double getWidth()  { return width;  }
  double getHeight() { return height; } 
  int count() {  return bunch.width(); }
  
  Image operator[] (const int & id);
  ImageSet& operator= (const ImageSet src) { copy(src); return *this;}


  // Takes a two-dimensional array of image ids, and outputs a single
  // image built of those images. **Expects a rectangle.**
  Image weave(std::vector< std::vector<int> >& matrix);  
  Image weaveAll(int x, int y);

  std::map<int, std::vector<int> > similarityGraph; 
  // Sort based on a tolerance and percentage: 
  //Arguments: 
  //Threshold : maximum variation between color channels
  //  if you have 230 in a red value of img2 and 215 in red of img2,
  //  the variation would be 15. So it's abs(pixel1[c]-pixel2[c]) 
  //pct : percentage of pixels that exceed the maximum
  // tolerance.
  
  // Return:
  // A map holding groups of ids that fall within 
  // threshold and percentage bounds.
  std::map<int, std::vector<int> > & sort(int thresh,double pct);

  std::vector< std::vector<int> > geneticAlgorithm(cimg_library::CImg<uchar> & mold, int iterations, int popcount, int thresh, double pct);

  double percentMatch(int img1, int img2);
  double percentMatch(cimg_library::CImg<uchar> &one, cimg_library::CImg<uchar> &two);
  double percentMatch(cimg_library::CImg<uchar> &mold, double moldMinX, double moldMinY, cimg_library::CImg<uchar> &two);
  int avgDistance(int img1, int img2);
  int medDistance(int img1, int img2);

};

#endif // _IMAGELOADER_
