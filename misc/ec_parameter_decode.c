#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
** Translation Table to decode (created by author)
*/
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

/*
** decodeblock
**
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
static void decodeblock( unsigned char *in, unsigned char *out )
{
    out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

int base64_decode(unsigned char *in, unsigned int inlen, unsigned char *out, unsigned int *outlen)
{
    int retcode = 0;
    unsigned char in_buf[4];
    unsigned char out_buf[3];
    int v;
    int i, len;

    *in_buf = (unsigned char) 0;
    *out_buf = (unsigned char) 0;
    int l = 0;
    int j = 0;
    while(l < inlen) {
        for( len = 0, i = 0; i < 4 && l < inlen; i++ ) {
            v = 0;
            while( l < inlen && v == 0 ) {
                v = in[l++];
                if( l < inlen ) {
                    v = ((v < 43 || v > 122) ? 0 : (int) cd64[ v - 43 ]);
                    if( v != 0 ) {
                        v = ((v == (int)'$') ? 0 : v - 61);
                    }
                }
            }
            if( l < inlen ) {
                len++;
                if( v != 0 ) {
                    in_buf[ i ] = (unsigned char) (v - 1);
                }
            }
            else {
                in_buf[i] = (unsigned char) 0;
            }
        }
        if( len > 0 ) {
            decodeblock(in_buf, out_buf);
            for( i = 0; i < len - 1; i++ ) {
                out[j++] = out_buf[i];
            }
        }
    }
    *outlen = j;
    return( retcode );
}

int bin_char2hex(int c) {
    if (c >= 0 && c <= 9) {
        return (c + '0');
    }

    if (c >= 10 && c <= 15) {
        return ('A' + c - 10);
    }

    exit (-1);
}

void bin2hex(unsigned char *in, int inlen, unsigned char *out) {
    int i = 0;
    while (i < inlen) {
        *out++ = bin_char2hex(in[i] >> 4);
        *out++ = bin_char2hex(in[i] & 0x0F);

        i++;
    }
    *out = '\0';

    return;
}

int str_decode_OBJECT_IDENTIFIER(unsigned char *data, size_t data_len, unsigned char *out_buf, size_t out_buf_len, size_t *real_len) {
    unsigned char *p = data;
    unsigned char *o = out_buf;
    int first = 1;
    while (p-data < data_len) {
        if ( (*p & 0x80) == 0 ) { // one byte
            if (first) {
                int l = sprintf(o, "%d.%d.", *p/40, *p % 40);
                o += l;
                p++;

                first = 0;
            } else {
                int l = sprintf(o, "%d.", *p);
                o += l;
                p++;
            }
        } else { // two byte
            size_t v1 = (*p & 0x7F);
            size_t v2 = (*(p+1) & 0x7F);
            int l = sprintf(o, "%ld.", (v1 <<= 7) | v2);
            o += l;
            p += 2;
        }
    }
    // delete last '.'
    o -= 1;
    *o = '\0';
    *real_len = o - out_buf;

    return 0;
}

int main(int argc, char *argv[]) {
    FILE *in;
    FILE *out;

    if (argc < 2) {
        fprintf(stderr, "\tUsage: %s <key_pem_file>\n", argv[0]);
        exit(-1);
    }

    in = fopen(argv[1], "r");
    if (in == NULL) {
        fprintf(stderr, "Error: open read file %s\n", argv[1]);
        exit(-2);
    }

    char buf[8192];
    char *line;
    while (!feof(in)) {
        line = fgets(buf, sizeof(buf), in);
        if (strstr(buf, "BEGIN EC PARAMETERS") != 0) {
            line = fgets(buf, sizeof(buf), in);
            char *ptr = line + strlen(line)-1;
            while (isspace(*ptr)) {
                *ptr = '\0';
                ptr--;
            }
            break;
        }
    };
    fclose(in);

    printf("EC Parameter Base64: %s\n", line);
    unsigned char bin[1024];
    unsigned int outlen;
    base64_decode(line, strlen(line), bin, &outlen);
    if (bin[0] != 0x06) {
        fprintf(stderr, "Wrong EC parameter format 0x%x\n", bin[0]);
        exit(-3);
    }

    unsigned int obj_len = bin[1];
    if (outlen != obj_len + 2) {
        fprintf(stderr, "Wrong EC parameter expected length %d, real data length %d\n", obj_len, outlen-2);
        exit(-3);
    }

    unsigned char hex[1024];
    bin2hex(bin, outlen, hex);
    printf("EC Parameter Hex: %s\n", hex);

    size_t obj_str_len;
    unsigned char obj_str[1024];
    str_decode_OBJECT_IDENTIFIER(bin+2, obj_len, obj_str, sizeof(obj_str), &obj_str_len);
    printf("EC Parameter OBJ ID: %s\n", obj_str);

    return 0;
}
