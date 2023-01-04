all:
	g++ -Wall -Wextra main.cpp ast.cpp standardize.cpp st.cpp st_types.cpp cse_machine.cpp operators.cpp environment.cpp -o myrpal

clean:
	rm -f myrpal
	rm -f test
	rm -f output
	rm -f exec.txt

test:
	g++ runTests.cpp -lcppunit -o test && ./test
