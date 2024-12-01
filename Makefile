#
# Makefile
#

# Folders
SRC = src

# this is the directory I will compile from (i.e. the argument to -I)
IDIR := include

# all object files will be put here
OBJDIR := objdir

# Compiler (g++)
CCC = g++

# Compiling flags
CCFLAGS += -MMD -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Wold-style-cast

LDFLAGS += -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

# Object modules
OBJECTS = glad.o stb_image.o

MAIN = $(addprefix $(OBJDIR)/, main.o $(OBJECTS))

# Main objectives - created with 'make' or 'make main'.
run: $(MAIN)
	$(CCC) -I$(IDIR) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

# Part objectives
$(OBJDIR)/%.o:  $(SRC)/%.cc $(IDIR)/%.h | dir
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(SRC)/main.cc | dir
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/glad.o: $(SRC)/glad.c | dir
	gcc -I$(IDIR) -c $< -o $@

dir:
	@mkdir -p $(OBJDIR)

# Include automatically generated dependency files
-include $(wildcard $(OBJDIR)/*.d)

# 'make clean' removes object files and memory dumps.
clean:
	@ \rm -rf $(OBJDIR)/*.o $(OBJDIR)/*.d *.gch core

# 'make zap' also removes the executable and backup files.
zap: clean
	@ \rm -rf *.out *~

