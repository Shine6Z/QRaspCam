sudo apt-get install -y qt5-default cmake
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
