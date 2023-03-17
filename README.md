# myhttpd
My graduation design, a simple web server based on asio.  
## Dependent libraries
- boost.asio[ssl]
- boost.filesystem
- boost.interprocess
- boost.uuid
- tinyxml2
- glog  
## How to build
Download the source code and get into its directory, 
then ues commands below:
```
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
