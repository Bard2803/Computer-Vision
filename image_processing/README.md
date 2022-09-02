
##image processsing 

it applies different filters on the bmp image provided and the ouput file is the result of the particular filter application.
There are four different filters available:
- grayscale
- reflection
- blur
- edges which uses Sobel operator

to launch the program:
1. `$ make filter`
2. `$ ./filter -g images/yard.bmp out.bmp`

where -g is for grayscale filter. There are also -r, -b and -g accordingly.
second argument is the input file path and the third is the output file's name.
