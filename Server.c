#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "declarations.h"

#define PORT 8080
#define MAX_CLIENTS 10

sqlite3 *db;  // SQLite database connection

// Function prototypes
void* handle_client(void* client_socket);
void initialize_database();
void close_database();
void setup_server(int* server_fd, struct sockaddr_in* address);
void accept_connections(int server_fd);

// Initialize SQLite database
void initialize_database() {
    int rc = sqlite3_open("bank.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }
}

// Close SQLite database
void close_database() {
    sqlite3_close(db);
}

// Set up the server
void setup_server(int* server_fd, struct sockaddr_in* address) {
    int opt = 1;
    
    // Create socket file descriptor
    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach socket to the port 8080
    if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);

    // Bind the socket to the port 8080
    if (bind(*server_fd, (struct sockaddr*)address, sizeof(*address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // Listen for incoming connections
    if (listen(*server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

// Handle client requests
void* handle_client(void* client_socket) {
    int client_fd = *(int*)client_socket;
    free(client_socket);
    
    // Process client requests (pseudo-code, replace with actual logic)
    char buffer[1024] = {0};
    int read_size = read(client_fd, buffer, sizeof(buffer));
    if (read_size > 0) {
        // Handle the client's request
        // Example: process the buffer data and interact with the database
    }

    close(client_fd);
    pthread_exit(NULL);
}

// Accept and handle incoming client connections
void accept_connections(int server_fd) {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    while (1) {
        int* client_socket = malloc(sizeof(int));
        if ((*client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            free(client_socket);
            continue;
        }
        
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)client_socket) != 0) {
            perror("pthread_create");
            close(*client_socket);
            free(client_socket);
        }
        pthread_detach(thread_id);
    }
}

int main() {
    int server_fd;
    struct sockaddr_in address;

    initialize_database();
    setup_server(&server_fd, &address);
    accept_connections(server_fd);
    
    close_database();
    close(server_fd);
    return 0;
}
