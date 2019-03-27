# BEND++
A databender made in C++. Made to be fast, effective, full of features, and easy to use.

## Downloading source
Due to a dependency on a repository called `inith`, the actual command to be able to work on this is:

`git clone --recursive <URL-TO-THIS-REPO>`

Otherwise `inith` won't download and hence it won't compile.

## Modes
So far there are multiple modes. Not all modes are built the same however, due to differences in how they work, some modes are much faster than others.

- `SCATTER` : Randomizes bytes.
- `ZERO` : Sets random bytes to 0.
- `CHUNKS` : Randomizes chunks of bytes.
- `REVERSE` : Reverses chunks of bytes.
- `REMOVE` : Removes chunks of bytes. [*Note: If the amount of bytes to remove exceeds the file, this will crash the program*]
- `MOVE` : Moves chunks of bytes around.
- `SWAP` : Swaps chunks of bytes around.
- `ISWAP` : Same as `SWAP`, but using iterators. [*Note: `SWAP` is better and this mode is planned to be removed.*]
- `REPEAT` : Repeats chunks of bytes, replacing the following bytes.

There is also a special mode that is used only in `options.ini` if you need to enter all the modes without entering them. It's called `ALL`.

Some of these modes ignore certain options. For example only `REPEAT` uses the `repeat` option, and `SCATTER` & `ZERO` don't use the `chunksize` option.

**Note:** Some modes are slower than others. Be sure to remember this if you do a massive databend using the `ALL` mode. `MOVE`, `REMOVE` are notoriously slow, with
`REPEAT` also being comparitively much slower if you set the `repeat` option as very high.

## Configuring and Executing BEND++
All you need to databend is `m.exe` (which will have a better name in the future) and `options.ini`. They need to be in the same folder.
To configure the options of **BEND++**, you will need to edit `options.ini`, which is self-explanatory.
Once the options are up to your liking, execute `m.exe` and watch the databending work.

## Compilation
Compiling is as easy as running `make`. 
I suggest you install make if you do not have it - but if you'd rather directly use a terminal command:

`g++ -o m -g -m64 main.cpp bin/binbend.cpp bin/binbend.h bin/hexbend.cpp bin/hexbend.h`

**Please note**: You will need an x86-64 bit version of `g++` in order to have the full functionality of the program. Otherwise, you have a memory restriction of approximately 4GB. For files under than that you can use the i686/32-bit version. This is due to memory limitations with 32-bit systems.

## Last update notes
The glitched files will be saved to a folder called `output`. If this folder doesn't exist, it will be created.

For now, you can't set the savename for the file. This name is dynamically generated from the original file, the modes used to corrupt, and the options used.

You can now specify ranges in the options for `iters`, `repeats`, and `chunksize`.
