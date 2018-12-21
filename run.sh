for threads in 1 2 4 8
do

    for executions in {1..5}
    do

	./onelock/main $executions $threads
	./lock-coupling/main $executions $threads
	./lazy-lock/main $executions $threads
	./optimistic-lock/main $executions $threads
    done
done
