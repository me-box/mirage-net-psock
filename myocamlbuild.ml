open Ocamlbuild_plugin

let () =
  dispatch begin function
  | After_rules ->
      flag ["link"; "library"; "ocaml"; "byte"; "use_psoc"]
        (S ([A "-dllib"; A "-lnetif_stubs"]));
      flag ["link"; "library"; "ocaml"; "native"; "use_psoc"]
        (S ([A "-cclib"; A "-lnetif_stubs"]));
      flag ["link"; "ocaml"; "link_psoc"]
        (A "lib/libnetif_stubs.a");
      dep ["link"; "ocaml"; "use_psoc"]
        ["lib/libnetif_stubs.a"];
  | _ -> ()
  end
