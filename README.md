# proxytest

Utility to test a proxy by spamming requests.

> [!WARNING] This tool is meant to "stress test" a proxy server if configured for heavy load or executed on multiple machines. It can also be used as a "functionality test" if configured for light load.

---

## Dependencies

- `libcurl`

## Compiling

### Windows

```
g++ -std=c++17 main.cpp -o proxytest.exe -lcurl

```

### Linux

```
g++ -std=c++17 main.cpp -o proxytest -lcurl -lpthread

```
---

## Usage

### Execution

`./proxytest` (Linux) or `.\proxytest.exe` (Windows)

### Parameters

There are currently no available CLI parameters.
Test parameters are instead provided through stdin:

- **URL list filename**: the pathname (relative or absolute) containing a list of domains in the format `example.com`. Please do not provide URLs like `http://example.com` or `https://example.com`. <br>
One useful list is [this one](https://github.com/bensooter/URLchecker/blob/master/top-1000-websites.txt).
- **Proxy URL**: the URL of the HTTP proxy server we want to test.<br>
The format must be `http://<host>:<port>` or `https://<host>:<port>`
- **Number of URLs to fetch per iteration**: on every iteration, every instanced thread will pick this number of random URLs from the domain list and perform a request on them.
- **Number of iterations**: every thread will repeat the whole process this number of times.
- **Number of threads**: number of threads that will perform tot requests tot times.