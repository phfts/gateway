# To run

1. Configure routes.yml
```
global-rate-limit:
  time_unit: minute
  time_interval: 1
  max_requests: 120
  identification_methods:
    - IP