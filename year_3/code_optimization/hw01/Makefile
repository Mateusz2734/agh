REPEATS = 10000

build:
	g++ normalize$(N).cpp

run:
	./a.out < lab1.tex

profile:
	@mkdir -p docs

	@echo "Profiling with gprof (without optimization)"
	@g++ -pg normalize$(N).cpp
	@ echo "Without -O2" > docs/time$(N).txt
	@./a.out $(REPEATS) < lab1.tex >> docs/time$(N).txt
	gprof a.out gmon.out > docs/gprof$(N).txt
	gprof a.out gmon.out | ./gprof2dot -e 0.00001 -n 0.00001 -s -w -z"normalizeText" | dot -Gdpi=200 -Tpng -o docs/output$(N).png

	@echo "Profiling with gprof (with -O2)"
	@g++ -pg -O2 normalize$(N).cpp
	@ echo "With -O2" >> docs/time$(N).txt
	@./a.out $(REPEATS) < lab1.tex >> docs/time$(N).txt