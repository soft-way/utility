#include <stdio.h>
#include <stdlib.h>

int hex_char2bin(int c) {
    if (c >= '0' && c <= '9') {
        return (c - '0');
    }

    if (c >= 'A' && c <= 'F') {
        return (c - 'A' + 10);
    }

    if (c >= 'a' && c <= 'f') {
        return (c - 'a' + 10);
    }
    return 0;
}

int hex2bin(unsigned char *in, int in_len, unsigned char *out, int *out_len) {
    unsigned char buf[8192];
    // remove space char
    int i = 0;
    int hex_len = 0;
    while (i < in_len) {
        if (isspace(in[i])) {
            i++;
            continue;
        }
        if (!isxdigit(in[i])) {
            fprintf(stderr, "Non-hexdecimal char %c in file\n", in[i]);
            return -1;
        }

        if (hex_len >= sizeof(buf)) {
            fprintf(stderr, "Buffer size %d is not enough\n", sizeof(buf));
            return -1;
        }
        buf[hex_len++] = in[i++];
    }
    if (hex_len % 2 != 0) {
        fprintf(stderr, "Hexdecimal length %d is not even\n", hex_len);
        return -1;
    }

    if (hex_len == 0) {
        return 0;
    }

    i = 0;
    while (i < hex_len) {
        *out++ = hex_char2bin(buf[i+1]) + 16 * hex_char2bin(buf[i]);
        i += 2;
    }
    *out_len = hex_len / 2;

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
    char buf[8192];
    char bin_buf[8192];
    while (!feof(in)) {
        int read_len = fread(buf, 1, sizeof(buf), in);
        int bin_len = 0;
        int rc = hex2bin(buf, read_len, bin_buf, &bin_len);
        if (rc < 0) {
            fprintf(stderr, "Error found:\n%s\n", buf);
            break;
        } else if (rc == 0) {
            continue;
        }

        fwrite(bin_buf, 1, bin_len, out);
        total_out += bin_len;
    };

    fclose(in);
    fclose(out);

    fprintf(stdout, "Hexadecimal to binary, total binary byte(s): %d\n", total_out);

    return 0;
}
