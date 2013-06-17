#!/usr/bin/env bash

# Usage example:\r\n$./ur1ca.sh \\"http://www.google.com/search?q=bash+scripting\\"\r\nYour ur1 is: http://ur1.ca/17xpn\r\n$
#  ur1ca.sh
#  Shorten a long URL from bash command line,
#  using ur1.ca open source service.
#  ksaver (at identi.ca), Aug 2010.
#  Public Domain Code.
#  No warranty at all.
function _curl()
    /usr/bin/env curl -s -A 'Mozilla' "$@"

function html_txt()
    read string
    echo "$string" |sed -e 's/<[^>]*>//g'

function usage()
    prname=$(basename $0)
    echo -e "\tUsage:  $prname  <long-url>"
    echo -e "\tExamp:  $prname  http://www.google.com/search?q=bash+scripting"

function _main_()
    if [ -z "$1" ]
        usage
        UR1='http://ur1.ca/'
        LONG="$1"
        _curl -s "$UR1" -d"longurl=$LONG" |grep -i 'Your ur1 is' |html_txt
## Run script...
_main_ "$@"
 


