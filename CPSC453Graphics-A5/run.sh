#!/bin/bash
# Compile"
newline(){
    cat images/ascii.art
    printf "\n$1"
}

if [[ "${1}" = +("n"|"new") ]]; then
    make
    newline "1.make"
elif [[ "${1}" = +("c"|"clean") ]]; then
    make clean
    newline "2. make clean"
    exit 1;
elif [[ "${1}" = +("r"|"run") ]]; then
    ./Boilerplate.out
    newline "3. Boilerplate.out"
    exit 1
else
    make clean
    make
    newline "make clean => make"
fi
# Run
echo " => Boilerplate.out"
./Boilerplate.out
