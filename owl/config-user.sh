#!/bin/bash -e
#
# (C) Copyright 2015, actions Limited
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.

echo  "change to build user mode for android code, please clean andoid/out and owl/out, or rename them to out-userdebug"

cp ../android/device/actions/lemaker_guitar_pax_d820/config/recovery/init-user.rc ../android/device/actions/lemaker_guitar_pax_d820/config/recovery/init.rc

cp ../android/device/actions/lemaker_guitar_pax_d820/device-user.mk  ../android/device/actions/lemaker_guitar_pax_d820/device.mk

cp ../android/device/actions/lemaker_guitar_pax_d820/init-user.rc ../android/device/actions/lemaker_guitar_pax_d820/init.rc

cp ../android/device/actions/lemaker_guitar_pax_d820/system-user.prop ../android/device/actions/lemaker_guitar_pax_d820/system.prop

cp ../android/system/extras/su/Android-user.mk   ../android/system/extras/su/Android.mk

cp ../android/device/actions/common/prebuilt/apk/thirdparty/superuser/Android-user.mk  ../android/device/actions/common/prebuilt/apk/thirdparty/superuser/Android.mk

cp ../kernel/arch/arm/boot/dts/lemaker_guitar_pax_d820-user.dts  ../kernel/arch/arm/boot/dts/lemaker_guitar_pax_d820.dts

cp ../owl/s500/boards/android/lemaker_guitar_pax_d820/os-user.mk   ../owl/s500/boards/android/lemaker_guitar_pax_d820/os.mk 

cp ../owl/s500/boards/android/lemaker_guitar_pax_d820/uenv-user.txt  ../owl/s500/boards/android/lemaker_guitar_pax_d820/uenv.txt

mv ../android/out  ../android/out-userdebug

mv ../owl/out  ../owl/out-userdebug


