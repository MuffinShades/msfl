# msfl

C++ File encoder and decoder library for many popular file types

This library is planned to support the following file types:

*Note ones labeled with (M) mean they can only be read with the library since the file type expresses abiguity between programs

.txt
.bin
.ini
.inf
.cfg
.properties
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

#Currently I have implented

Deflate and Inflate for decoding png, zip, and other compressed formats

.txt
.bin
.inf
.ini
.cfg
.properties
.pak
.json (> 1mb / s)

#I almost have:

.png

#This library will also include the following library functions:

zlib -> Inflate and Deflate
.db
.zip

When i am done with a few more formats ill release the first version of the dll
