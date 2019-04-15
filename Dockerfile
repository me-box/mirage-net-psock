FROM ocaml/opam2:alpine-3.9-ocaml-4.07
MAINTAINER Richard Mortier <mort@cantab.net>

RUN sudo apk --no-cache add --update \
    alpine-sdk \
    autoconf \
    bash \
    gmp-dev \
    linux-headers \
    perl

RUN opam install -y dune
WORKDIR /mirage-net-psock
ADD . .
RUN sudo chown opam: -R .
RUN opam config exec -- \
    dune external-lib-deps --missing @all 2>&1 | tail -1 | cut -f3- -d" " | sh

ENTRYPOINT [ "opam", "config", "exec", "--" ]
