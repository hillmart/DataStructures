 ____  ____  _  _____ _     _     _____
/  __\/  __\/ \/  __// \   / \   /  __/
| | //|  \/|| ||  \  | |   | |   |  \  
| |_\\|    /| ||  /_ | |_/\| |_/\|  /_ 
\____/\_/\_\\_/\____\\____/\____/\____\
                                       
(Brielle)

Martin Hill, hillmart@oregonstate.edu

Description: 
My code takes in key value pairs, and using the key places them in a hash map. 
Then if you want to get a pair based on the key, it is O(1) average complexity 
to get it out because of the hash function. It automatically resizes and rehashes
when it reaches .75 of the load factor. I implemented open adressing by using a 
dynamic array. 

Instructions: 
1. make clean
2. make
3. ./test_ht
There are no inputs. These steps will run the program for you.

Limitations: 
This program functions as a hash map. There aren't memory errors or leaks.
It is a basic hashing function however so if you used different values or more 
values it might not work as well. It also only uses the convert function so you 
could test it on your own if you wanted to change the convert function to use 
something other than integers. But you would have to change the hash function.