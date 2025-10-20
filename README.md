# SystemC for Ubuntu

## Setup

Google TestのCMakeスクリプト参考

https://qiita.com/shohirose/items/30e39949d8bf990b0462

```sh
export CC=clang
export CXX=clang++
sudo apt -y install autotools-dev
git clone https://github.com/ninja-build/ninja.git
cd ninja
cmake -S . -Bbuild -DBUILD_TESTING=OFF
cmake --build build
sudo cmake --install build
git clone https://github.com/google/googletest.git
cd googletest
cmake -S . -Bbuild -G Ninja
cmake --build build
sudo cmake --install build
```

```sh
export CC=clang
export CXX=clang++
git clone https://github.com/accellera-official/systemc.git
cd systemc
switch 2.3.2
autoreconf .
../configure --prefix=/usr/local/systemc-2.3.2
make -j$(nproc)
make check
make install
```

```sh
export SYSTEMC="/usr/local/systemc-2.3.2"
export SYSTEMC_INCLUDE="$SYSTEMC/include"
export SYSTEMC_LIBDIR="$SYSTEMC/lib-linux64"
```



## Usage

```sh
cmake -S . -Bbuild -G Ninja
cmake --build build
./build/hello_test
```