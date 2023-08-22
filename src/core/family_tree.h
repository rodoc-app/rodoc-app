/// @file family_tree.h Header file for Family tree.
#ifndef family_tree_
#define family_tree_

#include <string>
#include <sstream>
#include <vector>
#include <ostream>
#include <map>
#include <set>
#include <memory>
#include <functional>
#include <json/json.h>
#include "file_parser.h"
#include "config.h"
#include "date.h"
#include "person.h"
#include "strings.h"
#include "family_tree_items.h"

/// Main class for holding all data of a family tree. Also it acts as a bridge to the core of the application.
class FamilyTree{
	public:
		/// Default constructor.
		FamilyTree();
		/// Add new event to the family tree.
		/// @return Pointer to the new event.
		Event* addEvent();
		/// Add new event to the family tree with known index.
		/// @param index Known index.
		/// @return Pointer to the new event.
		Event* addEvent(size_t index);
		/// Add file to the database.
		/// @param name Given name of the file.
		/// @param type If the file is general file, media or note.
		/// @return Pointer to the new file.
		File* addFile(const std::string& name, FileType type = GENERAL_FILE);
		/// Add file to the database with known index.
		/// @param name Given name of the file.
		/// @param index Known index.
		/// @param type If the file is general file, media or note.
		/// @return Pointer to the new file.
		File* addFile(const std::string& name, size_t index, FileType type = GENERAL_FILE);
		/// Add person to the database.
		/// @return Pointer to that new person.
		Person* addPerson();
		/// Add person to the database with known index.
		/// @param index Known index.
		/// @return Pointer to that new person.
		Person* addPerson(size_t index);
		/// Add relation to the family tree.
		/// @return Pointer to the new relation.
		Relation* addRelation();
		/// Add relation to the family tree with known index.
		/// @param index Known index.
		/// @return Pointer to the new relation.
		Relation* addRelation(size_t index);
		/// Append the whole project by another one. Note that the second family tree will be corrupted.
		/// Because all the indexes must be changed. Though the second tree won't be saved.
		/// @param other Second Family tree.
		void append(FamilyTree& other);
		/// Check the consistence of stored file.
		/// @return Bool if it is consistent and written details of all wrong files.
		std::pair<bool, std::string> checkFileConsistence();
		/// Clear all data and set it for a new project.
		void clear();
		/// Clear all project paths except for this one.
		void clearProjectPaths();
		/// Copy an existing file.
		/// @param filePath Path to the original file on the disk.
		/// @param type What type of file it is.
		/// @return Id of the new file.
		size_t copyFile(const std::string& filePath, FileType type = GENERAL_FILE);
		/// Create a new empty database.
		/// @param dirPath Path to the root directory.
		void createDatabase(const std::string& dirPath);
		/// Create an empty file.
		/// @param name Name of the file. Not the real name on disk.
		/// @param type What type of file it is.
		/// @return Id of the new file.
		size_t createEmptyFile(const std::string& name, FileType type = GENERAL_FILE);
		/// Switch if the help window should be opened at start up, also write it to configuration.
		/// @param help If it should be opened or not.
		void enableHelpOnStartup(bool help);
		/// Export this project in one single JSON file.
		/// @param filePath Path to the export file.
		/// @param includingEvents If the export should include events.
		/// @param includingRelations If the export should include relations.
		void exportProject(const std::string& filePath, bool includingEvents, bool includingRelations);
		/// Create resource file for diagrams for HTML export.
		/// @param target Where the resource dir should be (either a path to the directory or a file).
		/// @return String representing the path to the resources directory.
		std::string createResourcesDir(const std::string& target);
		/// Export given templates to an output file in JSON format.
		/// @param eventTemplates Vector of event templates.
		/// @param relTemplates Vector of relation templates.
		/// @param filename Path to the output file.
		void exportTemplates(const std::vector<size_t>& eventTemplates, const std::vector<size_t>& relTemplates, const std::string& filename);
		/// Get pointer to the event.
		/// @param id Id of the event.
		/// @return Get optionally pointer to the event or empty.
		std::optional<Event*> getEvent(size_t id);
		/// Get constant reference to the container of all events.
		/// @return Constant reference to the map of all events.
		const std::map<size_t, std::unique_ptr<Event>>& getEvents() const;
		/// Get pointer to given file.
		/// @param id Id of the file.
		/// @param type What type of file to get.
		/// @return Optionally pointer to the file or empty.
		std::optional<File*> getFile(size_t id, FileType type = GENERAL_FILE);
		/// Get the absolute path to the given file.
		/// @param absolutePath Where will be the absolute paths stored.
		/// @param id Id of the file.
		/// @param type Which type is used.
        void getFileAbsolutePath(std::string& absolutePath, size_t id, FileType type = GENERAL_FILE);
		/// Get constant reference to the vector of all files.
		/// @param type What type of files the vector has.
		/// @return Constant reference to the map of files with given type.
		const std::map<size_t, std::unique_ptr<File>>& getFiles(FileType type = GENERAL_FILE);
        /// Get all the orphan files.
		/// @return Vector of all files, that will be permanently deleted.
		std::vector<std::string> getOrphanFiles();
		/// Get the main person.
		/// @return Pointer to the main person.
		Person* getMainPerson();
		/// Get the container of persons.
		/// @return Constant reference to the container.
		const std::map<size_t, std::unique_ptr<Person>>& getPersons();
		/// Get pointer to the person by its id.
		/// @param id Id of the person.
		/// @return Optionally pointer to the person with its id or empty.
		std::optional<Person*> getPerson(size_t id);
		/// Get the constant reference to the paths to the roots of projects.
		/// @return Constant reference to the vector of all strings representing root directories.
		const std::vector<std::string>& getProjectPaths() const;
		/// Get pointer to the relation.
		/// @param id Id of the relation.
		/// @return Optionally pointer to the relation or empty.
		std::optional<Relation*> getRelation(size_t id);
		/// Get the constant reference to the map of all relations.
		/// @return Constant reference to the map of all relations.
		const std::map<size_t, std::unique_ptr<Relation>>& getRelations();
		/// Get relation suggestions to the given relation.
		/// @param relId Id of the given relation.
		/// @return vector of all suggestions.
		std::vector<RelationSuggestion> getRelationSuggestions(size_t relId);
		/// Get pointer to the settings.
		/// @return Pointer to the settings.
		Settings* getSettings();
		/// Get string representing URL to local file.
		/// @param id Index of the given file.
		/// @param fileUrl URL of the file.
		/// @param type If the file is general file, media or note.
		void getUrlFile(size_t id, std::string& fileUrl, FileType type = GENERAL_FILE);
		/// Import project.
		/// @param filePath Path to the file.
		/// @return If the import was successful or not.
        bool importProject(const std::string& filePath);
        /// Import project from root directory.
        /// @param dirPath Path to the root directory.
        /// @return If the import was successful or not.
        bool importRootDir(const std::string& dirPath);
        /// Import templates.
        /// @param filePath Path to the file.
        /// @return If the import was successful or not and number of imported templates.
        std::pair<bool, size_t> importTemplates(const std::string& filePath);
		/// If the root directory for the database was already set.
		/// @return True if the directory was set.
		bool isDirectorySet();
		/// If all changes are saved.
		/// @return True if all changes are saved, otherwise false.
		bool isSaved() const;
		/// Log error code to the parser.
		/// @param error What is the text of the error.
		void log(const std::string& error);
		/// Open a database from its root directory.
		/// @param dirPath Path to the root directory.
		/// @param errorMessage To show what was the potential error.
		/// @param backupFile Which file has a backup.
		/// @return First if the parsing was successful. Second if there exists backup.
		std::pair<bool, bool> openDatabase(const std::string& dirPath, std::string& errorMessage, std::string& backupFile);
		/// Read the database without setting dir path.
		/// @param errorMessage If error occurred show it there.
		/// @return If the parsing of the database was successful or not.
		bool openDatabase(std::string& errorMessage);
		/// Open database from a single file.
		/// @param filePath Path to the file.
		/// @return If the parsing was successful or not.
		bool openSingleFileDatabase(const std::string& filePath);
		/// Print the whole project to an output directory.
		/// @param dirPath Path to the directory.
		void printHtml(const std::string& dirPath);
		/// Print main person in HTML format.
		/// @param outputFile Output file.
		void printMainPersonHtml(const std::string& outputFile);
		/// Promote a relationship to be the persons main. If possible it will be applied to both persons.
		/// @param id Id of the relation that is to be promoted.
		void promoteRelation(size_t id);
		/// Remove backup file if there is any.
		void removeBackup();
		/// Remove event from family tree.
		/// @param id Id of the event.
		void removeEvent(size_t id);
		/// Remove event template and all events based on this template.
		/// @param id Id of the template.
		void removeEventTemplate(size_t id);
		/// Remove files that no one uses.
		/// @return Number of deleted files.
		size_t removeOrphanFiles();
		/// Remove main person from the tree. Also delete all of his appearances.
		void removePerson();
		/// Remove relation from the family tree.
		/// @param id Id of the relation which will be removed.
		void removeRelation(size_t id);
		/// Remove relation template and all relations based on this template.
		/// @param id Id of the template.
		void removeRelationTemplate(size_t id);
		/// Rename given file.
		/// @param id Id of the file.
		/// @param newFilename New file name for the file.
		/// @param type What type is the file.
		void renameFile(size_t id, const std::string& newFilename, FileType type);
		/// Restore backup file from the database. Also load the project again.
		/// @param backupFile Which is the backup file.
		/// @param errorMessage If error occurred show it there.
		/// @return If the parsing of restored database was successful or not.
        bool restoreBackup(const std::string& backupFile, std::string& errorMessage);
		/// Set the main person.
		/// @param p New main person.
		void setMainPerson(Person* p);
		/// Set the main person.
		/// @param id Id of the person.
		void setMainPerson(size_t id);
		/// Set the data about one relation.
		/// @param relId Id of the relation to be edited.
		/// @param pers1Id Id of the first person.
		/// @param pers2Id Id of the second person.
		/// @param templId Id of the used template.
		void setRelation(size_t relId, size_t pers1Id, size_t pers2Id, size_t templId);
		/// Set unsaved state.
		void setUnsaved();
		/// If help dialog should be shown.
		/// @return True if it should be shown.
		bool showHelpOnStartup();
		/// Store the whole database to its files.
		/// @return If the storing was successful.
		bool storeDatabase();
		/// Get the number of events based on this event template.
		/// @param templateId Id of the event template.
		/// @return Number of events using this event template.
		size_t templatesBasedOnEventTemplate(size_t templateId);
		/// Get the number of relations based on this relation template.
		/// @param templateId Id of the relation template.
		/// @return Number of relations using this relation template.
		size_t templatesBasedOnRelationTemplate(size_t templateId);
		/// Update events based on the changed template.
		/// @param templ Id of the changed template.
		void updateEventsWithTemplate(size_t templ);
		/// Write to the configuration file and before add new one if it is not present.
		/// @param newPath Path to the new project.
		/// @return True if it was added. False if it was already present.
		bool writeConfig(const std::string& newPath);
	private:
	    /// All the events in the tree.
		std::map<size_t, std::unique_ptr<Event>> allEvents_;
	    /// All files in app.
		std::map<size_t, std::unique_ptr<File>> allFiles_;
		/// All media in app.
		std::map<size_t, std::unique_ptr<File>> allMedia_;
		/// All notes in app.
		std::map<size_t, std::unique_ptr<File>> allNotes_;
	    /// Map of all persons in family tree.
		std::map<size_t, std::unique_ptr<Person>> allPersons_;
		/// All relations in the tree.
		std::map<size_t, std::unique_ptr<Relation>> allRelations_;
		/// Check file consistence of a single type.
		/// @param os Where to write error.
		/// @param type Which type to look for.
		bool checkFileTypeConsistence(std::ostream& os, FileType type);
	    /// Last free index for event. Always start from 1.
		size_t event_index_;
	    /// Last free id for file. Always start from 1.
		size_t file_index_;
		/// Get all files that would be removed.
		/// @param files Where to store these files.
		/// @param container Which map to use.
		/// @param type Which type we are using.
		void getGeneralOrphaFiles(std::vector<std::string>& files, const std::map<size_t, std::unique_ptr<File>>& container, FileType type);
		/// Append relation suggestions by a parent relations.
		/// @param suggestions Container of all suggestions.
		/// @param parent Which person is parent.
		/// @param child Which person is child.
		/// @param id Id of this relation.
		/// @param trait Trait of this relation.
		/// @param forbiddenPersonParent Which persons are forbidden to bind to with a parent.
		/// @param forbiddenPersonChild Which persons are forbidden to bind to with a child.
		void getParentSuggestions(std::vector<RelationSuggestion>& suggestions, const Person* parent, const Person* child, size_t id, Trait trait,
                            const std::set<size_t>& forbiddenPersonParent, const std::set<size_t>& forbiddenPersonChild);
		/// Append relation suggestions by partners relations.
		/// @param suggestions Container of all suggestions.
		/// @param person Which person is the main one.
		/// @param second Which person is the second one.
		/// @param id Id of this relation.
		/// @param forbiddenPersons Which persons are forbidden to bind to a given person.
		void getPartnersSuggestions(std::vector<RelationSuggestion>& suggestions, const Person* person, const Person* second, size_t id, const std::set<size_t>& forbiddenPersons);
		/// Append relation suggestions by siblings relations.
		/// @param suggestions Container of all suggestions.
		/// @param person Which person is the main one.
		/// @param second Which person is the second one.
		/// @param id Id of this relation. -- This will be changed
		/// @param forbiddenPersons Which persons are forbidden to bind to a given person.
		void getSiblingsSuggestions(std::vector<RelationSuggestion>& suggestions, const Person* person, const Person* second, size_t id, const std::set<size_t>& forbiddenPersons);
		/// Load one single file from the database.
		/// @param file Which file in database is being loaded.
		/// @param reader Which function read the data and load them.
		/// @param errorMessage Where will the error message stored.
		/// @return If the parsing was successful or not.
		bool loadDatabaseFile(const std::string& file, std::function<void(const Json::Value&)> reader, std::string& errorMessage);
		/// Main person showing as the centre of the tree.
		Person* mainPerson_;
		/// Last free id for media. Always start from 1.
		size_t media_index_;
		/// Last free id for note. Always start from 1.
		size_t note_index_;
		/// If help window should be shown.
		bool openHelp;
		/// Parser for working with locally stored database.
		Parser parser_;
		/// Last free id for person. Always start from 1.
		size_t person_index_;
		/// Print custom CSS.
		/// @param dirPath Path to the directory for the CSS.
		void printCss(const std::string& dirPath);
		/// Print all files from vector container.
		/// @param os Given stream.
		/// @param container Given container of files.
		/// @param label What is the label in JSON file.
		void printFiles(std::ostream& os, const std::map<size_t, std::unique_ptr<File>>& container, const std::string& label);
		/// Print all events.
		/// @param os Given output stream.
		void printEvents(std::ostream& os);
		/// Print single person to an output in html format.
		/// @param os Which output stream to use.
		/// @param links True if they should be links to other persons.
		/// @param p Which person is to be printed.
		void printHtmlPerson(std::ostream& os, bool links, Person* p);
		/// Print all the people into a stream in a semi-structured JSON format.
		/// @param os Given stream.
		void printPeople(std::ostream& os);
		/// Print all relations.
		/// @param os Given output stream.
		void printRelations(std::ostream& os);
		/// Paths to existing projects.
		std::vector<std::string> projectPaths;
        /// Read and load all events from its JSON value.
		/// @param value Loaded value from the source file.
		void readJsonEvents(const Json::Value& value);
		/// Read and load meta-data about files from JSON value.
		/// @param value Loaded value from the source file.
		/// @param container Which vector to use for storing files.
		/// @param label Which label was used in the JSON file.
		/// @param index Indexing of given container.
		void readJsonFiles(const Json::Value& value, std::map<size_t, std::unique_ptr<File>>& container, const std::string& label, size_t& index);
		/// Read and load people from given JSON value.
		/// @param value Loaded value from the file.
		void readJsonPeople(const Json::Value& value);
		/// Read and load all relations from its JSON value.
		/// @param value Loaded value from the source file.
		void readJsonRelations(const Json::Value& value);
		/// Last free index for relation.
		size_t relation_index_;
		/// Remove general orphan file.
		/// @param container Which container holding files is to be used.
		/// @param type Which type will be used.
		/// @param dir Which directory in database to use.
		/// @return Number of remove files.
		size_t removeGeneralOrphanFile(std::map<size_t, std::unique_ptr<File>>& container, FileType type, const std::string& dir);
		/// If all changes were saved or not.
		bool savedChanges_;
		/// Settings of the app.
		Settings settings_;
		/// Store file database when putting file to database. Also remove all orphan files.
		/// @param type What type of files are saved.
		void storeFileDatabase(FileType type);
};

#endif
