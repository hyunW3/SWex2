#!/bin/bash
cat ex.txt | sed 's/[0-9]\{4\}/&%/'

