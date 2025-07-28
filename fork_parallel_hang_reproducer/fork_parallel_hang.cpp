#include "llvm/Support/Parallel.h"
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <sys/wait.h>

std::mutex mtx;

void run_parallel_for(const char* label, size_t N)
{
    llvm::parallelFor(0, N, [=](size_t i)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "[" << label << "] task " << i << "\n";
        }
    });
}

int main()
{
    std::cout << "[Parent] Running initial parallelFor...\n";
    run_parallel_for("Parent", 2);
    std::cout << "[Parent] Forking...\n";
    pid_t pid = fork();

    if (pid == -1)
    {
        std::cout << "[Parent] Fork failed.\n";
        return 1;
    }
    if (pid == 0)
    {
        std::cout << "[Child] Running parallelFor in child...\n";
        run_parallel_for("Child", 2); // hangs here
        std::cout << "[Child] Done.\n";
        return 0;
    }

    // Parent waits for child
    int status = 0;
    waitpid(pid, nullptr, 0);
    std::cout << "[Parent] Child exited with status " << status << std::endl;
    return 0;
}