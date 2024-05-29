# To run

1. Configure routes.yml
```
global-rate-limit:
  time_unit: minute
  time_interval: 1
  max_requests: 120
  identification_methods:
    - IP
```

2. Download dependencies
```
$ make download
```

3. Install openssl and check SSL_INCLUDE_PATH and SSL_LIB_PATH in `Makefile`
```
# osx users can use brew
$ brew install openssl 
```

3. Run
```
$ make run
```