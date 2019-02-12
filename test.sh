export TEST_FILES=$(ls tests/*Test.h)
export FUNCS
for file in $TEST_FILES; do
	FUNCS=${FUNCS}$(cat $file | grep -e "void test" | sed -n -e 's/^.*void //p'| cut -d "(" -f1)
	echo "${FUNCS// /\n}" > function_symbols
	
	echo "#include <stdio.h>" > tests/main.c
	echo "#include \"macros.h\"" >> tests/main.c
	echo "int main() {" >> tests/main.c
	echo 'printf("=====================\\n");' >> tests/main.c
	while read line; do
		echo "run_test("$line");" >> tests/main.c
	done < function_symbols
	echo 'printf("=====================\\n");' >> tests/main.c
	echo "return errno; }" >> tests/main.c
	gcc -Idependencies tests/*.c src/*.c dependencies/libs/*.a -o test \
	 	-Wno-implicit-function-declaration -W#warnings
	rm function_symbols
	./test
	rm test
	rm tests/main.c
done