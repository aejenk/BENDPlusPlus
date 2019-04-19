# BEND++
A databender API made in C++. Made to be fast, effective, full of features, and easy to use.

## Downloading source
Due to a dependency on a repository called `inith`, the actual command to be able to work on this is:

`git clone --recursive <URL-TO-THIS-REPO>`

Otherwise `inith` won't download and hence it won't compile.

## CALBender
The `CALBender` and `CalMutation` is an implementation of the BEND API. It extends the `Bender` and `Mutation` interfaces.
It comes with default mutations set up and ready for use. This is what is released on the `Releases` page.

### CALModes
So far there are multiple mutations. Not all mutations are built the same however, due to differences in how they work, some mutations are much faster than others. In the following, a *chunk* refers to a chunk of bytes.

- `RANDOM` : Randomizes chunks.
- `REVERSE` : Reverses chunks.
- `REMOVE` : Removes chunks.
- `MOVE` : Moves chunks around.
- `NULL` : Sets chunks to 0.
- `SWAP` : Swaps chunks around.
- `REPEAT` : Repeats chunks with replacement.
- `INCREMENT` : Increments chunks by an amount.
- `RAINBOW` : Increments bytes with an increasing sequence.
- `ECHO` : Echoes bytes in a chunk into the following bytes.
- `AVERAGE` : Averages chunks.
- `INVERT` : Inverts chunks.
- `DRAG` : Copies the first byte in a chunk into the following bytes.
- `SORT` : Sorts chunks.
- `MAGNIFI` : Multiplies chunks by an amount.
- `MIRROR` : Mirrors chunks - making them symmetrical.
- `HANDSHAKE` : Adds the mirror of the chunk onto the chunk.
- `TIMEWARP` : Speeds up the chunk.

**Please Note:** Not all modes are made the same. Some modes may take much longer than others, for example `REPEAT` or `ECHO`.

### Configuring and Executing BEND++
All you need to databend is `bend.exe` (which will have a better name in the future) and `options.ini`. They need to be in the same folder.
To configure the options of **BEND++**, you will need to edit `options.ini`, which is self-explanatory.
Once the options are up to your liking, execute `bend.exe` and watch the databending work.

To note, `CALBender` does not discriminate between file types. Any and all files can be entered as an input to the program, although very large files may crash the application.

The repo has an example `options.ini` as follows:

```ini
[Main Options]
filename = GGQH.avi
loops = 10
modes = MAGNIFI

[CALMode Options]
iterations = 1-10
chunksize = 12800000-1280000000

[Repeat Options]
repeats = 1-10

[Increment Options]
incrementby = 1-255

[Rainbow Options]
rainsize = 1-5
raindelay = 6200-62000

[Echo Options]
echodecay = 0.0-1.0
echolength = 1-10

[Magnifi Options]
magnifyby = 0.9-1.1

[Timewarp Options]
speedupby = 2-5
```

; Possible modes
; modes = RANDOM REPEAT REVERSE REMOVE 
;       MOVE NULL SWAP INCREMENT 
;       RAINBOW ECHO AVERAGE
;       INVERT DRAG SORT
;       MAGNIFI MIRROR HANDSHAKE
;       TIMEWARP

### Compilation
Compiling is as easy as running `make`. 
I suggest you install make if you do not have it - but if you'd rather directly use a terminal command:

`g++ -o m -g -m64 main.cpp -std=c++17 bin/CalBender.cpp bin/CalMutations.cpp bin/Utility.cpp`

**Please note**: You will need an x86-64 bit version of `g++` in order to have the full functionality of the program. Otherwise, you have a memory restriction of approximately 4GB. For files under than that you can use the i686/32-bit version. This is due to memory limitations with 32-bit systems.
