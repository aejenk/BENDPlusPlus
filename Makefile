program:
		clang -o bend.exe -O2 -std=c++17 -Wno-deprecated-declarations -Xclang -flto-visibility-public-std -m64 \
	    main.cpp bin/CalBender.cpp bin/CalMutations.cpp

debug:
		clang -o bend.exe -g -O2 -std=c++17 -Wno-deprecated-declarations -Xclang -flto-visibility-public-std -m64 \
	    main.cpp bin/CalBender.cpp bin/CalMutations.cpp

alt:
		g++ -o bend.exe -O2 -std=c++17 -Wno-deprecated-declarations -m64 \
	    main.cpp bin/CalBender.cpp bin/CalMutations.cpp