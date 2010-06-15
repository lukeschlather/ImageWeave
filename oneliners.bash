
# make a bunch of images named 01-30, containing text 01-30.
for i in $(seq -w 1 30) ; do convert -background lightblue -fill black -pointsize 20 -size 24x18 label:${i} ${i}.gif; done