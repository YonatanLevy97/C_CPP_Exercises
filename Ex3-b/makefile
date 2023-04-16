tweets: tweets_generator.o markov_chain.o linked_list.o
	gcc tweets_generator.o markov_chain.o linked_list.o -o tweets_generator

snake: snakes_and_ladders.o markov_chain.o linked_list.o
	gcc snakes_and_ladders.o markov_chain.o linked_list.o -o snakes_and_ladders

snakes_and_ladders.o: snakes_and_ladders.c markov_chain.o linked_list.o
	gcc snakes_and_ladders.c -c

tweets_generator.o: tweets_generator.c markov_chain.o linked_list.o
	gcc tweets_generator.c -c

markov_chain.o: markov_chain.c markov_chain.h linked_list.o
	gcc markov_chain.c -c

linked_list.o: linked_list.c linked_list.h
	gcc linked_list.c -c
