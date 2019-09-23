TARGET := thumper
SOURCE := main.c

.PHONY: clean install

$(TARGET): $(SOURCE)
	gcc $(SOURCE) -o $(TARGET) -std=c99 -Wall -Wextra -Wpedantic -Ofast -march=native

clean:
	rm -f $(TARGET)

install: $(TARGET)
	./$(TARGET) landing.in ~/public_html/landing.html

