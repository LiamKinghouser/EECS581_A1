# EECS581_A1

- AI Model used: Gemini 3.7 Flash High

- Dates consulted: 9/8/2026

- Initial prompt:

Write a C program in /Users/kinghouser/Desktop/CodingProjects/EECSProjects/EECS581_A1 that reads a line of text and extracts a single valid IPv4 address (optionally followed by a port number) embedded anywhere in the input.

A candidate token consists strictly of digits, periods, and colons, all other characters are treated as garbage and skipped. An address consists of four period-separated octets (octet.octet.octet.octet), where each octet is 1 to 3 digits, valued 0 to 255, with no leading zeros unless the value is 0. An optional port may follow the fourth octet (:port), consisting of 1 to 5 digits, valued 0 to 65535, with the same leading-zero rule. If a colon is present, the port must be valid or the entire candidate token is rejected. Tokens must match the grammar fully without truncation, partial matching, extra separators, or adjacent periods/colons.

Implement the extraction function with this prototype: int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);

The function returns 1 if a valid address is found and 0 otherwise. On success, *outAddress is set to the 32-bit numerical value of the address and *outPort is set to the port number (or -1 if no port was provided). On failure, *outAddress is set to 0 and *outPort is set to -1.

Rules: Do not use string-to-number conversion functions (such as atoi, atol, atoll, strtol, strtoul, strtod, sscanf, or scanf with numeric conversions), IP-parsing libraries (such as inet_aton, inet_pton, inet_addr), or regular expression libraries. All parsing, validation, and digit accumulations must be written manually character-by-character. Standard character-classification functions such as isdigit are permitted.

Program requirements: In main, continuously prompt the user for input lines until the user enters END (case-sensitive), at which point print "Program terminated." and exit. For each line, call extractIPv4 and, if valid, print: Extracted IPv4 address: A.B.C.D (decimal value: N, port: P) where N is the 32-bit decimal value and P is the port number or the literal text "none".

- Fix #1 prompt:

the program should prompt the user with messages for the inputs. below is a sample run of how it should look.

Sample run:

Enter a string (or 'END' to quit): connecting to 192.168.1.1 now
Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)
Enter a string (or 'END' to quit): server=10.0.0.255:8080end
Extracted IPv4 address: 10.0.0.255 (decimal value: 167772415, port: 8080)
Enter a string (or 'END' to quit): 192a168.1.1.1
Extracted IPv4 address: 168.1.1.1 (decimal value: 2818638081, port: none)
Enter a string (or 'END' to quit): 192.168.1.1.
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): Connection from 192.168.1.1 refused
Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)
Enter a string (or 'END' to quit): 192.168.01.1
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): 1.2.3.4:99999
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): 12.34.56
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): no number here
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): END
Program terminated.

- Test cases (all test cases passed):

All of the provided test cases, as well as the following:

1.2.3.4:0
1.2.3.4:65535
1.2.3.4:65536
1.2.3.4:080
1.2.3.4:00
0.0.0.0:0
255.255.255.255:65535
256.0.0.1
10.256.0.1
192.168.0.01
1234.1.1.1
1.2.3.4:100000
1.2.3.4.5
1.2.3.4:
1.2.3.4::80
1.2.3.4:80:90
1.2.3.4.:80
1.2.3.4:80.
.1.2.3.4
1..2.3.4
http://192.168.1.1:80/index.html
ftp://user:pass@10.0.0.1:21/files
Primary 192.168.1.1 and backup 10.0.0.1:80
Bad 1.2.3.4.5 but good 172.16.1.1:443
failed:::192.168.1.1 but good 172.16.1.1:443

- Verification statement:

I understand every line of the code in this project. The code has been fully tested and works as intended. The code has no known bugs or limitations.