CC:=gcc

pthread: 1.pthread_create.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@
terminate: 2.terminate.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

terminate_improve: 2.terminate_improve.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

cond_test: 3.cond_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

stage1: stage1.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

stage2: stage2.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

stage3: stage3.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@