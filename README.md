# `rodoc`

[TODO] Add some text about the app.

# Build instructions

## Dependencies

- It is necessary to have UI library *Qt*.
- *Meson build system* for building from source.
- Also `C++` and a backend build system, preferably `ninja`.
- For creating documentation `doxygen`.

## Compilation

On Linux OS one can use script `autobuild.sh` which also accepts some additional options. Such as `help`, `doc`, `run` and `install`. Alternatively one can build the app from source by running the following commands:

```sh
cd src/
meson setup build/
meson compile -C build/
```

And for installing the app.

```sh
meson install -C build/
```

Also there is a Windows script `deploy.bat`, which compiles the source and prepare the app for release and installator.

## Doxygen

For documentation one can either call `./autobuild.sh doc` or manually:

```sh
cd src/
doxygen config.ini
cd ..
```

Now in `docs/` is documentation and `docs/html` are pages and `docs/latex` is prepared for PDF documentation, which can be done by calling:

```sh
cd docs/latex
make
```

And `refman.pdf` is generated PDF documentation.
