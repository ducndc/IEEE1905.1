# IEEE1905.1
Library about IEEE 1905.1

## Overview
This is a library followed by IEEE 1905.1, which is the protocol of the EasyMesh

## Install packages
### Doxygen
```sh
sudo apt install doxygen graphviz
```

## Build 
```sh
git clone https://github.com/ducndc/IEEE1905.1.git
cd IEEE1905.1
mkdir build && cd build
cmake ..
make -j4
make install
make distclean   # xoá sạch build

```
## Documents
```sh
doxygen Doxyfile
```

## References
[1] https://standards.ieee.org/ieee/1905.1/4995/

[2] https://www.wi-fi.org/file/wi-fi-easymesh-specification

[3] https://dev.iopsys.eu/multi-ap

[4] https://gitlab.com/prpl-foundation/prplmesh
