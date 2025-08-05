{
  description = "GBOS";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs";
  inputs.gbdk.url = "github:cleggacus/nix-gbdk-2020";

  outputs = { self, nixpkgs, gbdk, ... }:
  let
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
  in
  {
    devShell.x86_64-linux = pkgs.mkShell {
      buildInputs = [
        gbdk.packages.x86_64-linux.default
        pkgs.glibc
      ];

      shellHook = ''
        export PATH=${gbdk.packages.x86_64-linux.default}/bin:$PATH
        export LD_LIBRARY_PATH="${pkgs.stdenv.cc.cc.lib}/lib"
        export GBDK_HOME=${gbdk.packages.x86_64-linux.default}/
      '';
    };

    packages.x86_64-linux.default = pkgs.stdenv.mkDerivation {
      pname = "GBOS";
      version = "0.1";
      src = ./.;
      buildInputs = [ gbdk.packages.x86_64-linux.default pkgs.glibc ];

      buildPhase = ''
        export PATH=${gbdk.packages.x86_64-linux.default}/bin:$PATH
        export LD_LIBRARY_PATH="${pkgs.stdenv.cc.cc.lib}/lib"
        export GBDK_HOME=${gbdk.packages.x86_64-linux.default}/
        make
      '';

      installPhase = ''
        mkdir -p $out/bin
        cp obj/GBOS.gb $out/bin/
      '';

      cleanPhase = ''
        make clean
      '';

      meta = {
        description = "Game Boy OS built with GBDK";
        license = pkgs.lib.licenses.mit;
      };
    };
  };
}
