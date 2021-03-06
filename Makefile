CC=clang++

bin/igrow: obj/io_service_pool.o obj/safe_counter.o obj/array.o obj/atom.o obj/ligand.o obj/main.o
	${CC} -o $@ $^ -pthread -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread

obj/%.o: src/%.cpp
	${CC} -o $@ $< -c -std=c++11 -O2

clean:
	rm -f bin/igrow obj/*.o
