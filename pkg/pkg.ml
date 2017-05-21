#!/usr/bin/env ocaml
#use "topfind"
#require "topkg"
open Topkg

let () =
  Pkg.describe "mirage-net-unix" @@ fun c ->
  Ok [
    Pkg.mllib "lib/mirage-net-unix.mllib";
    Pkg.clib "lib/libnetif_stubs.clib";
  ]
