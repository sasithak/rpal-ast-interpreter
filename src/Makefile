all:
	g++ -Wall -Wextra -g main.cpp ast.cpp standardize.cpp st.cpp st_types.cpp cse_machine.cpp operators.cpp environment.cpp -o myrpal

clean:
	rm -f myrpal

check:
	make && python3 run_tests.py
