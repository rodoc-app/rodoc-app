/// @file person.h Header file for all classes closely related to person.
#ifndef person_h_
#define person_h_

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <json/json.h>
#include "date.h"
#include "strings.h"
#include "config.h"

/// Class for virtualization of folder structure for each person.
class VirtualDrive{
    public:
        /// Default constructor.
        VirtualDrive() = default;
        /// Constructor for known name.
        /// @param name Name of this folder.
        explicit VirtualDrive(const std::string& name);
        /// Add file to the folder by its id. Only if it is not already there.
        /// @param fileId Id of the file.
        void addFile(size_t fileId);
        /// Add sub-folder to this folder.
        /// @return Pointer to the new sub-folder.
        VirtualDrive* addSubdrive();
        /// Add sub-folder to this folder.
        /// @param name Name of the sub-folder.
        /// @return Pointer to the new sub-folder.
        VirtualDrive* addSubdrive(const std::string& name);
        /// Recursively copy the other drive. Used for import mainly.
        /// @param other Other drive.
        /// @param plusIndex What should be added to each file index.
        void copyDrive(const VirtualDrive& other, size_t plusIndex = 0);
        /// Get constant reference to the container of files.
        /// @return Constant reference to the container of files.
        const std::vector<size_t>& getFiles() const;
        /// Get the name of the folder.
        /// @return Name of the folder.
        const std::string& getName() const;
        /// Get constant reference to the container of files.
        /// @return Constant reference to the container of pointers to virtual drives.
        const std::vector<std::unique_ptr<VirtualDrive>>& getSubdrives() const;
        /// Load data from JSON value.
        /// @param value Given JSON value.
        void readJson(const Json::Value& value);
        /// Remove sub-folder and all its sub-folders and files.
        /// @param subfolderName Name of the sub-folder.
        void remove(const std::string& subfolderName);
        /// Remove one file in this folder.
        /// @param id Id of the file.
        void removeFile(size_t id);
        /// Rename this folder.
        /// @param name New name of this folder.
        void rename(const std::string& name);
        /// Vector of all used files.
        /// @param used Vector of all files contained in this folder and sub-folders.
        void usedFiles(std::vector<size_t>& used) const;
    private:
        /// Container of all files.
        std::vector<size_t> files_;
        /// Name of the folder.
        std::string name_;
        /// Container of pointers to the sub-folders.
        std::vector<std::unique_ptr<VirtualDrive>> subdrives_;
};

/// Print virtual drive to JSON format.
/// @param os Given output stream.
/// @param vd Given VirtualDrive.
/// @return Changed output stream.
std::ostream& operator<<(std::ostream& os, const VirtualDrive& vd);

///Gender types: Male, Female and Other.
enum Gender {Other, Male, Female};

/// Namespace for constants with gender enumeration.
namespace gender{
    /// All genders in a list.
    constexpr Gender AllGenders[] = {Other, Male, Female};
    /// Write the gender as a string.
    /// @param g Given Gender.
    /// @return Newly constructed string representing the gender.
    std::string str(Gender g);
}

/// Enumeration on special types of files.
enum FileType{GENERAL_FILE, MEDIA, NOTE};

/// One person in the family tree.
class Person{
	public:
		/// Default constructor.
		Person();
		/// Add new event as its id.
		/// @param id Its id.
		void addEvent(size_t id);
		/// Add new general relation as its id.
		/// @param id Its id.
		void addRelation(size_t id);
		/// Add tag to the person.
		/// @param tag Used tag.
		/// @param value Value of the tag.
		void addTag(const std::string& tag, const std::string& value);
		/// Get the pointer to the birth date.
		/// @return Pointer to the birth date.
		WrappedDate* birthDate();
		/// Get the pointer to the death date.
		/// @return Pointer to the death date.
		WrappedDate* deathDate();
		/// If there exists value with this tag.
		/// @param tag Given tag.
		/// @return True if it exists and then the value.
		std::pair<bool, const std::string&> existsTag(const std::string& tag) const;
		/// Export this person.
		/// @param os Given output stream.
		/// @param includingEvents If events should be included.
		/// @param includingRelations If relations should be included.
		void exportJson(std::ostream& os, bool includingEvents, bool includingRelations);
		/// Get the title after the name.
		/// @return Constant reference to the title.
		const std::string& getAfterTitle() const;
		/// Get the date of birth.
		/// @return Date of birth.
		const WrappedDate& getBirthDate() const;
		/// Get the string of place of birth.
		/// @return Constant reference to the string of the place of birth.
		const std::string& getBirthPlace() const;
		/// Get the date of death.
		/// @return Date of death.
		const WrappedDate& getDeathDate() const;
		/// Get the string representing the place of death.
		/// @return String representing the place of death.
		const std::string& getDeathPlace() const;
		/// Get constant reference to the list of all events.
		/// @return List of events ids.
		const std::vector<size_t>& getEvents() const;
		/// Get the index of fatherhood relation representing father.
		/// @return Index of the fatherhood relation.
		size_t getFather() const;
		/// Get the full name of a person in order: "Surname + MaidenName + Name".
		/// @return Full name of a person.
		std::string getFullName() const;
		/// Get constant reference to the root drive of files.
		/// @param type What type of files.
		/// @return Root drive for given file type.
		const VirtualDrive& getFilesRoot(FileType type = GENERAL_FILE) const;
		/// Get the pointer to the files root drive.
		/// @param type Type of the files.
		/// @return Pointer to the root drive.
		VirtualDrive* getFilesRootPointer(FileType type);
		/// Get the title in front.
		/// @return Constant reference to the title.
		const std::string& getFrontTitle() const;
		/// Get the gender of the person.
		/// @return Person's gender.
		Gender getGender() const;
		/// Get the id of the person.
		/// @return The id of the person.
		size_t getId() const;
		/// Get the maiden name.
		/// @return Constant reference to the maiden name.
		const std::string& getMaidenName() const;
		/// Get the index of motherhood relation representing mother.
		/// @return Index of the motherhood relation.
		size_t getMother() const;
		/// Get the name of the person.
		/// @return Constant reference to the name.
		const std::string& getName() const;
		/// Get the id of the partner.
		/// @return Id of the partnership relation.
		size_t getPartner() const;
		/// Get constant reference to the list of all relations.
		/// @return List of relations ids.
		const std::vector<size_t>& getRelations() const;
		/// Get the surname of the person.
		/// @return Constant reference to the surname.
		const std::string& getSurname() const;
		/// Get all tags.
		/// @return Constant reference to the tags container.
		const std::vector<std::pair<std::string, std::string>>& getTags() const;
		/// Import person from single file JSON. Some values may not be present from the import.
		/// @param value JSON value.
		void importJson(const Json::Value& value);
		/// If the person is alive or not.
		/// @return True if the person lives.
		bool isAlive() const;
		/// Print person details to an output in HTML syntax.
		/// @param os Output stream.
		void printHtml(std::ostream& os);
		/// Promote relation to the main one. If it is already promoted de-promote it.
		/// @param id Of the relation.
		/// @param type Type of the relation.
		void promoteRelation(size_t id, Trait type);
		/// Read data from JSON to this person.
		/// @param value Loaded JSON data from the file.
		void readJson(const Json::Value& value);
		/// Remove event if the person has it (by its id).
		/// @param id Its id.
		void removeEvent(size_t id);
		/// Remove relation if the person has it (by its id) that includes children and other special relations.
		/// @param id Its id.
		void removeRelation(size_t id);
		/// Remove tag from the person if it exists.
		/// @param tag Which tag will be removed.
		void removeTag(const std::string& tag);
		/// Set the title after the name.
		/// @param title The new title.
		void setAfterTitle(const std::string& title);
		/// Set the exact birth date.
		/// @param d Number of day.
		/// @param m Number of month.
		/// @param y Number of year.
		void setBirthDate(int y, int m, int d);
		/// Set the birth place by giving reference to the string.
		/// @return Reference to the birth place.
		std::string& setBirthPlace();
		/// Set the exact death date.
		/// @param d Number of day.
		/// @param m Number of month.
		/// @param y Number of year.
		void setDeathDate(int y, int m, int d);
		/// Set the death place by giving reference to the string.
		/// @return Reference to the death place.
		std::string& setDeathPlace();
		/// Set father for the person. If father is present put him into relations.
		/// @param father Index to fatherhood relation.
		void setFather(size_t father);
		/// Set the drive by copying the data from the other.
		/// @param drive Other drive.
		/// @param type What type of file is the drive.
		/// @param plusIndex What should be added to each file index.
		void setFilesRoot(const VirtualDrive& drive, FileType type = GENERAL_FILE, size_t plusIndex = 0);
		/// Set the title in front of a name.
		/// @param title The new title.
		void setFrontTitle(const std::string& title);
		/// Set the gender.
		/// @param g Given gender.
		void setGender(Gender g);
		/// Set the id of the person.
		/// @param id The new id.
		void setId(size_t id);
		/// Set if the person lives or not.
		/// @param lives True if the person is still alive.
		void setLives(bool lives);
		/// Set the maiden name by getting the reference to it.
		/// @return The reference to the maiden name.
		std::string& setMaidenName();
		/// Set mother for the person. If mother is present put her into relations.
		/// @param mother Index to motherhood relation.
		void setMother(size_t mother);
		/// Get the reference to the name for changing the name.
		/// @return The reference to the name string.
		std::string& setName();
		/// Set the partnership relation id.
		/// @param id Id of the partnership relation.
		void setPartner(size_t id);
		/// Get the reference to the surname for changing it.
		/// @return The reference to the surname string.
		std::string& setSurname();
		/// Get the string representation of person.
		/// @return String representing a person.
		std::string str() const;
		/// Update relation if it is special.
		/// @param relId Which id it has.
		/// @param trait Which trait it now has.
		void updateSpecialRelation(size_t relId, Trait trait);
	private:
	    /// Date of birth.
		WrappedDate dateOfBirth_;
		/// Date of death.
		WrappedDate dateOfDeath_;
		/// All events ids (from Family Tree) of the person.
		std::vector<size_t> events_;
		/// Index of relation with fatherhood trait.
		size_t father_;
	    /// Gender of the person.
		Gender gender_;
		/// Id of the person.
		size_t id_;
		/// If the person is alive.
		bool lives_;
		/// Maiden name of the person. Usually for a married woman.
		std::string maidenName_;
		/// Index of relation with motherhood trait.
		size_t mother_;
		/// Name of the person.
		std::string name_;
		/// Id of the partnership relation.
		size_t partner_;
		/// Place of birth.
		std::string placeOfBirth_;
		/// Place of death.
		std::string placeOfDeath_;
		/// All relations ids (from Family Tree).
		std::vector<size_t> relations_;
		/// Root drive for files.
		VirtualDrive rootFileDrive_;
		/// Root drive for media.
		VirtualDrive rootMediaDrive_;
		/// Root drive for notes.
		VirtualDrive rootNoteDrive_;
		/// Surname of the person.
		std::string surname_;
		/// Persons tags. First is the tag itself and second is the value.
		std::vector<std::pair<std::string, std::string>> tags_;
        /// Title that is after the name.
		std::string titleAfter_;
		/// Title that is in front of a name.
		std::string titleInFront_;
};

/// Print to JSON formatting container of unsigned integers with given label.
/// @param os Given stream where to print.
/// @param container Given container of ids.
/// @param label The label of JSON array.
void printSizeTContainer(std::ostream& os, const std::vector<size_t>& container, const std::string& label);

/// Write the data about the person in JSON format.
/// @param os Is the given stream.
/// @param p Is the given person.
/// @return Reference to the used output stream.
std::ostream& operator<<(std::ostream& os, const Person& p);

#endif
