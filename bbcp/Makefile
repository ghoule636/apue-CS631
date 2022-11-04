run: bbcp
	./bbcp test1.txt test2.txt

bbcp: bbcp.c
	cc bbcp.c -o bbcp
	echo Hello World! >> test1.txt

clean:
	rm -f bbcp test1.txt test2.txt
