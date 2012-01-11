#include <cstdlib>
#include <iostream>
#include <string>
#include "helpers.hpp"

static inline void write(std::ostream & os, std::string const & msg) {
    os << msg << '\n';
}

void log(std::string const & msg) {
    write(std::clog, msg);
}
void err(std::string const & msg) {
    write(std::cerr, msg);
}

void fail(std::string const & msg) {
    err(msg);
    std::exit(EXIT_FAILURE);
}
