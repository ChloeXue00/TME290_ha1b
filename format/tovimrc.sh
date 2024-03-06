#!/bin/bash

comments="\" For https://github.com/rhysd/vim-clang-format"
style="let g:clang_format#style_options = {"

file=$(cat .clang-format)

while read line
do
  if [[ ${line:0:1} == '#' ]]; then
    comments="${comments}\n$(echo "${line}" | tr \# \")"
    continue
  fi
  if [[ ${line} != *":"* ]]; then
    continue
  fi

  key=$(echo ${line} | cut -d: -f1 | xargs)
  value=$(echo ${line} | cut -d: -f2 |  sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')

  if echo "$value" | grep -qE '^-?[0-9]+$'; then
    style="${style}\n  \\ \"${key}\" : ${value},"
  else
    style="${style}\n  \\ \"${key}\" : \"${value}\","
  fi
done <<< "$(echo -e "$file")"

echo -e "${comments}\n${style%?}}"
