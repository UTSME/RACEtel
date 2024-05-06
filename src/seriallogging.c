#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>

#define SERIAL_PORT "/dev/ttyUSB0" // Should change to whatever this needs to be
#define LOG_FILE "serial_log.txt"

int main() {
    int serial_fd, log_fd;
    struct termios tty;

    // Open serial port
    serial_fd = open(SERIAL_PORT, O_RDONLY);
    if (serial_fd == -1) {
        perror("Error opening serial port");
        return 1;
    }

    // Open log file
    log_fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (log_fd == -1) {
        perror("Error opening log file");
        close(serial_fd);
        return 1;
    }

    // Configure serial port
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(serial_fd, &tty) != 0) {
        perror("Error getting serial port attributes");
        close(serial_fd);
        close(log_fd);
        return 1;
    }

    cfsetospeed(&tty, B9600); // Set baud rate
    tty.c_cflag &= ~PARENB; // Disable parity
    tty.c_cflag &= ~CSTOPB; // One stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS; // Disable hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable receiver, ignore modem control lines

    // Set input mode (non-canonical, no echo)
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Set raw input
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable special handling of received bytes

    // Set raw output
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes

    // Set time-out for read
    tty.c_cc[VMIN] = 0; // Minimum number of characters to read
    tty.c_cc[VTIME] = 10; // Time to wait for data (tenths of seconds)

    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        perror("Error setting serial port attributes");
        close(serial_fd);
        close(log_fd);
        return 1;
    }