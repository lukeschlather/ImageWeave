ImageWeave
==================
Author: Luke Schlather

This is a work-in-progress designed to take a bunch of images and a 'mold' image, and arrange the images so they look like the mold. It defines two functions which aim to do this: bruteForce, which is actually a greedy algorithm, and geneticAlgorithm, which while it does something resembling a genetica algorithm, pretty much doesn't work for the sample sizes I've tested (up to 500 images on a  3288x2466 mold.)

These blog posts have some example outputs, and talk a little bit about how I got the project to where it is:

[June 16th](http://www.flinchbaughschlather.com/luke/programming/2010/06/16/imageweave/)

[June 20th](http://www.flinchbaughschlather.com/luke/programming/2010/06/20/imageweave/)


#Building 
I have only tested this on Ubuntu 10.04. I did do some initial work a couple months ago on 9.10, so it should also be alright there. 

It relies on Boost's filesystem layer for file i/o, and the CImg library. 

    sudo apt-get install libboost-filesystem-dev

should get you what you need as far as boost.

     wget http://cimg.sourceforge.net/cimg-dev.deb
     dpkg -i cimg-dev.deb

Should get you the version of  CImg I'm using, though I pulled that copy as of 2010-04-08, so no guarantees after that date. :) 

If you have those things, it should be a simple matter of typing `make`. CImg is a template library, so compiling takes a little while.

##Use

At the moment, a typical usage might look like this:
    ./ImageWeave -m /home/project/testS.png -d /home/project/colors/ -R -t 50
    
Breaking these options down:

    -m /home/project/testS.png
This is the target "mold" image. You may use this flag multiple times to specify multiple targets. 
    -d /home/project/colors/
This is the directory where the program reads images. CImg should throw an error but ignore non-image files. 
    -R
Search the target directory recursively. Without this flag it uses only images in the root directory. 
    -b 
Instead of using a single threshold, run a battery of 10 tests with i/threshold as the threshold. I use it with `-t 100` to see the results at thresholds 10,20 ... 100 .
     -t50
This is the threshold for deciding if a color matches. Color is stored in 24 bits to define the color for a pixel, that is, three 8-bit unsigned chars which define the red, green, and blue color channels. The value should be from 0-255, otherwise it will overflow. Currently this doesn't actually do anything; the program runs a battery everywhere from 0-100 stepping by 10 so you can compare. 

Before the algorithm is run, ImageWeave resizes all of the source images so that they can easily be stored in memory and efficiently compared. By default, it is set to 24x18, which assumes that your mold and source images are all at a 4:3 aspect ratio, because that's what my camera has. Note that this will likely stretch images, or show them sideways. At a minimum I would suggest ensuring that the x dimension of your mold is evenly divisible by the x you choose for cell width, and the same with y and height. 
Cell Width and height can be set as follows:
     -x20
     -y10

It will drop a copy of the finished image[s] in the directory set by the flag -o:
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

There was a glibc : double free or corruption issue, but I believe I've fixed that...