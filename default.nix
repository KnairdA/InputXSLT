{ system ? builtins.currentSystem }:

let
  pkgs   = import <nixpkgs> { inherit system; };
  stdenv = pkgs.stdenv;

in stdenv.mkDerivation rec {
  name = "input-xslt";

  src = pkgs.lib.cleanSource ./.;

  buildInputs = with pkgs; [
    cmake boost xalanc xercesc discount
  ];

  meta = with stdenv.lib; {
    description = "InputXSLT";
    homepage    = https://tree.kummerlaender.eu/projects/xslt/input_xslt/;
    license     = stdenv.lib.licenses.asl20;
  };

  shellHook = ''
    export NIX_SHELL_NAME="${name}"
  '';
}
