# PROGRAMA
    PROG = fdtool
	TGZ = iago_20244409.tar.gz
# arquivos a compilar
	HDR = $(wildcard codigos/*.h)			# headers
	SRC = $(wildcard codigos/*.c)			# fontes C
	OBJ = $(SRC:codigos/%.c=codigos/objetos/%.o)	# objetos .o
	CC = gcc
	CFLAGS = -std=c11 -Wall -Wextra
.PHONY: all clean purge

codigos/objetos/%.o: codigos/%.c $(HDR)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 
	
clean:
	@rm -f *~ *.o codigos/*.o codigos/objetos/*.o

purge:  clean
	@rm -f $(PROG)

# gera arquivo .tar.gz para entrega
tgz: purge
	-rm -f $(TGZ)
	-tar --ignore-failed-read -czvf $(TGZ) codigos/ exemplos/ makefile relatorio.pdf
