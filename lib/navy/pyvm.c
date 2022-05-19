#include "pyvm.h"

void py_run(MarshalCode code)
{
    VecChar stack;
    ReaderStr codeReader;

    vec_init(&stack);
    reader_init(&codeReader, code.code._str, code.code._str.len);
}