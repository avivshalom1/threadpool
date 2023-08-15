CC = g++
CFLAGS = -std=c++2a -fdiagnostics-color=always -pedantic-errors -Wall -Wextra -DNDEBUG -o3 -Wextra -g -pthread -Wl,-rpath,PWD 


.PHONY = all
.PHONY = run
.PHONY = mkdir
.PHONY = create_template_src_test
.PHONY = open
.PHONY = cgdb
.PHONY = gitadd
.PHONY = g11
.PHONY = release
.PHONY = clean


thread_pool.out: $(OBJ)
	$(CC) -I../../include $(CFLAGS)  thread_pool.cpp ../../test/thread_pool_test.cpp -o../../bin/$@


run: 
	../../bin/thread_pool.out

cgdb:
	cgdb ../../bin/thread_pool.out

print: 
	@echo
	@echo $(DEPS)
	@echo
	@echo $(PWD)
	@echo
	@echo $(ARABIC)
	@echo
	@echo $(ARABIC)
	@echo
	
mkdir:
	@ mkdir -p $(ODIR)
	@ mkdir -p $(BDIR)
	@ mkdir -p $(LDIR)
	
new:
	touch $(VAR).cpp
	touch $(IDIR)$(VAR).hpp
	touch $(TDIR)$(VAR)_test.cpp


create_template_src_test:
	cp -n $(IDIR)/$(VAR).hpp $(VAR).cpp
	cp -n $(TDIR)template_test.cpp $(TDIR)$(VAR)_test.cpp


open:
	gedit $(VAR).cpp $(IDIR)/$(VAR).hpp $(TDIR)$(VAR)_test.cpp &
	
code:
	code $(VAR).cpp $(IDIR)/$(VAR).hpp $(TDIR)$(VAR)_test.cpp &



vlg:
	valgrind --leak-check=yes --track-origins=yes $(TARGET)
	

gdb:
	gdb $(TARGET)
	

gitadd:
	git add .
	git add ../../*$(VAR)*
	git status
		
	
g11: CFLAGS = $(g11)
g11: VERSION = g11
g11: clean
g11: all


release: CFLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
release: all


clean:
	rm -f $(TARGET) $(ODIR)$(VAR)*.o $(BDIR)$(VAR)*.out $(LDIR)$(VAR).so
