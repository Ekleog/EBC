#ifndef bbc_src_compile_ir_hpp
#define bbc_src_compile_ir_hpp 1

#include <iosfwd>
namespace llvm { class Module; }

llvm::Module * compile_ir(std::istream & is);

#endif // bbc_src_compile_ir_hpp
