
wget http://prdownloads.sourceforge.net/scons/scons-2.0.1.tar.gz
wget http://swtoolkit.googlecode.com/files/swtoolkit.0.9.1.zip

wget http://libjingle.googlecode.com/files/srtp-cvs.zip
wget http://prdownloads.sourceforge.net/project/expat/expat/2.0.1/expat-2.0.1.tar.gz

wget http://googletest.googlecode.com/files/gtest-1.6.0.zip

wget http://www.openssl.org/source/openssl-1.0.1.tar.gz

# for ios:
wget http://ios-static-libraries.googlecode.com/files/ios-libraries-2011-03-10-065803.zip

tar xvvfz scons-2.0.1.tar.gz
cd scons-2.0.1
python setup.py build
sudo python setup.py install
cd -

unzip swtoolkit.0.9.1.zip
chmod 755 swtoolkit/hammer.sh


mv srtp-cvs.zip talk/third_party #/srtp
cd talk/third_party
unzip srtp-cvs
mv srtp-cvs srtp
cd -


mv expat-2.0.1.tar.gz talk/third_party
cd talk/third_party
tar xvvfz expat-2.0.1.tar.gz
cd -

mv gtest-1.6.0.zip talk/third_party
cd talk/third_party
unzip gtest-1.6.0.zip
mv gtest-1.6.0 gtest
cd -

mv openssl-1.0.1.tar.gz talk/third_party
cd talk/third_party
tar xvvfz openssl-1.0.1
mv openssl-1.0.1 openssl
cd -

mv ios-libraries-2011-03-10-065803.zip talk/third_party
cd talk/third_party
unzip ios-libraries-2011-03-10-065803.zip
mv Binaries ios-libraries
cd -

