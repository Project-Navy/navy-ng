#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <lison/parser.h>
#include <navy/debug.h>


void debug_token(Lison token)
{
    
    switch (token.type)
    {
        case LISON_LIST:
        {

            printf("(");
            Lison obj;
            size_t i;
            vec_foreach(&token._list, obj, i)
            {
                debug_token(obj);
            }

            printf(")");
            break;
        }

        case LISON_SYMBOL:
        {
            printf("%s ", token._str.buf);
            break;
        }

        case LISON_STR: 
        {
            printf("\"%s\" ", token._str.buf);
            break;
        }

        case LISON_TRUE:
        {
            printf("#t ");
            break;
        }

        case LISON_NIL:
        {
            printf("nil ");
            break;
        }

        default:
        {
            printf("I'm too lazy to implement that\n");
            break;
        }

    }
}

int main(void)
{
    FILE *fp = fopen("./pkg/lison-test/test.lisp", "r");
    assert(fp != NULL);

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = calloc(1, size + 1);
    assert(buffer != NULL);
    fread(buffer, size, 1, fp);

    Lison master = lison_parse_cstr(buffer);

    if (master.type == LISON_ERROR)
    {
        log$("{} at line {}", master._error.message, master._error.line);
    }
    else  
    {
        debug_token(master);
        printf("\n");
    }

    lison_free(&master);
    free(buffer);
    return 0;
}