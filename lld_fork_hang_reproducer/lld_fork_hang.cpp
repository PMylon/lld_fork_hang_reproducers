#include "lld/Common/Driver.h"
#include "llvm/Support/raw_ostream.h"
#include <unistd.h>
#include <sys/wait.h>
#include <optional>
#include <array>
#include <iostream>
#include <cstdlib>

// Create a dummy object file for linking
bool createDummyObject(const char* filename)
{
    std::string cmd = "echo 'int x;' | clang -x c -c -o " + std::string(filename) + " -";
    return (std::system(cmd.c_str()) == 0);
}

LLD_HAS_DRIVER(elf)

std::optional<std::string> lldInvoke(const char *inPath, const char *outPath)
{
    const char* env = std::getenv("LLD_DISABLE_THREADS");
    bool disableThreads = (env && (std::string(env) == "1"));
    std::vector args{"ld.lld","-shared", inPath, "-o", outPath};
    if (disableThreads)
    {
        args.insert(args.begin() + 1, "--threads=1");
    }
    std::string errString;
    llvm::raw_string_ostream errStream(errString);
    auto lldRes = lld::lldMain(args, llvm::outs(), llvm::errs(),
                             {{lld::Gnu, &lld::elf::link}});
    bool noErrors = (!lldRes.retCode && lldRes.canRunAgain);
    if (!noErrors) {
        errStream.flush();
        return errString;
    }

    return {};
}

int main()
{
    const char* input1 = "dummy_kernel1.o";
    const char* input2 = "dummy_kernel2.o";
    const char* output1 = "out1.so";
    const char* output2 = "out2.so";

    std::cout << "[Parent] Compiling...\n";
    if (!createDummyObject(input1))
    {
        std::cerr << "[Parent] Failed to compile!\n";
        return 1;
    }

    std::cout << "[Parent] Linking...\n";
    if (auto err = lldInvoke(input1, output1))
    {
        std::cerr << "[Parent] Failed to link! Error: " << *err << std::endl;
        return 1;
    }

    std::cout << "[Parent] Forking...\n";
    pid_t pid = fork();

    if (pid == -1)
    {
        std::cout << "[Parent] Failed to fork!\n";
        return 1;
    }

    if (pid == 0)
    {
        std::cout << "[Child] Compiling...\n";
        if (!createDummyObject(input2))
        {
            std::cerr << "[Child] Failed to compile!\n";
            return 1;
        }

        std::cout << "[Child] Linking...\n";
        if (auto err = lldInvoke(input2, output2))
        {
            std::cerr << "[Child] Failed to link! Error: " << *err << std::endl;
            return 1;
        }

        std::cout << "[Child] Shared executable was created successfully!\n";
        return 0;
    }

    // Parent waits for child
    int status = 0;
    waitpid(pid, nullptr, 0);
    std::cout << "[Parent] Child finished!\n";
    return 0;
}