# memory-leak-reproduce

https://github.com/apache/pulsar-client-cpp/issues/55

## Reproduce

First, run the Pulsar 2.10.1 standalone on the host machine.

Then, run a `centos:7` docker container that mounts the project directory as `/app`.

```bash
export CONTAINER_ID=$(docker run -v $PWD:/app -itd centos:7)
docker exec -it $CONTAINER_ID /bin/bash
```

Install the necessary packages inside the container:

```bash
yum update -y
yum install -y gcc gcc-c++ unzip
```

You can find the pulsar-client-cpp released binaries here:
- [candidate-2](https://github.com/apache/pulsar-client-cpp/actions/runs/3238183311) bug exists
- [candidate-3](https://github.com/apache/pulsar-client-cpp/actions/runs/3294743638) bug fixed

This repo already contains these binaries.

## Test for candidate-3

Let's take candidate-3 as example first.

```bash
cd /app/binaries
unzip -q rpm-x86_64-candidate-3.zip
rpm -ivh x86_64/apache-pulsar-client-*.rpm
cd /app
g++ main.cc -std=c++11 -Wl,-rpath=/usr/lib -lpulsar
# Now a.out is generated.
```

Before running `./a.out`, you can open another terminal to login the container and run `./check_memory.sh` after running `./a.out`.

Then, run `./a.out` and see the outputs from `./check_memory.sh`.

The outputs of candidate-3:

```
LOOP_CNT: 0
+ cat /proc/865/status
+ grep VmData
VmData:    17548 kB
+ set +x
LOOP_CNT: 1
+ grep VmData
+ cat /proc/865/status
VmData:    17548 kB
...
LOOP_CNT: 73
+ cat /proc/865/status
+ grep VmData
VmData:    17548 kB
```

We can see the memory usage never increased.

## Test for candidate-2

Repeat the process in the previous section except installing the binaries of candidate-2:

```bash
# remove the previous installed binaries
rpm -e apache-pulsar-client-devel-3.0.0-1.x86_64
rpm -e apache-pulsar-client-3.0.0-1.x86_64
rpm -e apache-pulsar-client-debuginfo-3.0.0-1.x86_64
cd /app
rm -f a.out
cd /app/binaries
rm -rf x86_64/
unzip -q rpm-x86_64-candidate-2.zip
# ...
```

You can see the outputs:

```
LOOP_CNT: 0
+ cat /proc/1812/status
+ grep VmData
VmData:    17516 kB
+ set +x
LOOP_CNT: 1
+ cat /proc/1812/status
+ grep VmData
VmData:    17560 kB
...
LOOP_CNT: 47
+ cat /proc/1812/status
+ grep VmData
VmData:    18268 kB
+ set +x
```

The momory usage kept increasing.
