CXX=g++
CXXFLAGS=-g3 -Wall -Wextra -Wpedantic -Wshadow


OnRoto_Drafter: draft.o interact.o batter_database.o pitcher_database.o batter.o \
			pitcher.o team.o
	${CXX} -o OnRoto_Drafter interact.o draft.o batter_database.o pitcher_database.o \
				 team.o batter.o pitcher.o

interact.o: interact.cpp interact.h
	${CXX} ${CXXFLAGS} -c interact.cpp

draft.o: draft.cpp batter_database.h pitcher_database.h
	${CXX} ${CXXFLAGS} -c draft.cpp

batter_database.o: batter_database.cpp batter_database.h batter.h
	$(CXX) -c batter_database.cpp

pitcher_database.o: pitcher_database.cpp pitcher_database.h pitcher.h
	$(CXX) -c pitcher_database.cpp	

batter.o: batter.cpp batter.h
	$(CXX) -c batter.cpp

pitcher.o: pitcher.cpp pitcher.h
	$(CXX) -c pitcher.cpp

team.o: team.cpp team.h pitcher.h batter.h
	$(CXX) -c team.cpp

clean:
	@rm -r *.o OnRoto_Drafter
