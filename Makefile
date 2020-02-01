all: reader writer rw test_writer

rw: rw.c produce.o
	cc $< produce.o -o $@

reader: reader.c
	cc $< -o $@

writer: writer.c 
	cc $< -o $@

test_writer: test_writer.c
	cc $< -o $@

produce.o: produce.c
	cc -c $< -o $@

clean:
	rm -f reader writer test_writer rw produce.o
