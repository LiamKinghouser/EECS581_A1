/*
 * EECS581_A1: IPv4 Address Extractor

 * All of this code was written by AI (Gemini)
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * Checks if a character is part of a candidate token.
 * Candidate characters strictly include digits, periods, and colons.
 */
static int isCandidateChar(char c) {
    return isdigit((unsigned char)c) || c == '.' || c == ':';
}

/*
 * Validates and parses a candidate token slice [tok, tok + len).
 * If valid, computes the 32-bit address and port number.
 * Returns 1 on success, 0 on failure.
 */
static int parseCandidateToken(const char* tok, int len, unsigned long* outAddress, int* outPort) {
    int octets[4];
    int curr = 0;

    // Parse 4 period-separated octets
    for (int i = 0; i < 4; i++) {
        int start = curr;

        // Accumulate digits for the current octet
        while (curr < len && isdigit((unsigned char)tok[curr])) {
            curr++;
        }

        int octetLen = curr - start;

        // Octet must have between 1 and 3 digits
        if (octetLen < 1 || octetLen > 3) {
            return 0;
        }

        // Leading zero rule: no leading zeros unless the value is 0 (length 1)
        if (octetLen > 1 && tok[start] == '0') {
            return 0;
        }

        // Manual digit accumulation
        int val = 0;
        for (int d = 0; d < octetLen; d++) {
            val = val * 10 + (tok[start + d] - '0');
        }

        // Octet value must be between 0 and 255
        if (val < 0 || val > 255) {
            return 0;
        }

        octets[i] = val;

        // First 3 octets must be followed by a period
        if (i < 3) {
            if (curr >= len || tok[curr] != '.') {
                return 0;
            }
            curr++; // Skip period
        }
    }

    int portVal = -1;

    // After 4th octet, either token ends or an optional ':port' follows
    if (curr < len) {
        if (tok[curr] != ':') {
            // Unexpected character after 4th octet (e.g. extra period)
            return 0;
        }

        curr++; // Skip colon

        int portStart = curr;

        // Accumulate digits for the port
        while (curr < len && isdigit((unsigned char)tok[curr])) {
            curr++;
        }

        int portLen = curr - portStart;

        // Port must have between 1 and 5 digits
        if (portLen < 1 || portLen > 5) {
            return 0;
        }

        // Leading zero rule for port
        if (portLen > 1 && tok[portStart] == '0') {
            return 0;
        }

        // Manual digit accumulation for port
        long pval = 0;
        for (int d = 0; d < portLen; d++) {
            pval = pval * 10 + (tok[portStart + d] - '0');
        }

        // Port value must be between 0 and 65535
        if (pval < 0 || pval > 65535) {
            return 0;
        }

        // Ensure the entire candidate token has been consumed (no trailing chars/colons/dots)
        if (curr != len) {
            return 0;
        }

        portVal = (int)pval;
    }

    // Compute 32-bit address
    unsigned long address = ((unsigned long)octets[0] << 24) |
                            ((unsigned long)octets[1] << 16) |
                            ((unsigned long)octets[2] << 8)  |
                            ((unsigned long)octets[3]);

    if (outAddress != NULL) {
        *outAddress = address;
    }
    if (outPort != NULL) {
        *outPort = portVal;
    }

    return 1;
}

/*
 * Extracts a single valid IPv4 address (and optional port) embedded in str.
 * Returns 1 if a valid address is found and 0 otherwise.
 */
int extractIPv4(const char* str, unsigned long* outAddress, int* outPort) {
    if (str == NULL) {
        if (outAddress != NULL) *outAddress = 0;
        if (outPort != NULL) *outPort = -1;
        return 0;
    }

    int i = 0;
    while (str[i] != '\0') {
        // Skip non-candidate characters
        if (!isCandidateChar(str[i])) {
            i++;
            continue;
        }

        // Candidate token boundary identified
        int tokStart = i;
        while (str[i] != '\0' && isCandidateChar(str[i])) {
            i++;
        }
        int tokLen = i - tokStart;

        unsigned long addr = 0;
        int port = -1;

        if (parseCandidateToken(str + tokStart, tokLen, &addr, &port)) {
            if (outAddress != NULL) *outAddress = addr;
            if (outPort != NULL) *outPort = port;
            return 1;
        }
    }

    // No valid IPv4 address found
    if (outAddress != NULL) *outAddress = 0;
    if (outPort != NULL) *outPort = -1;
    return 0;
}

#ifndef NO_MAIN
int main(void) {
    char buffer[2048];

    while (1) {
        printf("Enter a string (or 'END' to quit): ");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        // Strip trailing newline and carriage return characters
        size_t len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[len - 1] = '\0';
            len--;
        }

        // Exit condition: END (case-sensitive)
        if (strcmp(buffer, "END") == 0) {
            printf("Program terminated.\n");
            break;
        }

        unsigned long address = 0;
        int port = -1;

        if (extractIPv4(buffer, &address, &port)) {
            unsigned int a = (unsigned int)((address >> 24) & 0xFF);
            unsigned int b = (unsigned int)((address >> 16) & 0xFF);
            unsigned int c = (unsigned int)((address >> 8) & 0xFF);
            unsigned int d = (unsigned int)(address & 0xFF);

            if (port == -1) {
                printf("Extracted IPv4 address: %u.%u.%u.%u (decimal value: %lu, port: none)\n",
                       a, b, c, d, address);
            } else {
                printf("Extracted IPv4 address: %u.%u.%u.%u (decimal value: %lu, port: %d)\n",
                       a, b, c, d, address, port);
            }
        } else {
            printf("Invalid input: no valid IPv4 address found\n");
        }
    }

    return 0;
}
#endif


