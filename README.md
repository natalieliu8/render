Language: C++
Simple program for class where without using classes, pointers, etc. you had to render a block of text w/ provided line maximums. E.g, if you were to input the statement "I like cats" with a provided line maxium of 4, it would reutrn:
"I 
like
cats"
The word 'like' would end up on the next line because it wouldn't fit into the alloted 4 characters of line 1. 
Returns 0 if all goes well, returns 1 if you cannot break up the text into line maximums, and returns 2 for invalid input. 
Rules include:
01. Words can be further broken down into word portions with dashes. If the expected line maximum was 4 characters, and the text stated 'tea-time', you could divide the row into 'tea-' and 'time' and return 0.
02. If there was a word longer than the provided line maximum, return 2, and break up the word as best as you could. 
