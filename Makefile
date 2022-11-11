all:
	cd src
	g++ -Wall -Wextra -g main.cpp ast.cpp standardize.cpp st.cpp st_types.cpp cse_machine.cpp operators.cpp environment.cpp -o myrpal

clean:
	cd src
	rm -f myrpal
	rm -f test
	rm -f output

check:
	cd src
	make && python3 run_tests.py

test:
	cd src
	g++ runTests.cpp -lcppunit -o test && ./test
