/// @file config.h Header file for configuration of the app. Also includes templates for events and relations.
#ifndef config_h_
#define config_h_

#include <vector>
#include <string>
#include <ostream>
#include <map>
#include <optional>
#include <json/json.h>
#include <climits>
#include <algorithm>
#include "strings.h"

/// Template for events. It specifies whether the event includes a date, a place or if more persons are involved and if they have some special roles.
class EventTemplate{
	public:
	    /// Default constructor.
	    EventTemplate();
	    /// If the event contains a date or not.
		/// @return True if it contains a date, false otherwise.
		bool containsDate() const;
		/// If the event contains a place or not.
		/// @return True if the event contains a place, false otherwise.
		bool containsPlace() const;
	    /// Get the description of the event.
		/// @return Constant reference to the description.
		const std::string& getDescription() const;
		/// Get the ID of this event template.
		/// @return ID of this event template.
		size_t getId() const;
		/// Get the vector of special roles.
		/// @return Constant reference to the vector of role names.
		const std::vector<std::string>& getRoles() const;
		/// Get the title of this event.
		/// @return Constant reference to the title.
		const std::string& getTitle() const;
		/// If there is more people involved in this event.
		/// @return True if more people are involved.
		bool hasMorePeopleInvolved() const;
		/// If the event has special roles.
		/// @return True if special roles were given.
		bool hasRoles() const;
        /// Read the data from JSON.
		/// @param value Given JSON value.
		void readJson(const Json::Value& value);
        /// Set if the template contains date or not.
		/// @param hasDate If the template contains a date.
		void setContainsDate(bool hasDate);
        /// Set if the template contains place or not.
        /// @param hasPlace If the template contains a place.
		void setContainsPlace(bool hasPlace);
		/// Set the description of the event by giving the reference to the string.
		/// @return Reference to the description.
		std::string& setDescription();
		/// Set the ID of this event template.
		/// @param id New ID.
		void setId(size_t id);
		/// Set if more people are involved in this event.
		/// @param morePeople True if more people are involved.
		void setMorePeopleInvolved(bool morePeople);
		/// Set the roles.
		/// @param roles Vector of all role names.
		void setRoles(std::vector<std::string>& roles);
		/// Set the title of the event by giving the reference to the string.
		/// @return Reference to the title.
		std::string& setTitle();
		/// Represent event template as a string.
		/// @return Newly constructed string representing the template.
		std::string str() const;
	private:
	    /// If the event contains a date or not.
		bool containsDate_;
		/// If the event contains a place or not.
		bool containsPlace_;
		/// Description of the event.
		std::string description_;
		/// Special ID of this template. May change during the execution.
		size_t id_;
		/// If more people are involved. Otherwise only one is involved.
		bool morePeopleInvolved_;
		/// Vector of special role names.
		std::vector<std::string> roles_;
		/// Title of the event.
		std::string title_;
};

/// Output the event template to stream in JSON format.
/// @param os Given stream.
/// @param et Given event template.
/// @return Reference to the changed stream.
std::ostream& operator<<(std::ostream& os, const EventTemplate& et);

/// Common traits of each relations.
enum Trait {None, Fatherhood, Motherhood, Partnership, Sibling};

/// Namespace for everything with relation traits.
namespace trait{
    /// All possible traits.
    constexpr Trait AllTraits[] = {None, Fatherhood, Motherhood, Partnership, Sibling};
    /// Represent trait as a string.
    /// @param t Given trait.
    /// @return String representing given trait.
    std::string str(Trait t);
}

/// Template for relation. What is the name, description. What is the generation difference. And how are persons in this relation called.
class RelationTemplate{
	public:
	    /// Default constructor.
	    RelationTemplate();
        /// Get the description.
		/// @return Constant reference to the description.
		const std::string& getDescription() const;
		/// Get the first name of the relation.
		/// @return Constant reference to the first name.
		const std::string& getFirstName() const;
		/// Get the generation difference.
		/// @return The difference between first and second generation.
		int getGenerationDifference() const;
	    /// Get the ID of this relation template.
		/// @return ID of this relation template.
		size_t getId() const;
		/// Get the second name of the relation.
		/// @return Constant reference to the second name.
		const std::string& getSecondName() const;
		/// Get the title of the relation.
		/// @return Constant reference to the title.
		const std::string& getTitle() const;
        /// Get the trait of the relationship.
		/// @return Trait of the relationship.
		Trait getTrait() const;
		/// Read data from JSON.
		/// @param value Given JSON value.
		void readJson(const Json::Value& value);
        /// Set the description of the relation by giving the reference to the description.
        /// @return Reference to the description.
		std::string& setDescription();
		/// Set the first name by giving the reference to the first name.
		/// @return Reference to the first name.
		std::string& setFirstName();
		/// Set the generation difference (first - second).
		/// @param diff Given difference.
		void setGenerationDifference(int diff);
		/// Set the ID of this relation template.
		/// @param id New ID.
		void setId(size_t id);
		/// Set the second name by giving the reference to the second name.
		/// @return Reference to the second name.
		std::string& setSecondName();
		/// Set the title of relation by giving the reference to the title.
		/// @return Reference to the title.
		std::string& setTitle();
        /// Set the trait for the relation.
		/// @param trait Given trait.
		void setTrait(Trait trait);
		/// Represent relation template as a string.
		/// @return Newly constructed string representing relation template.
		std::string str() const;
	private:
	    /// Description of the relation.
	    std::string description_;
		/// First name in the relation.
		std::string firstName_;
		/// Generation difference: first generation - second generation.
		int generationDiff_;
		/// Special ID of this template.
		size_t id_;
		/// Second name in the relation.
		std::string secondName_;
		/// Title of the relation.
		std::string title_;
		/// Special trait of this relation.
		Trait trait_;
};

/// Output the relation template to stream in JSON format.
/// @param os Given stream.
/// @param rt Given relation template.
/// @return Reference to the changed stream.
std::ostream& operator<<(std::ostream& os, const RelationTemplate& rt);

/// Enumeration for tab position.
enum TabPosition {North, South, East, West};

/// Struct for general color.
struct Color{
    /// Default constructor.
    /// @param r Red.
    /// @param g Green.
    /// @param b Blue.
    explicit Color(int r = 255, int g = 255, int b = 255);
    /// Red part of the color.
    int red;
    /// Green part of the color.
    int green;
    /// Blue part of the color.
    int blue;
    /// Read it from json value.
    void readJson(const Json::Value& value);
};

/// Output the color to stream in JSON format.
/// @param os Given stream.
/// @param color Given color.
/// @return Reference to the changed stream.
std::ostream& operator<<(std::ostream& os, const Color& color);

/// Struct for app settings.
struct AppSettings{
    /// Used font in the graphic boxes.
    std::string fontFamily;
    /// Size of the font used in the graphic boxes.
    int fontSize;
    /// Number of generations to show down.
    int genSizeDown;
    /// Number of generations to show up.
    int genSizeUp;
    /// Color for highlighted box.
	Color highlightedColor;
	/// Color of the lines.
	Color lineColor;
	/// Width of the line.
	int lineWidth;
	/// Color for proband.
    Color probandColor;
    /// Color for promoted relation.
    Color promoted;
    /// Border radius.
    int radius;
    /// Read and load data from JSON value.
    /// @param value Given JSON value.
    void readJson(const Json::Value& value);
    /// Set the default options.
    void setDefault();
    /// If event tab should be visible.
    bool showEventTab;
    /// If file tab should be visible.
    bool showFileTab;
    /// If general tab should be visible.
    bool showGeneralTab;
    /// If media tab should be visible.
    bool showMediaTab;
    /// If note tab should be visible.
    bool showNoteTab;
    /// If relation tab should be visible.
    bool showRelationTab;
    /// Size of the box in x axis.
    int sizeX;
    /// Size of the box in y axis.
    int sizeY;
    /// Splitter position one.
    int splitterPositionOne;
    /// Splitter position two.
    int splitterPositionTwo;
    /// Standard color.
    Color standard;
    /// Tab position of the information tab.
	TabPosition tabPosition;
	/// Color of the text.
	Color textColor;
};

/// Output the app settings to stream in JSON format.
/// @param os Given stream.
/// @param as Given app settings.
/// @return Reference to the changed stream.
std::ostream& operator<<(std::ostream& os, const AppSettings& as);

/// Class holding all data about the settings of the app and all templates.
class Settings{
	public:
		/// Default constructor.
		Settings();
		/// Add default event templates.
		void addDefaultEvents();
		/// Add default relation templates.
		void addDefaultRelations();
		/// Add event template to the vector. Also re-index if necessary (id = 0).
		/// @param et Given event template.
		/// @return Id of the template.
		size_t addEventTemplate(EventTemplate& et);
		/// Add relation event to the vector. Also re-index if necessary (id = 0).
		/// @param rt Given relation template.
		/// @return Id of the template.
		size_t addRelationTemplate(RelationTemplate& rt);
		/// Add tag to the vector. If it is not present.
		/// @param tag New tag.
		void addTag(const std::string& tag);
		/// Clear all data.
		void clear();
		/// Export only certain templates in JSON.
		/// @param os Which output stream to use for writing the data.
		/// @param relIds Vector of relation templates ids.
		/// @param eventIds Vector of event templates ids.
		void exportTemplates(std::ostream& os, const std::vector<size_t>& relIds, const std::vector<size_t>& eventIds);
		/// Get the app settings.
		/// @return Constant reference to the app settings.
		const AppSettings& getAppSettings() const;
		/// Get exactly one event template.
		/// @param id The id of the event.
		/// @return Optionally pointer to the template or nothing.
		std::optional<EventTemplate*> getEventTemplate(size_t id) const;
		/// Get all the event templates.
		/// @return Constant reference to the vector of all templates.
		const std::map<size_t, std::unique_ptr<EventTemplate>>& getEventTemplates() const;
		/// Get the global main person.
		/// @return Id of the main person.
		size_t getGlobalMainPerson() const;
		/// Get exactly one relation template.
		/// @param id The id of the relation.
		/// @return Optionally pointer to the template or nothing.
		std::optional<RelationTemplate*> getRelationTemplate(size_t id) const;
		/// Get all the relation templates.
		/// @return Constant reference to the vector of all templates.
		const std::map<size_t, std::unique_ptr<RelationTemplate>>& getRelationTemplates() const;
		/// Return the reference to all tags.
		/// @return Constant reference to the container.
		const std::vector<std::string>& getTags() const;
		/// Import templates from JSON value.
		/// @param value Given JSON Value.
		/// @return Number of imported templates.
        size_t importTemplates(const Json::Value& value);
        /// Get the free index for templates.
        /// @return Pair of event templates index and then relation templates.
        std::pair<size_t, size_t> indexes();
		/// Read the configuration from its JSON value.
		/// @param value Given JSON value.
		void readJson(const Json::Value& value);
		/// Re-index all templates to plus given number.
		/// @param plusEvent What is the number to add to event templates.
		/// @param plusRel What is the number to add to relation templates.
		void reIndex(size_t plusEvent, size_t plusRel);
		/// Remove event template from all events.
		/// @param index On which position the event in vector is.
		void removeEventTemplate(size_t index);
		/// Remove relation template from all relations.
		/// @param index On which position the relation in vector is.
		void removeRelationTemplate(size_t index);
		/// Remove tag from the container.
		/// @param tag Delete same tag if it exists.
		void removeTag(const std::string& tag);
		/// Get the reference to the app settings to edit it.
		/// @return Reference to the app settings.
		AppSettings& setAppSettings();
		/// Set the global main person.
		/// @param id Id of the new main person.
		void setGlobalMainPerson(size_t id);
	private:
	    /// Add single default event.
	    /// @param title Title of the new event template.
	    /// @param description Description of the new event template.
	    /// @param date If the event template contains date.
	    /// @param place If the event template contains date.
	    /// @param morePeople If more people are present.
	    /// @param roles Which roles, if any, the event template has.
	    void addDefaultEvent(const std::string& title, const std::string& description, bool date, bool place, bool morePeople, std::vector<std::string> roles);
	    /// Add single default relation.
	    /// @param title Title of the new relation template.
	    /// @param description Description of the new relation template.
	    /// @param firstName First name of the person.
	    /// @param secondName Second name of the person.
	    /// @param genDiff Generation difference.
	    /// @param trait Which trait it has.
	    void addDefaultRelation(const std::string& title, const std::string& description, const std::string& firstName, const std::string& secondName, int genDiff, Trait trait);
		/// Application settings.
		AppSettings appSettings_;
		/// First empty index for event template.
		size_t eventTemplateIndex_;
		/// All events templates.
		std::map<size_t, std::unique_ptr<EventTemplate>> eventTemplates_;
		/// Main person that is global, so that on app start it will be chosen as the main person.
		size_t globalMainPerson_;
		/// First empty index for relation template.
		size_t relationTemplateIndex_;
		/// All relationsTemplates.
		std::map<size_t, std::unique_ptr<RelationTemplate>> relationsTemplates_;
		/// All tags.
		std::vector<std::string> tags_;
};

/// Output all the settings to stream in JSON format.
/// @param os Given stream.
/// @param setting Given settings.
/// @return Reference to the changed stream.
std::ostream& operator <<(std::ostream& os, const Settings& setting);

#endif
