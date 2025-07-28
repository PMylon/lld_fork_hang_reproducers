# Reproducers for LLVM issue: lldMain causes hang when called from forked process

This repo contains two reproducers for a hang caused by parallelFor when lldMain is called from a forked process:

- lld_fork_hang_reproducer: Reproduces the hang by calling lld::lldMain
- fork_parallel_hang_reproducer: Reproduces the hang by calling directly the point of failure (parallelFor)

LLVM issue: https://github.com/llvm/llvm-project/issues/150918
## Build & Run
For lld_fork_hang_reproducer:
```
mkdir -p build && cd build
cmake -DLLVM_DIR=<path-to-lib/cmake/llvm> -DLLD_DIR=<path-to-lib/cmake/lld> ..
cmake --build .
```
To reproduce the hang do:
```
./lld_fork_hang
```
For the workaround (use --threads==1 for lld) do:
```
LLD_DISABLE_THREADS=1 ./lld_fork_hang
```

For fork_parallel_hang_reproducer:
```
mkdir -p build && cd build
cmake -DLLVM_DIR=<path-to-lib/cmake/llvm> ..
cmake --build .

./fork_parallel_hang
```
