TARGET := thumper
SOURCE := main.c

.PHONY: clean

$(TARGET): $(SOURCE)
	gcc $(SOURCE) -o $(TARGET) -std=c99 -Wall -Wextra -Wpedantic

clean:
	rm -f $(TARGET)
