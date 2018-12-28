# page-rank
Implement Google's PageRank in C++ using a graph ADT to represent relationships between web addresses.

////////////////////

# Goal

Implement Google's page rank.

# Input

Line 1 contains the number of lines (n) that will follow and the number of power iterations you need to perform. Each line from 2 to n will contain two URL’s – from_page and to_page separated by a space. This means from_page points to the URL to_page.

*the first power iteration is simply the starting point for page ranks*

*2 power iterations means one matrix multiplication*

*3 power iterations means two matrix multiplications*

# Output

Print the PageRank of all pages after n powerIterations in ascending alphabetical order of webpage. Also, round off the rank of the page to two decimal places.
