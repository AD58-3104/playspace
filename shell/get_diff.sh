#!/bin/bash
find_diff(){
 num=`diff $1 $2 | wc | awk '{print $1}'`
}

find_diff $1 $2
