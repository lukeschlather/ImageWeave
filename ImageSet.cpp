// Luke Schlather
// Saturday, April 17 2010
// Licensed under the LGPL
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
    for (int y=0;y<frameHeight;++y) {
      for(int a=0;a<width;++a) {
	for(int b=0;b<height;++b) {
	  tmp(a,b,0)= input( x*width + a,y*height + b,0); 
  	  tmp(a,b,1)= input( x*width + a,y*height + b,1); 
  	  tmp(a,b,2)= input( x*width + a,y*height + b,2); 
  	}
      }
    }
  }
  return ret;
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

	  double match = percentMatch(mold,x*width,y*height,bunch[population[current][x][y]]);
	  if (match > pct) {
	    currentQual+= match*5;
	  }
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
      
      int breedcount=ceil(popcount/3);
      // Take the top third and breed a random subset of them to populate the array.
      cout << " Best of the population: ";

      if(popcount>10) {
	for (int i=0;i<5;++i) {
	  cout << begin->first << " ";
	  newPop.push_back(population[(begin++)->second]);
	}
      }
      for (int i=0;i< breedcount;++i) {
	cout << begin->first << " ";
	if( (rand()%5) ) {
	  newPop.push_back(population[(begin++)->second]);
	} else {
	  newPop.push_back(randomConfiguration(frameWidth,frameHeight,this->count()));
	}
      }
      
      population=newPop;
      for (int i=0;i<breedcount*mutationRate;++i) {
	population[rand()%population.size()][rand()%frameWidth][rand()%frameHeight]=rand()%this->count();
      }
      while((int)population.size() < popcount) {
	  population.push_back(
			       mate(
				    newPop[rand()%breedcount],
				    newPop[rand()%breedcount]
				    ));

      }
    }
    cout << endl << "Best of iteration: " << iter << ": " << bestQuality << ", index: " << best << endl;
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

double ImageSet::percentMatch(CImg<uchar> &mold, double moldMinX, double moldMinY, CImg<uchar> &two) {
  double good=0;
  for (int x=0;x<width;++x) {
    for (int y=0;y<height;++y) {
      bool isgood=1;
      //unroll?
      for (int c=0;c<3;++c) {
	if ( abs( ((int) mold(moldMinX + x,moldMinY +y,c) )- (int)two(x,y,c) ) > threshold ) {
	  isgood=false;
	}
      }
      if (isgood) {
	++good;
      }
    }
  }
  return good/(width*(double)height);
}



double ImageSet::percentMatch(CImg<uchar> &one, CImg<uchar> &two) {
  double good=0;
  for (int x=0;x<width;++x) {
    for (int y=0;y<height;++y) {
      bool isgood=0;
      for (int c=0;c<3;++c) {
	if ( abs( ((int) one(x,y,c) )- (int)two(x,y,c) ) < threshold ) {
	  isgood=true;
	}
      }
      if (isgood) {
	++good;
      }
    }
  }
  return good/(width*(double)height);
}



// something resembling the algorithm... 
// pair<double,vector<Node>::iterator>  graphAdd(Node& currentNode, const int &id) {
//   map<double,vector<Node>::iterator> values;
//   vector<Node>::iterator end = currentNode.second.end();
//   pair<double,vector<Node>::iterator> best = pair<double,vector<Node>::iterator>(0,vector<Node>::iterator());

//   for ( vector<Node>::iterator begin = currentNode.second.begin(); begin !=end;++end) {

//     // calculate percent value at threshold, given depth
//     pair<double,vector<Node>::iterator> current = graphAdd(currentNode,id);
//     if (current.first > best.first) {
//       best=current;
//     }
//   }
//   return best;
// }


// map<int, vector<int> > & ImageSet::sort(int tol,double pct) {
//   int count = bunch.width();
//   threshold=tol;
//   Node root;
//   root.first=-1;
//   Node& currentNode = root;

//   for(int i=0; i<count; ++i) {
//     while ( currentNode.first == -1) {
//       int bestNode = -1;
//     double bestValue = 0;

//     }
//   return sorted;
// }


