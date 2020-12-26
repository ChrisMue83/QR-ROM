#!/bin/bash

echo "Disclaimer:"
echo " - This program is for experimental use only"
echo " - Don't use it as a single backup for files"
echo ""

make > /dev/null

if [ $1 == 'write' ]
then

    echo generating QR-codes of file: $2

    mkdir _temp_
    mkdir $3

    base64 -w 0 $2 > _temp_/ConvertedToBase64
    sha256sum $2 > $3/xxx_sha256_xxx

    split -b 1500 -a 3 --additional-suffix .split _temp_/ConvertedToBase64 _temp_/

    FILES=_temp_/*.split

    counter=0
    for f in $FILES
    do
        counter=$((counter+1))
        echo $counter > _temp_/cat.txt
        qrencode -o $f.png  "$(cat _temp_/cat.txt $f)"
    done

    montage -label %f -mode concatenate -tile 3x4 _temp_/*.png -resize 411x411 $3/outfile.png

    echo ""
    echo "Checksum:"
    cat $3/xxx_sha256_xxx

    rm -Rf _temp_
fi


if [ $1 == 'read' ]
then

    echo converting back QR-codes to $3/binary
    echo ""

    mkdir _temp_
    mkdir $3

    FILES=$2/*.png

    for f in $FILES
    do
        zbarimg $f > $f.out_zbarimg
    done

    sorted_out_zbarimg=$(find $2/ -name '*.out_zbarimg' -type f -print0 | sort -z | xargs -r0)
    cat $sorted_out_zbarimg > _temp_/concatenated_out_zbarimg

    ./extract _temp_/concatenated_out_zbarimg > _temp_/ConvertedToBase64
    base64 -d _temp_/ConvertedToBase64 > $3/binary

    echo ""
    echo "Checksum:"
    sha256sum $3/binary

    rm -rf _temp_
fi
