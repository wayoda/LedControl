#!/bin/bash

#the languages the output should generate
OUTPUT_LANGUAGES=("english" "german")

#the old langauge and the strings that should be searched
OLD_LANGAUGE="English"
OUTPUT_LANGUAGE_STRING="OUTPUT_LANGUAGE        = "
HTML_DIR="HTML_OUTPUT            = html"
EXAMPLE_DIR="EXAMPLE_PATH           = examples"

#the base folder for the doxygen output
DOXYGEN_BASE="doc"
ROOT_INDEX_LOCATION=".scripts"

for lang in "${OUTPUT_LANGUAGES[@]}"
do
    #create lang specific doxygen file
    cp Doxyfile Doxyfile.$lang

    #replace the language and add specific subfolder
    sed -i "s/$OUTPUT_LANGUAGE_STRING$OLD_LANGAUGE/$OUTPUT_LANGUAGE_STRING$lang/g" Doxyfile.$lang
    sed -i "s/$HTML_DIR/$HTML_DIR\/$lang/g" Doxyfile.$lang
    sed -i "s/$EXAMPLE_DIR/$EXAMPLE_DIR\/$lang/g" Doxyfile.$lang

    #check if replacement worked as intended
    grep OUTPUT_LANGUAGE Doxyfile.$lang
    grep HTML_OUTPUT Doxyfile.$lang

    #run doxygen
    doxygen Doxyfile.$lang

    #remove lang specific doxygen file
    rm Doxyfile.$lang
done

#create new root index file
rm $DOXYGEN_BASE/html/index.html
cp $ROOT_INDEX_LOCATION/index.html $DOXYGEN_BASE/html/
