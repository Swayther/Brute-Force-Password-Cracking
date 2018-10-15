# Boyer-Moore-Vs-Rabin-Karp

The aim of this project was to create an efficient CPU threaded password cracker in C++.

The initial password cracker had a simple design. The user enters a password between 1 - 5 characters. This password is then hashed and stored. The password cracker then attempts to crack the password using simple string concatenation to guess the password. The attempted is then hashed and compared with the stored password hash. If the hashes match, the password has been cracked.

These attempts are created from a char array. The char array is comprised of 62 characters including: 
Upper case alphabet
Lower case alphabet
‘ ’ & ‘.’ (A space & a full stop)

The hashing algorithm that was used was SHA256. This was selected over other algorithms to simulate a realistic test environment. Considering that SHA256 is one of the most popular hashing algorithms being used now that has NOT been cracked (unlike MD5), it seemed like the most logical choice. Of course, it also adds an element of realism as few companies store user's passwords in a plain text format 
Comparison of Boyer Moore and Rabin Karp password cracking algorithms with sha256 & MD5 hasing

user menu
----------
Rabin Karp Vs Boyer Moore

1. Enter amount of threads for the farm to run on
	(MUST be greater than 2)

2. Enter the ammount of test that you want to run
	(MUST be at least 1)

3. Enter the password you wish to search on
	(1 - 5 Chars with no special chars)
