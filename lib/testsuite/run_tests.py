#!/usr/bin/python2.7
import sys
import os
import commands
import re
import shutil



class colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'


def print_success(msg):
    space = ""
    for i in range(len(msg), 74):
        space = space + " "
    print(msg + "..." + space + "[ " + colors.OKGREEN + "ok" + colors.ENDC + " ]")
# end function print_success


def print_failed(msg):
    space = ""
    for i in range(len(msg), 74):
        space = space + " "
    print(msg + "..." + space + "[" + colors.FAIL + "fail" + colors.ENDC + "]")
# end function print_failed


def do_test(dir):
    with open(os.path.join(dir,"args"),'r') as argfile:
        args = argfile.read().replace('\n','')
        argfile.close()
    files = ""
    for root, dirs, fnames in os.walk(dir):
        for f in fnames:
            if(f.endswith(".orig")):
                # copy source
                file = os.path.join(dir,f)
                (source,sep,suffix) = file.rpartition('.')
                shutil.copy(file, source)

                # aggregate source files
                files = source + " " + files
    if files == "":
        return
    # run tester
    commands.getstatusoutput("./tester " + args + " " + files)

    # diff it with reference and then delete it
    diffs = 0
    for file in files.split():
        ret = commands.getstatusoutput('diff ' + file + " " + file + ".ref")
        if ret[0] != 0:
            diffs = diffs + 1
        os.remove(file)

    if diffs == 0:
        print_success(dir)
    else:
        print_failed(dir)
# end function do_test()



if __name__ == "__main__":
    # compilation
    ret = commands.getstatusoutput('make')
    if ret[0] != 0:
        print_failed("Compilation")
        sys.exit(1)
    else:
        print_success("Compilation")


    # run test for each subdirectory
    tests = [x[0] for x in os.walk(".")]

    for test in tests[1:]:
        do_test(test)

    # print some statistics

    sys.exit(0)
