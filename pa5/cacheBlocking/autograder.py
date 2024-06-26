#!/usr/bin/python3

import re
import os
import datetime
import random
import numpy
import subprocess

def generate_trace (
    n=1,
    path="./",
    dut='../matMul/matMul',
    trace_path="tests/trace_matMul_{}x{}.txt"
):

    with open(trace_path.format(n,n), "w") as infile:
        trace = subprocess.run(
            ['valgrind', '--tool=lackey', '--basic-counts=no', '--trace-mem=yes', '--log-fd=1',
                dut,
                '../matMul/tests/matrix_a_{}x{}.txt'.format(n,n),
                '../matMul/tests/matrix_b_{}x{}.txt'.format(n,n)],
            cwd=path,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            encoding='ASCII',
            timeout=datetime.timedelta(seconds=8).total_seconds(),
        )

        with open(path+".marker", "r") as marker_file:
            start = int(marker_file.readline(),16)
            end = int(marker_file.readline(),16)

        is_relevant_region = False
        for line in trace.stdout.splitlines():
            if line[1]=='L' or line[1]=='S' or line[1]=='M':
                addr = int(re.split(' |,',line)[2],16)
                if addr==start:
                    is_relevant_region = True
                elif addr==end:
                    is_relevant_region = False
                elif is_relevant_region and addr < 0xffff_ffff:
                    infile.write(line+'\n')
                else:
                    # print(line)
                    pass
            elif not line[0]=='I':
                # print(line)
                pass

def answers_from_csim ( test_name ):

    with open("answers/answer_{}.txt".format(test_name), "w") as outfile:
        csim = subprocess.run(
            ['../csim-ref', '-s', '2', '-E', '4', '-b', '4', '-l', '1',
            '-t', "tests/{}.txt".format(test_name)],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            encoding='ASCII',
            timeout=datetime.timedelta(seconds=8).total_seconds(),
        )
        outfile.write(csim.stdout)

def generate_test_suite():

    subprocess.run( ['make', '-B', '-C', '../matMul'], cwd='./', check=True, )
    os.makedirs("tests", exist_ok=True)
    os.makedirs("answers", exist_ok=True)

    for i in range(2,11):
        generate_trace ( i )
        answers_from_csim("trace_matMul_{}x{}".format( i, i ));

def test_matMul ( n, path="./", verbose=False ):

    try:
        with open("{}../matMul/answers/matrix_c_{}x{}.txt".format(path,n,n), "r") as outfile:
            answer = []
            for line in outfile.read().split('\n'):
                row = []
                for string in line.split(' '):
                    if string != '':
                        row.append(complex(string))
                if line != '':
                    answer.append(row)
    except EnvironmentError: # parent of IOError, OSError
        print ("../matMul/answers/matrix_c_{}x{}.txt missing".format(n,n))

    try:
        result = subprocess.run(
            ['./cacheBlocking', "../matMul/tests/matrix_a_{}x{}.txt".format(n,n), "../matMul/tests/matrix_b_{}x{}.txt".format(n,n)],
            cwd=path,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            encoding='ASCII',
            timeout=datetime.timedelta(seconds=8).total_seconds(),
        )

        resultlist = []
        for line in result.stdout.split('\n'):
            row = []
            for string in line.split(' '):
                if string != '':
                    row.append(complex(string))
            if line != '':
                resultlist.append(row)

        if verbose:
            print ("answer")
            print (answer)
            print ("result")
            print (resultlist)
        assert numpy.allclose(resultlist, answer), "The matrix multiplication result doesn't match answers/matrix_c_{}x{}.txt.".format(n,n)
        return True
    except subprocess.CalledProcessError as e:
        print (e.output)
        print ("Calling ./cacheBlocking returned non-zero exit status.")
    except ValueError as e:
        print (result.stdout)
        print ("Please check your output formatting.")
    except AssertionError as e:
        # print (csim.stdout)
        print (e.args[0])

    return False

def test_cacheBlocking ( n, test_name, path="./", verbose=False ):

    if not test_matMul ( n, path=path ):
        return False

    try:
        with open("{}answers/answer_{}.txt".format(path,test_name), "r") as outfile:
            answer = outfile.read()
    except EnvironmentError: # parent of IOError, OSError
        print ("answers/answer_{}.txt missing".format(test_name))

    answer_tallies = list(map(int, re.findall(r'\d+', answer)))

    try:
        generate_trace ( n=n, path=path, dut='./cacheBlocking', trace_path="cacheBlocking_trace_{}.txt".format(test_name) )
        csim = subprocess.run(
            ['../csim-ref', '-s', '2', '-E', '4', '-b', '4', '-l', '1',
            '-t', "cacheBlocking_trace_{}.txt".format(test_name)],
            cwd=path,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            encoding='ASCII',
            timeout=datetime.timedelta(seconds=12).total_seconds(),
        )

        if verbose:
            print (' '.join(csim.args))
            print ("answer")
            print (answer)
            print ("result")
            print (csim.stdout)

        result_tallies = list(map(int, re.findall(r'\d+', csim.stdout)))
        assert result_tallies[1] < answer_tallies[1], "Cache misses need to be less numerous than baseline."
        assert result_tallies[2] < answer_tallies[2], "Cache evictions need to be less numerous than baseline."
        return True
    except subprocess.CalledProcessError as e:
        print (e.output)
        print ("Calling ./cacheBlocking returned non-zero exit status.")
    except ValueError as e:
        print (result.stdout)
        print ("Please check your output formatting.")
    except AssertionError as e:
        print (csim.stdout)
        print (e.args[0])

    return False

def grade_cacheBlocking( path='./', verbose=False ):

    score = 0

    try:
        subprocess.run( ['make', '-B'], cwd=path, check=True, )
    except subprocess.CalledProcessError as e:
        print ("Couldn't compile cacheBlocking.")
        return score

    allpass = True
    for i in range(3,7):
        if test_cacheBlocking(i,"trace_matMul_{}x{}".format(i,i),path,verbose):
            score += 3
        else:
            allpass = False

    for i in range(7,10):
        if allpass:
            if test_cacheBlocking(i,"trace_matMul_{}x{}".format(i,i),path,verbose):
                score += 6
            else:
                allpass = False


    print ("Score on cacheBlocking: {} out of 30.".format(score))
    return score

if __name__ == '__main__':
    # generate_test_suite()
    grade_cacheBlocking(verbose=True)
    exit()
