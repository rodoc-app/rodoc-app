This is the main page for documentation of the program Rodoc.

The code is separated mainly into two parts:

1. Core
2. Graphics

Core is for working with the family tree and also for parsing files.

Graphics is just the graphical interface of the application.

This application uses [Jsoncpp library](https://open-source-parsers.github.io/jsoncpp-docs/doxygen/), [Qt graphics library](https://www.qt.io/) with version 6.

Database is a group of `json` files and directories. It follows this predefined structure (`#` is for a comment):

```text
[rootDir]          # Chosen by the user.
  - Persons.json   # All persons.
  - Config.json    # Configuration and templates.
  - Events.json    # Events.
  - Relations.json # Relations
  - Files.json     # Files metadata.
  - Media.json     # Media metadata.
  - Notes.json     # Notes metadata.
  - Media          # Directory for media.
    - [files]
  - Notes          # Directory for notes.
    - [file]
  - Files          # Directory for other files.
    - [files]
```

Also `Files`, `Notes` and `Media` are internally implemented the same way. The main difference is for the end user.
