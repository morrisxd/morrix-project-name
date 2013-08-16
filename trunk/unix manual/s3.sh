#!/bin/bash
echo "$1"
curl "http://tinyurl.com/create.php?source=indexpage&url=$1" | grep "copy('"
