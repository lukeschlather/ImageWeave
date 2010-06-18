ImageWeave
==================
Author: Luke Schlather

This is a work-in-progress designed to take a bunch of images and a 'mold' image, and arrange the images so they look like the mold. It defines two functions which aim to do this: bruteForce, which is actually a greedy algorithm, and geneticAlgorithm, which while it does what it says, pretty much doesn't work for the sample sizes I've tested (up to 500 images on a  3288x2466 mold.)


#Building 
I have only tested this on Ubuntu 10.04. I did do some initial work a couple months ago on 9.10, so it should also be alright there. 

It relies on Boost's filesystem layer for file i/o, and the CImg library. 

    sudo apt-get install libboost-filesystem-dev

should get you what you need as far as boost.

     wget http://cimg.sourceforge.net/cimg-dev.deb
     dpkg -i cimg-dev.deb

Should get you the version of  CImg I'm using, though I pulled that copy as of 2010-04-08, so no guarantees after that date. :) 


##Use

At the moment, a typical usage might look like this:
    ./ImageWeave -m /home/project/testS.png -d /home/project/colors/ -t50
    
Breaking these options down:

    -m /home/project/testS.png
This is the target "mold" image.
    -d /home/project/colors/
This is the directory where the program reads images. It does not recurse, and though it should simply throw an error if you give it something that isn't an image, behavior is undefined if non-image files exist in this directory.
     -t50
This is the threshold for deciding if a color matches. Color is stored in 24 bits to define the color for a pixel, that is, three 8-bit unsigned chars which define the red, green, and blue color channels. The value should be from 0-255, otherwise it will overflow. 

Cell Width and height can be set as follows:
     -x20
     -y10
They default to 24x18

It will drop a copy of the finished image in the directory set by the flag -o:
     -o /home/project/output/
/home/project/output/ is the default directory.

#Method

##Similarity metric
The program relies on a similarity function percentMatch, which is a very rudimentary way to determine if two images look alike. Using the above threshold, it considers two pixels to "match" if none of its constituent color channels (red, green, or blue) differ by more than the threshold. It counts the number of matching pixels, and returns the percentage of pixels that match between the two images.

##bruteForce
This method iterates over every cell in the image (by default defined as a 24x18 section of the image) and then compares every image in the ImageSet to that section, choosing the image that has the highest percentage of matching pixels, by percentMatch. All of the images are read in and scaled to the same size when the ImageSet is constructed. 

Currently, the bruteForce method uses a greedy strategy that penalizes image re-use by this formula:
    	match/=used[i]/100;

Where used[i] is the number of times the image has been used + 1. Initially I attempted this method which only marginally penalized repeats:
    	match-=used[i]/100;
Dividing seems to work better. 

#Bugs
Occasionally squiggles have shown up on a handful of cells in the output image. Also, I was getting glibc : double free or corruption stack traces that wouldn't go away until I rebooted. I'm not sure if it's possible I managed to corrupt glibc. But it did happen, and it's gone after a reboot.