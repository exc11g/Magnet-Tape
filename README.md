# Magnet-Tape

Magnet-Tape is a simple magnetic tape simulation program designed to sort integers. The program reads integers from an input file, sorts them, and writes the sorted integers to an output file. Additionally, it allows configuring delays for various tape operations through a configuration file.

## Command Line Arguments

The program accepts three command line arguments:

1. **input**: Path to the input file containing integers.
2. **output**: Path to the output file where the sorted integers will be written.
3. **config**: Path to the configuration file that specifies the delays for various tape operations.

### Example Usage

```sh
./magnet_tape input.txt output.txt config.txt
```

### Input File
```angular2html
34 7 23 32 5 62
```

### Output File
```angular2html
5 7 23 32 34 62
```

### Configuration file

The configuration file should contain exactly four integers, each on a new line, representing the delays (in milliseconds) for the following tape operations:

1. **Read Delay**: The delay for reading an integer from the tape.
2. **Write Delay**: The delay for writing an integer to the tape.
3. **Move Delay**: The delay for moving the tape head from one position to the next.
4. **Rewind Delay**: The delay for rewinding the tape to the beginning.

### Example Configuration file

```angular2html
10
15
5
20
```

### Example of running program

```angular2html
./magnet-tape ../input.txt ../output.txt ../config.txt
```