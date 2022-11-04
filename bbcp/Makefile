run: bbcp
	@echo Hello World! >> test1.txt
	@mkdir test
	./bbcp test1.txt test2.txt
	./bbcp test1.txt test/

debug: bbcp.c
	@cc -ggdb -Wall -Wextra -Werror bbcp.c -o bbcp
	@echo Hello World! >> test1.txt
	@mkdir test
	@gdb --args ./bbcp test1.txt test/

runTest:
	./testcp.sh -v -p bbcp

bbcp: bbcp.c
	@cc -Wall -Wextra -Werror bbcp.c -o bbcp

clean:
	rm -f bbcp test1.txt test2.txt test/test1.txt
	rmdir test
