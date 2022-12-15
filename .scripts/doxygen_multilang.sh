#!/bin/bash

#get the number of cores to limit the parallel builds
MAX_CORES="$(nproc)"

#the languages the output should generate
OUTPUT_LANGUAGES=("english" "german")

#the old langauge and the strings that should be searched
OLD_LANGAUGE="English"
OUTPUT_LANGUAGE_STRING="OUTPUT_LANGUAGE        = "
HTML_DIR="HTML_OUTPUT            = html"
EXAMPLE_DIR="EXAMPLE_PATH           = examples"
LATEX_DIR="LATEX_OUTPUT           = latex"

#the base folder for the doxygen output
DOXYGEN_BASE="doc"
ROOT_INDEX_LOCATION=".scripts"

build_lang(){
    local lang=$1

    #create lang specific doxygen file
    cp Doxyfile Doxyfile.$lang

    #replace the language and add specific subfolder
    sed -i "s/$OUTPUT_LANGUAGE_STRING$OLD_LANGAUGE/$OUTPUT_LANGUAGE_STRING$lang/g" Doxyfile.$lang
    sed -i "s/$HTML_DIR/$HTML_DIR\/$lang/g" Doxyfile.$lang
    sed -i "s/$LATEX_DIR/$LATEX_DIR\/$lang/g" Doxyfile.$lang
    sed -i "s/$EXAMPLE_DIR/$EXAMPLE_DIR\/$lang/g" Doxyfile.$lang
    sed -i "s/doxywarn.txt/doxywarn.$lang.txt/g" Doxyfile.$lang

    #check if replacement worked as intended
    grep OUTPUT_LANGUAGE Doxyfile.$lang
    grep HTML_OUTPUT Doxyfile.$lang

    #run doxygen
    doxygen Doxyfile.$lang
    if [ "$?" -ne 0 ];
    then
        rm Doxyfile.$lang
        return 1
    fi

    #build pdf file
    cd doc/latex/$lang/
    latexmk -f -pdf refman.tex
    cd ../../../

    #copy and rename reference manual
    mv doc/latex/$lang/refman.pdf refman_$lang.pdf

    #remove lang specific doxygen file
    rm Doxyfile.$lang
    
    return 0

}

#build as much language in parallel as possible
for lang in "${OUTPUT_LANGUAGES[@]}"
do
    #this calls a new subroutine for that language in the array
    build_lang $lang
    if [ "$?" -ne 0 ];
    then
        exit 1
    fi
done

#create new root index file
rm -f $DOXYGEN_BASE/html/index.html
cp $ROOT_INDEX_LOCATION/index.html $DOXYGEN_BASE/html/
