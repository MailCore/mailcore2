current_dir="`pwd`"

cd $current_dir/../ExternalsSrc/libetpan/build-android
chmod 777 ./build_local.sh
./build_local.sh
cd $current_dir

./build_local.sh

./build_java.sh