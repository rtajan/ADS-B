# ADS-B

This project is a cutom module for PyStreamPU

Implementation of PPM or PM modulation use in the aircraft's ADS-B system to transmit information


Install pystreampu in order to use it

https://github.com/aff3ct/pystreampu

and pybind11

```
pip install pybind11
```

### How to run it :

In your python environnement, in the build directory, run this command to generate the Makefile :

```
CMAKE_PREFIX_PATH=$(python -c "import streampu; print(streampu.get_cmake_dirs())") cmake ..
```


An example of use is shown in the chaine_PM.py file