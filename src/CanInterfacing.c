#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#define CAN_INTERFACE "can0"  // Change to match name again

int can_init() {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;

    // Create a socket
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) {
        perror("Error creating socket");
        return -1;
    }

    // Specify the CAN interface
    strcpy(ifr.ifr_name, CAN_INTERFACE);
    ioctl(s, SIOCGIFINDEX, &ifr);

    // Bind the socket to the CAN interface
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error binding socket to interface");
        close(s);
        return -1;
    }

    return s;
}

int can_send(int s, uint32_t id, uint8_t *data, uint8_t len) {
    struct can_frame frame;

    // Prepare the CAN frame
    frame.can_id = id;
    frame.can_dlc = len;
    memcpy(frame.data, data, len);

    // Send the CAN frame
    if (write(s, &frame, sizeof(frame)) != sizeof(frame)) {
        perror("Error sending CAN frame");
        return -1;
    }

    return 0;
}

int can_receive(int s, uint32_t *id, uint8_t *data, uint8_t *len) {
    struct can_frame frame;

    // Receive a CAN frame
    if (read(s, &frame, sizeof(frame)) != sizeof(frame)) {
        perror("Error receiving CAN frame");
        return -1;
    }

    // Extract ID, data, and length from the received frame
    *id = frame.can_id;
    *len = frame.can_dlc;
    memcpy(data, frame.data, *len);

    return 0;
}

int main() {
    int can_socket;
    uint32_t id;
    uint8_t data[8];
    uint8_t len;

    // Initialize CAN interface
    can_socket = can_init();
    if (can_socket < 0) {
        return 1;
    }

    // Example: Send a CAN message
    id = 0x123;
    data[0] = 0xAA;
    data[1] = 0xBB;
    len = 2;
    if (can_send(can_socket, id, data, len) < 0) {
        close(can_socket);
        return 1;
    }

    // Example: Receive a CAN message
    if (can_receive(can_socket, &id, data, &len) < 0) {
        close(can_socket);
        return 1;
    }
    printf("Received CAN message: ID=0x%x, Data=", id);
    for (int i = 0; i < len; i++) {
        printf("0x%x ", data[i]);
    }
    printf("\n");

    // Close CAN socket
    close(can_socket);

    return 0;
}
