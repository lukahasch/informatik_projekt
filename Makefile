mega:
	mkdir Mega 
	cp mega.ino ./Mega 
	cp -r shared ./Mega

nano:
	mkdir Nano 
	cp nano.ino ./Nano 
	cp -r shared ./Nano

clean:
	rm -rf Mega
	rm -rf Nano