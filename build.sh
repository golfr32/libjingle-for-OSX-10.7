
. setup_environment.sh

cd talk/third_party/expat-2.0.1
chmod 755 configure
./configure
cd -

cd talk/third_party/srtp
chmod 755 configure
./configure
cd -

cd talk
HAMMER_OPTS=--verbose $PATH_TO_SWTOOLKIT/hammer.sh
cd -

