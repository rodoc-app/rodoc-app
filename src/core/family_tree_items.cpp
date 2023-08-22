/// @file family_tree_items.cpp Source file for classes closely related to family tree.
#include "family_tree_items.h"

// =====================================================================
// Event
// =====================================================================

Event::Event(Settings* settings) : id_(1), settings_(settings), template_(1){}

void Event::addPerson(size_t person, const std::string& role){
	if(role == events::NO_ROLE){
        if(std::any_of(persons_.begin(), persons_.end(), [person](auto&& pair){return pair.second == person;}))
            return;
	}
	else{
		for(auto&& [role_pair, person_pair] : persons_){
			if(role_pair == role){
				person_pair = person;
				return;
			}
		}
	}
	persons_.push_back({role, person});
}

void Event::clearPersons(){
    persons_.clear();
}

WrappedDate* Event::date(){
    return &date_;
}

const WrappedDate& Event::getDate() const{
	return date_;
}

size_t Event::getId() const{
	return id_;
}

const std::vector<std::pair<std::string, size_t>>& Event::getPersons() const{
	return persons_;
}

const std::string& Event::getPlace() const{
	return place_;
}

const Settings& Event::getSettings() const{
	return *settings_;
}

size_t Event::getTemplate() const{
	return template_;
}

const std::string& Event::getText() const{
	return text_;
}

void Event::printHtml(std::ostream& os, const std::vector<std::pair<std::string, std::string>>& persons){
    auto optTemplate = settings_->getEventTemplate(template_);
    if(!optTemplate) return;
    os << "\t\t<h3>" << (*optTemplate)->getTitle() << "</h3>" << std::endl;
    os << "\t\t<p><strong>" << html::DESCRIPTION << "</strong> " << (*optTemplate)->getDescription() << "</p>" << std::endl;
    os << "\t\t<p><strong>" << html::NOTE << "</strong> " << text_ << "</p>";
    os << "\t\t<ul>" << std::endl;
    if((*optTemplate)->containsDate()) os << "\t\t\t<li>" << html::DATE << date_.str() << "</li>" << std::endl;
    if((*optTemplate)->containsPlace()) os << "\t\t\t<li>" << html::PLACE << place_ << "</li>" << std::endl;
    os << "\t\t</ul>" << std::endl;
    if((*optTemplate)->hasMorePeopleInvolved() || (*optTemplate)->hasRoles()){
        os << "\t\t<h4>" << html::PERSONS << "</h4>" << std::endl;
        os << "\t\t<ul>" << std::endl;
        for(auto&& [role, person] : persons){
            os << "\t\t\t<li>" << role << ": " << person << "</li>" << std::endl;
        }
        os << "\t\t</ul>" << std::endl;
    }
}

void Event::readJson(const Json::Value& value){
	id_ = value[jsonlabel::ID].asUInt64();
	template_ = value[jsonlabel::TEMPLATE].asUInt64();
	auto optTempl = settings_->getEventTemplate(template_);
	if(!optTempl) return;
	auto templ = *optTempl;
    for(Json::ArrayIndex i = 0; i < value[jsonlabel::PERSONS].size(); ++i){
        std::string role;
        if(value[jsonlabel::PERSONS][i][jsonlabel::ROLE].isNull())
            role = events::NO_ROLE;
        else
            role = value[jsonlabel::PERSONS][i][jsonlabel::ROLE].asString();
        addPerson(value[jsonlabel::PERSONS][i][jsonlabel::PERSON].asUInt64(), role);
    }
	if(templ->containsDate())
		date_.readJson(value[jsonlabel::DATE]);
	if(templ->containsPlace())
		place_ = value[jsonlabel::PLACE].asString();
	text_ = value[jsonlabel::TEXT].asString();
}

bool Event::removePerson(size_t id){
    auto it = std::find_if(persons_.begin(), persons_.end(), [id](auto&& pair){return pair.second == id;});
    persons_.erase(it);
    return persons_.size() == 0;
}

WrappedDate& Event::setDate(){
	return date_;
}

void Event::setId(size_t id){
	id_ = id;
}

void Event::setPlace(const std::string& place){
	place_ = place;
}

void Event::setTemplate(size_t templ){
	template_ = templ;
}

void Event::setText(const std::string& text){
	text_ = text;
}

std::string Event::str() const{
    std::stringstream ss;
    auto optTempl = settings_->getEventTemplate(template_);
    if(!optTempl) return "!NON EXISTING TEMPLATE!";
    auto templ = (*optTempl);
    ss << templ->getTitle();
    return ss.str();
}

std::vector<size_t> Event::updateToTemplate(){
    auto optTempl = settings_->getEventTemplate(template_);
    std::vector<size_t> removed;
    if(!optTempl) return removed;
    EventTemplate* templ = *optTempl;
    if(!templ->containsDate())
        date_.clear();
    if(!templ->containsPlace())
        place_ = EMPTY_STRING;
    if(!templ->hasRoles()){
        for(auto it = persons_.begin(); it != persons_.end(); ++it)
            it->first = events::NO_ROLE;
    }
    else{
        auto roles = templ->getRoles();
        for(auto it = persons_.begin(); it != persons_.end(); ++it){
            auto found = std::find_if(roles.begin(), roles.end(), [it](auto role){return it->first == role;});
            if(found == roles.end())
                it->first = events::NO_ROLE;
        }
    }
    if(!templ->hasMorePeopleInvolved() && !templ->hasRoles()){
        while(persons_.size() > 1){
            auto [role, id] = persons_[persons_.size() - 1];
            persons_.pop_back();
            removed.push_back(id);
        }
    }
    else if(!templ->hasMorePeopleInvolved() && templ->hasRoles()){
        for(auto it = persons_.begin(); it != persons_.end(); ){
            if(it->first == events::NO_ROLE){
                removed.push_back(it->second);
                persons_.erase(it);
            }
            else ++it;
        }
    }
    return removed;
}

// =====================================================================
// functions for Event
// =====================================================================

std::ostream& operator <<(std::ostream& os, const Event& e){
	auto optTempl = e.getSettings().getEventTemplate(e.getTemplate());
	if(!optTempl) return os;
	auto templ = *optTempl;
	os << "{\"" << jsonlabel::ID << "\":" << e.getId();
	os << ", \"" << jsonlabel::TEMPLATE << "\":" << e.getTemplate();
    os << ",\"" << jsonlabel::PERSONS << "\":[";
    bool first = true;
    for(auto&& [role, person] : e.getPersons()){
        if(!first) os << ",";
        first = false;
        os << "{\"" << jsonlabel::PERSON << "\":" << person;
        if(role != events::NO_ROLE)
            os << ",\"" << jsonlabel::ROLE << "\":\"" << role << "\"";
        os << "}";
    }
    os << "]";
	if(templ->containsDate())
		os << "," << e.getDate();
	if(templ->containsPlace())
		os << ", \"" << jsonlabel::PLACE << "\":\"" << e.getPlace() << "\"";
	os << ",\"" << jsonlabel::TEXT << "\":\"" << e.getText() << "\"";
	os << "}";
	return os;
}

// =====================================================================
// Relation
// =====================================================================

Relation::Relation(Settings* settings) : id_(1), person1_(0), person2_(0), settings_(settings), template_(0){}

size_t Relation::getFirstPerson() const{
	return person1_;
}

size_t Relation::getId() const{
	return id_;
}

size_t Relation::getSecondPerson() const{
	return person2_;
}

size_t Relation::getTemplate() const{
	return template_;
}

const std::string& Relation::getTitle() const{
    auto optRelTempl = settings_->getRelationTemplate(template_);
    if(!optRelTempl) return EMPTY_STRING;
    return (*optRelTempl)->getTitle();
}

size_t Relation::getTheOtherPerson(size_t current){
    if(person1_ == current)
        return person2_;
    return person1_;
}

const std::string& Relation::getTheOtherPersonName(size_t current){
    auto optTempl = settings_->getRelationTemplate(template_);
    if(!optTempl)
        return EMPTY_STRING;
    if(person1_ == current)
        return (*optTempl)->getSecondName();
    return (*optTempl)->getFirstName();
}

Trait Relation::getTrait(){
    auto optRelTempl = settings_->getRelationTemplate(template_);
    if(!optRelTempl) return None;
    return (*optRelTempl)->getTrait();
}

bool Relation::isOlderGeneration(size_t id){
    auto optTempl = settings_->getRelationTemplate(template_);
    if(!optTempl) return false;
    if(person1_ == id)
        return (*optTempl)->getGenerationDifference() < 0;
    return (*optTempl)->getGenerationDifference() > 0;
}

bool Relation::isParent(size_t personId){
    auto optTempl = settings_->getRelationTemplate(template_);
    if(!optTempl) return false;
    bool parent = (*optTempl)->getTrait() == Fatherhood || (*optTempl)->getTrait() == Motherhood;
    return parent && isOlderGeneration(personId);
}

bool Relation::isSameGenration(){
    auto optTempl = settings_->getRelationTemplate(template_);
    if(!optTempl) return false;
    return (*optTempl)->getGenerationDifference() == 0;
}

bool Relation::isYoungerGeneration(size_t id){
    auto optTempl = settings_->getRelationTemplate(template_);
    if(!optTempl) return false;
    if(person1_ == id)
        return (*optTempl)->getGenerationDifference() > 0;
    return (*optTempl)->getGenerationDifference() < 0;
}

void Relation::printHtml(std::ostream& os, const std::string& firstPerson, const std::string& secondPerson, const std::string& promoted){
    auto optTemplate = settings_->getRelationTemplate(template_);
    if(!optTemplate) return;
    os << "\t\t<h3>" << (*optTemplate)->getTitle();
    if(promoted != EMPTY_STRING) os << " <trait>[" << promoted << "]</trait>";
    os << "\t\t</h3>" << std::endl;
    os << "\t\t<p><strong>" << html::DESCRIPTION << "</strong> " << (*optTemplate)->getDescription() << "</p>" << std::endl;
    os << "\t\t<ul>" << std::endl;
    os << "\t\t\t<li>" << (*optTemplate)->getFirstName() << ": " << firstPerson << "</li>" << std::endl;
    os << "\t\t\t<li>" << (*optTemplate)->getSecondName() << ": " << secondPerson << "</li>" << std::endl;
    os << "\t\t</ul>" << std::endl;
    os << "\t\t<p>" << "The difference between these generations is " << (*optTemplate)->getGenerationDifference();
    os << ". <info>Which is the difference between the generation of ";
    os << (*optTemplate)->getFirstName() << " minus the generation of " << (*optTemplate)->getSecondName();
    os << ".</info></p>" << std::endl;
}

void Relation::readJson(const Json::Value& value){
	id_ = value[jsonlabel::ID].asUInt64();
	person1_ = value[jsonlabel::FIRST_PERSON].asUInt64();
	person2_ = value[jsonlabel::SECOND_PERSON].asUInt64();
	template_ = value[jsonlabel::TEMPLATE].asUInt64();
}

void Relation::setId(size_t id){
	id_ = id;
}

void Relation::setPersons(size_t person1, size_t person2){
	person1_ = person1;
	person2_ = person2;
}

void Relation::setTemplate(size_t templ){
	template_ = templ;
}

std::string Relation::str(const std::string& firstPerson, const std::string& secondPerson) const{
    std::stringstream ss;
    auto optTempl = settings_->getRelationTemplate(template_);
    if(!optTempl) return EMPTY_STRING;
    ss << (*optTempl)->getTitle() << ": " << (*optTempl)->getFirstName() << " (" << firstPerson << ") + ";
    ss << (*optTempl)->getSecondName() << " (" << secondPerson << ")";
    return ss.str();
}

// =====================================================================
// functions for Relation
// =====================================================================

std::ostream& operator<<(std::ostream& os, const Relation& r){
	os << "{\"" << jsonlabel::ID << "\":" << r.getId();
	os << ",\"" << jsonlabel::FIRST_PERSON << "\":" << r.getFirstPerson();
	os << ",\"" << jsonlabel::SECOND_PERSON << "\":" << r.getSecondPerson();
	os << ",\"" << jsonlabel::TEMPLATE << "\":" << r.getTemplate();
	os << "}";
	return os;
}

// ====================================================================
// RelationSuggestion
// ====================================================================

RelationSuggestion::RelationSuggestion(size_t lockedP, size_t suggestedP, bool par, bool sym, Trait t)
  : lockedPerson(lockedP), parent(par), suggestedPerson(suggestedP), symetrical(sym), trait(t){}

// =====================================================================
// File
// =====================================================================

File::File() : id_(1){}

const std::string& File::getFilename() const{
	return filename_;
}

size_t File::getId() const{
	return id_;
}

std::string File::getRealName() const{
	std::stringstream ss;
	ss << id_ << "-" << filename_;
	return ss.str();
}

void File::readJson(const Json::Value& value){
	id_ = value[jsonlabel::ID].asUInt64();
	filename_ = value[jsonlabel::NAME].asString();
}

void File::setFilename(const std::string& fileName){
	filename_ = fileName;
}

void File::setId(size_t id){
	id_ = id;
}

// =====================================================================
// functions for File
// =====================================================================

std::ostream& operator <<(std::ostream& os, const File& f){
	os << "{\"" << jsonlabel::ID << "\":" << f.getId();
	os << ", \"" << jsonlabel::NAME << "\":\"" << f.getFilename() << "\"";
	os << "}";
	return os;
}
