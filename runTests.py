import sys
import subprocess
import os

run_test_function = "run_test"

def runFunctionsStub(functions):
	functions = list(map(lambda x: x[:x.find("(")], functions))
	return "".join(list(map(lambda x: f"{run_test_function}({x});", functions)))

def generateMainFromStub(filename, functions):
	return f"#include \"{filename}\"\n #include \"tests/macros.h\"\nint main() {{ setup(); {runFunctionsStub(functions)} teardown(); \
	printf(\"Tests executed \033[1m{len(functions)}\033[0m\"); puts(\"\"); puts(\"\"); return errno;}}"

def run_tests_for_file(filename):
	print(f"\033[1mPreparing tests for '{filename.split('/')[-1][:-2]}'\033[0m")
	print("\033[1m"+"-"*40+"\033[0m")

	functions = subprocess.getoutput(f"cat {filename} | grep -e \"void test\" |\
				sed -n -e 's/^.*void //p'").split("\n")

	with open(f"temp_test.c", "w") as testfile:
		testfile.write(generateMainFromStub(filename,functions))

	print("Compiling...")
	os.system(f"gcc -Idependencies -o run_test temp_test.c \
		{filename[:-1]+'c'} $(find src/ -name '*.c' -a ! -name '*main.c') dependencies/libs/*.a")
	print("Running...")
	ret = os.system(f"./run_test")

	os.system("rm run_test temp_test.c")

	return ret

def main(args):
	exit_code = 0
	for name in args[1:]:
		print(name)
		if name == "main.c":
			continue
		ret = run_tests_for_file(name)
		if ret != 0:
			exit_code = ret
	if (exit_code):
		print("\x1B[31mOne or more test failed!\x1B[0m")
	sys.exit(exit_code>>8)

if __name__ == "__main__":
	main(sys.argv)
	