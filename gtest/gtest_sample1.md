# gtest 单元测试框架
## gtest安装
通过cmake工具链进行安装。
源代码地址为<https://github.com/google/googletest>
```bash
git clone https://github.com/google/googletest
mkdir build
cd build
cmake ..
make
```
运行上述文件之后，会在`build/lib/`目录下生成`libgtest.a`和`libgtest_main.a`两个静态库。可以选择将这两个静态库拷贝到系统目录:
```bash
# 拷贝库文件到/usr/lib
cp libtest*.a /usr/lib
# 拷贝头文件(位于googletest/目录下)到 /usr/include
cp -a include/gtest /usr/include
```

## gtest测试代码用例sample1
在`googletest/samples`目录下有相关的gtest使用范例。
sample1测试用例实现了两个功能函数`Factorial(int n)`求阶乘和`IsPrime(int n)`检查素数。

sample1.h
```c++
#ifndef GTEST_SAMPLES_SAMPLE1_H_
#define GTEST_SAMPLES_SAMPLE1_H_

int Factorial(int n);

int IsPrime(int n);
```

sample1.cc（.cc是Linux、Unix下为C++源文件的默认扩展名。使用GCC、G++在Linux、Unix下可以打开和编译）
```c++
#include "sample1.h"

int Fatorial(int n) {
    int result = 1;
    for(int i = 1; i <= n; i++){
        result *= i;
    }
    return result;
}

bool IsPrime(int n) {
    //Trivial case 1: small number
    if (n <= 1) return false;
    //Trivial case 2: even number
    if (n % 2 == 0) return n == 2;
    
    for (int i = 3; ; i += 2) {
        //we only have to try i up to the square root of n
        if (i > n/i) break;
        //now, we have i <= n/i < n
        //if n is divisible by i, n is not prime
        if (n % i == 0) return false;
    }
    // n has no integer factor in range (1, n), and thus is prime
    return true;
}
```

sample1_unitest.cc
```c++
//第一步：include所有头文件，一定要包含gtest
#include <limits.h>
#include "sample1.h"
#include "gtest/gtest.h"
namespace { //代码命名空间隔离
//第二步：使用 TEST macro来定义unit test
//TEST 有两个参数：1. test case name; 2. test name
//每一个unit test都是一个独立的assertion语句
//在google test中，测试被分成test cases，以此来保证代码是有组织的

//注意test case name命名
TES（FactorialTest, Nagative）{
    EXPECT_EQ(1, Factorial(-5));
    EXPECT_EQ(1, Factorial(-1));
    EXPECT_EQ(Factorial(-10), 0);
}

TEST(FactorialTest, Zero) {
    EXPECT_EQ(1, Factorial(0));
}

TEST(FactorialTest, Positive) {
    EXPECT_EQ(1, Factorial(1));
    EXPECT_EQ(2, Factorial(2));
    EXPECT_EQ(6, Factorial(3));
    EXPECT_EQ(40320, Factoiral(8));
}

TEST(IsPrimeTest, Negative) {
    EXPECT_FALSE(IsPrime(-1));
    EXPECT_FALSE(IsPrime(-2));
    EXPECT_FALASE(IsPrime(INT_MIN));
}

TEST(IsPrimeTest, Trivial) {
    EXPECT_FALSE(IsPrime(0));
    EXPECT_FALSE(IsPrime(1));
    EXPECT_TRUE(IsPrime(2));
    EXPECT_TRUE(IsPrime(3));
}

TEST(IsPrimeTest, Positive) {
    EXPECT_FALSE(IsPrime(4));
    EXPECT_TRUE(IsPrime(5));
    EXPECT_FALSE(IsPrime(6));
    EXPECT_TRUE(IsPrime(23));
}

} //namespace

//第三步，运行 RUN_ALL_TESTS() in main()
```

这里要注意，test_case_name和test_name可以任意取，但是不能包含下划线.

sample1_unittest_main.cc （这里选择了与官方sample一致的代码，提供一个executable的main函数）
```c++
#include <iostream>
#include "gtest/gtest.h"

#if GTEST_OS_ESP8266 || GTEST_OS_ESP32
#if GTEST_OS_ESP8266
extern "C" {
#endif
void setup() {
  testing::InitGoogleTest();
}

void loop() { RUN_ALL_TESTS(); }

#if GTEST_OS_ESP8266
}
#endif

#else

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from %s\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
```

## gtest sample编译
1. 我们要测试的函数位于`sample1.cc`中，这个文件需要独立编译一个库
2. 我们的测试用例位于`sample1_unittest.cc`中，这个文件与`sample1_unittest_main.cc`联合起来行程一个可执行文件

编写CMakeLists.txt如下
```cmake
cmake_minimum_required(VERSION 3.10)

# enables testing for this directory and below
enable_testing()
# 这里需要注意使用到了Threads库，对应于下述的链接${CMAKE_THREAD_LIBS_INIT}
find_package(Threads REQUIRED)

# 设置googletest的根目录，用于找到gtest的头文件和libgtest.a
set(GTEST_ROOT /headless/thirdpart_lib/googletest)

include_directories(${GTEST_ROOT}/googletest/include)
link_directories(${GTEST_ROOT}/build/lib)

add_library(sample1 SHARED
    sample1.cc
)

# to include the sample1.h
target_include_directories(sample1 PUBLIC
    ${GTEST_ROOT}/googletest/samples
)

add_executable(sample1_unittest_main
    sample1_unittest_main.cc
    sample1_unittest.cc
)
target_link_libraries(sample1_unittest_main
    sample1
    gtest
)
target_link_libraries(sample1_unittest_main
    ${CMAKE_THREAD_LIBS_INIT}
)
```

这里为了方便，我把`CMakeLists.txt` 放置在了和sample1.cc等源文件相同的路径下。同样适用cmake编译。

## 测试结果
```bash
root@97b340d23f5a:~/thirdpart_lib/googletest/googletest/samples/build# ./sample1_unittest_main
Running main() from /headless/thirdpart_lib/googletest/googletest/samples/sample1_unittest_main.cc
[==========] Running 6 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from FactorialTest
[ RUN      ] FactorialTest.Negative
[       OK ] FactorialTest.Negative (0 ms)
[ RUN      ] FactorialTest.Zero
[       OK ] FactorialTest.Zero (0 ms)
[ RUN      ] FactorialTest.Positive
[       OK ] FactorialTest.Positive (0 ms)
[----------] 3 tests from FactorialTest (0 ms total)

[----------] 3 tests from IsPrimeTest
[ RUN      ] IsPrimeTest.Negative
[       OK ] IsPrimeTest.Negative (0 ms)
[ RUN      ] IsPrimeTest.Trivial
[       OK ] IsPrimeTest.Trivial (0 ms)
[ RUN      ] IsPrimeTest.Positive
[       OK ] IsPrimeTest.Positive (0 ms)
[----------] 3 tests from IsPrimeTest (0 ms total)

[----------] Global test environment tear-down
[==========] 6 tests from 2 test suites ran. (0 ms total)
[  PASSED  ] 6 tests.
```

