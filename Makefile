CFLAGS = -W -Wall -ansi -pedantic -g -std=c99
FILE1 = huffman
FILE2 = occlist
FILE3 = huffmantree
FILE4 = codelist
FILE5 = compress
FILE6 = bit_op
FILE7 = functions
FILE8 = decompress
EXE = huffman

all: $(FILE1).o $(FILE2).o $(FILE3).o $(FILE4).o $(FILE5).o $(FILE7).o $(FILE8).o #$(FILE6).o
	gcc $^ -o $(EXE) $(CFLAGS)

$(FILE2).o: $(FILE2).c $(FILE2).h
	gcc -c $< -o $@ $(CFLAGS)

$(FILE3).o: $(FILE3).c $(FILE3).h
	gcc -c $< -o $@ $(CFLAGS)

$(FILE4).o: $(FILE4).c $(FILE4).h
	gcc -c $< -o $@ $(CFLAGS)

$(FILE5).o: $(FILE5).c $(FILE5).h
	gcc -c $< -o $@ $(CFLAGS)

#$(FILE6).o: $(FILE6).c $(FILE6).h
#	gcc -c $< -o $@ $(CFLAGS)

$(FILE7).o: $(FILE7).c $(FILE7).h
	gcc -c $< -o $@ $(CFLAGS)

$(FILE8).o: $(FILE8).c $(FILE8).h
	gcc -c $< -o $@ $(CFLAGS)

main.o: $(FILE1).c $(FILE1).h 
	gcc -c $< -o $@ $(CFLAGS)

clean:
	rm -rf *.o

xclean: clean
	rm -rf huffman
