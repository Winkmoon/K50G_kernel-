# how to build kernel

1. 拉取编译环境

   ```bash
   repo init -u https://android.googlesource.com/kernel/manifest \ 
   -b common-android12-5.10-2022-01
   ```

2. 将.repo/manifests/default.xml中这一句删掉：

   <project path="common" name="kernel/common" revision="android12-5.10-2022-01" upstream="android12-5.10-2022-01" dest-branch="android12-5.10-2022-01" />

   因为这个branch名字被改了，需要手动下载源码。

3. repo sync

4. 手动下载源码：https://android.googlesource.com/kernel/common/+/refs/tags/android12-5.10-2022-01_r1, 

   解压到common文件夹下。

5. 编译命令：

   ```bash
   LTO=thin BUILD_CONFIG=common/build.config.gki.aarch64.module build/build.sh
   ```