let
  pkgs = import <nixpkgs> {
    overlays = [
      (import ./gbdk.nix)
    ];
  };
in
pkgs.mkShell {
  buildInputs = [
    pkgs.gbdk-2020
    pkgs.glibc
    pkgs.stdenv.cc.cc.lib
  ];

  shellHook = ''
    export PATH=${pkgs.gbdk-2020}/bin:$PATH
    export LD_LIBRARY_PATH="${pkgs.stdenv.cc.cc.lib}/lib"
    export GBDK_HOME=${pkgs.gbdk-2020}/
  '';
}
