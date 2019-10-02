sudo apt-get install -y qt5-default cmake patchelf
git clone https://github.com/cedricve/raspicam
cd raspicam
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
cd ..
rm -rf raspicam
git clone https://github.com/bgallois/QRaspCam.git
cd QRaspCam/client/
qmake client.pro
make
sudo cp client /usr/local/bin/
