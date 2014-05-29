#!/usr/bin/python2.7
import sys
import os



if __name__ == "__main__":

    print "test"

    for root, dirs, files in os.walk("./"):
        print "Root: " + root
        print  dirs
        #print "Files: " + files
    sys.exit(0)
