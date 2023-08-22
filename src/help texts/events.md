# Events

**Event** entity allows to describe a particular event and associate it with the following pieces of information:

- **Date** (e.g., *20. 7. 2023*)
- **Place** (e.g., *Prague, Czech Republic*)
- **List of an arbitrary number of persons** acting as participants

In order to make the concept of events more generic and not limit ourselves to certain hard-wired event types only, each event is derived from a particular **event template**.

## Event Templates

Each template has the following characteristics:

- **Title** (e.g., *Wedding*)
- **Description** (e.g., *A social event mainly between soon to be wife and husband.*)
- Flag activating a **date** (e.g., *enabled*)
- Flag activating a **place** (e.g., *disabled*)
- Arbitrary number of **roles** allowing to distinguish individual **participating persons** (e.g., *groom*, *bride*, ...)
- Flag allowing **additional participating persons** without any roles assigned

## Predefined Templates

Event templates must be created first before they can be used. While it is possible to introduce specific user-defined templates, several widely used templates were already predefined and are available for import:

- **Wedding**
- **Divorce**
- **Funeral**
- **Graduation**
