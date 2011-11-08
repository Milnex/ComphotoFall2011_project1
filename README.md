Project of Computational Photography, Fall 2011, NTU
Copyright 2011 Ming-Ying Chung. All rights reserved.


Requirement
===========
OpenCV library 2.2
Wt C++ web toolkit


Installation
============
git clone git://github.com/Milnex/comphoto2011.git
cp comphoto2011 /path/to/source
mkdir /path/to/build && cd /path/to/build 
cmake /path/to/source
make
ln -s /usr/share/Wt/resources bin/resources
ln -s /path/to/source/static bin/static


Run
===
bin/Comphoto2011_main --docroot bin --http-address 0.0.0.0 --http-port 8080
Now you can browse 127.0.0.1:8080

Demonstration
=============
After built, there is a demo program **bin/demo** to demonstrate the filter.

