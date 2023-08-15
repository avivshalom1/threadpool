VAR := $(patsubst %.cpp,%, $(wildcard *.cpp))
PWD := $(patsubst  ,,$(subst src/$(VAR),, $(shell pwd)))

IDIR = $(PWD)include/
TDIR = $(PWD)test/
ODIR = $(PWD)obj/
BDIR = $(PWD)bin/
LDIR = $(PWD)lib/


CC = g++
CFLAGS = -std=c++11 -fdiagnostics-color=always -pedantic-errors -Wall -Wextra -DNDEBUG -o3 -Wextra -g -pthread -Wl,-rpath,PWD 
LDFLAGS = -std=c++11 -fdiagnostics-color=always -pedantic-errors -Wall -g -pthread -Wl,-rpath,PWD
g11= -std=c++11 -fdiagnostics-color=always -lpthread -Wl,-rpath,PWD
gc98e= -std=c++98 -fdiagnostics-color=always -pedantic-errors -Wall -Wextra -DNDEBUG -o3 -lpthread -Wl,-rpath,PWD
gd98e= -std=c++98 -fdiagnostics-color=always -pedantic-errors -Wall -g -lpthread -Wl,-rpath,PWD
gc98= -std=c++98 -fdiagnostics-color=always -Wextra -DNDEBUG -o3 -lpthread -Wl,-rpath,PWD
gd98= -std=c++98 -fdiagnostics-color=always -g -lpthread -Wl,-rpath,PWD


VERSION =

VPATH = $(IDIR) $(TDIR) $(ODIR) $(BDIR) $(LDIR)


TARGET = $(BDIR)$(VAR).out
OBJ = $(VAR).o $(VAR)_test.o $(VAR).so $(SHAREDLIB)
DEPS := $(patsubst $(VAR).o:,,$(shell gcc -MM $(VAR).cpp -I$(IDIR)))
ARABIC = $(subst \,,$(patsubst %.hpp,%.o,$(subst $(IDIR),$(ODIR), $(patsubst %$(VAR).hpp,,$(patsubst $(VAR).cpp,,$(DEPS))))))
SHAREDLIB = $(subst \,,$(patsubst %.hpp,%.so,$(subst $(IDIR),$(LDIR), $(patsubst %$(VAR).hpp,,$(patsubst $(VAR).cpp,,$(DEPS))))))


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

all: mkdir $(VAR).out


$(VAR).out: $(OBJ)
	$(CC) $(CFLAGS) $(ODIR)$(VAR)_test.o $(LDIR)$(VAR).so -I$(IDIR) -o$(TARGET) -lm


$(VAR).o: $(VAR).cpp
	$(CC) -c $(CFLAGS) -fPIC  $(LDFLAGS) $< -o$(ODIR)$@ -I$(IDIR)


$(VAR).so: $(VAR).o $(SHAREDLIB)
	$(CC) $(CFLAGS) -shared $(ODIR)$(VAR).o $(ARABIC) $(LDFLAGS) $(SHAREDLIB) -o $(LDIR)$@ -I$(IDIR)


$(VAR)_test.o: $(VAR)_test.cpp
	$(CC) -c $(CFLAGS) $(TDIR)$(VAR)_test.cpp -o$(ODIR)$@ -I$(IDIR)


$(SHAREDLIB):
#	$(shell echo $(subst lib,src,$(patsubst %.so,%,$(SHAREDLIB))) | xargs -n 1 cp -v makefile)
#	$(shell xargs -n 1 cp -v makefile<<<$(subst lib,src,$(patsubst %.so,%,$(SHAREDLIB))))
	$(subst lib,src,$(patsubst %.so,cd % && make $(VERSION);,$(SHAREDLIB)))

run: $(TARGET)
	$(TARGET)


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
	

cgdb:
	cgdb $(TARGET)
	
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
