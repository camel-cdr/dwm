# My custom dwm build

## Patches
* center
* fakefullscreen
* noborder
* pertag
* stacker
* swallow
* vanitygaps

## Install
```sh
git clone https://gitlab.com/CamelCoder/dwm
cd dwm
sudo make clean install
```

## Color Emoji support
For color emoji support install libxft with the bgra patch.

### On Arch based systems
```sh
git clone https://aur.archlinux.org/libxft-bgra.git
cd libxft-bgra
# Libxft-bgra isn't properly packaged, its therefore needed to replace the architecture manually.
# This can be done by replacing <<ARCH>> with your architecture and runnung the following command:
# sed -i "s/x86_64/<<ARCH>>/g" PKGBUILD
makepkg -si
```

### Other
```sh
git clone --depth=1 https://gitlab.com/zanc/xft
cd xft && ./configure && sudo make instal
sudo ldconfig && reboot
```


## Statusbar
The statusbar is can be configured using `xsetroot -name "..."`.


## Documentation / Keybinds
```sh
man dwm
```
