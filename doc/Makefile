all: docs


docs:
	doxygen doxygen.config


zip: ucfk4.zip


ucfk4.zip: html/*
	zip -r $@ html/

clean:
	rm -r html
