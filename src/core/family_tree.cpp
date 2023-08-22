/// @file family_tree.cpp Source file for Family tree.
#include "family_tree.h"

// =====================================================================
// Family tree
// =====================================================================

FamilyTree::FamilyTree() : event_index_(1), file_index_(1), mainPerson_(nullptr), media_index_(1), note_index_(1), person_index_(1), relation_index_(1), savedChanges_(true){
    auto [help, paths] = parser_.readConfig();
    openHelp = help;
    projectPaths = std::move(paths);
}

Event* FamilyTree::addEvent(){
    allEvents_.insert({event_index_, std::make_unique<Event>(&settings_)});
    allEvents_.at(event_index_)->setId(event_index_);
    savedChanges_ = false;
    return allEvents_.at(event_index_++).get();
}

Event* FamilyTree::addEvent(size_t index){
    allEvents_.insert({index, std::make_unique<Event>(&settings_)});
    allEvents_.at(index)->setId(index);
    savedChanges_ = false;
    return allEvents_.at(index).get();
}

File* FamilyTree::addFile(const std::string& name, FileType type){
    savedChanges_ = false;
    switch (type){
        case MEDIA:
            allMedia_.insert({media_index_, std::make_unique<File>()});
            allMedia_.at(media_index_)->setId(media_index_);
            allMedia_.at(media_index_)->setFilename(name);
            return allMedia_.at(media_index_++).get();
        case NOTE:
            allNotes_.insert({note_index_, std::make_unique<File>()});
            allNotes_.at(note_index_)->setId(note_index_);
            allNotes_.at(note_index_)->setFilename(name);
            return allNotes_.at(note_index_++).get();
        case GENERAL_FILE:
        default:
            allFiles_.insert({file_index_, std::make_unique<File>()});
            allFiles_.at(file_index_)->setId(file_index_);
            allFiles_.at(file_index_)->setFilename(name);
            return allFiles_.at(file_index_++).get();
    }
}

File* FamilyTree::addFile(const std::string& name, size_t index, FileType type){
    savedChanges_ = false;
    switch (type){
        case MEDIA:
            allMedia_.insert({index, std::make_unique<File>()});
            allMedia_.at(index)->setId(index);
            allMedia_.at(index)->setFilename(name);
            return allMedia_.at(index).get();
        case NOTE:
            allNotes_.insert({index, std::make_unique<File>()});
            allNotes_.at(index)->setId(index);
            allNotes_.at(index)->setFilename(name);
            return allNotes_.at(index).get();
        case GENERAL_FILE:
        default:
            allFiles_.insert({index, std::make_unique<File>()});
            allFiles_.at(index)->setId(index);
            allFiles_.at(index)->setFilename(name);
            return allFiles_.at(index).get();
    }
}

Person* FamilyTree::addPerson(){
    savedChanges_ = false;
	allPersons_.insert({person_index_, std::make_unique<Person>()});
	allPersons_.at(person_index_)->setId(person_index_);
	mainPerson_ = allPersons_.at(person_index_).get();
	return allPersons_.at(person_index_++).get();
}

Person* FamilyTree::addPerson(size_t index){
    savedChanges_ = false;
	allPersons_.insert({index, std::make_unique<Person>()});
	allPersons_.at(index)->setId(index);
	mainPerson_ = allPersons_.at(index).get();
	return allPersons_.at(index).get();
}

Relation* FamilyTree::addRelation(){
    savedChanges_ = false;
    allRelations_.insert({relation_index_, std::make_unique<Relation>(&settings_)});
    allRelations_.at(relation_index_)->setId(relation_index_);
    return allRelations_.at(relation_index_++).get();
}

Relation* FamilyTree::addRelation(size_t index){
    savedChanges_ = false;
    allRelations_.insert({index, std::make_unique<Relation>(&settings_)});
    allRelations_.at(index)->setId(index);
    return allRelations_.at(index).get();
}

void FamilyTree::append(FamilyTree& other){
    savedChanges_ = false;
    auto [eventPlus, relPlus] = settings_.indexes();
    --eventPlus;
    --relPlus;
    other.getSettings()->reIndex(eventPlus, relPlus);
    size_t plusEvent = event_index_ - 1;
    size_t plusRel = relation_index_ - 1;
    size_t plusPerson = person_index_ - 1;
    size_t plusMedia = media_index_ - 1;
    size_t plusNote = note_index_ - 1;
    size_t plusFile = file_index_ - 1;
    std::map<size_t, size_t> eventTemplates;
    std::map<size_t, size_t> relTemplates;
    for(auto&& [id, eventTempl] : other.getSettings()->getEventTemplates()){
        auto realId = settings_.addEventTemplate((*eventTempl));
        eventTemplates.insert({id + eventPlus, realId});
    }
    for(auto&& [id, relTempl] : other.getSettings()->getRelationTemplates()){
        auto realId = settings_.addRelationTemplate((*relTempl));
        relTemplates.insert({id + relPlus, realId});
    }
    std::string absolutePath;
    std::string filename;
    for(auto&& [id, file] : other.getFiles(GENERAL_FILE)){
        other.getFileAbsolutePath(absolutePath, id, GENERAL_FILE);
        bool success = parser_.copyFile(absolutePath, parser::FILES_DIR, filename, id + plusFile);
        if(success){
            File* f = addFile(filename, id + plusFile, GENERAL_FILE);
            f->setId(id + plusFile);
            file_index_ = file_index_ < f->getId() ? f->getId() + 1 : file_index_;
        }
    }
    for(auto&& [id, media] : other.getFiles(MEDIA)){
        other.getFileAbsolutePath(absolutePath, id, MEDIA);
        bool success = parser_.copyFile(absolutePath, parser::MEDIA_DIR, filename, id + plusMedia);
        if(success){
            File* f = addFile(filename, id + plusMedia, MEDIA);
            f->setId(id + plusMedia);
            media_index_ = media_index_ < f->getId() ? f->getId() + 1 : media_index_;
        }
    }
    for(auto&& [id, note] : other.getFiles(NOTE)){
        other.getFileAbsolutePath(absolutePath, id, NOTE);
        bool success = parser_.copyFile(absolutePath, parser::NOTES_DIR, filename, id + plusNote);
        if(success){
            File* f = addFile(filename, id + plusNote, NOTE);
            f->setId(id + plusNote);
            note_index_ = note_index_ < f->getId() ? f->getId() + 1 : note_index_;
        }
    }
    for(auto&& [id, event] : other.getEvents()){
        Event* newEvent = addEvent(id + plusEvent);
        newEvent->setId(id + plusEvent);
        newEvent->setDate() = event->getDate();
        newEvent->setText(event->getText());
        newEvent->setPlace(event->getPlace());
        newEvent->setTemplate(eventTemplates.at(event->getTemplate() + eventPlus));
        for(auto&& [role, person] : event->getPersons())
            newEvent->addPerson(person + plusPerson, role);
        event_index_ = event_index_ < newEvent->getId() ? newEvent->getId() + 1 : event_index_;
    }
    for(auto&& [id, rel] : other.getRelations()){
        Relation* newRel = addRelation(id + plusRel);
        newRel->setId(id + plusRel);
        newRel->setTemplate(relTemplates.at(rel->getTemplate() + relPlus));
        newRel->setPersons(rel->getFirstPerson() + plusPerson, rel->getSecondPerson() + plusPerson);
        relation_index_ = relation_index_ < newRel->getId() ? newRel->getId() + 1 : relation_index_;
    }
    for(auto&& [id, pp] : other.getPersons()){
        const Person* person = pp.get();
        Person* p = addPerson(id + plusPerson);
        p->setAfterTitle(person->getAfterTitle());
        Date date = person->getBirthDate().getFirstDate();
        p->birthDate()->setFirstDate(date.getYear(), date.getMonth(), date.getDay());
        date = person->getBirthDate().getSecondDate();
        p->birthDate()->setSecondDate(date.getYear(), date.getMonth(), date.getDay());
        p->birthDate()->setText() = person->getBirthDate().getText();
        p->birthDate()->updateDate();
        p->setBirthPlace() = person->getBirthPlace();
        date = person->getDeathDate().getFirstDate();
        p->deathDate()->setFirstDate(date.getYear(), date.getMonth(), date.getDay());
        date = person->getDeathDate().getSecondDate();
        p->deathDate()->setSecondDate(date.getYear(), date.getMonth(), date.getDay());
        p->deathDate()->setText() = person->getDeathDate(). getText();
        p->deathDate()->updateDate();
        p->setDeathPlace() = person->getDeathPlace();
        p->setFather(person->getFather() == 0 ? 0 : person->getFather() + plusRel);
        p->setFrontTitle(person->getFrontTitle());
        p->setGender(person->getGender());
        p->setId(id + plusPerson);
        p->setLives(person->isAlive());
        p->setMaidenName() = person->getMaidenName();
        p->setMother(person->getMother() == 0 ? 0 : person->getMother() + plusRel);
        p->setName() = person->getName();
        p->setPartner(person->getPartner() == 0 ? 0 : person->getPartner() + plusRel);
        p->setSurname() = person->getSurname();
        for(auto&& eventId : person->getEvents())
            p->addEvent(eventId + plusEvent);
        for(auto&& relId : person->getRelations())
            p->addRelation(relId + plusRel);
        for(auto&& [tag, value] : person->getTags()){
            p->addTag(tag, value);
            settings_.addTag(tag);
        }
        p->setFilesRoot(person->getFilesRoot(GENERAL_FILE), GENERAL_FILE, plusFile);
        p->setFilesRoot(person->getFilesRoot(MEDIA), MEDIA, plusMedia);
        p->setFilesRoot(person->getFilesRoot(NOTE), NOTE, plusNote);
        person_index_ = person_index_ < p->getId() ? p->getId() + 1 : person_index_;
    }
}

std::pair<bool, std::string> FamilyTree::checkFileConsistence(){
    std::stringstream ss;
    bool problem = false;
    problem |= checkFileTypeConsistence(ss, GENERAL_FILE);
    problem |= checkFileTypeConsistence(ss, MEDIA);
    problem |= checkFileTypeConsistence(ss, NOTE);
    return {problem, ss.str()};
}

bool FamilyTree::checkFileTypeConsistence(std::ostream& os, FileType type){
    std::string directory;
    std::set<std::string> database;
    std::vector<std::string> found;
    switch(type){
        case MEDIA:
            for(auto&& [id, file] : allMedia_)
                database.emplace(file->getRealName());
            parser_.checkFileConsistency(database, found, parser::MEDIA_DIR);
            directory = "Media";
            break;
        case NOTE:
            for(auto&& [id, file] : allNotes_)
                database.emplace(file->getRealName());
            parser_.checkFileConsistency(database, found, parser::NOTES_DIR);
            directory = "Notes";
            break;
        case GENERAL_FILE:
        default:
            for(auto&& [id, file] : allFiles_)
                database.emplace(file->getRealName());
            parser_.checkFileConsistency(database, found, parser::FILES_DIR);
            directory = "Files";
            break;
    }
    if(database.size() != 0){
        os << "These files were not located in " << directory << " directory:" << std::endl;
        for(auto&& f : database)
            os << "  - " << f << std::endl;
    }
    if(found.size() != 0){
        os << "These files were found extra in " << directory << " directory:" << std::endl;
        for(auto&& f : found)
            os << "  + " << f << std::endl;
    }
    return database.size() != 0 || found.size() != 0;
}

void FamilyTree::clear(){
    allRelations_.clear();
    allEvents_.clear();
    allFiles_.clear();
    allMedia_.clear();
    allNotes_.clear();
    allPersons_.clear();
    event_index_ = 1;
    relation_index_ = 1;
    file_index_ = 1;
    media_index_ = 1;
    note_index_ = 1;
    person_index_ = 1;
    mainPerson_ = nullptr;
    settings_.clear();
    parser_.clear();
    savedChanges_ = true;
}

void FamilyTree::clearProjectPaths(){
    projectPaths.clear();
    parser_.writeConfig(openHelp);
}

size_t FamilyTree::copyFile(const std::string& filePath, FileType type){
    std::string fileName;
    bool success;
    switch (type){
        case MEDIA:
            success = parser_.copyFile(filePath, parser::MEDIA_DIR, fileName, media_index_);
            break;
        case NOTE:
            success = parser_.copyFile(filePath, parser::NOTES_DIR, fileName, note_index_);
            break;
        case GENERAL_FILE:
        default:
            success = parser_.copyFile(filePath, parser::FILES_DIR, fileName, file_index_);
            break;
    }
    if(success){
        const auto file = addFile(fileName, type);
        return file->getId();
    }
    return 0;
}

size_t FamilyTree::createEmptyFile(const std::string& name, FileType type){
    const auto file = addFile(name, type);
    switch (type){
        case MEDIA:
            parser_.createEmptyFile(parser::MEDIA_DIR, file->getRealName());
            return file->getId();
        case NOTE:
            parser_.createEmptyFile(parser::NOTES_DIR, file->getRealName());
            return file->getId();
        case GENERAL_FILE:
        default:
            parser_.createEmptyFile(parser::FILES_DIR, file->getRealName());
            return file->getId();
    }
}

void FamilyTree::createDatabase(const std::string& dirPath){
	parser_.makeNewDatabase(dirPath);
	storeDatabase();
}

std::string FamilyTree::createResourcesDir(const std::string& target){
    return parser_.createResourcesDir(target);
}

void FamilyTree::enableHelpOnStartup(bool help){
    openHelp = help;
    parser_.writeConfig(projectPaths, help);
}

void FamilyTree::exportTemplates(const std::vector<size_t>& eventTemplates, const std::vector<size_t>& relTemplates, const std::string& filename){
    std::stringstream ss;
    settings_.exportTemplates(ss, relTemplates, eventTemplates);
    parser_.writeJSON(ss.str(), filename, false);
}

void FamilyTree::exportProject(const std::string& filePath, bool includingEvents, bool includingRelations){
    std::stringstream ss;
    ss << "{\"" << jsonlabel::EXPORT << "\":";
    ss << "{\"" << jsonlabel::PERSONS << "\":[";
    bool first = true;
    for(auto&& [id, pPerson] : allPersons_){
		if(!first) ss << ",";
		first = false;
		pPerson->exportJson(ss, includingEvents, includingRelations);
	}
	ss << "]";
	std::set<size_t> eventTemplates;
	if(includingEvents){
        ss << ",\"" << jsonlabel::EVENTS << "\":[";
        first = true;
        for(auto&& [id, event] : allEvents_){
            if(!first) ss << ",";
            first = false;
            ss << (*event);
            eventTemplates.insert(event->getTemplate());
        }
        ss << "]";
	}
	std::set<size_t> relTemplates;
	if(includingRelations){
        ss << ",\"" << jsonlabel::RELATIONS << "\":[";
        first = true;
        for(auto&& [id, rel] : allRelations_){
            if(!first) ss << ",";
            first = false;
            ss << (*rel);
            relTemplates.insert(rel->getTemplate());
        }
        ss << "]";
	}
    std::vector<size_t> events(eventTemplates.begin(), eventTemplates.end());
    std::vector<size_t> rels(relTemplates.begin(), relTemplates.end());
    if(events.size() > 0 || rels.size() > 0){
        ss << ",\"" << jsonlabel::TEMPLATES << "\":";
        settings_.exportTemplates(ss, rels, events);
    }
    ss << "}}";
    parser_.writeJSON(ss.str(), filePath, false);
}

std::optional<Event*> FamilyTree::getEvent(size_t id){
    if(allEvents_.contains(id))
        return allEvents_.at(id).get();
    std::stringstream ss;
    ss << "Given event with id " << id << " is not present in databse.";
    parser_.log(ss.str());
    return {};
}

const std::map<size_t, std::unique_ptr<Event>>& FamilyTree::getEvents() const{
    return allEvents_;
}

std::optional<File*> FamilyTree::getFile(size_t id, FileType type){
    switch(type){
        case MEDIA:
            if(allMedia_.contains(id))
                return allMedia_.at(id).get();
            else{
                std::stringstream ss;
                ss << "Given media with id " << id << " is not present in databse.";
                parser_.log(ss.str());
               return {};
            }
        case NOTE:
            if(allNotes_.contains(id))
                return allNotes_.at(id).get();
            else{
                std::stringstream ss;
                ss << "Given note with id " << id << " is not present in databse.";
                parser_.log(ss.str());
               return {};
            }
        case GENERAL_FILE:
        default:
            if(allFiles_.contains(id))
                return allFiles_.at(id).get();
            else{
                std::stringstream ss;
                ss << "Given file with id " << id << " is not present in databse.";
                parser_.log(ss.str());
               return {};
            }
    }
}

void FamilyTree::getFileAbsolutePath(std::string& absolutePath, size_t id, FileType type){
     const std::string* dir;
     switch(type){
        case MEDIA:
            dir = &parser::MEDIA_DIR;
            break;
        case NOTE:
            dir = &parser::NOTES_DIR;
            break;
        case GENERAL_FILE:
        default:
            dir = &parser::FILES_DIR;
            break;
    }
    auto optFile = getFile(id, type);
    if(optFile)
        absolutePath = parser_.getAbsoluteFilePath(*dir, (*optFile)->getRealName());
}

const std::map<size_t, std::unique_ptr<File>>& FamilyTree::getFiles(FileType type){
    switch(type){
        case MEDIA:
            return allMedia_;
        case NOTE:
            return allNotes_;
        case GENERAL_FILE:
        default:
            return allFiles_;
    }
}

void FamilyTree::getGeneralOrphaFiles(std::vector<std::string>& files, const std::map<size_t, std::unique_ptr<File>>& container, FileType type){
    std::map<size_t, bool> usedFiles;
    for(auto&& [id, file] : container)
        usedFiles.insert({id, false});
    for(auto&& [id, person] : allPersons_){
        std::vector<size_t> used;
        person->getFilesRoot(type).usedFiles(used);
        for(auto&& fileId : used){
            usedFiles[fileId] = true;
        }
    }
    for(auto&& [id, used] : usedFiles){
        if(!used){
            auto optFile = getFile(id, type);
            if(optFile){
                files.push_back((*optFile)->getRealName());
            }
        }
    }
}

Person* FamilyTree::getMainPerson(){
	return mainPerson_;
}

std::optional<Person*> FamilyTree::getPerson(size_t id){
    if(allPersons_.contains(id))
        return allPersons_.at(id).get();
    else{
        std::stringstream ss;
        ss << "Given person with id " << id << " is not present in database.";
        parser_.log(ss.str());
        return {};
    }
}

const std::map<size_t, std::unique_ptr  <Person>>& FamilyTree::getPersons(){
	return allPersons_;
}

const std::vector<std::string>& FamilyTree::getProjectPaths() const{
    return projectPaths;
}

std::optional<Relation*> FamilyTree::getRelation(size_t id){
    if(allRelations_.contains(id))
        return allRelations_.at(id).get();
    else{
        std::stringstream ss;
        ss << "Given relation with id " << id << " is not present in databse.";
        parser_.log(ss.str());
        return {};
    }
}

const std::map<size_t, std::unique_ptr<Relation>>& FamilyTree::getRelations(){
    return allRelations_;
}

void FamilyTree::getParentSuggestions(std::vector<RelationSuggestion>& suggestions, const Person* parent, const Person* child, size_t id, Trait trait,
                                      const std::set<size_t>& forbiddenPersonParent, const std::set<size_t>& forbiddenPersonChild){
    for(auto&& relId : parent->getRelations()){
        auto optRel = getRelation(relId);
        if(!optRel || relId == id) continue;
        Trait t = (*optRel)->getTrait();
        if(forbiddenPersonChild.contains((*optRel)->getTheOtherPerson(parent->getId()))) continue;
        switch(t){
            case Partnership: // second parent
                suggestions.push_back(RelationSuggestion(child->getId(), (*optRel)->getTheOtherPerson(parent->getId()), false, false, trait == Motherhood ? Fatherhood : Motherhood));
                break;
            case Motherhood:
            case Fatherhood:
                if((*optRel)->isParent(parent->getId())) // Sibling
                    suggestions.push_back(RelationSuggestion(child->getId(), (*optRel)->getTheOtherPerson(parent->getId()), false, true, Sibling));
                break;
            default:
                continue;
        }
    }
    for(auto&& relId : child->getRelations()){
        auto optRel = getRelation(relId);
        if(!optRel || relId == id) continue;
        Trait t = (*optRel)->getTrait();
        if(forbiddenPersonParent.contains((*optRel)->getTheOtherPerson(child->getId()))) continue;
        switch(t){
            case Sibling: // add childs
                suggestions.push_back(RelationSuggestion(parent->getId(), (*optRel)->getTheOtherPerson(child->getId()), true, false, trait));
                break;
            case Motherhood:
            case Fatherhood:
                if(!(*optRel)->isParent(child->getId())) // add partners
                    suggestions.push_back(RelationSuggestion(parent->getId(), (*optRel)->getTheOtherPerson(child->getId()), false, true, Partnership));
                break;
            default:
                continue;
        }
    }
}

void FamilyTree::getPartnersSuggestions(std::vector<RelationSuggestion>& suggestions, const Person* person, const Person* second, size_t id, const std::set<size_t>& forbiddenPersons){
    for(auto&& relId : person->getRelations()){
        auto optRel = getRelation(relId);
        if(optRel && relId != id){
            if(forbiddenPersons.contains((*optRel)->getTheOtherPerson(person->getId()))) continue;
            Trait t = (*optRel)->getTrait();
            if((t == Motherhood || t == Fatherhood) && (*optRel)->isParent(person->getId()))
                suggestions.push_back(RelationSuggestion(second->getId(), (*optRel)->getTheOtherPerson(person->getId()), true, false, t));
        }
    }
}

void FamilyTree::getSiblingsSuggestions(std::vector<RelationSuggestion>& suggestions, const Person* person, const Person* second, size_t id, const std::set<size_t>& forbiddenPersons){
    for(auto&& relId : person->getRelations()){
        auto optRel = getRelation(relId);
        if(optRel && relId != id){
            if(forbiddenPersons.contains((*optRel)->getTheOtherPerson(person->getId()))) continue;
            Trait t = (*optRel)->getTrait();
            if((t == Motherhood || t == Fatherhood) && !(*optRel)->isParent(mainPerson_->getId()))
                suggestions.push_back(RelationSuggestion(second->getId(), (*optRel)->getTheOtherPerson(person->getId()), false, false, t));
        }
    }
}


std::vector<RelationSuggestion> FamilyTree::getRelationSuggestions(size_t relId){
    std::vector<RelationSuggestion> suggestions;
    auto optRel = getRelation(relId);
    if(!optRel || mainPerson_ == nullptr) return suggestions;
    Relation* rel = *optRel;
    auto optSecPers = getPerson(rel->getTheOtherPerson(mainPerson_->getId()));
    if(!optSecPers) return suggestions;
    Person* secondPerson = *optSecPers;
    Trait trait = rel->getTrait();
    std::set<size_t> forbiddenFirstPersons;
    std::set<size_t> forbiddenSecondPersons;
    for(auto&& relIds : mainPerson_->getRelations()){
        auto optRel_t = getRelation(relIds);
        if(optRel_t) forbiddenFirstPersons.insert((*optRel_t)->getTheOtherPerson(mainPerson_->getId()));
    }
    forbiddenFirstPersons.insert(mainPerson_->getId());
    for(auto&& relIds : secondPerson->getRelations()){
        auto optRel_t = getRelation(relIds);
        if(optRel_t) forbiddenSecondPersons.insert((*optRel_t)->getTheOtherPerson(secondPerson->getId()));
    }
    forbiddenSecondPersons.insert(secondPerson->getId());
    switch(trait){
        case Motherhood:
            if(rel->isParent(mainPerson_->getId()))
                getParentSuggestions(suggestions, mainPerson_, secondPerson, relId, Motherhood, forbiddenFirstPersons, forbiddenSecondPersons);
            else
                getParentSuggestions(suggestions, secondPerson, mainPerson_, relId, Motherhood, forbiddenSecondPersons, forbiddenFirstPersons);
            break;
        case Fatherhood:
            if(rel->isParent(mainPerson_->getId()))
                getParentSuggestions(suggestions, mainPerson_, secondPerson, relId, Fatherhood, forbiddenFirstPersons, forbiddenSecondPersons);
            else
                getParentSuggestions(suggestions, secondPerson, mainPerson_, relId, Fatherhood, forbiddenSecondPersons, forbiddenFirstPersons);
            break;
        case Partnership: // Share children
            getPartnersSuggestions(suggestions, mainPerson_, secondPerson, relId, forbiddenFirstPersons);
            getPartnersSuggestions(suggestions, secondPerson, mainPerson_, relId, forbiddenSecondPersons);
            break;
        case Sibling: // Share parents.
            getSiblingsSuggestions(suggestions, mainPerson_, secondPerson, relId, forbiddenFirstPersons);
            getSiblingsSuggestions(suggestions, secondPerson, mainPerson_, relId, forbiddenSecondPersons);
            break;
        case None:
        default:
            return suggestions;
    }
    return suggestions;
}

Settings* FamilyTree::getSettings(){
	return &settings_;
}

bool FamilyTree::importProject(const std::string& filePath){
    FamilyTree importFT;
    bool success = importFT.openSingleFileDatabase(filePath);
    if(success) append(importFT);
    return success;
}

bool FamilyTree::importRootDir(const std::string& dirPath){
    FamilyTree importFT;
    std::string err;
    std::string bak;
    auto [success, backup] = importFT.openDatabase(dirPath, err, bak);
    if(success) append(importFT);
    return success;
}

std::pair<bool, size_t> FamilyTree::importTemplates(const std::string& filePath){
    savedChanges_ = false;
    Json::Value value;
    bool success = parser_.readJSONFile(filePath, value, false);
    size_t imported = settings_.importTemplates(value);
    return {success, imported};
}

bool FamilyTree::isDirectorySet(){
	return parser_.isRootDirectorySet();
}

bool FamilyTree::isSaved() const{
    return savedChanges_;
}

void FamilyTree::log(const std::string& error){
    parser_.log(error);
}

bool FamilyTree::loadDatabaseFile(const std::string& file, std::function<void(const Json::Value&)> reader, std::string& errorMessage){
    Json::Value root;
    bool success = parser_.readJSONFile(file, root);
    reader(root);
    if(!success) errorMessage = "File " + file + " is corrupted.";
    return success;
}

std::pair<bool, bool> FamilyTree::openDatabase(const std::string& dirPath, std::string& errorMessage, std::string& backupFile){
    bool succes = parser_.setDatabase(dirPath);
    bool backup = parser_.containsBackupFile(backupFile);
    savedChanges_ = true;
	if(!succes){
		errorMessage = "Directory does not exists or does not contain all files or directories.";
		return {succes, backup};
	}
	return {openDatabase(errorMessage), backup};
}

bool FamilyTree::openDatabase(std::string& errorMessage){
    bool success = loadDatabaseFile(parser::JSON_CONFIG, [this](const Json::Value& root){this->settings_.readJson(root);}, errorMessage);
    if(!success) return success; // It is not worth to look trough others.
    success = loadDatabaseFile(parser::JSON_PERSONS, [this](const Json::Value& root){this->readJsonPeople(root);}, errorMessage);
    if(!success) return success;
    success = loadDatabaseFile(parser::JSON_MEDIA, [this](const Json::Value& root){this->readJsonFiles(root, this->allMedia_, jsonlabel::MEDIA, media_index_);}, errorMessage);
    if(!success) return success;
    success = loadDatabaseFile(parser::JSON_FILES, [this](const Json::Value& root){this->readJsonFiles(root, this->allFiles_, jsonlabel::FILES, file_index_);}, errorMessage);
    if(!success) return success;
    success = loadDatabaseFile(parser::JSON_NOTES, [this](const Json::Value& root){this->readJsonFiles(root, this->allNotes_, jsonlabel::NOTES, note_index_);}, errorMessage);
    if(!success) return success;
    success = loadDatabaseFile(parser::JSON_EVENTS, [this](const Json::Value& root){this->readJsonEvents(root);}, errorMessage);
    if(!success) return success;
    success = loadDatabaseFile(parser::JSON_RELATIONS, [this](const Json::Value& root){this->readJsonRelations(root);}, errorMessage);
    if(!success) return success;
	auto optPerson = getPerson(settings_.getGlobalMainPerson());
	if(optPerson) mainPerson_ = (*optPerson);
	return success;
}

bool FamilyTree::openSingleFileDatabase(const std::string& filePath){
    Json::Value root;
    bool success = parser_.readJSONFile(filePath, root, false);
    if(!success) return false;
    if(root[jsonlabel::EXPORT].isNull()) return false;
    if(!root[jsonlabel::EXPORT][jsonlabel::TEMPLATES].isNull())
        settings_.readJson(root[jsonlabel::EXPORT][jsonlabel::TEMPLATES]);
    if(!root[jsonlabel::EXPORT][jsonlabel::EVENTS].isNull())
        readJsonEvents(root[jsonlabel::EXPORT]);
    if(!root[jsonlabel::EXPORT][jsonlabel::RELATIONS].isNull())
        readJsonRelations(root[jsonlabel::EXPORT]);
    for(Json::ArrayIndex i = 0; i < root[jsonlabel::EXPORT][jsonlabel::PERSONS].size(); ++i){
        Person p;
		p.importJson(root[jsonlabel::EXPORT][jsonlabel::PERSONS][i]);
		allPersons_.insert({p.getId(), std::make_unique<Person>(std::move(p))});
		person_index_ = person_index_ <= p.getId() ? p.getId() + 1 : person_index_;
    }
    return true;
}

std::vector<std::string> FamilyTree::getOrphanFiles(){
    std::vector<std::string> files;
    getGeneralOrphaFiles(files, allFiles_, GENERAL_FILE);
    getGeneralOrphaFiles(files, allMedia_, MEDIA);
    getGeneralOrphaFiles(files, allNotes_, NOTE);
    return files;
}

void FamilyTree::printCss(const std::string& dirPath){
    std::stringstream ss;
    ss << "body {" << std::endl;
    ss << "  margin: 0;" << std::endl;
    ss << "  padding: 0;" << std::endl;
    ss << "  font-family: Arial, sans-serif;" << std::endl;
    ss << "  font-size: 16px;" << std::endl;
    ss << "  line-height: 1.5;" << std::endl;
    ss << "  padding-left: 50px;" << std::endl;
    ss << "  padding-right: 50px;" << std::endl;
    ss << "  padding-top: 20px;" << std::endl;
    ss << "  padding-bottom: 20px;" << std::endl;
    ss << "}" << std::endl;
    ss << std::endl;
    ss << "li {" << std::endl;
    ss << "  list-style: none;" << std::endl;
    ss << "}" << std::endl;
    ss << std::endl;
    ss << "info{" << std::endl;
    ss << "  color: #888;" << std::endl;
    ss << "}" << std::endl;
    ss << std::endl;
    ss << "h1 {" << std::endl;
    ss << "  border: 4px solid #000;" << std::endl;
    ss << "  display: inline-block;" << std::endl;
    ss << "  margin: auto;" << std::endl;
    ss << "  width: 100%;" << std::endl;
    ss << "}" << std::endl;
    ss << std::endl;
    ss << "trait{" << std::endl;
    ss << "  font-style: italic;" << std::endl;
    ss << "}" << std::endl;
    ss << "@media print {" << std::endl;
    ss << "  img{" << std::endl;
    ss << "    width: 100%;" << std::endl;
    ss << "  }" << std::endl;
    ss << "}" << std::endl;
    parser_.writeHtml(ss.str(), dirPath, html::CUSTOM_CSS);
}

void FamilyTree::printEvents(std::ostream& os){
	os << "{\"" << jsonlabel::EVENTS << "\":[";
	bool first = true;
	for(auto&& [id, event] : allEvents_){
		if(!first) os << ",";
		first = false;
		os << (*event);
	}
	os << "]}";
}

void FamilyTree::printFiles(std::ostream& os, const std::map<size_t, std::unique_ptr<File>>& container, const std::string& label){
	os << "{ \"" << label << "\":[";
	bool first = true;
	for(auto&& [id, file] : container){
		if(!first) os << ",";
		first = false;
		os << (*file);
	}
	os << "]}";
}

void FamilyTree::printHtml(const std::string& dirPath){
    std::stringstream index;
    printCss(dirPath);
    index << "<html>" << std::endl;
    index << "\t<head>" << std::endl;
    index << "\t\t<title>" << html::ALL_PERSONS << "</title>" << std::endl;
    index << "\t\t<link rel=\"stylesheet\" href=\"" << html::CUSTOM_CSS << "\">" << std::endl;
    index << "\t</head>" << std::endl;
    index << "\t<body>" << std::endl;
    index << "\t\t<h1 align=\"center\">" << html::ALL_PERSONS << "</h1>" << std::endl;
    index << "\t\t<ul>" << std::endl;
    for(auto&& [id, person] : allPersons_){
        std::stringstream ss;
        printHtmlPerson(ss, true, person.get());
        std::stringstream name;
        name << id << ".html";
        parser_.writeHtml(ss.str(), dirPath, name.str());
        index << "\t\t\t<li><a href=\"" << id << ".html\">" << person->str() << "</a></li>" << std::endl;
    }
    index << "\t\t</ul>" << std::endl;
    index << "\t</body>" << std::endl;
    index << "</html>" << std::endl;
    std::string indexHtml = "index.html";
    parser_.writeHtml(index.str(), dirPath, indexHtml);
}

void FamilyTree::printHtmlPerson(std::ostream& os, bool links, Person* p){
    os << "<html>" << std::endl;
    os << "\t<head>" << std::endl;
    os << "\t\t<title>" << p->str() << "</title>" << std::endl;
    os << "\t\t<link rel=\"stylesheet\" href=\"" << html::CUSTOM_CSS << "\">" << std::endl;
    os << "\t</head>" << std::endl;
    os << "\t<body>" << std::endl;
    if(links) os << "\t\t<div align=\"right\"><a href=\"index.html\">" << html::ALL_PERSONS << "</a></div>" << std::endl;
    p->printHtml(os);
    os << "\t\t<h2>" << html::FAMILY_TREE << "</h2>" << std::endl;
    os << "\t\t<p align=\"center\">" << std::endl;
    os << "\t\t\t<img src=\"" << html::RESOURCES << html::PREFIX_FAMILY_TREE << p->getId() << ".png\" alt=\"" << html::FAMILY_TREE_DIAGRAM << "\">" << std::endl;
    os << "\t\t</p>" << std::endl;
    if(p->getRelations().size() > 0) os << "\t\t<h2>" << html::RELATIONS << "</h2>" << std::endl;
    os << "\t\t<h3>" << html::CLOSE_FAMILY << "</h3>" << std::endl;
    os << "\t\t<p align=\"center\">" << std::endl;
    os << "\t\t\t<img src=\"" << html::RESOURCES << html::PREFIX_CLOSE << p->getId() << ".png\" alt=\"" << html::CLOSE_FAMILY_DIAGRAM << "\">" << std::endl;
    os << "\t\t</p>" << std::endl;
    for(auto&& relId : p->getRelations()){
        auto optRelation = getRelation(relId);
        if(!optRelation) continue;
        auto optPerson1 = getPerson((*optRelation)->getFirstPerson());
        auto optPerson2 = getPerson((*optRelation)->getSecondPerson());
        if(!optPerson1 || !optPerson2) continue;
        std::string promoted = EMPTY_STRING;
        std::stringstream firstPerson;
        std::stringstream secondPerson;
        if(relId == p->getFather()) promoted = relation::FATHER;
        else if(relId == p->getMother()) promoted = relation::MOTHER;
        else if(relId == p->getPartner()) promoted = relation::PARTNER;
        if(links){
            firstPerson << "<a href=\"" << (*optPerson1)->getId() << ".html\">";
            secondPerson << "<a href=\"" << (*optPerson2)->getId() << ".html\">";
        }
        firstPerson << (*optPerson1)->str();
        secondPerson << (*optPerson2)->str();
        if(links){
            firstPerson << "</a>";
            secondPerson << "</a>";
        }
        (*optRelation)->printHtml(os, firstPerson.str(), secondPerson.str(), promoted);
    }
    if(p->getEvents().size() > 0)  os << "\t\t<h2>" << html::EVENTS << "</h2>" << std::endl;
    for(auto&& eventId : p->getEvents()){
        auto optEvent = getEvent(eventId);
        if(!optEvent) continue;
        std::vector<std::pair<std::string, std::string>> persons;
        for(auto&& [role, id] : (*optEvent)->getPersons()){
            auto optPerson = getPerson(id);
            if(!optPerson) continue;
            std::stringstream ss;
            if(links) ss << "<a href=\"" << (*optPerson)->getId() << ".html\">";
            ss << (*optPerson)->str();
            if(links) ss << "</a>";
            persons.push_back({role, ss.str()});
        }
        (*optEvent)->printHtml(os, persons);
    }
    os << "\t</body>" << std::endl;
    os << "</html>" << std::endl;
}

void FamilyTree::printMainPersonHtml(const std::string& outputDir){
    if(mainPerson_ == nullptr) return;
    std::stringstream ss;
    printCss(outputDir);
    printHtmlPerson(ss, false, mainPerson_);
    std::stringstream name;
    name << mainPerson_->getId() << ".html";
    parser_.writeHtml(ss.str(), outputDir, name.str());
}

void FamilyTree::printPeople(std::ostream& os){
	os << "{ \"" << jsonlabel::PERSONS << "\":[";
	bool first = true;
	for(auto&& [id, person] : allPersons_){
		if(!first) os << ",";
		first = false;
		os << (*person);
	}
	os << "]}";
}

void FamilyTree::printRelations(std::ostream& os){
	os << "{\"" << jsonlabel::RELATIONS << "\":[";
	bool first = true;
	for(auto&& [id, rel] : allRelations_){
		if(!first) os << ",";
		first = false;
		os << (*rel);
	}
	os << "]}";
}

void FamilyTree::promoteRelation(size_t id){
    auto optRel = getRelation(id);
    if(!optRel){
        std::stringstream ss;
        ss << "Given relation couldn't be promoted due to the absence of the relation.";
        parser_.log(ss.str());
        return;
    }
    Relation* rel = (*optRel);
    auto optPerson1 = getPerson(rel->getFirstPerson());
    auto optPerson2 = getPerson(rel->getSecondPerson());
    if(!optPerson1 || !optPerson2){
        std::stringstream ss;
        ss << "Persons in this relation do not exist. The relation was deleted.";
        parser_.log(ss.str());
        removeRelation(id);
        return;
    }
    Trait t;
    auto optTempl = settings_.getRelationTemplate(rel->getTemplate());
    if(!optTempl) return;
    t = (*optTempl)->getTrait();
    if(rel->isOlderGeneration(rel->getFirstPerson()) && (t == Fatherhood || t == Motherhood))
        (*optPerson2)->promoteRelation(id, t);
    else if(rel->isSameGenration() && t == Partnership){
        (*optPerson1)->promoteRelation(id, t);
        (*optPerson2)->promoteRelation(id, t);
    }
}

void FamilyTree::readJsonEvents(const Json::Value& value){
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::EVENTS].size(); ++i){
        Event e (&settings_);
		e.readJson(value[jsonlabel::EVENTS][i]);
		allEvents_.insert({e.getId(), std::make_unique<Event>(std::move(e))});
		event_index_ = event_index_ <= e.getId() ? e.getId() + 1 : event_index_;
	}
}

void FamilyTree::readJsonFiles(const Json::Value& value, std::map<size_t, std::unique_ptr<File>>& container, const std::string& label, size_t& index){
	for(Json::ArrayIndex i = 0; i < value[label].size(); ++i){
        File f;
		f.readJson(value[label][i]);
		container.insert({f.getId(), std::make_unique<File>(std::move(f))});
		index = index <= f.getId() ? f.getId() + 1 : index;
	}
}

void FamilyTree::readJsonPeople(const Json::Value& value){
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::PERSONS].size(); ++i){
        Person p;
		p.readJson(value[jsonlabel::PERSONS][i]);
		allPersons_.insert({p.getId(), std::make_unique<Person>(std::move(p))});
		person_index_ = person_index_ <= p.getId() ? p.getId() + 1 : person_index_;
	}
}

void FamilyTree::readJsonRelations(const Json::Value& value){
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::RELATIONS].size(); ++i){
        Relation r (&settings_);
		r.readJson(value[jsonlabel::RELATIONS][i]);
		allRelations_.insert({r.getId(), std::make_unique<Relation>(std::move(r))});
		relation_index_ = relation_index_ <= r.getId() ? r.getId() + 1 : relation_index_;
	}
}

void FamilyTree::removeBackup(){
    parser_.removeBackup();
}

size_t FamilyTree::removeGeneralOrphanFile(std::map<size_t, std::unique_ptr<File>>& container, FileType type, const std::string& dir){
    std::map<size_t, bool> usedFiles;
    size_t counter = 0;
    for(auto&& [id, file] : container)
        usedFiles.insert({id, false});
    for(auto&& [id, person] : allPersons_){
        std::vector<size_t> used;
        person->getFilesRoot(type).usedFiles(used);
        for(auto&& fileId : used){
            usedFiles[fileId] = true;
        }
    }
    for(auto&& [id, used] : usedFiles){
        if(!used){
            ++counter;
            auto optFile = getFile(id, type);
            if(optFile){
                parser_.removeFile((*optFile)->getRealName(), dir);
                container.erase(id);
            }
        }
    }
    return counter;
}

void FamilyTree::removeEvent(size_t id){
    savedChanges_ = false;
    auto event = getEvent(id);
    if(event){
        for(auto&& [role, personId] : (*event)->getPersons()){
            auto optPerson = getPerson(personId);
            if(optPerson) (*optPerson)->removeEvent(id);
        }
        allEvents_.erase(id);
    }
}

void FamilyTree::removeEventTemplate(size_t id){
    savedChanges_ = false;
    settings_.removeEventTemplate(id);
    for(auto it = allEvents_.begin(); it != allEvents_.end(); ++it){
        if(it->second->getTemplate() == id){
            for(auto&& person : it->second->getPersons()){
                auto optPerson = getPerson(person.second);
                if(optPerson) (*optPerson)->removeEvent(it->first);
            }
            allEvents_.erase(it);
        }
    }
}

void FamilyTree::removePerson(){
    savedChanges_ = false;
    if(mainPerson_ == nullptr) return;
    size_t index = mainPerson_->getId();
    for(auto&& relId : mainPerson_->getRelations())
        removeRelation(relId);
    for(auto&& eventId : mainPerson_->getEvents()){
        auto optEvent = getEvent(eventId);
        if(optEvent){
            bool removeEv = (*optEvent)->removePerson(index);
            if(removeEv){
                removeEvent(eventId);
            }
        }
    }
    allPersons_.erase(index);
    mainPerson_ = nullptr;
}

void FamilyTree::removeRelation(size_t id){
    savedChanges_ = false;
    auto optRel = getRelation(id);
    if(!optRel){
        return;
    }
    const Relation* rel = (*optRel);
    size_t firstPerson = rel->getFirstPerson();
    size_t secondPerson = rel->getSecondPerson();
    auto optPerson1 = getPerson(firstPerson);
    auto optPerson2 = getPerson(secondPerson);
    if(optPerson1){
        (*optPerson1)->removeRelation(rel->getId());
        (*optPerson1)->updateSpecialRelation(rel->getId(), None);
    }
    if(optPerson2){
        (*optPerson2)->removeRelation(rel->getId());
        (*optPerson2)->updateSpecialRelation(rel->getId(), None);
    }
    allRelations_.erase(rel->getId());
}

void FamilyTree::removeRelationTemplate(size_t id){
    savedChanges_ = false;
    settings_.removeRelationTemplate(id);
    for(auto it = allRelations_.begin(); it != allRelations_.end(); ++it){
        if(it->second->getTemplate() == id){
            size_t firstPerson = it->second->getFirstPerson();
            size_t secondPerson = it->second->getSecondPerson();
            auto optPerson1 = getPerson(firstPerson);
            auto optPerson2 = getPerson(secondPerson);
            if(optPerson1){
                (*optPerson1)->removeRelation(it->first);
                (*optPerson1)->updateSpecialRelation(it->first, None);
            }
            if(optPerson2){
                (*optPerson2)->removeRelation(it->first);
                (*optPerson2)->updateSpecialRelation(it->first, None);
            }
            allRelations_.erase(it);
        }
    }
}

void FamilyTree::renameFile(size_t id, const std::string& newFilename, FileType type){
    savedChanges_ = false;
    auto optFile = getFile(id, type);
    if(!optFile){
        return;
    }
    std::string originalName = (*optFile)->getRealName();
    (*optFile)->setFilename(newFilename);
    std::string newName = (*optFile)->getRealName();
    switch(type){
        case MEDIA:
            parser_.renameFile(originalName, newName, parser::MEDIA_DIR);
            break;
        case NOTE:
            parser_.renameFile(originalName, newName, parser::NOTES_DIR);
            break;
        case GENERAL_FILE:
        default:
            parser_.renameFile(originalName, newName, parser::FILES_DIR);
            break;
    }

}

bool FamilyTree::restoreBackup(const std::string& backupFile, std::string& errorMessage){
    parser_.restoreBackup(backupFile);
    allRelations_.clear();
    allEvents_.clear();
    allFiles_.clear();
    allMedia_.clear();
    allNotes_.clear();
    allPersons_.clear();
    event_index_ = 1;
    relation_index_ = 1;
    file_index_ = 1;
    media_index_ = 1;
    note_index_ = 1;
    person_index_ = 1;
    mainPerson_ = nullptr;
    settings_.clear();
    return openDatabase(errorMessage);
}

void FamilyTree::setMainPerson(Person* p){
	mainPerson_ = p;
}

void FamilyTree::setMainPerson(size_t id){
    auto optPerson = getPerson(id);
    if(optPerson)
        mainPerson_ = (*optPerson);
    else
        mainPerson_ = nullptr;
}

void FamilyTree::setRelation(size_t relId, size_t pers1Id, size_t pers2Id, size_t templId){
    savedChanges_ = false;
    auto optRel = getRelation(relId);
    Relation* rel;
    if(!optRel){
        std::stringstream ss;
        ss << "Relation couln't be updated so new one was created.";
        parser_.log(ss.str());
        rel = addRelation();
    }
    else
        rel = (*optRel);
    auto optOriginPerson1 = getPerson(rel->getFirstPerson());
    auto optOriginPerson2 = getPerson(rel->getSecondPerson());
    if(optOriginPerson1)
        (*optOriginPerson1)->removeRelation(relId);
    if(optOriginPerson2)
        (*optOriginPerson2)->removeRelation(relId);
    rel->setTemplate(templId);
    rel->setPersons(pers1Id, pers2Id);
    auto optPerson1 = getPerson(pers1Id);
    auto optPerson2 = getPerson(pers2Id);
    if(!optPerson1 || !optPerson2){
        std::stringstream ss;
        ss << "Persons in this relation do not exist. The relation was deleted.";
        parser_.log(ss.str());
        removeRelation(relId);
    }
    else{
        (*optPerson1)->addRelation(relId);
        (*optPerson2)->addRelation(relId);
    }
    if(optOriginPerson1)
        (*optOriginPerson1)->updateSpecialRelation(relId, rel->getTrait());
    if(optOriginPerson2)
        (*optOriginPerson2)->updateSpecialRelation(relId, rel->getTrait());
}

void FamilyTree::setUnsaved(){
    savedChanges_ = false;
}

bool FamilyTree::showHelpOnStartup(){
    return openHelp;
}

bool FamilyTree::storeDatabase(){
	if(parser_.isRootDirectorySet()){
		{
			std::stringstream ss;
			printPeople(ss);
			parser_.writeJSON(ss.str(), parser::JSON_PERSONS);
		}
		storeFileDatabase(GENERAL_FILE);
		storeFileDatabase(MEDIA);
		storeFileDatabase(NOTE);
		{
			std::stringstream ss;
			ss << settings_;
			parser_.writeJSON(ss.str(), parser::JSON_CONFIG);
		}
		{
			std::stringstream ss;
			printRelations(ss);
			parser_.writeJSON(ss.str(), parser::JSON_RELATIONS);
		}
		{
			std::stringstream ss;
			printEvents(ss);
			parser_.writeJSON(ss.str(), parser::JSON_EVENTS);
		}
		savedChanges_ = true;
		return true;
	}
	else{
		log("Directory was not set");
		return false;
	}
}

void FamilyTree::getUrlFile(size_t id, std::string& fileUrl, FileType type){
    std::string fileName;
    auto optFile = getFile(id, type);
    if(!optFile) return;
    switch (type){
        case MEDIA:
            fileName = (*optFile)->getRealName();
            parser_.getFileUrl(parser::MEDIA_DIR, fileName, fileUrl);
            break;
        case NOTE:
            fileName = (*optFile)->getRealName();
            parser_.getFileUrl(parser::NOTES_DIR, fileName, fileUrl);
            break;
        case GENERAL_FILE:
        default:
            fileName = (*optFile)->getRealName();
            parser_.getFileUrl(parser::FILES_DIR, fileName, fileUrl);
            break;
    }
}

void FamilyTree::storeFileDatabase(FileType type){
    std::stringstream ss;
    switch(type){
        case NOTE:
            removeGeneralOrphanFile(allNotes_, type, parser::NOTES_DIR);
			printFiles(ss, allNotes_, jsonlabel::NOTES);
			parser_.writeJSON(ss.str(), parser::JSON_NOTES);
			break;
        case MEDIA:
            removeGeneralOrphanFile(allMedia_, type, parser::MEDIA_DIR);
			printFiles(ss, allMedia_, jsonlabel::MEDIA);
			parser_.writeJSON(ss.str(), parser::JSON_MEDIA);
			break;
        case GENERAL_FILE:
        default:
            removeGeneralOrphanFile(allFiles_, type, parser::FILES_DIR);
			printFiles(ss, allFiles_, jsonlabel::FILES);
			parser_.writeJSON(ss.str(), parser::JSON_FILES);
			break;
    }
}

size_t FamilyTree::templatesBasedOnEventTemplate(size_t templateId){
    size_t counter = std::count_if(allEvents_.begin(), allEvents_.end(),
                                   [templateId](auto&& ev){return ev.second->getTemplate() == templateId;});
    return counter;
}

size_t FamilyTree::templatesBasedOnRelationTemplate(size_t templateId){
    size_t counter = std::count_if(allRelations_.begin(), allRelations_.end(),
                                   [templateId](auto&& rel){return rel.second->getTemplate() == templateId;});
    return counter;
}

void FamilyTree::updateEventsWithTemplate(size_t templ){
    for(auto&& [id, event] : allEvents_){
        if(event->getTemplate() == templ){
            auto removed = event->updateToTemplate();
            for(auto&& idp : removed){
                auto optPerson = getPerson(idp);
                if(!optPerson) continue;
                (*optPerson)->removeEvent(event->getId());
            }
            if(event->getPersons().size() == 0){
                removeEvent(event->getId());
            }
        }
    }
}

bool FamilyTree::writeConfig(const std::string& newPath){
    bool exists = std::any_of(projectPaths.begin(), projectPaths.end(), [newPath](auto&& path){return path == newPath;});
    if(!exists) projectPaths.push_back(newPath);
    parser_.writeConfig(projectPaths, openHelp);
    return !exists;
}
