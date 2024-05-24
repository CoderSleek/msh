mshgen:
	gcc msh.c mshutils.c mshrunpipe.c mshjump.c mshpathqueue.c mshrunbasic.c -o msh

mshdebug:
	gcc -g msh.c mshutils.c mshrunpipe.c mshjump.c mshpathqueue.c mshrunbasic.c -o msh

clean:
	rm ./msh