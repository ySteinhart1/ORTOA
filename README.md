# ORTOA-TEE

## Project Dependencies

### 1. OpenEnclave SDK

Follow the installation instructions found in the [OpenEnclave documentation](https://github.com/openenclave/openenclave/blob/master/docs/GettingStartedDocs/install_oe_sdk-Ubuntu_20.04.md).

### 2. Apache Thrift

First, install the tools and libraries required to build and install the Apache Thrift compiler and C++ libraries. The following instructions are for an Ubuntu Linux based system. Pulled from this [Thrift documentation](https://thrift.apache.org/docs/install/debian.html).

```bash
sudo apt-get install automake bison flex g++ git libboost-all-dev libevent-dev libssl-dev libtool make pkg-config
```

Then, install the Thrift compiler:

```bash
sudo apt install thrift-compiler
```

Finally, install the Thrift C++ library:

```bash
sudo apt install libthrift-dev
```

### 3. Redis C/C++ Client (`hiredis` & `redis-plus-plus`)

First, install [`hiredis`](https://github.com/redis/hiredis). _Note: do not install multiple versions of `hiredis`. Otherwise, there might be some bizarre conflicts._

```bash
# Clone hiredis
ORTOA/ $ git clone https://github.com/redis/hiredis.git
ORTOA/ $ cd hiredis

# Build hiredis
ORTOA/hiredis $ make
ORTOA/hiredis $ sudo make install
```

Then, install [`redis-plus-plus`](https://github.com/sewenew/redis-plus-plus). _Since `redis-plus-plus` depends on `hiredis`, ensure that `hiredis` is installed first._

```bash
# Clone redis-plus-plus
ORTOA/ $ git clone https://github.com/sewenew/redis-plus-plus.git
ORTOA/ $ cd redis-plus-plus

# Create the build directory
ORTOA/redis-plus-plus $ mkdir build
ORTOA/redis-plus-plus $ cd build

# Build redis-plus-plus
ORTOA/redis-plus-plus/build $ cmake -DREDIS_PLUS_PLUS_CXX_STANDARD=17 ..
ORTOA/redis-plus-plus/build $ make
ORTOA/redis-plus-plus/build $ sudo make install
```

Finally, you can clean up the repo by deleting the `hiredis/` and `redis-plus-plus/` directories. This is because (by default) they are installed at `/usr/local`.

### [Optional] Dev Dependencies

| Name           | Installation                        |
| -------------- | ----------------------------------- |
| `clang-format` | `sudo apt-get install clang-format` |

## Python Package (SDK)

Our package requires `python>=3.8.*` and can be initialized via `pip`:

```bash
# Create & activate a virtual environment
ORTOA/ $ python3 -m venv .venv
ORTOA/ $ . .venv/bin/activate

# Install the package
ORTOA/ $ pip install -e extras/
```

The dev dependencies can be installed via `pip` as well:

```bash
ORTOA/ $ pip install -e extras/[dev]
```

## Sourcing Scripts & Utilities

We provide a bash script library `srcipts/ortoa-lib.sh` that houses a collection of scripts and utilities to manage the `ORTOA` environment. It can be sourced as follows:

```bash
ORTOA/ $ source scripts/ortoa-lib.sh
```

## Building the codebase

```bash
# Create build directory
ORTOA/ $ mkdir build && cd build

# Build
ORTOA/build/ $ cmake ..
ORTOA/build/ $ make
```

## Running ORTOA

### After building the codebase & sourcing the scripts...

#### `ortoa-simulate`

```bash
ORTOA/ $ ortoa-simulate -h
```

#### `ortoa-client-run`

```bash
ORTOA/ $ ortoa-client-run -h
```
