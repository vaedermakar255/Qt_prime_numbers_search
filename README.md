# Qt prime numbers search
 The program use various algorithms to search for prime numbers, including my own.  
 
 ![Image alt](https://github.com/vaedermakar255/Qt_prime_numbers_search/raw/master/image/image.PNG)
 
 # Current problems/bugs/imperfections:
 - planned to introduce multithreading support;
 - search stop is delayed due to a rare QApplication::processEvents() method call. However, a frequent call to this method will entail a noticeable decrease in the speed of the algorithms;
