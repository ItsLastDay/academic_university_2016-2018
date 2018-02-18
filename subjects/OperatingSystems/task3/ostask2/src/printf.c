#include <printf.h>

/*
 * Function commented out to get rid of "defined but not used" warning.
static void printf_test()
{
    printf("Hello, world! 100%% loaded\n");
    printf("Interrupt %d on %s %c %c occured\n", 123, "PIT", 'a', 'b');
    printf("%d %i\n%u %o %x\n", 10, 10, 10, 10, 10);
    printf("%d %i\n%u %o %x\n", -10, -10, -10, -10, -10);
    printf("%hhd %hhi\n%hhu %hho %hhx\n", 10, 10, 10, 10, 10);
    printf("%hhd %hhi\n%hhu %hho %hhx\n", -10, -10, -10, -10, -10);
    printf("%hd %hi\n%hu %ho %hx\n", 1210, 1210, 1210, 1210, 1210);
    printf("%hd %hi\n%hu %ho %hx\n", -1210, -1210, -1210, -1210, -1210);
    printf("%ld %li\n%lu %lo %lx\n", 10, 10, 10, 10, 10);
    printf("%ld %li\n%lu %lo %lx\n", -10l, -10l, -10l, -10l, -10l);
    printf("%lld %lli\n%llu %llo %llx\n", 10ll, 10, 10, 10, 10);
    // Does not work with '-10' without 'll'. 
    // `va_args(args, long long)` returns inadequate value in that case, because
    // '-10' is not converted to `long long` when it is put into `va_list`.
    printf("%lld %lli\n%llu %llo %llx\n", -10ll, -10ll, -10ll, -10ll, -10ll);

    printf("\n\n\n");

    char buf[50];
    snprintf(buf, 50, "privet, privet, privet, privet, privet, privet, privet, privet\n");
    printf("%s", buf);
    snprintf(buf, 50, "Interrupt %d on %s %c %c occured\n", 123, "PIT", 'a', 'b');
    printf("%s", buf);
    snprintf(buf, 50, "%d %i\n%u %o %x\n", 10, 10, 10, 10, 10);
    printf("%s", buf);
    snprintf(buf, 50, "%d %i\n%u %o %x\n", -10, -10, -10, -10, -10);
    printf("%s", buf);
    snprintf(buf, 50, "%hhd %hhi\n%hhu %hho %hhx\n", 10, 10, 10, 10, 10);
    printf("%s", buf);
    snprintf(buf, 50, "%hhd %hhi\n%hhu %hho %hhx\n", -10, -10, -10, -10, -10);
    printf("%s", buf);
    snprintf(buf, 50, "%hd %hi\n%hu %ho %hx\n", 1210, 1210, 1210, 1210, 1210);
    printf("%s", buf);
    snprintf(buf, 50, "%hd %hi\n%hu %ho %hx\n", -1210, -1210, -1210, -1210, -1210);
    printf("%s", buf);
    snprintf(buf, 50, "%ld %li\n%lu %lo %lx\n", 10, 10, 10, 10, 10);
    printf("%s", buf);
    snprintf(buf, 50, "%ld %li\n%lu %lo %lx\n", -10, -10, -10, -10, -10);
    printf("%s", buf);
    snprintf(buf, 50, "%lld %lli\n%llu %llo %llx\n", 10, 10, 10, 10, 10);
    printf("%s", buf);
    snprintf(buf, 50, "%lld %lli\n%llu %llo %llx\n", -10ll, -10ll, -10ll, -10ll, -10ll);
    printf("%s", buf);

    while (1);
}
*/

static void 
out_char(
        char ch,
        int is_sprintf,
        char **output_buf_p,
        size_t max_char,
        unsigned int *num_parsed_p)
{
    ++(*num_parsed_p);
    if (is_sprintf) 
    {
        if (*num_parsed_p + 1 <= max_char)
        {
            **output_buf_p = ch;
            ++(*output_buf_p);
        }
    } 
    else 
    {
        write_byte(ch);
    }
}

static void 
out_str(
        const char *str,
        int is_sprintf,
        char **output_buf_p,
        size_t max_char,
        unsigned int *num_parsed_p)
{
    while(*str)
    {
        out_char(*str, is_sprintf, output_buf_p,
                max_char, num_parsed_p);
        ++str;
    }
}

static char number_buf[70];

static char digit_conv(short digit)
{
    if (digit < 10)
    {
        return digit + '0';
    }
    return digit - 10 + 'A';
}

static void make_signed_number_buf(long long val, int base)
{
    int top = 0;
    if (val == 0)
    {
        number_buf[0] = '0';
        number_buf[1] = 0;
        return;
    }

    short is_minus = 0;
    if (val < 0) 
    {
        is_minus = 1;
        val = -val;
    }

    while (val > 0)
    {
        number_buf[top] = digit_conv(val % base);
        val /= base;
        top++;
    }

    for (int i = 0; top - i - 1 > i; i++)
    {
        char tmp = number_buf[top - i - 1];
        number_buf[top - i - 1] = number_buf[i];
        number_buf[i] = tmp;
    }

    if (is_minus)
    {
        for (int i = top; i > 0; i--)
        {
            number_buf[i] = number_buf[i - 1];
        }
        ++top;
        number_buf[0] = '-';
    }

    number_buf[top] = 0;
}

static void make_unsigned_number_buf(unsigned long long val, int base)
{
    int top = 0;

    if (val == 0)
    {
        number_buf[0] = '0';
        number_buf[1] = 0;
        return;
    }

    while (val > 0) 
    {
        number_buf[top] = digit_conv(val % base);
        val /= base;
        top++;
    }

    for (int i = 0; top - i - 1 > i; i++)
    {
        char tmp = number_buf[top - i - 1];
        number_buf[top - i - 1] = number_buf[i];
        number_buf[i] = tmp;
    }

    number_buf[top] = 0;
}

static int 
format_parser(
        int is_sprintf,
        const char *format,
        va_list args,
        char *output_buf,
        size_t max_char
        )
{
    unsigned int num_parsed_symbols = 0;

    while (*format) 
    {
        char cur_fmt = *format;
        // %d, i, u, o, x. Possibly with modifiers: hh, h, l, ll. 
        // %c, s.

        if (cur_fmt == '%') 
        {
            ++format;            
            if (*format == 'c')
            {
                char symb = va_arg(args, int);
                out_char(symb, is_sprintf,
                       &output_buf, max_char, &num_parsed_symbols); 
            }
            else if (*format == 's')
            {
                const char *str = va_arg(args, const char *);
                out_str(str, is_sprintf, 
                        &output_buf, max_char, &num_parsed_symbols);
            }
            else if (*format == '%')
            {
                char symb = '%';
                out_char(symb, is_sprintf,
                       &output_buf, max_char, &num_parsed_symbols); 
            }
            else 
            {
                short num_h = 0, num_l = 0;
                cur_fmt = *format;
                while (cur_fmt != 'd' && cur_fmt != 'i' && cur_fmt != 'u' 
                        && cur_fmt != 'o' && cur_fmt != 'x' && cur_fmt)
                {
                    if (cur_fmt == 'h')
                    {
                        ++num_h;
                    }
                    else if (cur_fmt == 'l')
                    {
                        ++num_l;
                    } 
                    ++format;
                    cur_fmt = *format;
                }

                if (!(*format))
                {
                    break;
                }

                int base = 10;
                if (cur_fmt == 'o')
                {
                    base = 8;
                }
                if (cur_fmt == 'x')
                {
                    base = 16;
                }

                unsigned short mask_short = -1;
                unsigned char mask_char = -1;
                unsigned long mask_long = -1;
                unsigned int mask_int = -1;

                //error: ‘short unsigned int’ is 
                //promoted to ‘int’ when passed through ‘...’ [-Werror]
                if (cur_fmt == 'd' || cur_fmt == 'i')
                {
                    long long tmp;
                    if (num_h == 0 && num_l == 0)
                    {
                        tmp = va_arg(args, int);
                        tmp &= mask_int;
                    }
                    else if (num_h == 2)
                    {
                        tmp = va_arg(args, int);
                        tmp &= mask_char;
                    }
                    else if (num_h == 1)
                    {
                        tmp = va_arg(args, int);
                        tmp &= mask_short;
                    }
                    else if (num_l == 1)
                    {
                        tmp = va_arg(args, long);
                        tmp &= mask_long;
                    }
                    else if(num_l == 2)
                    {
                        tmp = va_arg(args, long long);
                    }
                    make_signed_number_buf(tmp, base);
                } else {
                    unsigned long long tmp;
                    if (num_h == 0 && num_l == 0)
                    {
                        tmp = va_arg(args, unsigned int);
                        tmp &= mask_int;
                    } 
                    else if (num_h == 2)
                    {
                        tmp = va_arg(args, int);
                        tmp &= mask_char;
                    }
                    else if (num_h == 1)
                    {
                        tmp = va_arg(args, int);
                        tmp &= mask_short;
                    }
                    else if (num_l == 1)
                    {
                        tmp = va_arg(args, unsigned long);
                        tmp &= mask_long;
                    }
                    else if(num_l == 2)
                    {
                        tmp = va_arg(args, unsigned long long);
                    }
                    make_unsigned_number_buf(tmp, base);
                }

                out_str(number_buf, is_sprintf, 
                        &output_buf, max_char, &num_parsed_symbols);
            }
        } 
        else 
        {
            out_char(cur_fmt, is_sprintf, &output_buf, 
                    max_char, &num_parsed_symbols);
        }     

        ++format;
    }

    if (is_sprintf)
    {
        *output_buf = 0;
    }

    return num_parsed_symbols;
}

int vprintf(const char *format, va_list args)
{
    return format_parser(0, format, args, NULL, 0); 
}

int printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int parsed_chars = vprintf(format, args);
    va_end(args);
    return parsed_chars;
}

int snprintf(
        char *output_buf, 
        size_t max_char, 
        const char *format,
        ...)
{
    va_list args;
    va_start(args, format);
    int parsed_chars = vsnprintf(output_buf, max_char, format, args);
    va_end(args);
    return parsed_chars;
}

int vsnprintf(
        char *output_buf,
        size_t max_char,
        const char *format,
        va_list args)
{
    return format_parser(1, format, args, output_buf, max_char);
}
