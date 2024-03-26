# byte-wizard
C++ File encoder and decoder library for many popular file types

This library is planned to support the following file types:

*Note ones labeled with (M) mean they can only be read with the library since the file type expresses abiguity between programs

.txt
.bin
.ini
.inf
.cfg
.properties (M)
.pak (M)
.json
.xml
.png
.zip
.jpg / .jpeg
.wav
.gz
.db (M)
.ttf
.bmp
.yml
.gif

I may add these files after i add the ones above

.mov
.mp4
.mp3
.rar
.7z
.ogg
.pdf
.doc

Currently I have implented

Inflate for decoding png, zip, and other compressed formats

.txt
.bin
.inf
.ini
.cfg
.properties
.pak
.json

I almost have:

Deflate for encoding png, zip, and other compressed formats

.png
.db
.zip

When i am done with a few more formats ill release the first version of the dll
