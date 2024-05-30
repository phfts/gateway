FROM alpine:latest
RUN apk update \
&& apk upgrade \
&& apk add --no-cache \
  clang \
  clang-dev \
  alpine-sdk \
  dpkg \
  cmake \
  ccache \
  openssl-dev

WORKDIR /app
COPY . .
RUN make build-deps

ENV REDIS_HOST tcp://redis:6379

CMD [ "make", "run" ]


