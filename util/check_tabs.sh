#! /bin/bash

OUTPUT=$(grep -nRP '\t' --include=\*.{cpp,hpp})

if [[ $OUTPUT ]]; then
    echo "Error: Tab characters found!"
    echo $OUTPUT
    exit 1
fi

