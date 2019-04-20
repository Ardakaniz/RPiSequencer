#!/bin/bash

# blynk v0.6.1 installation
echo "*** BLYNK BUILD ***"
wget https://github.com/blynkkk/blynk-library/releases/download/v0.6.1/Blynk_Release_v0.6.1.zip
unzip Blynk_Release_v0.6.1
mv libraries/Blynk ./Blynk
rm -r libraries tools Blynk_Release_v0.6.1.zip
cd Blynk/linux
./build.sh raspberry
cd ../../

# RtMidi installation: http://www.music.mcgill.ca/~gary/rtmidi/index.html
echo "*** RTMIDI BUILD ***"
wget http://www.music.mcgill.ca/~gary/rtmidi/release/rtmidi-3.0.0.tar.gz
tar xzf rtmidi-3.0.0.tar.gz
rm rtmidi-3.0.0.tar.gz
mv rtmidi-3.0.0 rtmidi
cd rtmidi
sudo apt-get install autoconf libtool automake
./autogen.sh
make
