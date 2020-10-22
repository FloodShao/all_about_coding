# GTest的使用
## gtest安装
通过cmake工具链进行安装。
源代码地址为<https://github.com/google/googletest>
```bash
git clone https://github.com/google/googletest
mkdir build
cd build
cmake ..
make
#将gtest安装到/usr/local/include/目录下
#GTestConfig.cmake安装在/usr/local/lib/cmake/GTest
make install
```
## gtest_example的使用
上述方法使用CMake编译连接gtest_example项目文件
在`gtest_example/build/tests`目录下运行`gtest_example_test`可执行文件，就可以看到gtest运行结果。