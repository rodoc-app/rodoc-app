/// @file file_parser.h The header file for working with the database and other files.
#ifndef file_parser_h_
#define file_parser_h_

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <json/json.h>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <set>
#include "strings.h"

/// Namespace for all strings representing files and directories in the database.
namespace parser{
    /// Directory for all documents and files in general.
    const std::string FILES_DIR = "Files";
    /// JSON file-name for configuration and templates.
    const std::string JSON_CONFIG = "Config.json";
    /// JSON file-name for events.
    const std::string JSON_EVENTS = "Events.json";
    /// JSON file-name for files meta-data.
    const std::string JSON_FILES = "Files.json";
    /// JSON file-name for media meta-data.
    const std::string JSON_MEDIA = "Media.json";
    /// JSON file-name for notes meta-data.
    const std::string JSON_NOTES = "Notes.json";
    /// JSON file-name for persons.
    const std::string JSON_PERSONS = "Persons.json";
    /// JSON file-name for relations.
    const std::string JSON_RELATIONS = "Relations.json";
    /// Directory for all pictures and other media.
    const std::string MEDIA_DIR = "Media";
    /// Directory for all notes.
    const std::string NOTES_DIR = "Notes";
    /// File for writing error log.
    const std::string ERROR_LOG = ".error.log";
}

/// Main class for working with the database.
class Parser{
	public:
	    /// Default constructor.
	    Parser();
	    /// Check the consistency of files.
	    /// @param database Which files are stored in database, these will be removed from the list if found.
	    /// @param found Files found and not present in database.
	    /// @param dir Which directory to look into.
	    void checkFileConsistency(std::set<std::string>& database, std::vector<std::string>& found, const std::string& dir);
	    /// Clear stored data.
	    void clear();
	    /// If the database store some backup files, which may be used. Note there can be only one backup file (when the app is working properly).
	    /// @param backupFile If there is backup there will be stored the name of the file.
	    /// @return True if there is a backup file.
	    bool containsBackupFile(std::string& backupFile);
	    /// Create a new file by copying existing file on the disk.
		/// @param filePath Path to the original file on the disk.
		/// @param dir Which directory to use in database of the root directory.
		/// @param fileName Is the name of the file in directory. Use for return value.
		/// @param id Id of the new file, to create the real name.
		/// @return True if the process was successful, false otherwise.
		bool copyFile(const std::string& filePath, const std::string& dir, std::string& fileName, size_t id);
		/// Create an empty file.
		/// @param dir Which directory in database to use.
		/// @param fileName What is the real file name in its directory.
		void createEmptyFile(const std::string& dir, const std::string& fileName);
		/// Create resource file for diagrams for HTML export.
		/// @param target Where the resource directory should be (either a path to the directory or an html file).
		/// @return String representing the path to the resources directory.
		std::string createResourcesDir(const std::string& target);
		/// Get the absolute path to the file in database.
		/// @param dir Which directory should have the file.
		/// @param fileName What is the file-name in its directory.
		/// @return String representing the absolute path.
		std::string getAbsoluteFilePath(const std::string& dir, const std::string& fileName) const;
		/// Get the url to the file for opening the file.
		/// Format: 'file://[file path]'.
		/// @param dir Which directory should have the file.
		/// @param fileName What is the file-name in its directory.
		/// @param fileUrl String representing the URL in specified format.
		void getFileUrl(const std::string& dir, const std::string& fileName, std::string& fileUrl) const;
	    /// Whether the directory has been already set.
		/// @return True if the directory was already chosen.
		bool isRootDirectorySet();
		/// Log error to the error file.
		/// @param error What error occurred.
		void log(const std::string&  error);
		/// Make new empty database. Create all directories and files.
		/// @param dirPath Path to the chosen root directory.
		void makeNewDatabase(const std::string& dirPath);
		/// Read config file if it exists and return vector of all saved paths.
		/// @return If help should be shown and vector of strings representing paths to the directories.
		std::pair<bool, std::vector<std::string>> readConfig();
		/// Load the content of JSON file to Json::Value format.
		/// @param fileName File in the root directory to be used.
		/// @param root Where to store parsed data from the file.
		/// @param inDatabase If the file is in database or not, if it is not, then it is for import.
		/// @return True if the parsing was successful. False otherwise.
		bool readJSONFile(const std::string& fileName, Json::Value& root, bool inDatabase = true);
		/// Remove backup files if there is any.
		void removeBackup();
		/// Remove file with given name in given directory.
		/// @param filename Name of the file.
		/// @param dir Given directory with the file.
		void removeFile(const std::string& filename, const std::string& dir);
		/// Rename a file in its directory.
		/// @param originalName Original name of the file.
		/// @param changedName New changed name of the file.
		/// @param dir Directory where the file is.
		void renameFile(const std::string& originalName, const std::string& changedName, const std::string& dir);
		/// Restore backup file.
		/// @param backupFile Which backup file is going to be restored.
        void restoreBackup(const std::string& backupFile);
		/// Set the root directory and check its content.
		/// @param dirPath Path to the root directory.
		/// @return True if all files and directories exist.
		bool setDatabase(const std::string& dirPath);
		/// Write only current project path to the configuration.
		/// @param help If help window should be shown at the start-up.
		void writeConfig(bool help);
		/// Write given paths to the root directories to the configuration file.
		/// @param rootPaths Vector of all paths to the directories.
		/// @param help If help window should be shown at the start-up.
		void writeConfig(const std::vector<std::string>& rootPaths, bool help);
        /// Write HTML text to an output file (or for CSS).
		/// @param html HTML text to put in an output file.
		/// @param output Output file path.
		void writeHtml(const std::string& html, const std::string& output);
		/// Write HTML text to an output file (or for CSS).
		/// @param html HTML text to put in an output file.
		/// @param dirPath Path to the root directory.
		/// @param filename Name of the file in root directory.
		void writeHtml(const std::string& html, const std::string& dirPath, const std::string& filename);
		/// Write data in JSON formatting to its file. Before create a backup and after saving delete the backup.
		/// @param data Preprocessed JSON format string of data.
		/// @param file The name of the JSON file in the root directory.
		/// @param inDatabase If the file is in database or not, if it is not it is for export.
		void writeJSON(const std::string& data, const std::string& file, bool inDatabase = true);
	private:
	    /// If there is backup for this file-name.
	    /// @param backupFile If this backup file exists put there the name of it.
	    /// @param filename Name of the file checked for having backup.
	    /// @return True if the backup exists.
	    bool containsBackup(std::string& backupFile, const std::string& filename);
	    /// Create all (empty) directories in the root directory.
		void createAllDirs();
		/// Create an empty JSON file.
		/// @param fileName Name of the file in the root directory.
		void createJsonFile(const std::string& fileName);
		/// Path to the root directory.
		std::filesystem::path root_;
		/// Path to the configuration file.
		std::filesystem::path configPath_;
};

#endif
