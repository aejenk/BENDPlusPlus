program:
		clang -o bend.exe -O2 -Wno-deprecated-declarations -Xclang -flto-visibility-public-std -m64 \
	    main.cpp bin/binbend.cpp bin/mutations.cpp

debug:
		clang -o bend.exe -g -O2 -Wno-deprecated-declarations -Xclang -flto-visibility-public-std -m64 \
	    main.cpp bin/binbend.cpp bin/mutations.cpp bin/timetaker.cpp

alt:
		g++ -o bend.exe -O2 -Wno-deprecated-declarations -m64 \
	    main.cpp bin/binbend.cpp bin/mutations.cpp bin/timetaker.cpp