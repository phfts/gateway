.PHONY: download run test

SSL_INCLUDE_PATH=/opt/homebrew/opt/openssl@3/include 
SSL_LIB_PATH=/opt/homebrew/opt/openssl@3/lib
GTEST_INCLUDE_PATH=/opt/homebrew/Cellar/googletest/1.14.0/include
GTEST_LIB_PATH=/opt/homebrew/Cellar/googletest/1.14.0/lib

download:
	@rm -rf ./lib/
	@mkdir lib
	@if [ -n "$$(tail -c 1 requirements.txt)" ]; then echo >> requirements.txt; fi
	@while read -r line; do test -e "$$line"; line=$$(echo "$$line" | sed 's/git //'); cd lib; git clone "$$line"; cd ..; done < requirements.txt

run:
	mkdir -p bin
	g++ src/main.cpp -std=c++17 -I$(SSL_INCLUDE_PATH) -L$(SSL_LIB_PATH) -o bin/main -lssl -lcrypto
	./bin/main

test:
	mkdir -p bin
	g++ tests/main.cpp -std=c++17 -I$(SSL_INCLUDE_PATH) -L$(SSL_LIB_PATH) -I$(GTEST_INCLUDE_PATH) -L$(GTEST_LIB_PATH)  -o bin/test -lssl -lcrypto -lgtest
	./bin/test	