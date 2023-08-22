/// @file strings.h Is file containing all English strings.
#ifndef strings_h_
#define strings_h_

#include <string>

/// Empty string.
const std::string EMPTY_STRING = "";

// =====================================================================
// Person strings
// =====================================================================

/// Namespace for strings related to a person.
namespace person{
    /// String for unknown tags.
    const std::string UNKNOWN_STR = "Unknown";
    /// String for default surname.
    const std::string SURNAME_STR = "Surname";
    /// String for default name.
    const std::string NAME_STR = "Name";
    /// String for default root directories.
    const std::string ROOT = "root";
    /// String for date interval from.
    const std::string DATE_FROM = "From: ";
    /// String for date interval to.
    const std::string DATE_TO = " to: ";
}

// =====================================================================
// Tag chooser strings
// =====================================================================

/// Namespace for strings shown in tag chooser dialog.
namespace tagchooser{
    /// Last custom tag choice.
    const std::string CUSTOM_TAG = "+ Custom tag";
}

// =====================================================================
// Add file
// =====================================================================

/// Namespace for strings shown in add file dialog.
namespace addfile{
    /// String for no extension.
    const std::string NONE = "None";
}

// =====================================================================
// General buttons texts.
// =====================================================================

/// Namespace for general texts on buttons.
namespace button{
    /// Text for adding new element.
    const std::string ADD_NEW = "Add new";
    /// Text for cancelling.
    const std::string CANCEL = "Cancel";
    /// text for removing element.
    const std::string REMOVE = "Remove";
}

// =====================================================================
// Exceptions and errors strings.
// =====================================================================

/// Namespace for errors and exceptions messages.
namespace error{
    /// String for catching unknown exception.
    const std::string UNKNOWN_EXCEP_STR = "Unknown exception catch.";
    /// String for adding before exception description.
    const std::string EXCEPTION_STR = "Exception: ";
    /// String shown when database is corrupted.
    const std::string CORRUPTED_DATABASE = "Given database is corrupted: ";
    /// String for showing that some boxes were left empty.
    const std::string EMPTY_BOXES = "All boxes must not be empty.";
    /// No file chosen and empty box.
    const std::string NO_CHOSEN_FILE = "No file was chosen.";
    /// When person tries to add "no role" role, which is allocated for no roles.
    const std::string NO_ROLE = "Given role 'no role' cannot be used. It would be considered as no special role.";
    /// When in any box where inserted forbidden characters.
    const std::string FORBIDDEN_CHARS = "Text cannot contain forbidden characters. &lt; &gt; \"";
}

// =====================================================================
// Default relations.
// =====================================================================

/// Namespace for strings for default relations.
namespace relation{
    /// String for Fatherhood.
    const std::string FATHERHOOD = "Fatherhood";
    /// String for Fatherhood description.
    const std::string FATHERHOOD_DESC = "Relation between father and his child.";
    /// String for father.
    const std::string FATHER = "Father";
    /// String for child.
    const std::string CHILD = "Child";
    /// String for Motherhood.
    const std::string MOTHERHOOD = "Motherhood";
    /// String for Motherhood description.
    const std::string MOTHERHOOD_DESC = "Relation between mother and her child.";
    /// String for mother.
    const std::string MOTHER = "Mother";
    /// String for Partnership.
    const std::string PARTNERSHIP = "Partnership";
    /// String for description of partnership.
    const std::string PARTNERSHIP_DESC = "Relation between two partners.";
    /// String for partner.
    const std::string PARTNER = "Partner";
    /// String for Marriage.
    const std::string MARRIAGE = "Marriage";
    /// String for Marriage description.
    const std::string MARRIAGE_DESC = "Bound by law between two people. Usually a man (husband) and a woman (wife).";
    /// String for wife.
    const std::string WIFE = "Wife";
    /// String for husband.
    const std::string HUSBAND = "Husband";
    /// String for title for adoptive fatherhood.
    const std::string ADOPTIVE_FATHERHOOD = "Adoptive fatherhood";
    /// String for description for adoptive fatherhood.
    const std::string DESC_ADOPTIVE_FATHERHOOD = "Fatherhood provided by adopting a child.";
    /// String for adoptive father.
    const std::string ADOPTIVE_FATHER = "Adoptive father";
    /// String for adopted child.
    const std::string ADOPTED_CHILD = "Adopted child";
    /// String for title for adoptive motherhood
    const std::string ADOPTIVE_MOTHERHOOD = "Adoptive motherhood";
    /// String for description for adoptive motherhood.
    const std::string DESC_ADOPTIVE_MOTHERHOOD = "Motherhood provided by adopting a child.";
    /// String for adoptive father.
    const std::string ADOPTIVE_MOTHER = "Adoptive mother";
    /// String for brotherhood.
    const std::string BROTHERHHOD = "Brotherhood";
    /// String for brotherhood description.
    const std::string DESC_BROTHERHOOD = "Relation between two brothers.";
    /// String for brother.
    const std::string BROTHER = "Brother";
    /// String for sisterhood.
    const std::string SISTERHOOD = "Sisterhood";
    /// String for sisterhood description.
    const std::string DESC_SISTERHOOD = "Relation between two sisters.";
    /// String for sister.
    const std::string SISTER = "Sister";
    /// String for siblings.
    const std::string SIBLINGS = "Siblings";
    /// String for siblings description.
    const std::string DESC_SIBLINGS = "Relation between brother and sister.";
}

// =====================================================================
// Default events.
// =====================================================================

/// Namespace for strings for default events.
namespace events{
    /// String for wedding.
    const std::string WEDDING = "Wedding";
    /// String for description of wedding.
    const std::string DESC_WEDDING = "A social event mainly between soon to be wife and husband.";;
    /// String for groom.
    const std::string GROOM = "Groom";
    /// String for bride.
    const std::string BRIDE = "Bride";
    /// String for best man.
    const std::string BEST_MAN = "Best man";
    /// String for maid of honor.
    const std::string MAID_OF_HONOR = "Maid of Honor";
    /// String for divorce.
    const std::string DIVORCE = "Divorce";
    /// String for divorce description.
    const std::string DESC_DIVORCE = "Divorce is the process of terminating a marriage or marital union.";
    /// String for former husband.
    const std::string FORMER_HUSBAND = "Former husband";
    /// String for former wife.
    const std::string FORMER_WIFE = "Former wife";
    /// String for funeral.
    const std::string FUNERAL = "Funeral";
    /// String for funeral description.
    const std::string DESC_FUNERAL = "A funeral is a ceremony connected with the final disposition of a corpse, such as a burial or cremation, with the attendant observances.";
    /// String for dead person.
    const std::string DEAD_PERSON = "Dead person";
    /// String for graduation.
    const std::string GRADUATION = "Graduation";
    /// String for graduation description.
    const std::string DESC_GRADUATION = "Graduation is the awarding of a diploma to a student by an educational institution.";
    /// String for graduate.
    const std::string GRADUATE = "Graduate";
    /// String for no role in event.
    const std::string NO_ROLE = "no role";
}

// =====================================================================
// HTML Tags and everything with html.
// =====================================================================

/// Namespace for html tags and everything with html.
namespace html{
    /// Where will resources be saved.
    const std::string RESOURCES = "resources";
    /// Prefix for close family diagram.
    const std::string PREFIX_CLOSE = "/close-";
    /// Prefix for family tree diagram.
    const std::string PREFIX_FAMILY_TREE = "/tree-";
    /// Custom CSS file name.
    const std::string CUSTOM_CSS = "styles.css";
    /// General information.
    const std::string GENERAL_INFO = "General information";
    /// Birth date.
    const std::string BIRTH_DATE = "Birth date: ";
    /// Birth place.
    const std::string BIRTH_PLACE = "Birth place: ";
    /// Date of death.
    const std::string DEATH_DATE = "Date of death: ";
    /// Place of death.
    const std::string DEATH_PLACE = "Place of death: ";
    /// Gender.
    const std::string GENDER = "Gender: ";
    /// Custom tags.
    const std::string CUSTOM_TAGS = "Custom tags:";
    /// Description.
    const std::string DESCRIPTION = "Description:";
    /// Note.
    const std::string NOTE = "Note:";
    /// Date.
    const std::string DATE = "Date: ";
    /// Place.
    const std::string PLACE = "Place: ";
    /// Persons.
    const std::string PERSONS = "Persons:";
    /// All persons.
    const std::string ALL_PERSONS = "All persons";
    /// Events.
    const std::string EVENTS = "Events";
    /// Family tree.
    const std::string FAMILY_TREE = "Family tree";
    /// Family tree diagram.
    const std::string FAMILY_TREE_DIAGRAM = "Family tree diagram";
    /// Relations.
    const std::string RELATIONS = "Relations";
    /// Close family.
    const std::string CLOSE_FAMILY = "Close family";
    /// Close family diagram.
    const std::string CLOSE_FAMILY_DIAGRAM = "Close family diagram";
}

// =====================================================================
// JSON Labels.
// =====================================================================

/// Namespace for all JSON labels.
namespace jsonlabel{
    /// JSON Label for title.
    const std::string TITLE = "title";
    /// JSON Label for id.
    const std::string ID = "id";
    /// JSON Label for description.
    const std::string DESCRIPTION = "description";
    /// JSON Label for if the event contains a date or not.
    const std::string CONTAINS_DATE = "contains date";
    /// JSON Label for if the event contains more people.
    const std::string MORE_PEOPLE = "more people";
    /// JSON Label for if the event has special roles.
    const std::string SPECIAL_ROLES = "special roles";
    /// JSON Label for name.
    const std::string NAME = "name";
    /// JSON Label for first name.
    const std::string FIRST_NAME = "first name";
    /// JSON Label for second name.
    const std::string SECOND_NAME = "second name";
    /// JSON Label for trait.
    const std::string TRAIT = "trait";
    /// JSON Label for generation difference.
    const std::string  GEN_DIFF = "generation difference";
    /// JSON Label for relations templates.
    const std::string REL_TEMPL = "relation templates";
    /// JSON Label for event templates.
    const std::string EVE_TEMPL = "event templates";
    /// JSON Label for type.
    const std::string TYPE = "type";
    /// JSON Label for date.
    const std::string DATE = "date";
    /// JSON Label for last date.
    const std::string LAST_DATE = "last date";
    /// JSON Label for text.
    const std::string TEXT = "text";
    /// JSON Label for tags.
    const std::string TAGS = "tags";
    /// JSON Label for single tag.
    const std::string TAG = "tag";
    /// JSON Label for relations.
    const std::string RELATIONS = "relations";
    /// JSON Label for events.
    const std::string EVENTS = "events";
    /// JSON Label for surname.
    const std::string SURNAME = "surname";
    /// JSON Label for maiden name.
    const std::string MAIDEN_NAME = "maiden name";
    /// JSON Label for value.
    const std::string VALUE = "value";
    /// JSON Label for birth.
    const std::string BIRTH = "birth";
    /// JSON Label for death.
    const std::string DEATH = "death";
    /// JSON Label for virtual drive.
    const std::string VIRTUAL_DRIVE = "virtual drive";
    /// JSON Label for media
    const std::string MEDIA = "media";
    /// JSON Label for notes.
    const std::string NOTES = "notes";
    /// JSON Label for files.
    const std::string FILES = "files";
    /// JSON Label for persons.
    const std::string PERSONS = "persons";
    /// JSON Label for for template.
    const std::string TEMPLATE = "template";
    /// JSON Label for for templates.
    const std::string TEMPLATES = "templates";
    /// JSON Label for first person.
    const std::string FIRST_PERSON = "first person";
    /// JSON Label for second person.
    const std::string SECOND_PERSON = "second person";
    /// JSON Label for place.
    const std::string PLACE = "place";
    /// JSON Label for role.
    const std::string ROLE = "role";
    /// JSON Label for person.
    const std::string PERSON = "person";
    /// JSON Label for gender.
    const std::string GENDER = "gender";
    /// JSON Label for if the person lives or not.
    const std::string LIVES = "lives";
    /// JSON Label for title in front of a name.
    const std::string FRONT_TITLE = "title in front";
    /// JSON Label for title after the name.
    const std::string AFTER_TITLE = "title after";
    /// JSON Label for father.
    const std::string FATHER = "father";
    /// JSON Label for mother.
    const std::string MOTHER = "mother";
    /// JSON Label for partner.
    const std::string PARTNER = "partner";
    /// JSON Label for setting moveable tabs.
    const std::string MOVEABLE_TABS = "moveable tabs";
    /// JSON Label for tab position.
    const std::string TAB_POSITION = "tab position";
    /// JSON Label for global main person.
    const std::string GLOBAL_MAIN_PERSON = "global main person";
    /// JSON Label if an event template contains a place.
    const std::string CONTAINS_PLACE = "contains place";
    /// JSON Label for app settings if general tab should be visible.
    const std::string SHOW_GENERAL_TAB = "show general tab";
    /// JSON Label for app settings if event tab should be visible.
    const std::string SHOW_EVENT_TAB = "show event tab";
    /// JSON Label for app settings if file tab should be visible.
    const std::string SHOW_FILE_TAB = "show file tab";
    /// JSON Label for app settings if media tab should be visible.
    const std::string SHOW_MEDIA_TAB = "show media tab";
    /// JSON Label for app settings if note tab should be visible.
    const std::string SHOW_NOTE_TAB = "show note tab";
    /// JSON Label for app settings if relation tab should be visible.
    const std::string SHOW_RELATION_TAB = "show relation tab";
    /// JSON Label for app settings.
    const std::string APP_SETTINGS = "app settings";
    /// JSON Label for drives.
    const std::string DRIVES = "drives";
    /// JSON Label for export.
    const std::string EXPORT = "export";
    /// JSON Label for red.
    const std::string RED = "red";
    /// JSON Label for blue.
    const std::string BLUE = "blue";
    /// JSON Label for green.
    const std::string GREEN = "green";
    /// JSON Label for font family.
    const std::string FONT_FAMILY = "font family";
    /// JSON Label for font size.
    const std::string FONT_SIZE = "font size";
    /// JSON Label highlighted color.
    const std::string HIGHLIGHTED_COLOR = "highlighted color";
    /// JSON Label for proband color.
    const std::string PROBANT_COLOR = "proband color";
    /// JSON Label promoted color.
    const std::string PROMOTED_COLOR = "promoted color";
    /// JSON Label standard color.
    const std::string STANDARD_COLOR = "standard color";
    /// JSON Label for size in the x axis.
    const std::string X_SIZE = "x size";
    /// JSON Label for size in the y axis.
    const std::string Y_SIZE = "y size";
    /// JSON Label for how many generations are being shown upwards.
    const std::string GEN_SIZE_UP = "number of generations up";
    /// JSON Label for how many generations are being shown downwards.
    const std::string GEN_SIZE_DOWN = "number of generations down";
    /// JSON Label for directory.
    const std::string DIR = "dir";
    /// JSON Label for root.
    const std::string ROOT = "root";
    /// JSON Label for help.
    const std::string HELP = "help";
    /// JSON Label for splitter position one.
    const std::string SPLITTER_POSITION_ONE = "splitter position one";
    /// JSON Label for splitter position two.
    const std::string SPLITTER_POSITION_TWO = "splitter position two";
    /// JSON Label for the color of text.
    const std::string TEXT_COLOR = "text color";
    /// JSON Label for the line color.
    const std::string LINE_COLOR = "line color";
    /// JSON Label for line width.
    const std::string LINE_WIDTH = "line width";
    /// JSON Label border radius.
    const std::string BORDER_RADIUS = "radius";
}

#endif
