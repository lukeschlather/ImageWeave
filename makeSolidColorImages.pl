# Perl script to generate a bunch of images, each with a single color.
# Using this to see if my genetic algorithm does anything resembling 
# "The right thing" - this should be really easy for it to weave an image
# tapestry out of.
# don't try this at home kids

my $x = 24;
my $y = 18;
my $count = 30;

if ($#ARGV > 2) {
    $x = $ARGV[0];
    $y = $ARGV[1];
    $count = $ARGV[2];
}


for (my $r=0;$r<256;$r+=$count) {
    for (my $g=0;$g<256;$g+=$count) {
	for (my $b=0;$b<256;$b+=$count) {
		`convert -size 28x18 \'xc:rgb($r,$g,$b)\'  $r\_$g\_$b.gif`;
		print "convert -size 28x18 \'xc:rgb($r,$g,$b)\'  $r\_$g\_$b.gif\n";
	}
    }
}
