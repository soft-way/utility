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

void hexdump(void *buf, size_t buflen) {
    /*
    OFFSET   : 00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
    00000000h: 53 55 42 44 49 52 53 20  3d 20 73 72 63 20 64 6f |SUBDIRS...src.do|
    00000010h: 63 73 0d 0a 44 49 53 54  5f 53 55 42 44 49 52 53 |cs..DIST.SUBDIRS|
    00000020h: 20 3d 20 73 72 63 20 64  6f 63 73 0d 0a 0d 0a 64 |...src.docs....d|
    00000030h: 69 73 74 63 6c 65 61 6e  3a 0d 0a 09 2e 2f 63 6c |istclean......cl|
    00000040h: 65 61 6e 75 70 2e 73 68  0d 0a                   |eanup.sh..      |
    */

    int i, j, offset = 0;
    char line[68];
    printf("OFFSET   : 00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F\n");
    for (i=0; i < buflen; i += 16) {
        for (j=0; j < 16; j++) {
            int s;
            if (i+j < buflen) {
                unsigned char b = ((unsigned char*)buf)[i+j];
                if (j <= 7) {
                    s = 0;

                } else {
                    s = 1;
                }
                sprintf(line + j*3 + s, "%02hhx ", b);
                line[50+j] = (isalnum(b) ? b : '.');
            } else {
                sprintf(line + j*3 + s, "   ");
                line[50+j] = ' ';
            }
        }
        line[23] = ' ';
        line[24] = ' ';
        line[48] = ' ';
        line[49] = '|';
        line[66] = '|';
        line[67] = '\0';
        printf("%08xh: %s\n", offset, line);
        offset += 16;
    }
}

int
ber_decode_SEQUENCE( unsigned char  * seq,
                     unsigned char ** data,
                     unsigned long * data_len,
                     unsigned long * field_len )
{
   unsigned long  len, length_octets;


   if (!seq){
      printf("Input is null\n");
      return 1;
   }
   if (seq[0] != 0x30){
      printf("Input is not sequence\n");
      return 2;
   }
   // short form lengths are easy
   //
   if ((seq[1] & 0x80) == 0) {
      len = seq[1] & 0x7F;

      *data = &seq[2];
      *data_len  = len;
      *field_len = 1 + (1) + len;
      return 0;
   }

   length_octets = seq[1] & 0x7F;

   if (length_octets == 1) {
      len = seq[2];

      *data = &seq[3];
      *data_len  = len;
      *field_len = 1 + (1 + 1) + len;
      return 0;
   }

   if (length_octets == 2) {
      len = seq[2];
      len = len << 8;
      len |= seq[3];

      *data = &seq[4];
      *data_len  = len;
      *field_len = 1 + (1 + 2) + len;
      return 0;
   }

   if (length_octets == 3) {
      len = seq[2];
      len = len << 8;
      len |= seq[3];
      len = len << 8;
      len |= seq[4];

      *data = &seq[5];
      *data_len  = len;
      *field_len = 1 + (1 + 3) + len;
      return 0;
   }

   // > 3 length octets implies a length > 16MB
   //
   return 3;
}

int
ber_decode_OBJ_IDENTIFIER( unsigned char  * o,
                     unsigned char ** data,
                     unsigned long * data_len,
                     unsigned long * field_len )
{
   unsigned long  len, length_o;


   if (!o){
      printf("Input is null\n");
      return 1;
   }
   if (o[0] != 0x06){
      printf("Input is not obj identifier\n");
      return 2;
   }
   // short form lengths are easy
   //
   len = o[1] & 0x7F;
   *data = &o[2];
   *data_len  = len;
   *field_len = 1 + (1) + len;

   return 0;
}

int
ber_decode_INTEGER( unsigned char   * ber_int,
                    unsigned char  ** data,
                    unsigned long  * data_len,
                    unsigned long  * field_len )
{
   unsigned long  len, length_octets;

   if (!ber_int){
      printf("Input is null\n");
      return 1;
   }
   if (ber_int[0] != 0x02){
      printf("Input is not INTEGER\n");
      return 2;
   }

   // ber encoded integers are alway signed. So it may be that the very first
   // byte is just a padding 0x00 value because the following byte has the msb
   // set and without the padding the value would indicate a negative value.
   // However, opencryptoki always stores big integers 'unsigned' meaning
   // even when the msb is set, there is no preceding 0x00. Even more some
   // tests may fail e.g. the size in bytes of a modulo big integer should be
   // modulo bits / 8 which is not true with preceeding 0x00 byte.

   // short form lengths are easy
   //
   if ((ber_int[1] & 0x80) == 0) {
      len = ber_int[1] & 0x7F;
      *data      = &ber_int[2];
      *data_len  = len;
      /*
      if (ber_int[2] == 0x00) {
	      *data      = &ber_int[3];
	      *data_len  = len - 1;
      }
      */
      *field_len = 1 + 1 + len;
      return 0;
   }

   length_octets = ber_int[1] & 0x7F;

   if (length_octets == 1) {
      len = ber_int[2];
      *data      = &ber_int[3];
      *data_len  = len;
      if (ber_int[3] == 0x00) {
	      *data      = &ber_int[4];
	      *data_len  = len - 1;
      }
      *field_len = 1 + (1 + 1) + len;
      return 0;
   }

   if (length_octets == 2) {
      len = ber_int[2];
      len = len << 8;
      len |= ber_int[3];
      *data      = &ber_int[4];
      *data_len  = len;
      if (ber_int[4] == 0x00) {
	      *data      = &ber_int[5];
	      *data_len  = len - 1;
      }
      *field_len = 1 + (1 + 2) + len;
      return 0;
   }

   if (length_octets == 3) {
      len = ber_int[2];
      len = len << 8;
      len |= ber_int[3];
      len = len << 8;
      len |= ber_int[4];
      *data      = &ber_int[5];
      *data_len  = len;
      if (ber_int[5] == 0x00) {
	      *data      = &ber_int[6];
	      *data_len  = len - 1;
      }
      *field_len = 1 + (1 + 3) + len;
      return 0;
   }

   // > 3 length octets implies a length > 16MB which isn't possible for
   // the coprocessor
   //
   return 1;
}

int
ber_decode_OCTET_STRING( unsigned char  * str,
                         unsigned char ** data,
                         unsigned long * data_len,
                         unsigned long * field_len )
{
   unsigned long  len, length_octets;

   // I only support decoding primitive OCTET STRINGS
   //

   if (!str){
      printf("Input is null\n");
      return 1;
   }
   if (str[0] != 0x04){
      return 1;
   }
   // short form lengths are easy
   //
   if ((str[1] & 0x80) == 0) {
      len = str[1] & 0x7F;

      *data = &str[2];
      *data_len  = len;
      *field_len = 1 + (1) + len;
      return 0;
   }

   length_octets = str[1] & 0x7F;

   if (length_octets == 1) {
      len = str[2];

      *data = &str[3];
      *data_len  = len;
      *field_len = 1 + (1 + 1) + len;
      return 0;
   }

   if (length_octets == 2) {
      len = str[2];
      len = len << 8;
      len |= str[3];

      *data = &str[4];
      *data_len  = len;
      *field_len = 1 + (1 + 2) + len;
      return 0;
   }

   if (length_octets == 3) {
      len = str[2];
      len = len << 8;
      len |= str[3];
      len = len << 8;
      len |= str[4];

      *data = &str[5];
      *data_len  = len;
      *field_len = 1 + (1 + 3) + len;
      return 0;
   }

   // > 3 length octets implies a length > 16MB
   //
   return 1;
}

int str_OBJ_ID(unsigned char *data, size_t data_len, unsigned char *out_buf) {
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

    return 0;
}

void decode_ec_params(unsigned char *base64, unsigned long len) {
    printf("EC Parameter Base64: %s\n", base64);
    unsigned char bin[1024];
    unsigned int outlen;
    base64_decode(base64, len, bin, &outlen);
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
    printf("EC Parameter:\n");
    hexdump(bin, outlen);

    size_t obj_str_len;
    unsigned char obj_str[1024];
    str_OBJ_ID(bin+2, obj_len, obj_str);
    printf("EC Parameter OBJ ID: %s\n\n", obj_str);
}

void decode_key(unsigned char *base64, unsigned long len) {
    printf("Key Base64: %s\n", base64);
    unsigned char ber[1024];
    unsigned int outlen;
    base64_decode(base64, len, ber, &outlen);
    printf("\nKey binary(%d):\n", outlen);
    hexdump(ber, outlen);

    unsigned char *ber_ptr;
    unsigned char *key_ber;
    unsigned long key_ber_len;
    unsigned long field_len;
    ber_ptr = ber;
    int rc = ber_decode_SEQUENCE(ber_ptr, &key_ber, &key_ber_len, &field_len);
    if (rc != 0) {
        printf("Error: decode key ber\n");
        exit(1);
    } else {
        printf("Key ber len: %d, field len: %d\n", key_ber_len, field_len);
    }
    
    ber_ptr = key_ber;
    unsigned char *version_ptr;
    unsigned long version_len;
    printf("Version(%d):\n", key_ber_len);
    hexdump(key_ber, key_ber_len);
    rc = ber_decode_INTEGER(ber_ptr, &version_ptr, &version_len, &field_len);
    if (rc != 0) {
        printf("Error: decode version\n");
        exit(1);
    } else {
        printf("Version(%d): \n", version_len);
        hexdump(version_ptr, version_len);
        printf("Version field len: %d\n", field_len);
    }
    
    ber_ptr += field_len;
    unsigned char *objs_ber;
    unsigned long objs_ber_len;
    rc = ber_decode_SEQUENCE(ber_ptr, &objs_ber, &objs_ber_len, &field_len);
    if (rc != 0) {
        printf("Error: decode objs ber\n");
        exit(1);
    } 
    printf("Objects: \n");
    hexdump(objs_ber, objs_ber_len);
    printf("Objs field len: %d\n", objs_ber_len);
    
    unsigned char *obj_ber;
    unsigned long obj_ber_len;
    ber_ptr = objs_ber;
    rc = ber_decode_OBJ_IDENTIFIER(ber_ptr, &obj_ber, &obj_ber_len, &field_len);
    if (rc != 0) {
        printf("Error: decode obj1 ber\n");
        exit(2);
    }
    printf("Obj1 field len: %d\n", field_len);
    unsigned char obj_str[128];
    str_OBJ_ID(obj_ber, obj_ber_len, obj_str);
    printf("OBJ1 ID: %s\n\n", obj_str);
    
    ber_ptr += field_len;
    rc = ber_decode_OBJ_IDENTIFIER(ber_ptr, &obj_ber, &obj_ber_len, &field_len);
    if (rc != 0) {
        printf("Error: decode obj2 ber\n");
        exit(3);
    }
    printf("Obj2 field len: %d\n", field_len);
    str_OBJ_ID(obj_ber, obj_ber_len, obj_str);
    printf("OBJ2 ID: %s\n\n", obj_str);
    
    unsigned char *priv_key_ber;
    unsigned long priv_key_len;
    ber_ptr += field_len;
    rc = ber_decode_OCTET_STRING(ber_ptr, &priv_key_ber, &priv_key_len, &field_len);
    if (rc != 0) {
        printf("Error: decode private key ber\n");
        exit(4);
    }
    printf("Private key(%d):\n", priv_key_len);
    hexdump(priv_key_ber, priv_key_len);
}

void decode_pubkey(unsigned char *base64, unsigned long len) {
    printf("Key Base64: %s\n", base64);
    unsigned char ber[1024];
    unsigned int outlen;
    base64_decode(base64, len, ber, &outlen);
    printf("\nKey binary(%d):\n", outlen);
    hexdump(ber, outlen);

    unsigned char *ber_ptr;
    unsigned char *key_ber;
    unsigned long key_ber_len;
    unsigned long field_len;
    ber_ptr = ber;
    int rc = ber_decode_SEQUENCE(ber_ptr, &key_ber, &key_ber_len, &field_len);
    if (rc != 0) {
        printf("Error: decode key ber\n");
        exit(1);
    } else {
        printf("Key ber len: %d, field len: %d\n", key_ber_len, field_len);
    }
    
    ber_ptr = key_ber;
    unsigned char *version_ptr;
    unsigned long version_len;
    printf("Version(%d):\n", key_ber_len);
    hexdump(key_ber, key_ber_len);
    rc = ber_decode_INTEGER(ber_ptr, &version_ptr, &version_len, &field_len);
    if (rc != 0) {
        printf("Error: decode version\n");
        exit(1);
    } else {
        printf("Version(%d): \n", version_len);
        hexdump(version_ptr, version_len);
        printf("Version field len: %d\n", field_len);
    }
    
    ber_ptr += field_len;
    unsigned char *objs_ber;
    unsigned long objs_ber_len;
    rc = ber_decode_SEQUENCE(ber_ptr, &objs_ber, &objs_ber_len, &field_len);
    if (rc != 0) {
        printf("Error: decode objs ber\n");
        exit(1);
    } 
    printf("Objects: \n");
    hexdump(objs_ber, objs_ber_len);
    printf("Objs field len: %d\n", objs_ber_len);
    
    unsigned char *obj_ber;
    unsigned long obj_ber_len;
    ber_ptr = objs_ber;
    rc = ber_decode_OBJ_IDENTIFIER(ber_ptr, &obj_ber, &obj_ber_len, &field_len);
    if (rc != 0) {
        printf("Error: decode obj1 ber\n");
        exit(2);
    }
    printf("Obj1 field len: %d\n", field_len);
    unsigned char obj_str[128];
    str_OBJ_ID(obj_ber, obj_ber_len, obj_str);
    printf("OBJ1 ID: %s\n\n", obj_str);
    
    ber_ptr += field_len;
    rc = ber_decode_OBJ_IDENTIFIER(ber_ptr, &obj_ber, &obj_ber_len, &field_len);
    if (rc != 0) {
        printf("Error: decode obj2 ber\n");
        exit(3);
    }
    printf("Obj2 field len: %d\n", field_len);
    str_OBJ_ID(obj_ber, obj_ber_len, obj_str);
    printf("OBJ2 ID: %s\n\n", obj_str);
    
    unsigned char *priv_key_ber;
    unsigned long priv_key_len;
    ber_ptr += field_len;
    rc = ber_decode_OCTET_STRING(ber_ptr, &priv_key_ber, &priv_key_len, &field_len);
    if (rc != 0) {
        printf("Error: decode private key ber\n");
        exit(4);
    }
    printf("Private key(%d):\n", priv_key_len);
    hexdump(priv_key_ber, priv_key_len);
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

    unsigned char base64[8192];
    unsigned char ec_params_base64[8192];
    unsigned char buf[8192];
    unsigned char *line;
    int in_key_block = 0;
    int in_ec_parameter_block = 0;
    int i = 0;
    int j = 0;
    while (!feof(in)) {
        line = fgets(buf, sizeof(buf), in);
        if ((strstr(buf, "BEGIN") != 0) && (strstr(buf, "PRIVATE KEY") != 0)) {
            in_key_block = 1;
            continue;
        }
        if ((strstr(buf, "END") != 0) && (strstr(buf, "PRIVATE KEY") != 0)) {
            base64[i] = '\0';
            decode_key(base64, i);
        }
        
        if ((strstr(buf, "BEGIN") != 0) && (strstr(buf, "PUBLIC KEY") != 0)) {
            in_key_block = 1;
            continue;
        }
        if ((strstr(buf, "END") != 0) && (strstr(buf, "PUBLIC KEY") != 0)) {
            base64[i] = '\0';
            printf("Public key:\n");
            decode_pubkey(base64, i);
        }

        if ((strstr(buf, "BEGIN") != 0) && (strstr(buf, "EC PARAMETERS") != 0)) {
            //printf("In EC Params block\n");
            in_ec_parameter_block = 1;
            continue;
        }
        if ((strstr(buf, "END") != 0) && (strstr(buf, "EC PARAMETERS") != 0)) {
            ec_params_base64[j] = '\0';
            decode_ec_params(ec_params_base64, j);
        }
        if (in_key_block) {
            char *buf_ptr = buf;
            while (*buf_ptr != '\0') {
                if(isspace(*buf_ptr)) {
                    buf_ptr++;
                    continue;
                }
                base64[i++] = *buf_ptr;
                buf_ptr++;
            }
        }
        if (in_ec_parameter_block) {
            char *buf_ptr = buf;
            while (*buf_ptr != '\0') {
                if(isspace(*buf_ptr)) {
                    buf_ptr++;
                    continue;
                }
                ec_params_base64[j++] = *buf_ptr;
                buf_ptr++;
            }
        }
    };
    fclose(in);

    return 0;
}
