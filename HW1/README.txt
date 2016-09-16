Homework 1 Assignment

Author: Ayzha Ward
Contact: adward4@uh.edu
COSC 6377 Fall 2016

This software included in this package is for the intent of demonstrating a simple client-server(-web server)connection via TCP sockets for retrieving the content of a web page. The idea behind this simulation is to have a client connect to a local server and request a specific URL. In turn, the local server then establishes a connection to a web server to retrieve the content of the specified URL. This content is then saved within the same directory that the client and server are being executed in. The server then prints a message to the standard output detailing that the page has been downloaded and specifying the number of bytes that it has written to a file containing the web content.The file that the web content does not have a specified extension so in  order to view it's contents the user will have to append a ".html" to the end of the file name.

The graphic below depicts this three-way interaction:

+-------------------+
|                   |    GET request sent
|                   |    to Web Server      +--------------+                   +-----------+
|                   |   <---------------+   |              | URL sent to Local |           |
|  Web Server for   |                       | Local Server | Server            |  Client   |
|  Requested        |                       |              | <--------------+  |           |
|  Host             |    +--------------->  +--------------+                   +-----------+
|                   |     URL information
+-------------------+     returned to
                         local server

In order to make this interaction work, a TCP socket is created so that the client can interact with the local server. Additionally, another TCP socket on the local server is created to pass information between these two entities. Once a connection between the local server and client has been established, the local server then creates another TCP socket to interact with the web server that it will use to retrieve the information for the requested URL page. The local server code has been written in a way to ensure that it does not close the port it is listening on after handling a client request. It will wait for another client connection to be made.

Additional resources used to help create the client and sever source code files and to better understand their interactions are listed below:


http://forums.codeguru.com/showthread.php?517963-sending-an-http-GET-request-in-C
http://stackoverflow.com/questions/12039366/remove-http-header-info
http://www.tutorialspoint.com/http/http_methods.htm
http://www.c4learn.com/c-programming/c-reference/fprintf-function/
http://www.tutorialspoint.com/cprogramming/c_file_io.htm
http://beej.us/guide/bgnet/output/html/multipage/index.html


Author's Comments:

Overall, I think this assignment was a big step in getting my feet wet with socket programming. There are things that I wish I could have figured out or implemented in addition to my submitted work, but time constraints have put a limit on this. This section just highlights features that would have been beneficial to have for this software. The most prominent of these is being able to save the md5sum of the URL as the file name. I understand that the popen() command can be used to handle this, but I had issues with being able to read and write to the command-line to implement this. I do believe, had it been successful, it would have definitely been useful. Another issue, is being able to process images and pdfs. An issue that occurred with pdfs is that they would come up blank whereas images would not be recognized at all. Maybe in the future I will be able to address these issues.
