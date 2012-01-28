#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>
#include <llvm/Module.h>
#include <llvm/Support/raw_ostream.h>
#include "compile_ir.hpp"

int main(int argc, char** argv) {
    if (argc < 2) return 1;
    std::string input_fn(argv[1]); std::ifstream input(input_fn.c_str());
    std::string base_fn(input_fn.substr(0, input_fn.size() - 3));
    std::string err; int ret;

    if (input_fn.rfind(".bf") != input_fn.size() - 3) return 2;

    llvm::Module * module = compile_ir(input);
    if (!module) return 3;
    llvm::raw_fd_ostream out_ll((base_fn + ".ll").c_str(), err);
    if (!err.empty()) return 4;
    module->print(out_ll, nullptr);

    std::printf(("Outputting to " + base_fn + ".s ...\n").c_str());
    ret = std::system(("llc " + base_fn + ".ll").c_str());
    if (ret) return 5;

    std::printf(("Outputting to " + base_fn + ".o ...\n").c_str());
    ret = std::system(("as " + base_fn + ".s -o " + base_fn + ".o").c_str());
    if (ret) return 6;

    std::printf(("Outputting to " + base_fn + ".exe ...\n").c_str());
    ret = std::system(("gcc " + base_fn + ".o -o " + base_fn + ".exe").c_str());
    if (ret) return 7;
}
