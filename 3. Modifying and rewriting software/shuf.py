import random, sys
from optparse import OptionParser


class randline:
    def __init__(self, input_f, lohi):
        if input_f == "-" or input_f == "no input":
            self.lines = sys.stdin.readlines()
            if self.lines == 0:
                return
        elif lohi:
            self.lines = input_f
        else:
            f = open(input_f, 'r')
            self.lines = f.readlines()
            f.close()

    def chooseline(self, count, repeat, parser):
        if count == "all" and repeat:
            while True:
                # infinite loop
                temp_line = random.choice(self.lines)
                try:
                    sys.stdout.write(temp_line)
                except:
                    parser.error("User hits exit")
        if count == "all":
            count = len(self.lines)
        for i in range(count):
            if len(self.lines) == 0:
                break
            temp_line = random.choice(self.lines)
            if repeat:
                sys.stdout.write(temp_line)
            else:
                sys.stdout.write(temp_line)
                self.lines.remove(temp_line)


def main():
    # read inputs and options
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE
    %prog -i lo-hi [OPTION]... 
    
    Output a random permutation of its input lines."""
    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--head-count", default="all",
                      action="store", dest="count",
                      help="Output at most COUNT lines.(default: all lines)")
    parser.add_option("-i", "--input-range",
                      action="store", dest="lowhi",
                      help="Input came from a file containing the range of decimal integers lo...hi. ")
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="Repeat output values, that is, select with replacement. (default: false)")
    options, args = parser.parse_args(sys.argv[1:])

    try:
        count = int(options.count)
    except:
        if options.count == "all":
            count = "all"
        else:
            parser.error("invalid input for -n")
    if count is int and count < 0:
        parser.error("negative count: {0}".
                     format(count))

    try:
        nums = options.lowhi.split('-')
        lohi = True
    except:
        lohi = False
    repeat = bool(options.repeat)
    length = len(args)
    if lohi:
        if length > 0:
            parser.error("Too many arguments")
        try:
            num1 = int(nums[0])
        except:
            parser.error("invalid input range for -i")
        try:
            num2 = int(nums[1])
        except:
            parser.error("invalid input range for -i")
        if num1 >= num2:
            parser.error("Invalid input range for -i")
        input_f = []
        for i in range(num1, num2+1):
            input_f.append(str(i) + '\n')
    else:
        if length == 0:
            input_f = "no input"
        elif length > 1:
            parser.error("Too many arguments")
        else:
            input_f = args[0]

    try:
        generator = randline(input_f, lohi)
        generator.chooseline(count, repeat, parser)
    except IOError as err:
        errno, strerror = err.args
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))


if __name__ == "__main__":
    main()
