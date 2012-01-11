fl = [ # file list
    'AST.cpp',
    'helpers.cpp',
    'parse.cpp',
    'Tokenizer.cpp',
    'Visitor.cpp',
]

test_fl = fl + [
    'test.cpp',
]

env = Environment()
env.Append(CPPFLAGS='-std=c++0x -ggdb3 ')
env.Append(CPPFLAGS='-Wall -Wextra -pedantic ')

env.Program('test', test_fl);
