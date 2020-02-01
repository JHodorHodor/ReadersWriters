#!/bin/bash

if [ $# != 1 ]; then
	echo Syntax: $0 input_file_path
	exit 1
fi

make || exit 1

echo input file: $1
rm -f reader.out.*

./rw --test < $1 | sort > test.A

./rw < $1
cat reader.out.* | sort > test.B

if diff test.A test.B >/dev/null; then
    echo "CORRECT"
else
    echo "INCORRECT"
fi

echo "Sizes:"
ls -l reader.out.*

rm -f reader.out.* test.A test.B test.agg
