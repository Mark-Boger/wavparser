# wavparser

Basic wavparser.

## Build
```bash
mkdir build && cd build
cmake ..
make
```
Use -Dtest=ON to have unit tests. This will also download gmock and gtest when making
the project.

## Usage

```
./wavparser <wav file> <out file>
```

This will take a wav file and output a file in the sox .dat format