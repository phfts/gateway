.PHONY: download

SSL_INCLUDE_PATH=/opt/homebrew/opt/openssl@3/include 
SSL_LIB_PATH=/opt/homebrew/opt/openssl@3/lib

download:
	@rm -rf ./lib/
	@mkdir lib
	@if [ -n "$$(tail -c 1 requirements.txt)" ]; then echo >> requirements.txt; fi
	@while read -r line; do test -e "$$line"; line=$$(echo "$$line" | sed 's/git //'); cd lib; git clone "$$line"; cd ..; done < requirements.txt

run:
	mkdir -p bin
	g++ src/main.cpp -std=c++17 -I$(SSL_INCLUDE_PATH) -L$(SSL_LIB_PATH) -o bin/main -lssl -lcrypto
	./bin/main