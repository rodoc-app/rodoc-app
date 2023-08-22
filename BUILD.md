## Závislosti

- Je třeba mít staženou knihovnu *Qt*.
- Je nutné mít stažený *Meson build system*.
- Samozřejmostí je překladač kódu `C++` a backend build system, jako je třeba `ninja`.
- Pro dokumentaci je třeba mít `doxygen`.

## Kompilace

- Ke kompilaci lze použít `autobuild.sh`, nebo ručně:

```sh
cd src/
meson setup build/
meson compile -C build/

# Pro instalaci
meson install -C build/
```

- Nebo lze také použít script `deploy.bat`, který kód kompiluje a připraví na deployment.

## Doxygen

- Opět lze použít skript s příslušným příznakem `./autobuild.sh doc` (provede se i build). Nebo:

```sh
cd src/
doxygen config.ini
cd ..

# Nyní v adresáři docs/ je dokumentace.
# V docs/html jsou stránky.
# V docs/latex/ potom latexová dokumentace.
cd docs/latex
make # pro vytvoření refman.pdf dokumentace
```
