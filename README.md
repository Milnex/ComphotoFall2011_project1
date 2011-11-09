Project of Computational Photography, Fall 2011, NTU.  
Copyright 2011 Ming-Ying Chung. All rights reserved.  


Important Note 
==============  
In this project, I experimentally adapt Wt C++ library as the user interface.  
Thus there is an integrated web site that demonstrates my filter programs and 
also includes my report. Unfortunately, it's a little hard to install Wt library 
on MS Windows Platform (you need to have C++ boost library installed in advance).  

Although I've tried to provide the binary file, it's probably won't work due to  
the lack of Wt/boost static library (and only suit for *nix-like system).  
Thus **I suggest directly browse my report & online demo through [this URL](http://140.112.247.130:9091/)**.    
  
Their is also a command line program which runs detail enhancement directly. It's  
described below.   


Requirement  
===========  
CMake 2.6+  
OpenCV library 2.2
Boost C++ library 
Wt C++ web toolkit 


Source Code  
=========== 
`git clone git://github.com/Milnex/comphoto2011.git    
cp comphoto2011 /path/to/source`  


Compilation   
============  
`mkdir /path/to/build && cd /path/to/build   
cmake /path/to/source  
make  
ln -s /usr/share/Wt/resources bin/resources  
ln -s /path/to/source/static bin/static`

To build the demo program only, run  
`make demo`  


Execution  
=========  
`bin/server.wt --docroot bin --http-address 0.0.0.0 --http-port 8080`   
Now you can browse the report & program at 127.0.0.1:8080 


Demonstration
=============
After built, there is a demo program named **bin/demo** to demonstrate the filter.    
