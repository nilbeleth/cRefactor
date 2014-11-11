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
# end class colors

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

def do_test(dir, verbose=False):
    if not os.path.isdir(dir):
        print "ERROR: no test named \"%s\"." % dir
        return 1

    output = ""

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
                output += "  cp " + file + " " + source + "\n"

                # aggregate source files
                files = source + " " + files
    if files == "":
        print "ERROR: no files found in test directory \"%s\"" % dir
        return 1

    # run tester
    ret = commands.getstatusoutput("./tester " + args + " " + files)
    output += "  ./tester " + args + " " + files + "\n"
    for line in ret[1].split("\n"):
        output += "  " + line + "\n"

    # diff it with reference and then delete it
    diffs = 0
    errmsg = dir + ":\n"
    for file in files.split():
        ret = commands.getstatusoutput('diff ' + file + " " + file + ".ref")
        if ret[0] != 0:
            diffs = diffs + 1
            errmsg += 'diff ' + file + " " + file + ".ref\n"
            errmsg += ret[1] + "\n\n"
        os.remove(file)

    if diffs == 0:
        print_success(dir)
    else:
        print_failed(dir)
        with open("results",'a') as file:
            file.write(errmsg)
            file.close()
    if verbose:
        print output
# end function do_test()

def run_all_tests():
    # run test for each subdirectory
    tests = [x[0] for x in os.walk(".")]

    for test in sorted(tests[1:]):
        do_test(test)
# end function run_all_tests()

if __name__ == "__main__":
    try:
        os.remove('results')
    except OSError as ex:
        pass    # ignore if no 'results'

    # compilation
    ret = commands.getstatusoutput('make')
    if ret[0] != 0:
        print_failed("Compilation")
        sys.exit(1)
    else:
        print_success("Compilation")


    #run_all_tests()
    if len(sys.argv) == 1:
        run_all_tests()
    else:
        for test in sys.argv[1:]:
            do_test(test, True)

    # print some statistics

    sys.exit(0)
