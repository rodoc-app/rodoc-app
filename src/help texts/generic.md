# Welcome to rodoc

This is a brief introduction to this application for organizing family trees. Each **family tree** is saved and organized in a **project**.

## Project

Each **project** is stored in a specific directory on a filesystem, assuming that the entire subtree of the project root directory should not be used for different purposes.

Various kinds of entities can be used throughout the project to express all the intended information related not just to persons and their relations.

## Persons

Each family tree primarily contains a list of **persons**, each having basic information about themselves, such as a **name**, **surname**, etc. Each person can also have multiple user-defined **tags**.

## Relations

To bind two particular **persons** together, we use the concept of a **relation** to record their real-world interconnection.

## Events

**Persons** may also attend various types of events, such as weddings, allowing these to be associated with a **place**, **date**, as well as allowing the persons to participate in specific roles.

## Files

Finally, each **person** can have an unlimited number of physical files of arbitrary types, logically divided into three groups of **files**, **media**, and **notes**.
