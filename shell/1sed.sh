#!/bin/bash
echo $PATH | sed 's/:/\n/g' | sed 's/$/>>>/g' | sed 's/^/<<</g'

