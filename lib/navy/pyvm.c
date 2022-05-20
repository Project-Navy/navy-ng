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

            case TYPE_INT:
            {
                log$("{}", args.data[i]._int);
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

static BuiltinMap builtins(void)
{
    BuiltinMap builtins;

    map_init(&builtins);

    map_set(&builtins, "debug_log", debug_log);

    return builtins;
}

MarshalObjectOption py_run(MarshalCode code)
{
    MarshalVec stack;
    ReaderCStr codeReader;
    BuiltinMap funcbuiltin = builtins();
    MarshalMap locals;

    vec_init(&stack);
    map_init(&locals);
    reader_init(&codeReader, code.code._str.buf, code.code._str.len);

    while (!reader_eof(&codeReader))
    {
        uint8_t opcode = reader_next(&codeReader);
        uint8_t arg = reader_next(&codeReader);

        switch (opcode)
        {
            case LOAD_NAME:
            {
                MarshalObject obj = {.type = TYPE_NULL};

                Str funcname = code.names._vec.data[arg]._str;
                if (strlen(funcname.buf) != funcname.len)
                {
                    funcname.buf[funcname.len] = 0;
                }

                PyBuiltin *func = map_get(&funcbuiltin, funcname.buf);
                MarshalObject *local = map_get(&locals, funcname.buf);

                if (func != NULL)
                {
                    obj.type = TYPE_BUILTIN;
                    obj._builtin = *func;
                }

                if(local != NULL)
                {
                    obj = *local;
                }

                if (obj.type == TYPE_NULL)
                {
                    panic$("{} is not defined", code.names._vec.data[arg]._str);
                }

                vec_push(&stack, obj);

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

                if (func.type != TYPE_BUILTIN)
                {
                    panic$("TODO !");
                }
                else  
                {
                    vec_push(&stack, func._builtin(args));
                }

                vec_free(&args);

                break;
            }

            case STORE_NAME:
            {
                Str name = code.names._vec.data[arg]._str;
                if (strlen(name.buf) != name.len)
                {
                    name.buf[name.len] = 0;
                }

                map_set(&locals, name.buf, vec_pop(&stack));
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
                map_deinit(&locals);
                return SOME(MarshalObjectOption, ret);
            }

            case BINARY_ADD:
            {
                int64_t tos = vec_pop(&stack)._int;
                stack.data[stack.length - 1]._int += tos;
                break;
            }
            
            default: 
            {
                panic$("Unknown opcode {}", opcode);
            }
        }
    }

    map_deinit(&funcbuiltin);
    map_deinit(&locals);
    return NONE(MarshalObjectOption);
}