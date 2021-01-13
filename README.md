# BleatCode

A programming learning platform. Unlike LeetCode, BleatCode is free and there are no "premium" privileges - all goats are comrades in the BleatCode community. Languages are also detected automatically based on the file extension. It allows users to contribute problems which can be approved by moderators.

# Client Blueprint
The client side allows users to do seven basic tasks:
1. Register and login from the welcome panel, logging in will invoke the list panel
2. Choose problems from the list panel
3. Invoke a viewing panel by clicking on a problem (description, input constraints, examples, etc.)
4. Invoke a solution panel from the viewing panel (solution explanation with time and space complexities for a solution)
5. Submit a code solution file from the viewing panel
6. Receive the verdict (how many test cases passed, time taken, space taken, which test case it failed on and the resulting compiler error OR time limit exceeded, unknown file type, etc.)
7. Save solution files on backend

# Server Blueprint

The server side handles the following tasks:
1. Create accounts and verify logins to create login sessions
2. Send the problem list to newly connected clients
3. Receive a packet containing the solution file and miscellaneous information (problem ID, client ID, etc.)
4. Compile the solution file and send the verdict to the respective client
5. Save solution files

# Topics Used

The following topics are implemented in this project:
1. Allocating memory and structs
2. Working with files and file information
3. Sockets
4. Signals (to handle disconnections)
5. Shared memory (from multithreading)
6. Shell commands


