#!/usr/bin/env python3

import os
import re
import sys

sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
import wpi

# Files and directories which should be excluded from processing
regexExclude = re.compile(wpi.regexExclude())

# Handle running in either the root or styleguide directories
configPath = ""
if os.getcwd().rpartition(os.sep)[2] == "styleguide":
    configPath = ".."
else:
    configPath = "."

# Recursively create list of files in given directory
files = [os.path.join(dp, f) for dp, dn, fn in
         os.walk(os.path.expanduser(configPath)) for f in fn]

# Apply regex filters to list
files = [f for f in files if not regexExclude.search(f)]

if not files:
    print("Error: no files found to format", file=sys.stderr)
    sys.exit(1)

for name in files:
    # List names of files as they are processed if verbose flag was given
    if len(sys.argv) > 1 and sys.argv[1] == "-v":
        print("Processing", name,)

    newlines = 0

    # Remove all but one EOF newline, or append one if necessary
    with open(name, "r+") as file:
        # Get file size
        file.seek(0, os.SEEK_END)
        size = file.tell()

        # Seek to last character in file
        file.seek(size - 1)

        # While last character is a newline
        while file.read(1) == "\n":
            newlines = newlines + 1

            # Seek to character before newline
            file.seek(size - 1 - newlines)

        if newlines < 1:
            # Append newline to end of file
            file.seek(size)
            file.write("\n")
        elif newlines > 1:
            # Truncate all but one newline
            file.truncate(size - (newlines - 1))
