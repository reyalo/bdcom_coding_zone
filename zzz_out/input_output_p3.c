#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF_LEN 128

int main(void) {
    unsigned int sip = 0, dip = 0;
    int len = 0, sport = 0, dport = 0;
    char buf[BUF_LEN] = {0};

    
    sip = 0x01010101;
    dip = 0x02020202;
    len = 44;
    sport = 23;
    dport = 54177;

    // Get the current time
    time_t now;             // now is variable which can store sytem time.
    struct tm *tm_info;     // a pointer to struct tm varible

    time(&now);             // store system time to now
    tm_info = localtime(&now);  // convert time_t value to struct tm value,
                                // struct tm is human readble



    // Format the time into a string similar to "Fri Feb 12 02:16:09 EST 2025"
    char time_buf[BUF_LEN];
    strftime(time_buf, BUF_LEN, "%a %b %d %H:%M:%S %Z %Y", tm_info);

    // Format the final output string
    snprintf(buf, BUF_LEN, "%s, IP src = %d.%d.%d.%d, dst = %d.%d.%d.%d, len = %d, TCP sport = %d, dport = %d",
             time_buf,
             (sip >> 24) & 0xFF, (sip >> 16) & 0xFF, (sip >> 8) & 0xFF, sip & 0xFF,
             (dip >> 24) & 0xFF, (dip >> 16) & 0xFF, (dip >> 8) & 0xFF, dip & 0xFF,
             len, sport, dport);

    // Print the formatted output
    printf("\n\n%s\n\n", buf);

    return 0;
}
