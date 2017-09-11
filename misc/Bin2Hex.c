#include <stdio.h>
#include <stdlib.h>

int isspace(int c) {
    if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
        return 1;
    }

    return 0;
}

int ishexdigit(int c) {
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
        return 1;
    }

    return 0;
}

int bin_char2hex(int c) {
    if (c >= 0 && c <= 9) {
        return (c + '0');
    }

    if (c >= 10 && c <= 15) {
        return ('A' + c - 10);
    }

    fprintf(stderr, "Error value: %d\n", c);
    exit (-1);
}

int bin2hex_format(unsigned char *in, int in_len, unsigned char *out, int *out_len) {
    if (in == NULL  || out == NULL || out_len == NULL) {
        return 0;
    }

    int i=0;
    unsigned char *p = out;
    while (i < in_len) {
        if (i != 0) {
            if (i % 4 == 0) { // add one space for each 4 bytes
                *p++ = ' ';
            }
            if (i % 16 == 0) { // add more one space for each 8 bytes
                *p++ = ' ';
            }
            if (i % 32 == 0 && i != 0) { // add line feed for each 32 bytes
                *p++ = '\n';
            }
        }
        *p++ = bin_char2hex(in[i] >> 4);
        *p++ = bin_char2hex(in[i] & 0x0F);
        i++;
    }
    *p++ = '\n';
    *p = '\0';
    *out_len = p-out;

    return *out_len;
}

int bin2hex(unsigned char *in, int in_len, unsigned char *out, int *out_len) {
    int i = 0;
    while (i < in_len) {
        *out++ = bin_char2hex(in[i] >> 4);
        *out++ = bin_char2hex(in[i] & 0x0F);

        i++;
    }

    *out_len = in_len * 2;
    return *out_len;
}

int main(int argc, char *argv[]) {
    FILE *in;
    FILE *out;

    if (argc < 3) {
        fprintf(stderr, "\tUsage: %s <src_file> <dest_file>\n\n", argv[0]);
        exit(-1);
    }

    in = fopen(argv[1], "r");
    if (in == NULL) {
        fprintf(stderr, "Error: open read file %s\n", argv[1]);
        exit(-1);
    }

    out = fopen(argv[2], "wb");
    if (out == NULL) {
        fprintf(stderr, "Error: open write file %s\n", argv[2]);
        exit(-1);
    }

    int total_out = 0;
    int read_len = 0;
    char buf[8192];
    char hex_buf[8192];
    while (!feof(in)) {
        read_len = fread(buf, 1, sizeof(buf), in);
        int hex_len = 0;
        int rc = bin2hex_format(buf, read_len, hex_buf, &hex_len);
        if (rc < 0) {
            fprintf(stderr, "Error found %s\n", buf);
            break;
        } else if (rc == 0) {
            continue;
        }

        fwrite(hex_buf, 1, hex_len, out);
        total_out += hex_len;
    };

    fclose(in);
    fclose(out);

    fprintf(stdout, "Binary to hexadecimal, total binary byte(s): %d\n", read_len);

    return 0;
}
