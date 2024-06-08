/* othello-ai - A othello playing ai using the minmax algorithm.
 * Copyright (C) 2024  Jonatan Wincent <jonatan.wincent@proton.me>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 {
  description = "";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { nixpkgs, utils, ... }: 
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
        othello-ai = (with pkgs; stdenv.mkDerivation {
          name = "othello-ai";
          src = ./.;

          buildPhase = "make";
          installPhase = ''
            mkdir -p $out/bin
            mv ./othello-ai $out/bin
          '';
        });
      in rec {
        defaultApp = utils.lib.mkApp {
          drv = defaultPackage;
        };
        defaultPackage = othello-ai;
        devShell = pkgs.mkShell {
          packages = with pkgs; [
            cmake
            valgrind
          ];
          shellHook = ''
            exec zsh
          '';
        };
      }
    );
}
