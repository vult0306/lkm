ifdef ARCH
	COMPILER = $(CC)
else
	COMPILER = gcc
endif

CFLAGS = -Wall -Wextra -std=c99
TARGET = app
SRC = test_app.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(COMPILER) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)