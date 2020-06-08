# My custom dwm build

## Patches
* center
* fakefullscreen
* noborder
* pertag
* stacker
* swallow
* vanitygaps

## Emoji support
For emoji support install libxft with the bgra patch.

### On Arch based systems
```sh
git clone https://aur.archlinux.org/packages/libxft-bgra/
cd libxft-bgra
# Libxft-bgra isn't properly packaged, its therefore needed to replace the architecture manually.
# This can be done by replacing <<ARCH>> with your architecture and runnung the following command:
# sed -i "s/x86_64/<<ARCH>>/g" PKGBUILD
makepkg -si
```

### Other
```sh
git clone https://gitlab.com/zanc/xft
./configure && sudo make instal
sudo ldconfig && reboot
```

## Install
```sh
git clone https://gitlab.com/CamelCoder/dwm
cd dwm
sudo make clean install
```

## Statusbar
The statusbar is can be configured using `xsetroot -name "..."`.


## Documentation / Keybinds
```sh
man dwm
```
