EXEC=covid
DIR=build

.PHONY: all, check, clean

all: build
	make -C $(DIR)

$(DIR):
	mkdir $(DIR)
	cd $(DIR); cmake ..; cd ..

check: all
	./$(DIR)/$(EXEC)

clean:
	rm -rf $(DIR)
