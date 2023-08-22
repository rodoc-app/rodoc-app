# Person

Each **person** has a set of values that can be edited:

- **Titles** before and after a full name (e.g., *M.Sc.* or *Ph.D.*)
- **Name** (e.g., *John*)
- **Surname** (e.g., *Swanson*)
- **Birthname** (e.g., *Smith*)
- **Gender** (e.g., *Male*)
- **Date of birth** (e.g., *23. 2. 2000*)
- **Place of birth** (e.g., *Prague*)
- Flag for an **alive** person, otherwise a **date** and **place of death**

## Dates

Every **date** value can actually be:

- **Unknown** as a special value for missing information
- Just a **single date** (e.g., *23. 2. 2000*)
- **Range** of two dates (e.g., *From: 23. 2. 2000 to: 25. 2. 2000*)

Each date can also be associated with an optional **note** allowing to hold any information intended.

## Tags

**Tag** is a pair of a **name** and its **value**. Each person can be associated with an arbitrary number of such tags. However, they need to be unique. Whenever a new tag name is used for the first time, it becomes globally available within the entire project.
