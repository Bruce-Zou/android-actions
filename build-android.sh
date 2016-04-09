rm android/out
rm owl/out
ln -s  android/out-pcba android/out
ln -s owl/out-pcba owl/out
./autobuild.sh config
./autobuild.sh u-boot
./autobuild.sh kernel
cd android/
source build/envsetup.sh
lunch
mmm device/actions/common/packages/AndroidPcba/
#make -j8
cd ../
./autobuild.sh

