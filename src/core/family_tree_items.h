/// @file family_tree_items.h Header file for classes closely related to family tree such as Event, Relation and File.
#ifndef family_tree_items_
#define family_tree_items_

#include <string>
#include <sstream>
#include <vector>
#include <ostream>
#include <json/json.h>
#include "config.h"
#include "date.h"
#include "strings.h"

/// Class holding data for a single event.
class Event{
	public:
		/// Default constructor.
		/// @param settings The settings used in app (to access the template).
		explicit Event(Settings* settings);
		/// Add person to the vector.
		/// @param person Id of the person.
		/// @param role Special role for this person, or empty if he does not have special role.
		void addPerson(size_t person, const std::string& role = events::NO_ROLE);
        /// Clear all persons.
		void clearPersons();
		/// Get the pointer to the date.
		/// @return Pointer to the wrapped date.
		WrappedDate* date();
		/// Get the date of this event.
		/// @return Constant reference to the date.
		const WrappedDate& getDate() const;
		/// Get the id of this event.
		/// @return Id of the event.
		size_t getId() const;
		/// Get the vector of persons.
		/// @return Constant reference to the vector of persons.
		const std::vector<std::pair<std::string, size_t>>& getPersons() const;
		/// Get the place of this event.
		/// @return Constant reference to the place.
		const std::string& getPlace() const;
		/// Get the settings.
		/// @return Constant reference to the settings.
		const Settings& getSettings() const;
		/// Get the template for this event.
		/// @return Id of the template.
		size_t getTemplate() const;
		/// Get the text note of the event.
		/// @return Constant reference to the text.
		const std::string& getText() const;
		/// Print event to an output stream in HTML format.
		/// @param os Given output stream,
		/// @param persons List of pairs for role&person names.
		void printHtml(std::ostream& os, const std::vector<std::pair<std::string, std::string>>& persons);
		/// Read the relation from JSON value.
		/// @param value The JSON value from file.
		void readJson(const Json::Value& value);
		/// Remove person from the list of all persons.
		/// @param id Id of the person.
		/// @return True if there is no other person left (thus left out event), otherwise false.
		bool removePerson(size_t id);
		/// Set the date for this event.
		/// @return Reference to the date.
		WrappedDate& setDate();
		/// Set the id for this event.
		/// @param id Given id.
		void setId(size_t id);
		/// Set the place for this event.
		/// @param place New place.
		void setPlace(const std::string& place);
		/// Set the template of this event.
		/// @param templ Id of the new template.
		void setTemplate(size_t templ);
		/// Set the text for this event.
		/// @param text Given new text.
		void setText(const std::string& text);
        /// Get the event representation as a string.
        /// @return Newly constructed string representing the event.
		std::string str() const;
		/// Update this event to the changed template.
		/// @return Vector of all persons which were removed.
		std::vector<size_t> updateToTemplate();
	private:
	    /// Date of this event.
		WrappedDate date_;
		/// Id of this event.
		size_t id_;
        /// People with their roles.
		std::vector<std::pair<std::string, size_t>> persons_;
		/// Place of this event.
		std::string place_;
		/// Settings used in app.
		Settings* settings_;
		/// Template for this event.
		size_t template_;
		/// Text (note or value) for this event.
		std::string text_;

};

/// Using << operator on event to print event in JSON formatting.
/// @param os Given output stream.
/// @param e Given event.
/// @return Changed output stream.
std::ostream& operator <<(std::ostream& os, const Event& e);

/// Class holding data for a relationship.
class Relation{
	public:
	    /// Default constructor.
	    /// @param settings Pointer to the settings of the app.
	    explicit Relation(Settings* settings);
	    /// Get the first person of the relation.
		/// @return Id of the first person.
		size_t getFirstPerson() const;
		/// Get the id of the relation.
		/// @return Id of the relation.
		size_t getId() const;
		/// Get the second person of the relation.
		/// @return Id of the second person.
		size_t getSecondPerson() const;
		/// Get the template of the relation.
		/// @return Id of the template.
		size_t getTemplate() const;
		/// Get the title of the relation.
		/// @return Constant reference to the title.
		const std::string& getTitle() const;
		/// Get the other person from relation.
		/// @param current Current person.
		/// @return Other person in the relation from current.
		size_t getTheOtherPerson(size_t current);
		/// Get the name of the other person with respect to the given person as a string.
		/// @param current Given person id.
		/// @return The name of the other person.
		const std::string& getTheOtherPersonName(size_t current);
		/// Get the trait of the relation.
		/// @return Trait of this relation.
		Trait getTrait();
		/// If the given person from the relation is from older generation.
		/// @param id Id of the given person.
		/// @return True if the generation difference from given person - other person is < 0.
		bool isOlderGeneration(size_t id);
		/// If the relation is of motherhood or fatherhood trait and the other person is younger.
		/// @param personId Id of the person whether it is father or mother.
		/// @return True if all given properties hold, otherwise false.
		bool isParent(size_t personId);
		/// If the generation difference is 0.
		/// @return True if the generation difference is 0.
		bool isSameGenration();
		/// If the given person from the relation is from younger generation.
		/// @param id Id of the given person.
		/// @return True if the generation difference from given person - other person is > 0.
		bool isYoungerGeneration(size_t id);
		/// Print relation to an output stream in HTML format.
		/// @param os Given output stream,
		/// @param firstPerson Name of the first person.
		/// @param secondPerson Name of the second person.
		/// @param promoted What is the name of the promoted version.
		void printHtml(std::ostream& os, const std::string& firstPerson, const std::string& secondPerson, const std::string& promoted);
		/// Read the relation from JSON value.
		/// @param value The JSON value from file.
		void readJson(const Json::Value& value);
	    /// Set the id of this relation.
		/// @param id New id.
		void setId(size_t id);
		/// Set persons for this relation.
		/// @param person1 Id of the first person.
		/// @param person2 Id of the second person.
		void setPersons(size_t person1, size_t person2);
		/// Set the relation template.
		/// @param templ Id of the template.
		void setTemplate(size_t templ);
		/// Get the string representation of relation.
		/// @param firstPerson String representing the first person.
		/// @param secondPerson String representing the second person.
        /// @return String representing the relation.
		std::string str(const std::string& firstPerson, const std::string& secondPerson) const;
	private:
	    /// Id of this relation.
		size_t id_;
	    /// First person of the relation.
		size_t person1_;
		/// Second person of the relation.
		size_t person2_;
		/// Used settings holding the templates.
		Settings* settings_;
		/// Relation template it is using.
		size_t template_;
};

/// Output the relation in JSON format.
/// @param os Given output stream.
/// @param r Given relation.
/// @return Changed output stream.
std::ostream& operator<<(std::ostream& os, const Relation& r);

/// Struct for relation suggestions.
struct RelationSuggestion{
    public:
        /// Default constructor.
        /// @param lockedP Locked person.
        /// @param suggestedP Suggested person.
        /// @param par If it is parent.
        /// @param sym If it is symmetrical.
        /// @param t Given trait.
        RelationSuggestion(size_t lockedP, size_t suggestedP, bool par, bool sym, Trait t);
        /// The one locked person.
        size_t lockedPerson;
        /// If the locked person should be parent or not.
        bool parent;
        /// Suggested person for the relation.
        size_t suggestedPerson;
        /// If the relation is symmetrical or not.
        bool symetrical;
        /// What relation should it be.
        Trait trait;
};

/// Meta-data about one file (media or note). Its real path is [id]-[filename].
class File{
	public:
	    /// Default constructor.
	    File();
	    /// Get the file name.
		/// @return Constant reference to the file name.
		const std::string& getFilename() const;
		/// Get the id of the file.
		/// @return Id of the file.
		size_t getId() const;
		/// Get the real name of the file. Which is [id]-[filename]
		/// @return The real name of the file.
		std::string getRealName() const;
		/// Read the file meta-data from JSON value.
		/// @param value The JSON value of the file.
		void readJson(const Json::Value& value);
	    /// Set the file name.
		/// @param fileName Given name of the file.
		void setFilename(const std::string& fileName);
		/// Set the id of the file.
		/// @param id Given id.
		void setId(size_t id);
	private:
	    /// File name.
		std::string filename_;
		/// Id of the file.
		size_t id_;
};

/// Output file to JSON format.
/// @param os Given stream.
/// @param f Given file.
/// @return Changed stream.
std::ostream& operator <<(std::ostream& os, const File& f);

#endif
