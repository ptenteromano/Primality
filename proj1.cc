/**********************************************************************
 *
 * CISC 4080, Project 1:  Testing For Primes
 *
 * Philip Tenteromano
 * Febuary 7th, 2018
 * Professor Werschulz
 * Computer Algorithms
 *
 **********************************************************************/

// system includes
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <libgen.h>

// Walter Brown's library
#include "random.h"

// Added <assert> for preconditions
#include <assert.h>

// be lazy
using namespace std;
using namespace std::chrono;

// Added functions for Proj1
int mod_mult(long int a, long int b, long int n);	// Simplifies Modular products
bool check_two(long int num);		// Try / catch to assert num >= 2 

// function prototypes
void usage(string progname);
void check_prime(long int number);
bool is_prime_exact(long int number);
bool is_prime_prob(long int number);
long int pow_mod(long int a, long int b, long int n);

// usual main function
int main(int argc, char** argv)
{
   // command-line parameter processing
   string progname{basename(argv[0])};
   if (argc != 2) {
      usage(progname);
      return 1;
   }
   ifstream data_stream{argv[1]};
   if (!data_stream) {
      cerr << progname << ": couldn't open " << argv[1] << endl;
      return 2;
   }

   // comment this out if debugging
   randomize();                         

   // print "false" and "true" for bool values
   cout << boolalpha;

   // finally:check primality of each number in the file
   for (long int number; data_stream >> number; ) {
      if (number <= 1)
         cout << number <<" <= 2: ignored\n";
      else 
         check_prime(number);
   }
	//using my pow_mod() funct to calculate question 1.25 from last week's HW
	cout<<endl<<"Checking the the exponential Mod function using exercise 1.25:\n"
	    	  <<"(2^125 MOD 127)\n";
	cout<<"The answer is!?\t"<<pow_mod(2, 125, 127)<<endl<<endl;

}	//end Main

// print usage msg (for error msgs)
void usage(string progname)
{
   cerr << "No file found!!! Exiting...\n";
   exit(2);
}

// precondition: number >= 2
// checks whether number is prime, printing the result (as well as how
// long the calculation took), using both the exact and the
// probabilistic algorithms 
void check_prime(long int number)
{
   cout << "checking " << number << ":\n";
   // exact algorithm
   auto start = system_clock::now();
   bool prime_result = is_prime_exact(number);
   auto end = system_clock::now();
   duration<double> elapsed_seconds = end - start;
   cout << "  exact says " << prime_result
        << " (" << elapsed_seconds.count() << " seconds),\n";
   // probabilistic algorithm
   start = system_clock::now();
   prime_result = is_prime_prob(number);
   end = system_clock::now();
   elapsed_seconds = end - start;
   cout << "  probabilistic says " << prime_result
        << " (" << elapsed_seconds.count() << " seconds)\n";
}

// Simple Try / Catch function to use in primality tests
bool check_two(long int num)
{
	try {						//Try to find an error
		assert(num >= 2);
		return false;
	} 
	catch(...) {				// Catch the error, return a bool value
		cout<<"Number must be >= 2\n";
		return true;
	}
}

// Exact Primality test, using the find_sqr function on number
// Loops through all k values from 2 to sqr(n)
// Checks for number % k through all those values
// If k divides number, number is not prime
bool is_prime_exact(long int number)
{
	if(check_two(number))		// assert num >= 2
		return false;

	for(int k=2; k*k<=number; k++) {	// for loop, up to floor(sqr(n))
		if(number % k == 0)			// Since if a*a = n, then k>a means k*a > n
			return false;			// finding mod k == 0 returns false for primality
	}

	return true;				//otherwise, return true
}

// Probablistic Primality test, using Fermat's Little Theorm
// Only using one k value, check k^number-1 MOD number == 1
// Assuming == 1, then number is likely prime... BUT could not be!
bool is_prime_prob(long int number)
{
	if(check_two(number))
		return false;

	int k = 10;		// initalize k value!
	if(k >= number)	 	// k has to fall between 1 and num-1 
		k = number - 1;
	
	if( pow_mod(k, number-1, number) == 1)	// call power Mod function - using modulus rules
		return true;					// by Fermat's little theorem, return true if == 1
	else
		return false;					//else, definitely not prime!
}

/*
// Integer Square root algorithm
// Iterative method, using two consecutive x values
// Using Newton's Algorithm which converges on the floor of sqr(n)
int find_sqr(long int a)
{
	// x0 guess is a/4 so a MUST be > 3 or floating pt error
	if (a <= 3)		//handle this by returning proper sqr(a < 4)
		return 1;

	int x0 = a/4;	// initalize variables
	int x1 = 0;		// x1 will be given new value
		
	do {		//loop indefinitely until x1^2 = a is found
		x1 = 0.5*(x0 + (a/x0));		// primary formula, always casted as INT 
		x0 = x1;					// x0 becomes new x1 value
	}while(x1 * x1 > a);			// values are SHRINKING towards a, loop until <= a

	return x1;			//return int value
} */

// Added this function for modular arithmetic
// Mainly in the form of a*b mod n
// Simplifies Mod of two integer products
int mod_mult(long int a, long int b, long int n)
{
	int x = (a % n) * (b % n);	// MOD a, MOD b, multiply the mods (<n * <n)

	return ( x % n );			// Return the Mod of the product

	// This funct is used in the pow_mod function heavily
}

// This function does modulus on extremely large numbers
// Using mod arithmetic rules to break down exponents
// Moding smaller portions of the problem recursively until
// the exponent == 0	*** Recursive function
long int pow_mod(long int a, long int b, long int n)
{
   	assert(b>=0 && n>=1);	// Asserting positive exponent and modulus n
	
   	if(b==0)					// BASE CASE: return 1 when exponent == 0
        return 1;
   	else if (b%2==0) {		// if the exponent is EVEN
	    int x = pow_mod(a, b/2, n); 	// recursive, divide EVEN exponent in half

	    return mod_mult(x,x,n);		// a^e is now (a^(e/2)^2), so.. call mod_mult!
   	}
	else		// Otherwise, exponent is ODD, take product of 
		return mod_mult( a%n, pow_mod(a,b-1,n), n);		// a^1 mod n * a^e-1 mod n
}	
	

