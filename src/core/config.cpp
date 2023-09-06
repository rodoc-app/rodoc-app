/// @file config.cpp Source code for configuring the app such as special evens and relations.
#include "config.h"

// =====================================================================
// EventTemplate
// =====================================================================

EventTemplate::EventTemplate() : containsDate_(false), containsPlace_(false), id_(0), morePeopleInvolved_(false){}

bool EventTemplate::containsDate() const{
	return containsDate_;
}

bool EventTemplate::containsPlace() const{
	return containsPlace_;
}

const std::string& EventTemplate::getDescription() const{
	return description_;
}

size_t EventTemplate::getId() const{
	return id_;
}

const std::vector<std::string>& EventTemplate::getRoles() const{
	return roles_;
}

const std::string& EventTemplate::getTitle() const{
	return title_;
}

bool EventTemplate::hasMorePeopleInvolved() const{
	return morePeopleInvolved_;
}

bool EventTemplate::hasRoles() const{
	return (roles_.size() != 0);
}

void EventTemplate::readJson(const Json::Value& value){
	title_ = value[jsonlabel::TITLE].asString();
	id_ = value[jsonlabel::ID].asUInt64();
	description_ = value[jsonlabel::DESCRIPTION].asString();
	containsDate_ = value[jsonlabel::CONTAINS_DATE].asInt();
	containsPlace_ = value[jsonlabel::CONTAINS_PLACE].asInt();
	morePeopleInvolved_ = value[jsonlabel::MORE_PEOPLE].asInt();
	if(!value[jsonlabel::SPECIAL_ROLES].isNull()){
		for(Json::ArrayIndex index = 0; index < value[jsonlabel::SPECIAL_ROLES].size(); ++index){
			roles_.push_back(value[jsonlabel::SPECIAL_ROLES][index][jsonlabel::NAME].asString());
		}
	}
}

void EventTemplate::setContainsDate(bool hasDate){
    containsDate_ = hasDate;
}

void EventTemplate::setContainsPlace(bool hasPlace){
    containsPlace_ = hasPlace;
}

std::string& EventTemplate::setDescription(){
	return description_;
}

void EventTemplate::setId(size_t id){
    id_ = id;
}

void EventTemplate::setMorePeopleInvolved(bool morePeople){
	morePeopleInvolved_ = morePeople;
}

void EventTemplate::setRoles(std::vector<std::string>& roles){
    roles_.clear();
    std::copy(roles.begin(), roles.end(),  std::back_inserter(roles_));
}

std::string& EventTemplate::setTitle(){
	return title_;
}

std::string EventTemplate::str() const{
    std::stringstream ss;
    ss << title_ << " (" << description_ << ")";
    return ss.str();
}

// =====================================================================
// functions for EventTemplate
// =====================================================================

std::ostream& operator <<(std::ostream& os, const EventTemplate& et){
	os << "{\"" << jsonlabel::TITLE << "\":\"" << et.getTitle() << "\"";
	os << ",\"" << jsonlabel::ID << "\":" << et.getId();
	os << ",\""<< jsonlabel::DESCRIPTION << "\":\"" << et.getDescription() << "\"";
	os << ", \"" << jsonlabel::CONTAINS_DATE << "\":" << et.containsDate();
	os << ", \"" << jsonlabel::CONTAINS_PLACE << "\":" << et.containsPlace();
	os << ", \"" << jsonlabel::MORE_PEOPLE << "\":" << et.hasMorePeopleInvolved();
	if(et.hasRoles()){
		bool first = true;
		os << ", \"" << jsonlabel::SPECIAL_ROLES << "\": [";
		for(auto&& str : et.getRoles()){
			if(!first){
				os << ",";
			}
			os << "{\"" << jsonlabel::NAME << "\":\"" << str << "\"}";
			first = false;
		}
		os << "]";
	}
	os << "}";
	return os;
}

// =====================================================================
// Trait
// =====================================================================

std::string trait::str(Trait t){
    switch(t){
        case Fatherhood:
            return "Fatherhood";
        case Motherhood:
            return "Motherhood";
        case Partnership:
            return "Partnership";
        case Sibling:
            return "Sibling";
        case None:
        default:
            return "No special trait";
    }
}

// =====================================================================
// RelationTemplate
// =====================================================================

RelationTemplate::RelationTemplate() : generationDiff_(0), id_(0), trait_(None){}

const std::string& RelationTemplate::getDescription() const{
	return description_;
}

const std::string& RelationTemplate::getFirstName() const{
	return firstName_;
}

int RelationTemplate::getGenerationDifference() const{
	return generationDiff_;
}

size_t RelationTemplate::getId() const{
	return id_;
}

const std::string& RelationTemplate::getSecondName() const{
	return secondName_;
}

const std::string& RelationTemplate::getTitle() const{
	return title_;
}

Trait RelationTemplate::getTrait() const{
	return trait_;
}

void RelationTemplate::readJson(const Json::Value& value){
	title_ = value[jsonlabel::TITLE].asString();
	id_ = value[jsonlabel::ID].asUInt64();
	description_ = value[jsonlabel::DESCRIPTION].asString();
	firstName_ = value[jsonlabel::FIRST_NAME].asString();
	secondName_ = value[jsonlabel::SECOND_NAME].asString();
	generationDiff_ = value[jsonlabel::GEN_DIFF].asInt();
	trait_ = Trait(value[jsonlabel::TRAIT].asInt());
}

std::string& RelationTemplate::setDescription(){
	return description_;
}

std::string& RelationTemplate::setFirstName(){
    return firstName_;
}

void RelationTemplate::setGenerationDifference(int diff){
	generationDiff_ = diff;
}

void RelationTemplate::setId(size_t id){
    id_ = id;
}

std::string& RelationTemplate::setSecondName(){
    return secondName_;
}

std::string& RelationTemplate::setTitle(){
	return title_;
}

void RelationTemplate::setTrait(Trait trait){
	trait_ = trait;
}

std::string RelationTemplate::str() const{
    std::stringstream ss;
    ss << title_ << " [" << trait::str(trait_) << "] (" << description_ << ")";
    return ss.str();
}

// =====================================================================
// functions for RelationTemplate
// =====================================================================

std::ostream& operator <<(std::ostream& os, const RelationTemplate& rt){
	os << "{\"" << jsonlabel::TITLE << "\":\"" << rt.getTitle() << "\"";
	os << ",\"" << jsonlabel::ID << "\":" << rt.getId();
	os << ",\"" << jsonlabel::DESCRIPTION << "\":\"" << rt.getDescription() << "\"";
	os << ", \"" << jsonlabel::FIRST_NAME << "\":\"" << rt.getFirstName() << "\"";
	os << ", \"" << jsonlabel::SECOND_NAME << "\":\"" << rt.getSecondName() << "\"";
	os << ", \"" << jsonlabel::GEN_DIFF << "\":" << rt.getGenerationDifference();
	os << ", \"" << jsonlabel::TRAIT << "\":" << rt.getTrait();
	os << "}";
	return os;
}

// =====================================================================
// Color
// =====================================================================

Color::Color(int r, int g, int b){
    red = r;
    green = g;
    blue = b;
}

void Color::readJson(const Json::Value& value){
    red = value[jsonlabel::RED].asInt();
    green = value[jsonlabel::GREEN].asInt();
    blue = value[jsonlabel::BLUE].asInt();
}

std::ostream& operator<<(std::ostream& os, const Color& color){
    os << "{";
    os << "\"" << jsonlabel::RED << "\":" << color.red;
    os << ",\"" << jsonlabel::GREEN << "\":" << color.green;
    os << ",\"" << jsonlabel::BLUE << "\":" << color.blue;
    os << "}";
    return os;
}

// =====================================================================
// AppSettings
// =====================================================================

void AppSettings::readJson(const Json::Value& value){
    tabPosition = TabPosition(value[jsonlabel::TAB_POSITION].asInt());
    showEventTab = value[jsonlabel::SHOW_EVENT_TAB].asInt();
    showFileTab = value[jsonlabel::SHOW_FILE_TAB].asInt();
    showGeneralTab = value[jsonlabel::SHOW_GENERAL_TAB].asInt();
    showMediaTab = value[jsonlabel::SHOW_MEDIA_TAB].asInt();
    showNoteTab = value[jsonlabel::SHOW_NOTE_TAB].asInt();
    showRelationTab = value[jsonlabel::SHOW_RELATION_TAB].asInt();
    fontFamily = value[jsonlabel::FONT_FAMILY].asString();
    fontSize = value[jsonlabel::FONT_SIZE].asInt();
    probandColor.readJson(value[jsonlabel::PROBANT_COLOR]);
    promoted.readJson(value[jsonlabel::PROMOTED_COLOR]);
    standard.readJson(value[jsonlabel::STANDARD_COLOR]);
    highlightedColor.readJson(value[jsonlabel::HIGHLIGHTED_COLOR]);
    sizeX = value[jsonlabel::X_SIZE].asInt();
    sizeY = value[jsonlabel::Y_SIZE].asInt();
    genSizeUp = value[jsonlabel::GEN_SIZE_UP].asInt();
    genSizeDown = value[jsonlabel::GEN_SIZE_DOWN].asInt();
    splitterPositionOne = value[jsonlabel::SPLITTER_POSITION_ONE].asInt();
    splitterPositionTwo = value[jsonlabel::SPLITTER_POSITION_TWO].asInt();
    textColor.readJson(value[jsonlabel::TEXT_COLOR]);
    lineColor.readJson(value[jsonlabel::LINE_COLOR]);
    lineWidth = value[jsonlabel::LINE_WIDTH].asInt();
    radius = value[jsonlabel::BORDER_RADIUS].asInt();
}

void AppSettings::setDefault(){
    tabPosition = North;
    showEventTab = true;
    showFileTab = true;
    showGeneralTab = true;
    showMediaTab = true;
    showNoteTab = true;
    showRelationTab = true;
    fontFamily = "Liberation Sans";
    fontSize = 10;
    probandColor = Color(255, 190, 111);
    promoted = Color(143, 240, 164);
    standard = Color(246, 245, 244);
    highlightedColor = Color(153, 193, 241);
    sizeX = 250;
    sizeY = 80;
    genSizeUp = -1;
    genSizeDown = -1;
    splitterPositionOne = 2;
    splitterPositionTwo = 5;
    textColor = Color(0, 0, 0);
    lineColor = Color(0, 0, 0);
    lineWidth = 3;
    radius = 5;
}

std::ostream& operator<<(std::ostream& os, const AppSettings& as){
    os << "{";
    os << "\"" << jsonlabel::TAB_POSITION << "\":" << as.tabPosition;
    os << ",\"" << jsonlabel::SHOW_GENERAL_TAB << "\":" << as.showGeneralTab;
    os << ",\"" << jsonlabel::SHOW_EVENT_TAB << "\":" << as.showEventTab;
    os << ",\"" << jsonlabel::SHOW_FILE_TAB << "\":" << as.showFileTab;
    os << ",\"" << jsonlabel::SHOW_MEDIA_TAB << "\":" << as.showMediaTab;
    os << ",\"" << jsonlabel::SHOW_NOTE_TAB << "\":" << as.showNoteTab;
    os << ",\"" << jsonlabel::SHOW_RELATION_TAB << "\":" << as.showRelationTab;
    os << ",\"" << jsonlabel::PROMOTED_COLOR << "\":" << as.promoted;
    os << ",\"" << jsonlabel::STANDARD_COLOR << "\":" << as.standard;
    os << ",\"" << jsonlabel::HIGHLIGHTED_COLOR << "\":" << as.highlightedColor;
    os << ",\"" << jsonlabel::PROBANT_COLOR << "\":" << as.probandColor;
    os << ",\"" << jsonlabel::FONT_FAMILY << "\":\"" << as.fontFamily << "\"";
    os << ",\"" << jsonlabel::FONT_SIZE << "\":" << as.fontSize;
    os << ",\"" << jsonlabel::X_SIZE << "\":" << as.sizeX;
    os << ",\"" << jsonlabel::Y_SIZE << "\":" << as.sizeY;
    os << ",\"" << jsonlabel::GEN_SIZE_UP << "\":" << as.genSizeUp;
    os << ",\"" << jsonlabel::GEN_SIZE_DOWN << "\":" << as.genSizeDown;
    os << ",\"" << jsonlabel::SPLITTER_POSITION_ONE << "\":" << as.splitterPositionOne;
    os << ",\"" << jsonlabel::SPLITTER_POSITION_TWO << "\":" << as.splitterPositionTwo;
    os << ",\"" << jsonlabel::TEXT_COLOR << "\":" << as.textColor;
    os << ",\"" << jsonlabel::LINE_COLOR << "\":" << as.lineColor;
    os << ",\"" << jsonlabel::LINE_WIDTH << "\":" << as.lineWidth;
    os << ",\"" << jsonlabel::BORDER_RADIUS << "\":" << as.radius;
    os << "}";
    return os;
}

// =====================================================================
// Settings
// =====================================================================

Settings::Settings() : appSettings_(AppSettings()), eventTemplateIndex_(1), globalMainPerson_(0), relationTemplateIndex_(1){}

void Settings::addDefaultEvent(const std::string& title, const std::string& description, bool date, bool place, bool morePeople, std::vector<std::string> roles){
    EventTemplate templ;
    templ.setTitle() = title;
    templ.setDescription() = description;
    templ.setContainsDate(date);
    templ.setContainsPlace(place);
    templ.setMorePeopleInvolved(morePeople);
    templ.setRoles(roles);
    addEventTemplate(templ);

}

void Settings::addDefaultEvents(){
    addDefaultEvent(events::WEDDING, events::DESC_WEDDING, true, true, true, {events::GROOM, events::BRIDE, events::BEST_MAN, events::MAID_OF_HONOR});
    addDefaultEvent(events::DIVORCE, events::DESC_DIVORCE, true, true, false, {events::FORMER_HUSBAND, events::FORMER_WIFE});
    addDefaultEvent(events::FUNERAL, events::DESC_FUNERAL, true, true, true, {events::DEAD_PERSON});
    addDefaultEvent(events::GRADUATION, events::DESC_GRADUATION, true, true, true, {events::GRADUATE});
}

void Settings::addDefaultRelation(const std::string& title, const std::string& description, const std::string& firstName, const std::string& secondName, int genDiff, Trait trait){
    RelationTemplate templ;
    templ.setTitle() = title;
    templ.setDescription() = description;
    templ.setFirstName() = firstName;
    templ.setSecondName() = secondName;
    templ.setGenerationDifference(genDiff);
    templ.setTrait(trait);
    addRelationTemplate(templ);
}

void Settings::addDefaultRelations(){
    addDefaultRelation(relation::FATHERHOOD, relation::FATHERHOOD_DESC, relation::FATHER, relation::CHILD, -1, Fatherhood);
	addDefaultRelation(relation::MOTHERHOOD, relation::MOTHERHOOD_DESC, relation::MOTHER, relation::CHILD, -1, Motherhood);
	addDefaultRelation(relation::ADOPTIVE_FATHERHOOD, relation::DESC_ADOPTIVE_FATHERHOOD, relation::ADOPTIVE_FATHER, relation::ADOPTED_CHILD, -1, Fatherhood);
	addDefaultRelation(relation::ADOPTIVE_MOTHERHOOD, relation::DESC_ADOPTIVE_MOTHERHOOD, relation::ADOPTIVE_MOTHER, relation::ADOPTED_CHILD, -1, Motherhood);
	addDefaultRelation(relation::PARTNERSHIP, relation::PARTNERSHIP_DESC, relation::PARTNER, relation::PARTNER, 0, Partnership);
	addDefaultRelation(relation::MARRIAGE, relation::MARRIAGE_DESC, relation::HUSBAND, relation::WIFE, 0, Partnership);
	addDefaultRelation(relation::BROTHERHHOD, relation::DESC_BROTHERHOOD, relation::BROTHER, relation::BROTHER, 0, Sibling);
	addDefaultRelation(relation::SISTERHOOD, relation::DESC_SISTERHOOD, relation::SISTER, relation::SISTER, 0, Sibling);
	addDefaultRelation(relation::SIBLINGS, relation::DESC_SIBLINGS, relation::BROTHER, relation::SISTER, 0, Sibling);
}

size_t Settings::addEventTemplate(EventTemplate& et){
	for(auto&& [id, eTempl] : eventTemplates_){
        if(et.getTitle() == eTempl->getTitle()){
            return id;
        }
	}
	if(et.getId() == 0){
		et.setId(eventTemplateIndex_++);
	}
	eventTemplates_.insert({et.getId(), std::make_unique<EventTemplate>(et)});
	eventTemplateIndex_ = eventTemplateIndex_ <= et.getId() ? et.getId() + 1 : eventTemplateIndex_;
	return et.getId();
}

size_t Settings::addRelationTemplate(RelationTemplate& rt){
	for(auto&& [id, rTempl] : relationsTemplates_){
        if(rt.getTitle() == rTempl->getTitle()){
            return id;
        }
	}
	if(rt.getId() == 0){
		rt.setId(relationTemplateIndex_++);
	}
	relationsTemplates_.insert({rt.getId(), std::make_unique<RelationTemplate>(rt)});
	relationTemplateIndex_ = relationTemplateIndex_ <= rt.getId() ? rt.getId() + 1 : relationTemplateIndex_;
    return rt.getId();
}

void Settings::addTag(const std::string& tag){
    if(std::any_of(tags_.begin(), tags_.end(), [tag](const std::string& t1){return tag == t1;})) return;
	tags_.push_back(tag);
}

void Settings::clear(){
    relationTemplateIndex_ = 1;
	eventTemplateIndex_ = 1;
	globalMainPerson_ = 0;
	relationsTemplates_.clear();
	eventTemplates_.clear();
	tags_.clear();
}

void Settings::exportTemplates(std::ostream& os, const std::vector<size_t>& relIds, const std::vector<size_t>& eventIds){
    os << "{\"" << jsonlabel::REL_TEMPL << "\":[";
    bool first = true;
    for(auto&& relId : relIds){
        auto optRel = getRelationTemplate(relId);
        if(optRel){
            if(!first) os << ",";
            first = false;
            os << *(*optRel);
        }
    }
    os << "],\"" << jsonlabel::EVE_TEMPL << "\":[";
    first = true;
    for(auto&& eventId : eventIds){
        auto optEvent = getEventTemplate(eventId);
        if(optEvent){
            if(!first) os << ",";
            first = false;
            os << *(*optEvent);
        }
    }
    os << "]}";
}

const AppSettings& Settings::getAppSettings() const{
    return appSettings_;
}

std::optional<EventTemplate*> Settings::getEventTemplate(size_t id) const{
    if(eventTemplates_.contains(id)){
        return eventTemplates_.at(id).get();
    }
    return {};
}

const std::map<size_t, std::unique_ptr<EventTemplate>>& Settings::getEventTemplates() const{
	return eventTemplates_;
}

size_t Settings::getGlobalMainPerson() const{
    return globalMainPerson_;
}

std::optional<RelationTemplate*> Settings::getRelationTemplate(size_t id) const{
    if(relationsTemplates_.contains(id)){
        return relationsTemplates_.at(id).get();
    }
    return {};
}

const std::map<size_t, std::unique_ptr<RelationTemplate>>& Settings::getRelationTemplates() const{
	return relationsTemplates_;
}

const std::vector<std::string>& Settings::getTags() const{
	return tags_;
}

size_t Settings::importTemplates(const Json::Value& value){
    size_t start = eventTemplates_.size() + relationsTemplates_.size();
    for(Json::ArrayIndex i = 0; i < value[jsonlabel::EVE_TEMPL].size(); ++i){
		EventTemplate et;
		et.readJson(value[jsonlabel::EVE_TEMPL][i]);
		et.setId(0);
		addEventTemplate(et);
	}
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::REL_TEMPL].size(); ++i){
		RelationTemplate rt;
		rt.readJson(value[jsonlabel::REL_TEMPL][i]);
		rt.setId(0);
		addRelationTemplate(rt);
	}
    size_t end = eventTemplates_.size() + relationsTemplates_.size();
    return end - start;
}

std::pair<size_t, size_t> Settings::indexes(){
    return {eventTemplateIndex_, relationTemplateIndex_};
}

void Settings::readJson(const Json::Value& value){
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::EVE_TEMPL].size(); ++i){
		EventTemplate et;
		et.readJson(value[jsonlabel::EVE_TEMPL][i]);
		addEventTemplate(et);
	}
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::REL_TEMPL].size(); ++i){
		RelationTemplate rt;
		rt.readJson(value[jsonlabel::REL_TEMPL][i]);
		addRelationTemplate(rt);
	}
	for(Json::ArrayIndex i = 0; i < value[jsonlabel::TAGS].size(); ++i){
		addTag(value[jsonlabel::TAGS][i][jsonlabel::TAG].asString());
	}
	globalMainPerson_ = value[jsonlabel::GLOBAL_MAIN_PERSON].asUInt64();
	appSettings_.readJson(value[jsonlabel::APP_SETTINGS]);
}

void Settings::reIndex(size_t plusEvent, size_t plusRel){
    for(auto&& [id, relT] : relationsTemplates_)
        relT->setId(relT->getId() + plusRel);
    for(auto&& [id, eventT] : eventTemplates_)
        eventT->setId(eventT->getId() + plusEvent);
}

void Settings::removeEventTemplate(size_t index){
	eventTemplates_.erase(index);
}

void Settings::removeRelationTemplate(size_t index){
	relationsTemplates_.erase(index);
}

void Settings::removeTag(const std::string& tag){
    auto it = std::find_if(tags_.begin(), tags_.end(), [tag](const std::string& t1){return t1 == tag;});
    if (it != tags_.end()) tags_.erase(it);
}

AppSettings& Settings::setAppSettings(){
    return appSettings_;
}

void Settings::setGlobalMainPerson(size_t id){
    globalMainPerson_ = id;
}

// =====================================================================
// functions for Settings
// =====================================================================

std::ostream& operator <<(std::ostream& os, const Settings& settings){
	os << "{ \"" << jsonlabel::EVE_TEMPL << "\": [";
	bool first = true;
	for(auto&& [id, et] : settings.getEventTemplates()){
		if(!first){
			os << ",";
		}
		os << *et;
		first = false;
	}
	first = true;
	os << "], \"" << jsonlabel::REL_TEMPL << "\" : [";
	for(auto&& [id, rt] : settings.getRelationTemplates()){
		if(!first){
			os << ",";
		}
		os << *rt;
		first = false;
	}
	os << "], \"" << jsonlabel::TAGS << "\":[";
	first = true;
	for(auto&& tag : settings.getTags()){
		if(!first){
			os << ",";
		}
		first = false;
		os << "{\"" << jsonlabel::TAG << "\":\"" << tag << "\"}";
	}
	os << "]";
	os << ",\"" << jsonlabel::APP_SETTINGS << "\":" << settings.getAppSettings();
	os << ", \"" << jsonlabel::GLOBAL_MAIN_PERSON << "\":" << settings.getGlobalMainPerson();
	os << "}";
	return os;
}
