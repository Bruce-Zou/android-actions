rm android/out
rm owl/out
cd android
ln -s out-pcba out
cd ../owl/
ln -s out-pcba out
cd ..
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

