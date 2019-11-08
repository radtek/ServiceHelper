# ServiceHelper
Utility for batch start/stop of Windows services both  locally and remote.

The program reads in a configuration file which contains the names of the servers and the service name that you wish to stop.
You can add as many servers as needed.  Since the configuration file is passed in on the command line, you can save several configurations.

There is a start wait time, which will monitor how long it's been since the service start was send and will post a message if that time has been exceeded.

The serial start toggle allows you to either start the services one by one or all at the same time.
When set to 1, it will start with the service at the top of the configuration file, issue the start command and wait until it has started before moving on to the next.
When set to 0, all services are issues the start command and the program will wait until all have started before exiting.
