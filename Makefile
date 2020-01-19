SRC	=	gpio.c minimal_gpio.c

OBJ	=	$(SRC:.c=.o)

all:		pcp-gpio

pcp-gpio:	$(OBJ)
	$Q echo [Link]
	$Q $(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)
	strip --strip-unneeded pcp-gpio

.c.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o pcp-gpio
	
