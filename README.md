# LOL Browser
.                       ![Logo](./logo/logo_nobg.png)

A custom-built web browser implementation in C++ featuring a handcrafted networking stack and a Qt-based user interface.

## Overview
![overview](./logo/image.png)

LOL Browser is an educational project designed to understand the core components of a web browser. Instead of relying on high-level networking libraries, it implements fundamental protocols from scratch, including DNS resolution, TCP/TLS sockets, and HTTP/HTTPS parsing.

## Features

-   **Custom Networking Stack**:
    -   **DNS Resolution**: Manual DNS queries using OS APIs (`getaddrinfo`) with a local caching layer.
    -   **TCP & TLS Sockets**: Raw socket implementation for HTTP and OpenSSL integration for HTTPS.
    -   **Connection Pooling**: Efficient management of persistent connections (Keep-Alive).
    -   **HTTP/1.1 Support**: Custom HTTP parser handling headers, chunked transfer encoding, and response codes.
-   **Caching System**:
    -   **DNS Cache**: In-memory caching of IP addresses to reduce lookup latency.
    -   **HTTP Cache**: Compliance with `Cache-Control` headers for efficient resource retrieval.
-   **User Interface**:
    -   Built with **Qt** for a responsive and cross-platform GUI.
    -   Simple navigation bar and content display area.

## Project Structure

```
LOL_Browser/
├── browser/            # Browser logic and controller
├── network/            # Networking stack
│   ├── dns/            # DNS resolution and caching
│   ├── http/           # HTTP client, parser, and cache
│   └── sockets/        # TCP, TLS, and connection pooling
├── render/             # Basic text rendering logic
├── ui/                 # Qt-based user interface
├── main.cpp            # Application entry point
├── lol_browser.pro     # QMake project file
└── Makefile            # Generated build file
```

## Prerequisites

Ensure you have the following installed on your system:

-   **C++ Compiler**: GCC or Clang supporting C++17.
-   **Qt 5**: Development libraries (`qt5-default`, `libqt5widgets5`, etc.).
-   **OpenSSL**: Development libraries (`libssl-dev`).

## Build Instructions

1.  **Clone the repository**:
    ```bash
    git clone <repository-url>
    cd LOL_Browser
    ```

2.  **Generate the Makefile**:
    ```bash
    qmake lol_browser.pro
    ```

3.  **Compile the project**:
    ```bash
    make
    ```

## Usage

After a successful build, run the browser:

```bash
./lol_browser_gui
```

1.  Enter a URL (e.g., `https://google.com` or `http://example.com`) in the address bar.
2.  Press **Go**.
3.  The response body will be displayed in the main text area.

