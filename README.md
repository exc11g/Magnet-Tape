# Magnet-Tape

Magnet-Tape is a simple magnetic tape simulation program designed to sort integers. The program reads integers from an input file, sorts them, and writes the sorted integers to an output file. Additionally, it allows configuring delays for various tape operations through a configuration file.

## Command Line Arguments

Usage: MyExecutable [options]

Options:  
**-h [ --help ]**         Show help message  
**-i [ --input ]** arg    Path to the input file containing space-separated **int32_t** numbers  
**-o [ --output ]** arg   Path to the output file where sorted numbers will be saved  
**-c [ --config ]** arg   Path to the config file containing five integers  
### Example of running program

```sh
./magnet-tape --input ../input.txt --output ../output.txt --config ../config.txt
```

### Input File
```sh
34 7 23 32 5 62
```

### Output File
```sh
5 7 23 32 34 62
```

### Configuration file

The configuration file should contain exactly four integers, each on a new line, representing the delays (in milliseconds) for the following tape operations:

1. **Read Delay**: The delay for reading an integer from the tape.
2. **Write Delay**: The delay for writing an integer to the tape.
3. **Move Delay**: The delay for moving the tape head from one position to the next.
4. **Rewind Delay**: The delay for rewinding the tape to the beginning.
5. **Memory**: Maximum memory usage.

### Example Configuration file

```sh
10
15
5
20
1000
```

### Usage or testing
#### Requirements:

- **Boost** version 1.74 or higher  
- **Catch2** version 2.x

#### Build
```shell
mkdir build && cd build
cmake ..
make tests # For testing
make tatlin
./tests
./tatlin *args*
```

