#include "lexer.h"
#include <stddef.h>
#include <stdbool.h>

#include <navy/debug.h>

static void lison_smart_push(VecChar *builder, VecStr *tokens)
{
    if (builder->length > 0)
    {
        vec_push(builder, '\0');
        vec_push(tokens, str$(builder->data));

        vec_clear(builder);
        vec_init(builder);
    }
}


VecStr lison_lex_str(Str s)
{
    char c;
    size_t i;
    bool is_str = false;

    VecChar builder;
    VecStr tokens;

    vec_init(&builder);
    vec_init(&tokens);

    str_foreach$(s, c, i)
    {
        switch(c)
        {
            case '\n':
            {
                lison_smart_push(&builder, &tokens);
                vec_push(&tokens, str$("\n"));
                break;
            }

            case '\r':
            case '\f':
            case '\v':
            {
                lison_smart_push(&builder, &tokens);
                break;
            }
            
            case '\t':
            case ' ':
            {
                if (!is_str)
                {
                    lison_smart_push(&builder, &tokens);
                }
                else
                {
                    vec_push(&builder, c);
                }

                break;
            }

            case '#':
            {
                lison_smart_push(&builder, &tokens);

                if (s.buf[i + 1] == 't')
                {
                    vec_push(&tokens, str$("#t"));
                }
                else
                {
                    vec_push(&builder, c);
                    lison_smart_push(&builder, &tokens);
                }

                break;
            }

            case '\'':
            case ')':
            case '(':
            {
                if (!is_str)
                {
                    lison_smart_push(&builder, &tokens);
                    vec_push(&builder, c);
                    lison_smart_push(&builder, &tokens);
                }
                else
                {
                    vec_push(&builder, c);
                }

                break;
            }

            case '\"':
            {
                vec_push(&builder, c);
                is_str = !is_str;
                break;
            }

            case ';':
            {
                while (c != '\0' && c != '\n' && c != -1)
                {
                    continue;
                }

                if (c == '\n')
                {
                    vec_push(&tokens, str$("\n"));
                }

                break;
            }

            default:
            {
                vec_push(&builder, c);
                break;
            }
        }
    }

    lison_smart_push(&builder, &tokens);
    vec_free(&builder);
    return tokens;
}