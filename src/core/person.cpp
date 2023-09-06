/// @file person.cpp Source file for all classes closely related to person.
#include "person.h"

// =====================================================================
// VirtualDrive
// =====================================================================

VirtualDrive::VirtualDrive(const std::string& name) : name_(name){}

void VirtualDrive::addFile(size_t fileId){
    bool exists = std::any_of(files_.begin(), files_.end(), [fileId](size_t file){return file == fileId;});
    if(exists) return;
    files_.push_back(fileId);
}

VirtualDrive* VirtualDrive::addSubdrive(){
    subdrives_.push_back(std::make_unique<VirtualDrive>());
    return subdrives_.at(subdrives_.size() - 1).get();
}

VirtualDrive* VirtualDrive::addSubdrive(const std::string& name){
    for(auto&& pSubdrive : subdrives_){
        if(pSubdrive->getName() == name) return pSubdrive.get();
    }
    subdrives_.push_back(std::make_unique<VirtualDrive>(name));
    return subdrives_.at(subdrives_.size() - 1).get();
}

void VirtualDrive::copyDrive(const VirtualDrive& other, size_t plusIndex){
    name_ = other.getName();
    files_.clear();
    subdrives_.clear();
    std::transform(other.getFiles().begin(), other.getFiles().end(), std::back_inserter(files_), [plusIndex](size_t index){return index + plusIndex;});
    for(auto&& subdrive : other.getSubdrives()){
        subdrives_.push_back(std::make_unique<VirtualDrive>(subdrive->getName()));
        subdrives_[subdrives_.size() - 1]->copyDrive(*subdrive, plusIndex);
    }
}

const std::vector<size_t>& VirtualDrive::getFiles() const{
    return files_;
}

const std::string& VirtualDrive::getName() const{
    return name_;
}

const std::vector<std::unique_ptr<VirtualDrive>>& VirtualDrive::getSubdrives() const{
    return subdrives_;
}

void VirtualDrive::readJson(const Json::Value& value){
    name_ = value[jsonlabel::NAME].asString();
    for(Json::ArrayIndex i = 0; i < value[jsonlabel::FILES].size(); ++i){
        addFile(value[jsonlabel::FILES][i][jsonlabel::ID].asUInt64());
    }
    for(Json::ArrayIndex i = 0; i < value[jsonlabel::DRIVES].size(); ++i){
        VirtualDrive* subdrive = addSubdrive();
        subdrive->readJson(value[jsonlabel::DRIVES][i]);
    }
}

void VirtualDrive::remove(const std::string& subfolderName){
    auto it = std::find_if(subdrives_.begin(), subdrives_.end(), [subfolderName](const std::unique_ptr<VirtualDrive>& drive){return drive->getName() == subfolderName;});
    if (it != subdrives_.end()) subdrives_.erase(it);
}

void VirtualDrive::removeFile(size_t id){
    auto it = std::find_if(files_.begin(), files_.end(), [id](size_t fileId){return id == fileId;});
    if (it != files_.end()) files_.erase(it);
}

void VirtualDrive::rename(const std::string& name){
    name_ = name;
}

void VirtualDrive::usedFiles(std::vector<size_t>& used) const{
    std::copy(files_.begin(), files_.end(), std::back_inserter(used));
    for(auto&& subdrive : subdrives_){
        subdrive->usedFiles(used);
    }
}

// =====================================================================
// functions for VirtualDrive
// =====================================================================

std::ostream& operator<<(std::ostream& os, const VirtualDrive& vd){
    os << "{";
    os << "\"" << jsonlabel::NAME << "\":\"" << vd.getName() << "\"";
    os << ",\"" << jsonlabel::FILES << "\":[";
    bool first = true;
    for(auto&& file : vd.getFiles()){
        if(!first){
            os << ",";
        }
        os << "{";
        first = false;
        os << "\"" << jsonlabel::ID << "\":" << file;
        os << "}";
    }
    os << "]";
    os << ",\"" << jsonlabel::DRIVES << "\":[";
    first = true;
    for(auto&& subdrive : vd.getSubdrives()){
        if(!first){
            os << ",";
        }
        first = false;
        os << *subdrive;
    }
    os << "]";
    os << "}";
    return os;
}

// =====================================================================
// Gender
// =====================================================================

std::string gender::str(Gender g){
    switch(g){
        case Male:
            return "Male";
        case Female:
            return "Female";
        case Other:
        default:
            return "Other";
    }
}

// =====================================================================
// Person
// =====================================================================

Person::Person() : father_(0), gender_(Other), id_(1), lives_(true), mother_(0), name_(person::NAME_STR), partner_(0),
  rootFileDrive_(std::move(VirtualDrive(person::ROOT))), rootMediaDrive_(std::move(VirtualDrive(person::ROOT))),
  rootNoteDrive_(std::move(VirtualDrive(person::ROOT))), surname_(person::SURNAME_STR){}

void Person::addEvent(size_t id){
    bool exists = std::any_of(events_.begin(), events_.end(), [id](size_t event){return event == id;});
    if (exists) return;
	events_.push_back(id);
}

void Person::addRelation(size_t id){
    bool exists = std::any_of(relations_.begin(), relations_.end(), [id](size_t rel){return rel == id;});
    if(exists) return;
	relations_.push_back(id);
}

void Person::addTag(const std::string& tag, const std::string& value){
	for(auto&& [t, v] : tags_){
		if(t == tag){
			v = value;
			return;
		}
	}
	tags_.push_back(std::make_pair(tag,value));
}

WrappedDate* Person::birthDate(){
	return &dateOfBirth_;
}

WrappedDate* Person::deathDate(){
	return &dateOfDeath_;
}

std::pair<bool, const std::string&> Person::existsTag(const std::string& tag) const{
    for(auto&& [t, v] : tags_){
		if(t == tag){
			return {true, v};
		}
	}
	return {false, EMPTY_STRING};
}

void Person::exportJson(std::ostream& os, bool includingEvents, bool includingRelations){
    os << "{\"" << jsonlabel::ID << "\":" << getId();
	os << ",\"" << jsonlabel::NAME << "\":\"" << getName() << "\"";
	os << ",\"" << jsonlabel::SURNAME << "\":\"" << getSurname() << "\"";
	os << ",\"" << jsonlabel::MAIDEN_NAME << "\":\"" << getMaidenName() << "\"";
	os << ",\"" << jsonlabel::GENDER << "\":" << getGender();
	os << ", \"" << jsonlabel::BIRTH << "\":{" << getBirthDate();
	os << ", \"" << jsonlabel::PLACE << "\":\"" << getBirthPlace() << "\"}";
	os << ", \"" << jsonlabel::LIVES << "\":" << isAlive();
	os << ", \"" << jsonlabel::FRONT_TITLE << "\":\"" << getFrontTitle() << "\"";
	os << ",\"" << jsonlabel::AFTER_TITLE << "\":\"" << getAfterTitle() << "\"";
	if(!isAlive()){
		os << ", \"" << jsonlabel::DEATH << "\":{" << getDeathDate();
		os << ", \"" << jsonlabel::PLACE << "\":\"" << getDeathPlace() << "\"}";
	}
	if(includingRelations){
        os << ", \"" << jsonlabel::FATHER << "\":" << getFather();
        os << ", \"" << jsonlabel::MOTHER << "\":" << getMother();
        os << ", \"" << jsonlabel::PARTNER << "\":" << getPartner();
        printSizeTContainer(os, getRelations(), jsonlabel::RELATIONS);
	}
	if(includingEvents){
        printSizeTContainer(os, getEvents(), jsonlabel::EVENTS);
	}
	os << ",\"" << jsonlabel::TAGS << "\":[";
	bool first = true;
	for(auto&& [tag, value] : getTags()){
		if(!first){
			os <<",";
		}
		first = false;
		os << "{\"" << jsonlabel::TAG << "\":\"" << tag << "\"";
		os <<  ",\"" << jsonlabel::VALUE << "\":\"" << value << "\"}";
	}
	os << "]";
	os << "}";
}

const std::string& Person::getAfterTitle() const{
	return titleAfter_;
}

const WrappedDate& Person::getBirthDate() const{
	return dateOfBirth_;
}

const std::string& Person::getBirthPlace() const{
	return placeOfBirth_;
}

const WrappedDate& Person::getDeathDate() const{
	return dateOfDeath_;
}

const std::string& Person::getDeathPlace() const{
	return placeOfDeath_;
}

const std::vector<size_t>& Person::getEvents() const{
	return events_;
}

size_t Person::getFather() const{
	return father_;
}

std::string Person::getFullName() const{
    std::stringstream ss;
    ss << surname_ << " ";
    if(maidenName_ != ""){
        ss << maidenName_ << " ";
    }
    ss << name_;
    return ss.str();
}

const VirtualDrive& Person::getFilesRoot(FileType type) const{
    switch(type){
        case NOTE:
            return rootNoteDrive_;
        case MEDIA:
            return rootMediaDrive_;
        case GENERAL_FILE:
        default:
            return rootFileDrive_;
    }
}

VirtualDrive* Person::getFilesRootPointer(FileType type){
    switch(type){
        case NOTE:
            return &rootNoteDrive_;
        case MEDIA:
            return &rootMediaDrive_;
        case GENERAL_FILE:
        default:
            return &rootFileDrive_;
    }
}

const std::string& Person::getFrontTitle() const{
	return titleInFront_;
}

Gender Person::getGender() const{
	return gender_;
}

size_t Person::getId() const{
	return id_;
}

const std::string& Person::getMaidenName() const{
	return maidenName_;
}

size_t Person::getMother() const{
	return mother_;
}

const std::string& Person::getName() const{
	return name_;
}

size_t Person::getPartner() const{
    return partner_;
}

const std::vector<size_t>& Person::getRelations() const{
	return relations_;
}

const std::string& Person::getSurname() const{
	return surname_;
}

const std::vector<std::pair<std::string, std::string>>& Person::getTags() const{
	return tags_;
}

void Person::importJson(const Json::Value& value){
    id_ = value[jsonlabel::ID].asUInt64();
	name_ = value[jsonlabel::NAME].asString();
	surname_ = value[jsonlabel::SURNAME].asString();
	maidenName_ = value[jsonlabel::MAIDEN_NAME].asString();
	gender_ = Gender(value[jsonlabel::GENDER].asInt());
	dateOfBirth_.readJson(value[jsonlabel::BIRTH][jsonlabel::DATE]);
	placeOfBirth_ = value[jsonlabel::BIRTH][jsonlabel::PLACE].asString();
	lives_ = value[jsonlabel::LIVES].asInt();
	titleInFront_ = value[jsonlabel::FRONT_TITLE].asString();
	titleAfter_ = value[jsonlabel::AFTER_TITLE].asString();
	if(!lives_){
		dateOfDeath_.readJson(value[jsonlabel::DEATH][jsonlabel::DATE]);
		placeOfDeath_ = value[jsonlabel::DEATH][jsonlabel::PLACE].asString();
	}
	if(!value[jsonlabel::EVENTS].isNull()){
        for(Json::ArrayIndex i = 0; i < value[jsonlabel::EVENTS].size(); ++i){
		addEvent(value[jsonlabel::EVENTS][i][jsonlabel::ID].asUInt64());
        }
	}
	if(!value[jsonlabel::RELATIONS].isNull()){
        father_ = value[jsonlabel::FATHER].asUInt64();
        mother_ = value[jsonlabel::MOTHER].asUInt64();
        partner_ = value[jsonlabel::PARTNER].asUInt64();
        for(Json::ArrayIndex i = 0; i < value[jsonlabel::RELATIONS].size(); ++i){
            addRelation(value[jsonlabel::RELATIONS][i][jsonlabel::ID].asUInt64());
        }
	}
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::TAGS].size(); ++i){
		addTag(value[jsonlabel::TAGS][i][jsonlabel::TAG].asString(), value[jsonlabel::TAGS][i][jsonlabel::VALUE].asString());
	}
}

bool Person::isAlive() const{
	return lives_;
}

void Person::printHtml(std::ostream& os){
    os << "\t\t<h1 align=\"center\">" << str() << "</h1>" << std::endl;
    os << "\t\t<h2>" << html::GENERAL_INFO << "</h2>" << std::endl;
    os << "\t\t<ul>" << std::endl;
    os << "\t\t\t<li>" << html::BIRTH_DATE << dateOfBirth_.str() << "</li>" << std::endl;
    os << "\t\t\t<li>" << html::BIRTH_PLACE << placeOfBirth_ << "</li>" << std::endl;
    if(!lives_){
        os << "\t\t\t<li>" << html::DEATH_DATE << dateOfDeath_.str() << "</li>" << std::endl;
        os << "\t\t\t<li>" << html::DEATH_PLACE << placeOfDeath_ << "</li>" << std::endl;
    }
    os << "\t\t\t<li>" << html::GENDER << gender::str(gender_) << "</li>" << std::endl;
    os << "\t\t</ul>" << std::endl;
    if(tags_.size() > 0){
        os << "\t\t<h3>" << html::CUSTOM_TAGS << "</h3>" << std::endl;
        os << "\t\t<ul>" << std::endl;
        for(auto&& [tag, value] : tags_){
            os << "\t\t\t<li>" << tag << ": " << value << "</li>" << std::endl;
        }
        os << "\t\t</ul>" << std::endl;
    }
}

void Person::promoteRelation(size_t id, Trait type){
    switch(type){
        case Fatherhood:
            father_ = father_ == id ? 0 : id;
            break;
        case Motherhood:
            mother_ = mother_ == id ? 0 : id;
            break;
        case Partnership:
            partner_ = partner_ == id ? 0 : id;
            break;
        case Sibling:
        case None:
        default:
            break;
    }
}

void Person::readJson(const Json::Value& value){
	id_ = value[jsonlabel::ID].asUInt64();
	name_ = value[jsonlabel::NAME].asString();
	surname_ = value[jsonlabel::SURNAME].asString();
	maidenName_ = value[jsonlabel::MAIDEN_NAME].asString();
	gender_ = Gender(value[jsonlabel::GENDER].asInt());
	dateOfBirth_.readJson(value[jsonlabel::BIRTH][jsonlabel::DATE]);
	placeOfBirth_ = value[jsonlabel::BIRTH][jsonlabel::PLACE].asString();
	lives_ = value[jsonlabel::LIVES].asInt();
	titleInFront_ = value[jsonlabel::FRONT_TITLE].asString();
	titleAfter_ = value[jsonlabel::AFTER_TITLE].asString();
	if(!lives_){
		dateOfDeath_.readJson(value[jsonlabel::DEATH][jsonlabel::DATE]);
		placeOfDeath_ = value[jsonlabel::DEATH][jsonlabel::PLACE].asString();
	}
	father_ = value[jsonlabel::FATHER].asUInt64();
	mother_ = value[jsonlabel::MOTHER].asUInt64();
	partner_ = value[jsonlabel::PARTNER].asUInt64();
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::EVENTS].size(); ++i){
		addEvent(value[jsonlabel::EVENTS][i][jsonlabel::ID].asUInt64());
	}
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::RELATIONS].size(); ++i){
		addRelation(value[jsonlabel::RELATIONS][i][jsonlabel::ID].asUInt64());
	}
	rootMediaDrive_.readJson(value[jsonlabel::MEDIA]);
	rootFileDrive_.readJson(value[jsonlabel::FILES]);
	rootNoteDrive_.readJson(value[jsonlabel::NOTES]);
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::TAGS].size(); ++i){
		addTag(value[jsonlabel::TAGS][i][jsonlabel::TAG].asString(), value[jsonlabel::TAGS][i][jsonlabel::VALUE].asString());
	}
}

void Person::removeEvent(size_t id){
    auto it = std::find_if(events_.begin(), events_.end(), [id](size_t event){return id == event;});
    if(it != events_.end()) events_.erase(it);

}

void Person::removeRelation(size_t id){
    auto it = std::find_if(relations_.begin(), relations_.end(), [id](size_t rel){return rel == id;});
    if(it != relations_.end()) relations_.erase(it);
}

void Person::removeTag(const std::string& tag){
    auto it = std::find_if(tags_.begin(), tags_.end(), [tag](auto&& pair){return pair.first == tag;});
	if(it != tags_.end()) tags_.erase(it);
}

void Person::setAfterTitle(const std::string& title){
	titleAfter_ = title;
}

void Person::setBirthDate(int y, int m, int d){
    dateOfBirth_.setFirstDate(y, m, d);
    dateOfBirth_.setSecondDate(0,0,0);
    dateOfBirth_.setText() = EMPTY_STRING;
}

std::string& Person::setBirthPlace(){
	return placeOfBirth_;
}

void Person::setDeathDate(int y, int m, int d){
    dateOfDeath_.setFirstDate(y, m, d);
    dateOfDeath_.setSecondDate(0,0,0);
    dateOfDeath_.setText() = EMPTY_STRING;
}

std::string& Person::setDeathPlace(){
	return placeOfDeath_;
}

void Person::setFather(size_t father){
    if(father_ != 0){
        addRelation(father_);
    }
	father_ = father;
}

void Person::setFrontTitle(const std::string& title){
	titleInFront_ = title;
}

void Person::setGender(Gender g){
	gender_ = g;
}

void Person::setId(size_t id){
	id_ = id;
}

void Person::setLives(bool lives){
    lives_ = lives;
}

std::string& Person::setMaidenName(){
	return maidenName_;
}

void Person::setMother(size_t mother){
    if(mother_ != 0){
        addRelation(mother_);
    }
	mother_ = mother;
}

std::string& Person::setName(){
	return name_;
}
void Person::setPartner(size_t id){
    partner_ = id;
}

std::string& Person::setSurname(){
	return surname_;
}

void Person::setFilesRoot(const VirtualDrive& drive, FileType type, size_t plusIndex){
    switch(type){
        case MEDIA:
            rootMediaDrive_.copyDrive(drive, plusIndex);
            break;
        case NOTE:
            rootNoteDrive_.copyDrive(drive, plusIndex);
            break;
        case GENERAL_FILE:
        default:
            rootFileDrive_.copyDrive(drive, plusIndex);
            break;
    }
}

std::string Person::str() const{
    std::stringstream ss;
    ss << titleInFront_;
    if(titleInFront_ != ""){
        ss << " ";
    }
    ss << name_ << " " << surname_;
    if(maidenName_ != ""){
        ss << " (" << maidenName_ << ") ";
    }
    if(titleAfter_ != ""){
        ss << " ";
    }
    ss << titleAfter_;
    return ss.str();
};

void Person::updateSpecialRelation(size_t relId, Trait trait){
    auto it = std::find(relations_.begin(), relations_.end(), relId);
    if(father_ == relId && (trait != Fatherhood || it == relations_.end()))
        father_ = 0;
    else if(mother_ == relId && (trait != Motherhood || it == relations_.end()))
        mother_ = 0;
    else if(partner_ == relId && (trait != Partnership || it == relations_.end()))
        partner_ = 0;
}

// =====================================================================
// functions for Person
// =====================================================================

void printSizeTContainer(std::ostream& os, const std::vector<size_t>& container, const std::string& label){
	os << ", \"" << label << "\":[";
	bool first = true;
	for(auto&& index : container){
		if(!first){
			os <<",";
		}
		first = false;
		os << "{\"" << jsonlabel::ID << "\":" << index <<  "}";
	}
	os << "]";
}

std::ostream& operator<<(std::ostream& os, const Person& p){
	os << "{\"" << jsonlabel::ID << "\":" << p.getId();
	os << ",\"" << jsonlabel::NAME << "\":\"" << p.getName() << "\"";
	os << ",\"" << jsonlabel::SURNAME << "\":\"" << p.getSurname() << "\"";
	os << ",\"" << jsonlabel::MAIDEN_NAME << "\":\"" << p.getMaidenName() << "\"";
	os << ",\"" << jsonlabel::GENDER << "\":" << p.getGender();
	os << ", \"" << jsonlabel::BIRTH << "\":{" << p.getBirthDate();
	os << ", \"" << jsonlabel::PLACE << "\":\"" << p.getBirthPlace() << "\"}";
	os << ", \"" << jsonlabel::LIVES << "\":" << p.isAlive();
	os << ", \"" << jsonlabel::FRONT_TITLE << "\":\"" << p.getFrontTitle() << "\"";
	os << ",\"" << jsonlabel::AFTER_TITLE << "\":\"" << p.getAfterTitle() << "\"";
	if(!p.isAlive()){
		os << ", \"" << jsonlabel::DEATH << "\":{" << p.getDeathDate();
		os << ", \"" << jsonlabel::PLACE << "\":\"" << p.getDeathPlace() << "\"}";
	}
	os << ", \"" << jsonlabel::FATHER << "\":" << p.getFather();
	os << ", \"" << jsonlabel::MOTHER << "\":" << p.getMother();
	os << ", \"" << jsonlabel::PARTNER << "\":" << p.getPartner();
	printSizeTContainer(os, p.getEvents(), jsonlabel::EVENTS);
	printSizeTContainer(os, p.getRelations(), jsonlabel::RELATIONS);
	os << ", \"" << jsonlabel::MEDIA << "\":" << p.getFilesRoot(MEDIA);
	os << ", \"" << jsonlabel::NOTES << "\":" << p.getFilesRoot(NOTE);
	os << ", \"" << jsonlabel::FILES << "\":" << p.getFilesRoot(GENERAL_FILE);
	os << ",\"" << jsonlabel::TAGS << "\":[";
	bool first = true;
	for(auto&& [tag, value] : p.getTags()){
		if(!first){
			os <<",";
		}
		first = false;
		os << "{\"" << jsonlabel::TAG << "\":\"" << tag << "\"";
		os <<  ",\"" << jsonlabel::VALUE << "\":\"" << value << "\"}";
	}
	os << "]";
	os << "}";
	return os;
}
