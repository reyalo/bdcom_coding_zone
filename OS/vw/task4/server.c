void os_demo_socket_text_process(void) {
    int sock;
    struct soaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char recvBuf[64];
    char sendBuf[512];  // Can expand as needed
    int recvLen;

    sock = so_socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        Print("[Error] so_socket() failed\n");
        return;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("10.10.10.11");  // Switch's IP

    if (so_bind(sock, (struct soaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        Print("[Error] so_bind() failed\n");
        so_close(sock);
        return;
    }

    Print("[UDP Server] Listening on port %d...\n", SERVER_PORT);

    while (1) {
        recvLen = so_recvfrom(sock, recvBuf, sizeof(recvBuf), 0,
                              (struct soaddr *)&clientAddr, &addrLen);
        if (recvLen < 2) {
            Print("[Warning] Message too short\n");
            continue;
        }

        int msgType = recvBuf[0];
        int msgLen  = recvBuf[1];
        if (msgLen > sizeof(recvBuf) - 2) msgLen = sizeof(recvBuf) - 2;

        char filename[61];
        memcpy(filename, &recvBuf[2], msgLen);
        filename[msgLen] = '\0';

        if (msgType == REMOTE_COMMUNICATION_MSG_TYPE_QRY) {
            Print("[Server] Filename request: %s\n", (int)filename);

            // File handling
            FCB_POINT *fp = file_open(filename, "r", NULL);
            if (!fp) {
                snprintf(sendBuf, sizeof(sendBuf), "ERROR: Cannot open '%s'", filename);
                so_sendto(sock, sendBuf, strlen(sendBuf), 0,
                          (struct soaddr *)&clientAddr, addrLen);
                continue;
            }

            memset(sendBuf, 0, sizeof(sendBuf));
            int readLen = file_read(fp, sendBuf, sizeof(sendBuf) - 1);
            file_close(fp);

            // Send file content back
            so_sendto(sock, sendBuf, readLen, 0,
                      (struct soaddr *)&clientAddr, addrLen);

        } else if (msgType == REMOTE_COMMUNICATION_MSG_TYPE_CFM) {
            Print("[Server] Received CFM: %s\n", (int)&recvBuf[2]);
            snprintf(sendBuf, sizeof(sendBuf), "How do you do %s %d",
                     inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            so_sendto(sock, sendBuf, strlen(sendBuf), 0,
                      (struct soaddr *)&clientAddr, addrLen);
        } else {
            Print("[Warning] Unknown message type: %d\n", msgType);
        }
    }

    so_close(sock);
}
