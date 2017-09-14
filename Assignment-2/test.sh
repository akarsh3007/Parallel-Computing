#!/bin/sh

TEST1=$(./main 1 0 10 10000 1 2> /dev/null)
if ./approx $TEST1 50;
then
    echo oktest1
else
    echo notok "./main 1 0 10 10000 1" should give roughly "50"
    exit 1
fi

TEST2=$(./main 2 0 10 10000 1 2> /dev/null)
if ./approx $TEST2 333.333;
then
    echo oktest2
else
    echo notok "./main 2 0 10 10000 1" should give roughly "333.333"
    exit 1
fi
