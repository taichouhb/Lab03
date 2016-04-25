We start out by making sure that an argument of a filename is given and exits if it isn't given. We then open the file and exits if it cannot open the file.

We then read in each line of the file. If we're on the first line, we set the offset to that value on the first line. If we're on the second line, we then store the page number with the value given on that line. For any other line, we convert the given address to hex. 

To get the page number, we shift the address given left 48 bits and then shifting it right 48 bits to clear the 64 bit address we're given so we can work with a 16 bit address. We then shift our result right by the offset to get the page number address. 

To get the offset address, we left shift the address 48 bits plus the page number offset and then right shift it by the same amount. We effectively cleared the address of the page number address and are left with the offset address. 

We then print our results, increase our index variable by one to know which line we are on, and go on to the next given address until the document is finished reading.

We then free the buffer we were using and close the file. 
