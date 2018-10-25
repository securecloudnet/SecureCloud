
Debian
====================
This directory contains files used to package securecloudd/securecloud-qt
for Debian-based Linux systems. If you compile securecloudd/securecloud-qt yourself, there are some useful files here.

## securecloud: URI support ##


securecloud-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install securecloud-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your securecloudqt binary to `/usr/bin`
and the `../../share/pixmaps/securecloud128.png` to `/usr/share/pixmaps`

securecloud-qt.protocol (KDE)

