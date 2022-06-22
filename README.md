# Neural network for mnist handwritten digits from scratch in C++

## Usage

0. get the mnist training and testing data [here](http://yann.lecun.com/exdb/mnist/) and extract it into a directory named `data/`
1. make any changes you would like to the meta parameters of the network in the source code.
2.
```sh
make
./train
./evaluate
```

(If you re-build, you'll probably have to delete the previous binaries beforehand.)

## References
* https://www.youtube.com/watch?v=aircAruvnKk&list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi
* http://yann.lecun.com/exdb/mnist/
* https://www.youtube.com/watch?v=ReOxVMxS83o
* https://github.com/markkraay/mnist-from-scratch

## Story time

In 2019, college me was really into two things: neural networks and C/C++. So I tried to combine the two into one project: why not try to build a neural net, from scratch, in C++? After weeks of trying to understand and implement the math, it still did not work. And while I came back from time to time, I never managed to solve the last few remaining problems, so I pretty much considered it a lost cause.

3 years later I get inspired by a youtube video, a conversation with a former classmate and a recent re-write of another old project of mine. So I sit down one afternoon, get familiar with the codebase again, realize how awful it is, then get it working in 3 hours with 90.6% accuracy. The code is still awful, and will probably stay that way, but at least it works now and I have bragging rights.
