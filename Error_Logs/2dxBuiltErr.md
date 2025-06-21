## 背景信息：

- 系统信息：`Linux 5.15.0-67-generic #74~20.04.1-Ubuntu  x86_64 x86_64 x86_64 GNU/Linux`

- cocos版本：3.17

## 遇到问题：

- cocos2dx官方提供的Chipmunk预编译包中的glibc版本过旧，导致无法使用。

## 解决办法：

1、放弃预编译包，从源代码编译

​	命令如下：

```bash
cd InternTest/CardGame/cocos2d/external && wget https://github.com/slembcke/Chipmunk2D/archive/refs/tags/Chipmunk-7.0.3.tar.gz
tar -xzf Chipmunk-7.0.3.tar.gz
cd Chipmunk2D-Chipmunk-7.0.3 && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_DEMOS=OFF -DBUILD_SHARED=OFF
make -j$(nproc)
```

2、在Ubuntu20.04中编译该版本Chipmunk源代码时，将cpHastySpace.c包含的头文件中的`#include <sys/sysctl.h>` 使用`#ifdef _APPLE_`包装起来，即：

```bash
#ifdef __APPLE__
#include <sys/sysctl.h>
#endif
```

即可正确编译。

3、替换原来的库文件， 问题解决。

```bash
cp src/libchipmunk.a ../../chipmunk/prebuilt/linux/64-bit/libchipmunk.a
```

> Authored By Ryan Mendez 🎯.