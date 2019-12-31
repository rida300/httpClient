Sources:
https://github.com/CyberChimeraUSA/C-Networking/tree/master/C-SendHTTPRequest
https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c/12784812
http://souptonuts.sourceforge.net/code/http_post.c.html
https://gist.github.com/nolim1t/126991/ae3a7d36470d2a81190339fbc78821076a4059f7
https://stackoverflow.com/questions/11208299/how-to-make-an-http-get-request-in-c-without-libcurl
https://aticleworld.com/http-get-and-post-methods-example-in-c/
https://cboard.cprogramming.com/c-programming/67822-socket-send-exits-app-unexceptively.html
https://www.w3resource.com/c-programming-exercises/string/c-string-exercise-27.php
 
To compile: use "gcc -std=gnu99 -g -Og httpClient.c -o httpClient" in the project directory
To run: ./httpClient people.cs.ksu.edu 80
Which page: /~eyv/
Result should be 200 OK, followed by html code
The output for this example is stored in RecordfileEYV.txt
 
Example 2:
./httpClient www.example.com
Which page: /
The output for this example is stored in RecordExample.txt
 
Example 3:
./httpClient www.k-state.edu 80
Which page: /seek/fall-2019/how-cnap-scientists-study-our-changing-brains/index.html
Result should be: 302 Found (because this page has been moved temporarily)
Output file for this example in RecordFile302.txt
 
Example 4:
./httpClient www.cisa.k-state.edu 80
Which page: /research/hardware-security.html
Result should be 301 Moved permanently
 
Example 5:
./httpClient www.cs.ksu.edu 80
which page: /undergraduate/minor/index.html
Result: 200 OK, followed by html
Output file for this example in RecordFileUndergrad.txt
 
Example 6:
./httpClient www.cs.ksu.edu 80
Which page: /student-success/learning-strategies/index.html
Result: 200 OK, followed by html
 
Example 7:
./httpClient codemirror.net
which page: /demo/matchtags.html
Result: 302, Moved temporarily
 
Strstr was used to check if a string contains a substring. The String.h library does not contain a safe version for this function since it iterates over the original string to find consecutive characters to determine if the substring exists.
 
Unexpected input:
Port: I checked for port numbers to be smaller than 65535. This is the largest TCP/IP port for IPv4 according to https://stackoverflow.com/questions/113224/what-is-the-largest-tcp-ip-network-port-number-allowable-for-ipv4
To ensure that the port number is infact a number, I used a helper function exited the program if the port was non-numeric. The helper function was inspired from https://www.geeksforgeeks.org/program-check-input-integer-string/
 
 