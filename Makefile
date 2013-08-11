CC=gcc
OBJECTS=main.o utilities.o
OUTPUT=main
CFLAGS=-wall -g

default: $(OUTPUT)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OUTPUT): $(OBJECTS)
	$(CC) -o $(OUTPUT) $(OBJECTS)

.PHONY: clean $(OUTPUT)
clean:
	-rm -f $(OBJECTS) $(OUTPUT)
