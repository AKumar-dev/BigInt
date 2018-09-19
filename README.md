# BigInt
A data type that is a pseudo infinite integer type, which can store very large integer values ( the number of digits that can be stored is limited only by available memory). This is a templatized class, which supports both Lists and Vectors containers (the versions of List and Vector in this repository were implemented by me, based on the STL). The other template is what type of data can be stored, and that is all primitive types except booleans.

### Supported operations (for both Negative, Positive, and '0' BigInts)
- Addition
- Subtraction
- Assignment
- Comparisons
- Write to output
- Read from input

![Addition of BigInts](https://i.imgur.com/7MW37KF.png)

![Subtraction of BigInts](https://i.imgur.com/Ku5aEah.png)

## Challenges/Learning
This project was quite challenging for me, mainly because of the way I approached it. These BIGINTs were supposed to be read in from a file, and they would then be stored. So as to not limit the size of a BIGINT that could be stored by reading in the data line by line (using strings), I had to read in char by character. This meant that the greatest value digit was stored at index 0, the next greatest at digit 1, and so on. 

However, when I wanted to add/subtract two of the digits, I would have to traverse over the number backwards, as that is how the operations work. But, since I would not want to use O(n) push_front on a vector to store this BigInt forwards, I decided to store the result backwards, and add a bool flag to determine whether the BigInt is stored forwards (1) or backwards (0). Another possible implementation that was presented was pre-determining the size of the resulting array to 1 greater than that of the greatest operand, but I did not want to waste the space.

The way my BigInts were implemented, it increased the number of cases for operations involving two BigInts to 4, due to the combinations of being stored backwards or forwards. This, combined with checking what the signs of the numbers are and which one is bigger (size or value? you decide), made for an overwhelming amount of test cases, which ultimately lead to my class being over 1,000 lines long. Granted I could have added more helper functions to reduce the lines of code, I was on a time crunch and that would honestly have been more confusing.

Oh, by the way, it turns out that my implementation was not necessarily better than the one where an array of a pre-determined size is created, as the way vectors resize is implemented, the block of memory doubles. This means that in some cases I only use small amounts of the 2nd half of the array, and the others go to waste. To perhaps remedy this issue (mainly in the case of subtraction).

  1. stop performing subtraction after we reach a location that is past the size of the smaller operand
  2. Traverse the bigger one to see if the rest of it actually has values or just evaluates to 0's
  3. Add the appropriate values (if any)
  
So, I would just be trading off space for time. You pick which is better.

**TL;DR** Use a list when you are using this class, as random access isn't needed, and vectors waste space.

*If you are a student reading this, wondering if you should copy this for your HW project, the answer is 'probably not'. Besides stating the obvious (that you risk losing some of whatever you have already invested in your very expensive education), this project was challenging for me, so if you are like me, the knowledge/experience you will gain from doing your own work is unparalleled to anything you could copy from the internet.*
