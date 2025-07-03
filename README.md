# proxytest

Utility to test a proxy by spamming requests.

[!WARNING] Untested.

---

## Compiling

### Windows

```
g++ -std=c++11 main.cpp -o fetch_threads.exe -lcurl

```

### Linux

```
g++ -std=c++11 main.cpp -o fetch_threads -lcurl -lpthread

```