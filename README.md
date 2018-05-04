# Primality
Testing Naive Primality vs. Fermat's little Theorem

Naive Primality testing: (Exact Test)
  Is N prime?
  Loop from 2 through (floor) sqr(N):
    at each iteration, if N % iteration == 0, the number is not prime.
    
    else if it passes every test, it's definitely prime.
    
Fermat's little theorem: (Probabilistic Test)
  Is N prime?
  Choose some K value between 2 and N-1.
  
  If K^(N-1) MOD N == 1: then the number MIGHT be prime. (There exist some exceptions).
  

The Program also times the running time.

