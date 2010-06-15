ImageWeave
==================
Author: Luke Schlather


ImageWeave is a program designed to take a set of images and a target image and 'weave' the images such that they resemble the target image, creating a sort of photo mosaic. 

The program defines two routines that are supposed to do this, one is effectively brute force, the other claims to be a genetic algorithm, but is far outperformed by brute force. 

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

#Method

##Similarity metric
The program relies on a similarity function percentMatch, which is a very rudimentary way to determine if two images look alike. Using the above threshold, it considers two pixels to "match" if none of its constituent color channels (red, green, or blue) differ by more than the threshold. It counts the number of matching pixels, and returns the percentage of pixels that match between the two images.

##bruteForce
This method iterates over every cell in the image (by default defined as a 24x18 section of the image) and then compares every image in the ImageSet to that section, choosing the image that has the highest percentage of matching pixels, by percentMatch. All of the images are read in and scaled to the same size when the ImageSet is constructed. 

Currently, the bruteForce method uses a greedy strategy that penalizes image re-use by this formula:
    	match/=used[i]/100;

Given that that array is initialized to 0, I am not entirely clear on how the program is functioning. Initially I attempted this method which only marginally penalized repeats:
    	match-=used[i]/100;
	
Actually, on some reflection this may be the version that gets good results. More on that when we turn to:

#Bugs
It does this a lot:
     *** glibc detected *** ./ImageWeave: double free or corruption (out): 0x00000000018d78b0 ***
     ======= Backtrace: =========
     /lib/libc.so.6(+0x775b6)[0x7f11e4fc35b6]
     /lib/libc.so.6(cfree+0x73)[0x7f11e4fc9e53]
     ./ImageWeave[0x406dc8]
     ./ImageWeave[0x471852]
     /lib/libc.so.6(__libc_start_main+0xfd)[0x7f11e4f6ac4d]
     ./ImageWeave[0x4044b9]
     ======= Memory map: ========
... and so on. Initially I thought this approach: 
    	match-=used[i]/100;
	
was the culprit, so changed it to the division version, banged out make && run, and walked away. While I was taking a break, I realized I was dividing by zero. Mysteriously it worked. There may be something funky with the Makefile. In any case, one of these approaches produces good results, both of them cause random glibc crashes, work remains to be done.
