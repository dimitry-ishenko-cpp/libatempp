# libatem++ – C++ Library for Blackmagic ATEM Switchers

The **libatem++** library allows one to control Blackmagic ATEM devices.

At the moment only very basic functionality has been implemented, such as
manipulation of inputs, M/Es and aux busses.

See the [Usage](#usage) section below if you are planning to develop
applications with **libatem++**.

## Installation

### Binary

Debian/Ubuntu/etc:

```shell
$ p=libatem++ v=1.0
$ wget https://github.com/dimitry-ishenko-cpp/libatempp/releases/download/v${v}/${p}_${v}_amd64.deb
$ sudo apt install ./${p}_${v}_amd64.deb
```

Install the development package, if you are planning to develop applications
with **libatem++**:
```shell
$ p=libatem++-dev v=1.0
$ wget https://github.com/dimitry-ishenko-cpp/libatempp/releases/download/v${v}/${p}_${v}_amd64.deb
$ sudo apt install ./${p}_${v}_amd64.deb
```

RaspberryPi:

```shell
$ p=libatem++ v=1.0
$ wget https://github.com/dimitry-ishenko-cpp/libatempp/releases/download/v${v}/${p}_${v}_armhf.deb
$ sudo apt install ./${p}_${v}_armhf.deb
```

Install the development package, if you are planning to develop applications
with **libatem++**:
```shell
$ p=libatem++-dev v=1.0
$ wget https://github.com/dimitry-ishenko-cpp/libatempp/releases/download/v${v}/${p}_${v}_armhf.deb
$ sudo apt install ./${p}_${v}_armhf.deb
```

### From source

Stable version (requires [CMake](https://cmake.org/) >= 3.1 and
[Asio](https://think-async.com/Asio) >= 1.18.1):

```shell
$ p=libatem++ v=1.0
$ wget https://github.com/dimitry-ishenko-cpp/libatempp/releases/download/v${v}/${p}-${v}.tar.bz2
$ tar xzf v${v}.tar.gz
$ mkdir ${p}-${v}/build
$ cd ${p}-${v}/build
$ cmake ..
$ make
$ sudo make install
```

Latest master (requires [git](https://git-scm.com/), [CMake](https://cmake.org/) >=
3.1 and [Asio](https://think-async.com/Asio) >= 1.18.1):

```shell
$ p=libatempp
$ git clone --recursive https://github.com/dimitry-ishenko-cpp/${p}.git
$ mkdir ${p}/build
$ cd ${p}/build
$ cmake ..
$ make
$ sudo make install
```

## Usage

_TODO_

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.