all:
	g++ -Wall -Wextra -g main.cpp ast.cpp standardize.cpp st.cpp st_types.cpp cse_machine.cpp operators.cpp environment.cpp -o myrpal

clean:
	rm -f myrpal
	rm -f test
	rm -f output

check:
	make && python3 run_tests.py

test:
	make && g++ runTests.cpp -lcppunit -o test && ./test
