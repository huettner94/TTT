# Run "make" for building executable file with GLUT and MinGW
# For running this executable, glut32.dll must be in your %PATH%
# Run "make clean" for removing build artifacts (including executable)

GLUT_HOME = Glut/

CC = gcc
LD = gcc
CFLAG = -c -Wall -std=c99 -pedantic -g $(OGL_INC) -D _STDCALL_SUPPORTED \
  -D _M_IX86 -D GLUT_NO_LIB_PRAGMA -D GLUT_NO_WARNING_DISABLE
OGL_INC = -I $(GLUT_HOME)
OGL_LIB = -L $(GLUT_HOME) -lglut32 -lopengl32 -lglu32

# .PHONY is recommended before dummy target
.PHONY: all
all:
	make TTT

# object files _before_ libs they depend on.
TTT: TTT.o graphics.o
	$(LD) TTT.o graphics.o -o TTT.exe $(OGL_LIB)  

TTT.o:
	$(CC) $(CFLAG) TTT.c

graphics.o:
	$(CC) $(CFLAG) graphics.c

.PHONY: clean
clean:
	del *.o *.exe
	
.PHONY: doc
# Create documentation from Doxyfile
doc:
	doxygen
