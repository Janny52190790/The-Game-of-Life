# The Game of Life
This assignment mainly ask as to make two files: universe.c, life.c.  We should also create Makefile and DESIGN.pdf.

## Automate compilation by using Makefile
```
$ make
```

## Cleaning
```
$ make clean
```

## Running

```
$ ./life -t -s -n <generation> -i <input file> -o <output file>
```

## the command-line options your program accepts

• -t : Specify that the Game of Life is to be played on a toroidal universe.\
• -s : Silence ncurses. Enabling this option means that nothing should be displayed by ncurses.\
• -n generations : Specify the number of generations that the universe goes through. The default
number of generations is 100.\
• -i input : Specify the input file to read in order to populate the universe. By default the input
should be stdin.\
• -o output : Specify the output file to print the final state of the universe to. By default the output
should be stdout.
