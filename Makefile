
SOURCES = vector3d.c \
los.c \
main.c


OBJECTS = $(SOURCES:%.c=%.o)

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc -Wall $(OBJECTS) -o $@ -lexpat -lm

clean:
	rm -f $(OBJECTS) $(TARGET)

%.o : %.c
	gcc -Wall -c $< -o $@

