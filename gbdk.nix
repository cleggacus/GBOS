self: super: {
  gbdk-2020 = super.stdenv.mkDerivation rec {
    pname = "gbdk-2020";
    version = "4.4.0";

    src = super.fetchurl {
      url = "https://github.com/gbdk-2020/gbdk-2020/releases/download/4.4.0/gbdk-linux64.tar.gz";
      sha256 = "1bgd2mch35vnxflg2ac3yqjv8x80j3kp6kf1q9rzmk0hfrv2waca";
    };

    nativeBuildInputs = [ super.patchelf ];

    phases = [ "unpackPhase" "installPhase" "fixupPhase" ];

    installPhase = ''
      mkdir -p $out
      cp -r * $out/
    '';

    fixupPhase = ''
      find $out/bin -type f -exec file {} \; | grep "ELF" | cut -d: -f1 | while read bin; do
        echo "Patching $bin"
        patchelf --set-interpreter ${super.glibc}/lib/ld-linux-x86-64.so.2 \
                 --set-rpath ${super.glibc}/lib \
                 "$bin"

        echo "Patching cc1"
        patchelf --set-interpreter ${super.glibc}/lib/ld-linux-x86-64.so.2 \
                 --set-rpath ${super.stdenv.cc.cc.lib}/lib:${super.glibc}/lib \
                 $out/libexec/sdcc/cc1
      done
    '';

    meta = with super.lib; {
      description = "Game Boy Development Kit 2020 (prebuilt)";
      homepage = "https://github.com/gbdk-2020/gbdk-2020";
      license = licenses.gpl3Plus;
      maintainers = with maintainers; [ ];
      platforms = platforms.linux;
    };
  };
}

