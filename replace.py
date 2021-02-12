#!/usr/bin/env python

import re
from sys import stdin, stdout

for line in stdin.readlines():
  line = line[:-1] # Trim the newline
  stripped = re.sub(r'[\'"].*[\'"]', '', line) # Ignore strings
  stripped = re.sub(r'/\*.*\*/', '', stripped) # Ignore multi-line comments
  m = re.match(r'.*?//(.*)', stripped) # Only match actual C++-style comments 
  if m:
    offset = len(m.group(1)) + 2
    content = line[:offset*-1] # Get the original line sans comment
    print '%s/* %s */' % (content, m.group(1)) # Combine the two with C-style comments
  else:
    print line
