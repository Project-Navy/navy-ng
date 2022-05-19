#include "pyvm.h"

#include <navy/debug.h>
#include <navy/pymarshal.h>
#include <cpython/opcode.h>

static MarshalObject debug_log(MarshalVec args)
{
    for (size_t i = 0; i < args.length; i++)
    {
        switch (args.data[i].type)
        {
            case TYPE_STRING:
            case TYPE_SHORT_ASCII:
            case TYPE_SHORT_ASCII_INTERNED:
            {
                log$("{}", args.data[i]._str);
                break;
            }
            default:
            {
                panic$("{} not supported", args.data[i].type);
            }
        }
    }

    return (MarshalObject) {.type = TYPE_NONE};
}

static MarshalMap builtins(void)
{
    MarshalMap builtins;

    map_init(&builtins);

    map_set(&builtins, "debug_log", debug_log);

    return builtins;
}

MarshalObjectOption py_run(MarshalCode code)
{
    MarshalVec stack;
    ReaderCStr codeReader;
    MarshalMap funcbuiltin = builtins();

    vec_init(&stack);
    reader_init(&codeReader, code.code._str.buf, code.code._str.len);

    while (!reader_eof(&codeReader))
    {
        uint8_t opcode = reader_next(&codeReader);
        uint8_t arg = reader_next(&codeReader);

        switch (opcode)
        {
            case LOAD_NAME:
            {
                vec_push(&stack, code.names._vec.data[arg]);
                break;
            }

            case LOAD_CONST:
            {
                vec_push(&stack, code.consts._vec.data[arg]);
                break;
            }

            case CALL_FUNCTION:
            {
                MarshalVec args;
                vec_init(&args);

                for (size_t i = 0; i < arg; i++)
                {
                    vec_push(&args, vec_pop(&stack));
                }

                MarshalObject func = vec_pop(&stack);

                if (strlen(func._str.buf) != func._str.len)
                {
                    func._str.buf[func._str.len] = 0;
                }

                PyBuiltin *funcptr = map_get(&funcbuiltin, func._str.buf);

                if (funcptr != NULL)
                {
                    (*funcptr)((args));
                    vec_free(&args);
                }
                else  
                {
                    panic$("{} is not defined", func._str);
                }

                // TODO
                break;
            }

            case POP_TOP:
            {
                if (stack.length)
                {
                    vec_pop(&stack);
                }

                break;
            }

            case RETURN_VALUE:
            {
                MarshalObject ret = vec_pop(&stack);
                vec_free(&stack);
                map_deinit(&funcbuiltin);
                return SOME(MarshalObjectOption, ret);
            }
            
            default: 
            {
                panic$("Unknown opcode {}", opcode);
            }
        }
    }

    return NONE(MarshalObjectOption);
}