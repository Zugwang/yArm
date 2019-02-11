echo "\n\n-- Installation des dépendances --\n\n"
apt install libsfml-dev cmake

echo "\n\n-- Installation de MyoLinux --\n\n"
cd /tmp/
git clone https://github.com/brokenpylons/MyoLinux.git
cd MyoLinux
mkdir build
cd build
cmake ..
make
make install

echo "\n\n-- Installation de TA-Lib --\n\n"
wget https://freefr.dl.sourceforge.net/project/ta-lib/ta-lib/0.4.0/ta-lib-0.4.0-src.tar.gz
tar -zxvf ta-lib-0.4.0-src.tar.gz
cd ta-lib
./configure --prefix=/usr
make
make install

echo "\n\n-- Tout est fini ! Merci de votre patience et à bientôt sur Communisme Airlines --"
