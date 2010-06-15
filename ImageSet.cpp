// Luke Schlather
// Saturday, April 17 2010
// Licensed under the GPLv3
#include<iostream>
#include "Loom.h"
#include<vector>
#include "ImageSet.h"
#include <boost/filesystem.hpp>
#include "LJFS_Utils.h"
using namespace std;
using namespace cimg_library;
namespace fs = boost::filesystem;


double ImageSet::width;
double ImageSet::height;

template <class T>
CImgView<T>::CImgView(CImg<T>& bas, double x, double y) : base(bas) {
  topX=x;
  topY=y;
  width=base.width();
  height=base.height();
}
template <class T>
CImgView<T>::CImgView(CImg<T>& bas, double x, double y,double wid, double hei) : base(bas) {
  topX=x;
  topY=y;
  width=wid;
  height=hei;
}
template <class T>
CImgView<T>::CImgView(CImg<T>& bas) : base(bas) {width=base.width(); height=base.height();}




ImageSet& ImageSet::copy(const ImageSet& src) {
  this->bunch=src.bunch;
  return *this;
}

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

ImageSet::ImageSet() {}

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


Configuration ImageSet::unravel(CImg<uchar> & input) {

  Configuration ret;
  int frameWidth = input.width() / width;
  int frameHeight = input.height() / height;
  CImg<uchar> tmp(width,height,1,3,0); 


  for (int x=0;x<frameWidth;++x) {
    ret.push_back(vector<int>());
    for (int y=0;y<frameHeight;++y) {
      for(int a=0;a<width;++a) {
	for(int b=0;b<height;++b) {
	  tmp(a,b,0)= input( x*width + a,y*height + b,0);
  	  tmp(a,b,1)= input( x*width + a,y*height + b,1);
  	  tmp(a,b,2)= input( x*width + a,y*height + b,2);
  	}
	cout << endl;
      }
      bunch.push_back(tmp);
      ret[x].push_back(bunch.size()-1);
    }

  }
  print(ret);
  return ret;
}



Image ImageSet::weave(vector< vector<int> >& matrix) {
  double frameWidth=matrix.size();
  double frameHeight=matrix[0].size();
  
  Image ret(frameWidth*width,frameHeight*height,1,3,0);
  
  for (int x=0;x<frameWidth;++x) {
    for (int y=0;y<frameHeight;++y) {
      for(int a=0;a<width;++a) {
  	for(int b=0;b<height;++b) {
	  CImg<uchar> &src =  bunch[matrix[x][y]];
	  ret( x*width + a,y*height + b,0)= src(a,b,0);
	  ret( x*width + a,y*height + b,1)= src(a,b,1);
	  ret( x*width + a,y*height + b,2)= src(a,b,2);
  	}
      }
    }
  }
  return ret;
}

vector< vector<int> > randomConfiguration(int frameWidth, int frameHeight,int max) {
  vector< vector<int> > current;
  for (int x=0;x<frameWidth;++x) {
    current.push_back(vector<int>());
    for (int y=0;y<frameHeight;++y) {
      //probably should use a better random; good enough for jazz. 
      current[x].push_back(rand()%max);
      //strictly speaking I guess jazz would need a good random number generator.
      // though perhaps jazz is about a sort of order through chaos.
      // so that could actually be compatible with a bad random number generator.
    }
  }
  return current;
}



Configuration mate(Configuration& one,Configuration &two) {
  Configuration ret;
  int left=0;
  int right=0;
  for(unsigned int i=0;i<one.size();++i) {
    ret.push_back(vector<int>());
    for(unsigned int j=0;j<one[0].size();++j) {
      if(rand()%2) {
	++right;
	ret[i].push_back(one[i][j]);
      } else {
	++left;
	ret[i].push_back(two[i][j]);
      }
    }
  }
  // cout << "ratio: " << right << " " << left << endl;
  // cout << "ret dimensions:" << ret.size() << " " << ret[0].size() << endl;
  // cout << "one dimensions:" << one.size() << " " << one[0].size() << endl;
  return ret;
}

std::vector< std::vector<int> > ImageSet::bruteForce (cimg_library::CImg<uchar> & mold, int thresh, double pct) {
  //this needs to be refactored away
  threshold=thresh;

  // Possibly better implemented as parameters; on the other hand, 
  // We don't want to be doing sub-pixel approximations. There's enough processing as it is.
  // For now, assume that we've given an image that can be evenly covered without changing
  // the image width/height of our tiles.
  int frameWidth = mold.width() / width;
  int frameHeight = mold.height() / height;

  // populate a randomized vector of configurations to serve as the initial population.
  Configuration ret;
  multimap<double,int> quality;
  int step = (frameWidth*frameHeight/60)+1;
  
  vector<double> used;
  used.resize(bunch.width(),0);
  printCurrentTime();
  cout << "Processing: " << endl << "[=" << flush;
  for(int x=0;x<frameWidth;++x) {
    ret.push_back(vector<int>());
    for (int y=0;y<frameHeight;++y) {
      if((x*y)%step==0) {
	cout << "=" << flush;
      }
      double bestMatch=0;
      int bestIndex=-1;
      for (int i=0;i<bunch.width();++i) {

	double match = percentMatch(CImgView<uchar>(mold,x*width,y*height,width,height),
				    bunch[i]);
	match/=used[i]/100;
	if (bestMatch<match) {
	  bestIndex=i;
	  bestMatch=match;
	}
      }
      cout << "Best Match: " << bestMatch << " Best Index: " << bestIndex << " Used:" << used[bestIndex] << endl;
      used[bestIndex] = used[bestIndex]+1;
      ret[x].push_back(bestIndex);
    }
  }
  cout << "=]" << endl;
  return ret;
}


Configuration ImageSet::geneticAlgorithm(CImg<uchar> & mold, int iterations, int popcount, int thresh, double pct,  int mutationRate) {
  //this needs to be refactored away
  threshold=thresh;


  int best=-1;
  int prevBestQuality = -2;
  int bestQuality= -1;
  //might want to provide ability to pass a seed in. Dunno what rand to use though.
  srand(time(0));



  // Possibly better implemented as parameters; on the other hand, 
  // We don't want to be doing sub-pixel approximations. There's enough processing as it is.
  // For now, assume that we've given an image that can be evenly covered without changing
  // the image width/height of our tiles.
  int frameWidth = mold.width() / width;
  int frameHeight = mold.height() / height;

  // populate a randomized vector of configurations to serve as the initial population.
  vector< Configuration > population;
  for (int config = 0; config<popcount; ++config) {
    population.push_back(randomConfiguration(frameWidth,frameHeight,this->count()));
  }
  
  for (int iter=0; iter < iterations ;++iter )  {
    multimap<double,int> quality;
    int step = (popcount/60)+1;
    printCurrentTime();
    cout << "Calculating quality [=";
    for (int current=0;current<popcount;++current) {
      //progress bar
      if(current%step == 0 ) {
	cout << "=" << flush;
      } 

      double currentQual=0;
      for(int x=0;x<frameWidth;++x) {
	for (int y=0;y<frameHeight;++y) {

	  double match = percentMatch(CImgView<uchar>(mold,x*width,y*height,width,height),
				      bunch[population[current][x][y]]);
	  // if (match > pct) {
	  //   currentQual+= match*5;
	  // }
	  currentQual+=match;

	}

      }
      quality.insert(pair<double,int>(currentQual,current));
    }
    cout << "]" << endl;

    if(iter+1<iterations) {
      vector < Configuration > newPop;
      multimap<double,int>::reverse_iterator begin,end;
      begin=quality.rbegin();
      best=begin->second;
      prevBestQuality=bestQuality;
      bestQuality=begin->first;
      end=quality.rend();
      
      int breedcount=ceil(popcount/2);
      // Take the top third and breed a random subset of them to populate the array.
      cout << " Best of the population: ";

      for (int i=0;i< breedcount;++i) {
	cout << begin->first << " ";
	if( (rand()%3) ) {
	  newPop.push_back(population[(begin++)->second]);
	} else {
	  newPop.push_back(randomConfiguration(frameWidth,frameHeight,this->count()));
	}
      }

      
      population=newPop;
      // We want to create mutations, but we want to protect the top
      // 10 configurations from mutating, unless we have fewer than 10
      // configurations.
      int mutatingPopSize = population.size();
      int complementMutatingPopSize = 0;
      if(popcount>10) {
	mutatingPopSize-=10;
	complementMutatingPopSize = 10;
      }

      for (int i=0;i<breedcount*mutationRate;++i) {
      	population[(rand()%(mutatingPopSize) +complementMutatingPopSize)][rand()%frameWidth][rand()%frameHeight]=rand()%this->count();
      }

      // We've finished mutating the breeders, and are now ready to breed them.
      while((int)population.size() < popcount) {
	population.push_back(
			     mate(
				  newPop[rand()%breedcount],
				  newPop[rand()%breedcount]
				  ));

      }
    }
    cout << endl << "Best of iteration " << iter << ": " << bestQuality << "/" << frameWidth*frameHeight << ", index: " << best << endl;
    // not the best idea... maybe
    // if( bestQuality == prevBestQuality ) {
    //   cout << "No change; quitting." << endl;
    //   break;
    // }
  }
  
  cout << "Frame: "  << frameWidth << " " << frameHeight << endl;
    
  
  print(population[best]);
  return population[best];
}


double ImageSet::percentMatch(int img1, int img2) {
  return percentMatch(bunch[img1],bunch[img2]);
}

double ImageSet::percentMatch(CImg<uchar> &one, CImg<uchar> &two) {
  return percentMatch(CImgView<uchar>(one),CImgView<uchar>(two));
}


double ImageSet::percentMatch(CImgView<uchar> one, CImgView<uchar> two) {
  double good=0;
  double wid=one.width;
  double hei=one.height;
  for (int x=0;x<wid;++x) {
    
    for (int y=0;y<hei;++y) {
      bool isgood=1;
      //unroll?
      for (int c=0;c<3;++c) {
	if ( abs( (int)one(x,y,c) - (int)two(x,y,c) ) > threshold ) {
	  isgood=false;
	}
      }
      
      
      if (isgood) {
	++good;
      }
    }
  }
  
  

  return good/(wid*(double)hei);
}



