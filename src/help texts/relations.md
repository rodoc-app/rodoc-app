# Relations

**Relation** entity allows to bind two particular persons together to represent their real-world relationship. Each relation has these two pieces of information:

- **First person** (e.g., *John Swanson*)
- **Second person** (e.g., *Roy Swanson*)

In order not to be restrained by a hard-wired list of permitted relations, the concept of a **relation template** was introduced.

## Relation Templates

Each template has the following characteristics:

- **Title** (e.g., *Fatherhood*)
- **Description** (e.g., *Relation between a father and a child*)
- Name of the **first person** (e.g., *father*)
- Name of the **second person** (e.g., *child*)
- **Generation difference** (e.g., *-1*)
- **Trait** (e.g., *Fatherhood*)

### Generation Difference

**Generation difference** describes which person is older and which one is younger. Any number can be possibly provided, but only **-1**, **0**, and **1** usually make sense.

- **-1** means that the first person is older,
- **0** means that both persons are in the same generation
- **1** means that the first person is younger.

### Trait

**Trait** determines how a particular **relation** is intended to be interpretted. The given list of possibilities shadows the real world.

- **No special trait** (e.g., *Uncle-Nephew*)
- **Motherhood** (e.g., *Mother-Daughter*)
- **Fatherhood** (e.g., *Father-Son*)
- **Partnership** (e.g., *Marriage*)
- **Sibling** (e.g., *Brother-Sister*)

### Predefined Templates

Relation templates must be created first before they can be used. While it is possible to introduce specific user-defined templates, several widely used templates were already predefined and are available for import:

- **Fatherhood**
- **Motherhood**
- **Adoptive motherhood**
- **Adoptive fatherhood**
- **Partnership**
- **Marriage**
- **Brotherhood**
- **Sisterhood**
- **Siblings**

## Promotion

When a **person** has several **relations** of the same and supported **trait** (e.g., *Father*, *Mother*, *Partner*, ...), one of them can be declared as **promoted**, i.e., considered as the main one.

## Suggestions

After a new **relation** is created, several **suggestions** for additional plausible relations that can be inferred are automatically offered in order to smooth the entire process.

- **Partnership relation** -> share children between both of them
- **Sibling relation** -> share parents
- **Parent relation** -> children are bound as siblings, partners as parents, and vice versa
