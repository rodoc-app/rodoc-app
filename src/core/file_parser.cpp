/// @file file_parser.cpp Source file for working with predefined database.
#include "file_parser.h"

Parser::Parser(){
    #ifdef _WIN32
        configPath_ = std::filesystem::path(getenv("APPDATA"));
    #else
        const char* xdgConfigHome = getenv("XDG_CONFIG_HOME");
        if(xdgConfigHome != nullptr){
            configPath_ = std::filesystem::path(getenv("XDG_CONFIG_HOME"));
        }
        if (configPath_.empty()) {
            configPath_ = std::filesystem::path(getenv("HOME")) / ".config";
        }
    #endif
    configPath_ = configPath_ / "rodoc";
    if(!std::filesystem::exists(configPath_)){
        std::filesystem::create_directories(configPath_);
    }
    configPath_ = configPath_ / "config.json";
    if(!std::filesystem::exists(configPath_))
        writeConfig(true);
}

void Parser::checkFileConsistency(std::set<std::string>& database, std::vector<std::string>& found, const std::string& dir){
    namespace fs = std::filesystem;
    fs::path dirPath = root_ / dir;
    try{
        for(fs::directory_iterator it (dirPath); it != fs::end(it) ; ++it){
            fs::path file = (*it);
            if(database.size() > 0 && database.contains(file.filename().string())){
                database.erase(file.filename().string());
            }
            else{
                found.push_back(file.filename().string());
            }
        }
    } catch(std::exception& e){
        log(e.what());
    }
}

void Parser::clear(){
    root_ = std::filesystem::path();
}

bool Parser::copyFile(const std::string& filePath, const std::string& dir, std::string& fileName, size_t id){
    try{
        namespace fs = std::filesystem;
        fs::path originalPath (filePath);
        std::stringstream ss;
        ss << id << "-" << originalPath.filename().string();
        fileName = originalPath.filename().string();
        fs::path newPath = root_ / dir / ss.str();
        return fs::copy_file(originalPath, newPath);
    } catch (std::exception& e){
        log(e.what());
        return false;
    }
}

bool Parser::containsBackup(std::string& backupFile, const std::string& filename){
    namespace fs = std::filesystem;
    fs::path file = root_ / filename;
    file += ".bak";
    if(fs::exists(file)){
        backupFile = file.string();
        return true;
    }
    return false;
}

bool Parser::containsBackupFile(std::string& backupFile){
    if(containsBackup(backupFile, parser::JSON_CONFIG)) return true;
    if(containsBackup(backupFile, parser::JSON_PERSONS)) return true;
    if(containsBackup(backupFile, parser::JSON_EVENTS)) return true;
    if(containsBackup(backupFile, parser::JSON_FILES)) return true;
    if(containsBackup(backupFile, parser::JSON_MEDIA)) return true;
    if(containsBackup(backupFile, parser::JSON_NOTES)) return true;
    if(containsBackup(backupFile, parser::JSON_RELATIONS)) return true;
    return false;
}

void Parser::createAllDirs(){
	namespace fs = std::filesystem;
	fs::path media = root_ / parser::MEDIA_DIR;
	fs::path files = root_ / parser::FILES_DIR;
	fs::path notes = root_ / parser::NOTES_DIR;
	fs::create_directory(root_);
	fs::create_directory(media);
	fs::create_directory(files);
	fs::create_directory(notes);
}

void Parser::createEmptyFile(const std::string& dir, const std::string& fileName){
    try{
        namespace fs = std::filesystem;
        fs::path filePath = root_ / dir / fileName;
        std::ofstream os;
        os.open(filePath);
        os << "";
    } catch (std::exception& e){
        log(e.what());
    }
}

void Parser::createJsonFile(const std::string& fileName){
	try{
		namespace fs = std::filesystem;
		fs::path file = root_ / fileName;
		std::ofstream os(file);
		os << "{}";
	} catch (std::exception& e){
		log(e.what());
	}
}

std::string Parser::createResourcesDir(const std::string& target){
    namespace fs = std::filesystem;
    fs::path targetPath (target);
    if(!fs::is_directory(targetPath)){
        targetPath = targetPath.parent_path();
    }
    targetPath = targetPath / html::RESOURCES;
    fs::create_directory(targetPath);
    return targetPath.string();
}

std::string Parser::getAbsoluteFilePath(const std::string& dir, const std::string& fileName) const{
    namespace fs = std::filesystem;
	fs::path filePath = root_ / dir / fileName;
    if(!filePath.is_absolute()){
        filePath = fs::absolute(filePath);
    }
	return filePath.string();
}

void Parser::getFileUrl(const std::string& dir, const std::string& fileName, std::string& fileUrl) const{
	namespace fs = std::filesystem;
	fs::path filePath = root_ / dir / fileName;
	filePath.make_preferred();
	fileUrl = "file://";
	fileUrl += filePath.string();
}

bool Parser::isRootDirectorySet(){
	return !root_.empty();
}

void Parser::log(const std::string&  error){
    if(!isRootDirectorySet()) return;
    try{
        namespace fs = std::filesystem;
        fs::path logFile = root_ / parser::ERROR_LOG;
        std::ofstream os;
        os.open(logFile, std::ios_base::app);
        auto time = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(time);
        os << std::put_time(std::localtime(&time_t), "%Y-%m-%d %X") << ": " << error << std::endl;
    } catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

void Parser::makeNewDatabase(const std::string& dirPath){
	root_ = std::filesystem::path(dirPath);
	createAllDirs();
	createJsonFile(parser::JSON_PERSONS);
	createJsonFile(parser::JSON_CONFIG);
	createJsonFile(parser::JSON_EVENTS);
	createJsonFile(parser::JSON_FILES);
	createJsonFile(parser::JSON_MEDIA);
	createJsonFile(parser::JSON_NOTES);
	createJsonFile(parser::JSON_RELATIONS);
}

std::pair<bool, std::vector<std::string>> Parser::readConfig(){
    std::vector<std::string> paths;
    bool help = true;
    try{
        std::ifstream ifs;
        ifs.open(configPath_);
        std::stringstream ss;
        std::string str;
        while(getline(ifs,str))
            ss << str;
        Json::Reader reader;
        Json::Value root;
        reader.parse(ss.str(), root);
        for(Json::ArrayIndex i = 0; i < root[jsonlabel::ROOT].size(); ++i)
            paths.push_back(root[jsonlabel::ROOT][i][jsonlabel::DIR].asString());
        help = root[jsonlabel::HELP].asInt();
	} catch(const std::exception& e){
		log(e.what());
	}
	return {help, paths};
}

bool Parser::readJSONFile(const std::string& fileName, Json::Value& root, bool inDatabase){
	try{
		namespace fs = std::filesystem;
		fs::path file;
		if(inDatabase) file = root_ / fileName;
		else file = fs::path(fileName);
		std::ifstream ifs;
		ifs.open(file);
		std::ostringstream ss;
		std::string str;
		while(getline(ifs,str))
			ss << str;
		Json::Reader reader;
		return reader.parse(ss.str(), root);
	} catch(const std::exception& e){
		log(e.what());
		return false;
	}
}

void Parser::removeBackup(){
    std::string toBeRemoved;
    if(containsBackupFile(toBeRemoved)){
        namespace fs = std::filesystem;
        try{
            fs::remove(toBeRemoved);
        } catch (std::exception& e){
            log(e.what());
        }
    }
}

void Parser::removeFile(const std::string& filename, const std::string& dir){
    namespace fs = std::filesystem;
	fs::path file = root_ / dir / filename;
    if(fs::exists(file)){
        try{
            fs::remove(file);
        } catch (std::exception& e){
            log(e.what());
        }
    }
}

void Parser::renameFile(const std::string& originalName, const std::string& changedName, const std::string& dir){
    namespace fs = std::filesystem;
	fs::path originalFile = root_ / dir / originalName;
    if(fs::exists(originalFile)){
        namespace fs = std::filesystem;
        fs::path changedFile = root_ / dir / changedName;
        try{
            fs::rename(originalFile, changedFile);
        } catch(std::exception& e){
            log(e.what());
        }
    }
}

void Parser::restoreBackup(const std::string& backupFile){
    namespace fs = std::filesystem;
    fs::path backup (backupFile);
    fs::path original = backup;
    try{
        original = root_ / original.stem();
        fs::remove(original);
        fs::rename(backup, original);
    } catch(std::exception& e){
        log(e.what());
    }
}

bool Parser::setDatabase(const std::string& dirPath){
	namespace fs = std::filesystem;
	root_ = fs::path(dirPath);
	bool success = true;
	success = success
		&& fs::exists(root_ / parser::JSON_CONFIG)    && (root_ / parser::JSON_CONFIG).extension()    == ".json"
		&& fs::exists(root_ / parser::JSON_EVENTS)    && (root_ / parser::JSON_EVENTS).extension()    == ".json"
		&& fs::exists(root_ / parser::JSON_MEDIA)     && (root_ / parser::JSON_MEDIA).extension()     == ".json"
		&& fs::exists(root_ / parser::JSON_NOTES)     && (root_ / parser::JSON_NOTES).extension()     == ".json"
		&& fs::exists(root_ / parser::JSON_PERSONS)   && (root_ / parser::JSON_PERSONS).extension()   == ".json"
		&& fs::exists(root_ / parser::JSON_RELATIONS) && (root_ / parser::JSON_RELATIONS).extension() == ".json"
		&& fs::exists(root_ / parser::JSON_FILES)     && (root_ / parser::JSON_FILES).extension()     == ".json"
		&& fs::exists(root_ / parser::MEDIA_DIR)      && fs::is_directory(root_ / parser::MEDIA_DIR)
		&& fs::exists(root_ / parser::NOTES_DIR)      && fs::is_directory(root_ / parser::NOTES_DIR)
		&& fs::exists(root_ / parser::FILES_DIR)      && fs::is_directory(root_ / parser::FILES_DIR);
    if(!success) clear();
	return success;
}

void Parser::writeConfig(bool help){
    try{
        std::ofstream out;
        out.open(configPath_);
        Json::Value root;
        Json::Reader reader;
        std::stringstream ss;
        ss << "{\"" << jsonlabel::ROOT << "\":[";
        if(isRootDirectorySet()){
            ss << "{\"" << jsonlabel::DIR << "\":\"" << root_ << "\"}";
        }
        ss << "],\"" << jsonlabel::HELP << "\":" << help;
        ss << "}";
        bool parsingSuccessful = reader.parse(ss.str(), root);
        if(parsingSuccessful){
            out << root << std::endl;
        }
    } catch(std::exception& e){
        std::stringstream ss;
        ss << "Writing to config file " << configPath_ << " was not possible due to an error: " << e.what() << ".";
        log(ss.str());
    }
}

void Parser::writeConfig(const std::vector<std::string>& rootPaths, bool help){
    try{
        std::ofstream out;
        out.open(configPath_);
        Json::Value root;
        Json::Reader reader;
        std::stringstream ss;
        ss << "{\"" << jsonlabel::ROOT << "\":[";
        bool first = true;
        for(auto&& rootPath : rootPaths){
            if(!first){
                ss << ",";
            }
            first = false;
            ss << "{\"" << jsonlabel::DIR << "\":\"" << rootPath << "\"}";
        }
        ss << "],\"" << jsonlabel::HELP << "\":" << help;
        ss << "}";
        bool parsingSuccessful = reader.parse(ss.str(), root);
        if(parsingSuccessful){
            out << root << std::endl;
        }
    } catch(std::exception& e){
        std::stringstream ss;
        ss << "Writing to config file " << configPath_ << " was not possible due to an error: " << e.what() << ".";
        log(ss.str());
    }
}

void Parser::writeHtml(const std::string& html, const std::string& output){
    try{
        std::ofstream out;
        out.open(output);
        out << html;
    } catch(std::exception& e){
        log(e.what());
    }
}

void Parser::writeHtml(const std::string& html, const std::string& dirPath, const std::string& filename){
    namespace fs = std::filesystem;
    fs::path dir (dirPath);
    if(!fs::is_directory(dir)) dir = dir.parent_path();
    fs::path filePath = dir / filename;
    writeHtml(html, filePath.string());
}

void Parser::writeJSON(const std::string& data, const std::string& fileName, bool inDatabase){
	Json::Value root;
	Json::Reader reader;
	namespace fs = std::filesystem;
	bool parsingSuccessful = reader.parse(data, root);
	fs::path file;
	fs::path backup;
	if(inDatabase){
        file = root_ / fileName;
        backup = file;
        backup += ".bak";
        try{
            fs::copy(file, backup);
        } catch(std::exception& e){
            log("Backup was not made for " + fileName + " due to an error: " + e.what() +".");
        }
	}
    else{
        file = fs::path(fileName);
    }
	if(parsingSuccessful){
		try{
			std::ofstream os;
			os.open(file);
			os << root << std::endl;
		} catch(std::exception& e){
			log(e.what());
		}
	}
	if(inDatabase){
        try{
            fs::remove(backup);
        } catch(std::exception& e){
            log("Backup for " + fileName + " was not deleted due to an error: " + e.what() +".");
        }
	}
}
