#ifndef ebc_src_compile_ir_hpp
#define ebc_src_compile_ir_hpp 1

#include <iosfwd>
namespace llvm { class Module; }

llvm::Module * compile_ir(std::istream & is);

#endif // ebc_src_compile_ir_hpp
