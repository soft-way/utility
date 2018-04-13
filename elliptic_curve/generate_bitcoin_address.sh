#!/bin/sh

#
# get bitcoin address from pub key hash hex file
#
if [ $# -lt 1 ]; then
    echo "Usage: $0 <number>"
    exit -1
fi

num=$1
c=1
while [[ $num > 0 ]]; do
    priv_key_file=/tmp/ec_key_secp256k1_$num.pem
    openssl ecparam -genkey -out $priv_key_file  -name secp256k1
    priv_key_hex=`openssl ec -in $priv_key_file -pubout -text 2>/dev/null | awk ' BEGIN {	in_block=0; } { \
        if (index($1, "priv") !=0) { \
            in_block=1 \
        } else if (index($1, "pub") != 0) { \
            in_block=0 \
        } else if (in_block == 1) { \
            print $1 \
        } \
    } ' | tr -d ':' | xxd -p -r | xxd -p`
    pub_key_hex=`openssl ec -in $priv_key_file -pubout -text 2>/dev/null | awk ' BEGIN {	in_block=0; } { \
        if (index($1, "pub") !=0) { \
            in_block=1 \
        } else if (index($1, "ASN1") != 0) { \
            in_block=0 \
        } else if (in_block == 1) { \
            print $1 \
        } \
    } ' | tr -d ':' | xxd -p -r | xxd -p`
    echo ""
    echo "Generate bitcoin address $c:"
    echo "  Private key(hex)-${#priv_key_hex}: $priv_key_hex"
    echo "  Public key(hex)-${#pub_key_hex}: $pub_key_hex"

    pub_key_256sum=`echo $pub_key_hex | xxd -p -r | sha256sum | awk ' { print $1 } '`
    echo "  Public key(sha256(p))-${#pub_key_256sum} : $pub_key_256sum"
    
    pub_key_rmd160_256sum=`echo  $pub_key_256sum | xxd -p -r | openssl rmd160 | awk ' { print $2 } '`
    echo "  Public key(rmd160(sha256(p)))-${#pub_key_rmd160_256sum}: $pub_key_rmd160_256sum"
    
    checksum_round1=`echo "00$pub_key_rmd160_256sum" | xxd -p -r | sha256sum | awk ' { print $1 } '`
    checksum_round2=`echo "$checksum_round1" | xxd -p -r | sha256sum | awk ' { print $1 } '`
    echo "  Public key(checksum)-${#checksum_round2}: $checksum_round2"

    checksum_4_bytes=`echo $checksum_round2 | cut -c1-8`
    echo "  Public key(checksum prefix 4 bytes)-${#checksum_4_bytes}: $checksum_4_bytes"

    addr_hex="00${pub_key_rmd160_256sum}${checksum_4_bytes}"
    echo "  Bitcoin address(Hex)-${#addr_hex}: $addr_hex"
    
    address=`echo $addr_hex | xxd -p -r | base58`
    echo "  Bitcoin address-${#address}: $address"
        
    rm $priv_key_file
    
    ((num-=1))
    ((c+=1))
done

exit 0
