/// @file dialogs.cpp Source file for implementing all dialogs.
#include "dialogs.h"

// =====================================================================
// DateDialog
// =====================================================================

DateDialog::DateDialog(FamilyTree* FT, WrappedDate* date, QWidget* parent)
  : QDialog(parent), date_(date), FT_(FT), ui(new Ui::DateDialog){
	ui->setupUi(this);
	if(date_ == nullptr){
        reject();
        return;
	}
	QDate ones (1000,1,1);
    ui->dateEdit->setMinimumDate(ones);
    ui->rangeDateEdit->setMinimumDate(ones);
    ui->unknown->setChecked(date_->isUnknown());
    ui->range->setChecked(!date_->isExact());
    unknownDate(date_->isUnknown());
    ui->rangeDateEdit->setEnabled(!date_->isExact());
    ui->dateEdit->setDate(QDate(date_->getFirstDate().getYear(), date_->getFirstDate().getMonth(), date_->getFirstDate().getDay()));
    ui->rangeDateEdit->setDate(QDate(date_->getSecondDate().getYear(), date_->getSecondDate().getMonth(), date_->getSecondDate().getDay()));
	ui->note->setText(QString::fromStdString(date_->getText()));
	connect(ui->range, SIGNAL(toggled(bool)), ui->rangeDateEdit, SLOT(setEnabled(bool)));
	connect(ui->unknown, SIGNAL(toggled(bool)), this, SLOT(unknownDate(bool)));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveDate()));
	connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshDates()));
}

DateDialog::~DateDialog(){
	delete ui;
}

void DateDialog::saveDate(){
	if(date_ == nullptr) return;
	if(containsForb(ui->note->toPlainText())){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
        return;
    }
    if(ui->unknown->isChecked()){
        date_->setFirstDate(0,0,0);
        date_->setSecondDate(0,0,0);
        date_->setText() = EMPTY_STRING;
    }
    else{
        date_->setFirstDate(ui->dateEdit->date().year(), ui->dateEdit->date().month(), ui->dateEdit->date().day());
        if(ui->range->isChecked())
            date_->setSecondDate(ui->rangeDateEdit->date().year(), ui->rangeDateEdit->date().month(), ui->rangeDateEdit->date().day());
        else
            date_->setSecondDate(0,0,0);
        date_->setText() = ui->note->toPlainText().toStdString();
    }
	date_->updateDate();
	FT_->setUnsaved();
	accept();
}

void DateDialog::unknownDate(bool isUnknown){
    ui->rangeDateEdit->setEnabled(!isUnknown && ui->range->isChecked());
    ui->range->setEnabled(!isUnknown);
    ui->note->setEnabled(!isUnknown);
    ui->dateEdit->setEnabled(!isUnknown);
}

// =====================================================================
// TagChooser
// =====================================================================

TagChooserDialog::TagChooserDialog(FamilyTree* FT, const QString& tag, const QString& value, QWidget* parent)
  : QDialog(parent), FT_(FT), ui(new Ui::TagChooserDialog){
	ui->setupUi(this);
	size_t index = 0;
	bool exists = false;
	for(auto&& t : FT->getSettings()->getTags()){
		ui->tagComboBox->addItem(QString::fromStdString(t));
		if(QString::fromStdString(t) == tag){
			ui->tagComboBox->setCurrentIndex(index);
			exists = true;
			ui->customTagLabel->setEnabled(false);
            ui->customTagEdit->setEnabled(false);
		}
		++index;
	}
	ui->tagComboBox->addItem(QString::fromStdString(tagchooser::CUSTOM_TAG));
	ui->valueEdit->setText(value);
	if(value == "") newOne_ = true;
	else newOne_ = false;
	if(!exists){
		ui->tagComboBox->setCurrentIndex(index);
		ui->customTagEdit->setText(tag);
	}
	connect(ui->tagComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeVisibleWidgets(int)));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveTag()));
	connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshTagsList()));
}

TagChooserDialog::~TagChooserDialog(){
	delete ui;
}

void TagChooserDialog::changeVisibleWidgets(int index){
	if(ui->tagComboBox->count() - 1 == index){
		ui->customTagLabel->setEnabled(true);
		ui->customTagEdit->setEnabled(true);
	}
	else{
		ui->customTagLabel->setEnabled(false);
		ui->customTagEdit->setEnabled(false);
		ui->customTagEdit->setText("");
	}
}

void TagChooserDialog::saveTag(){
	if(FT_ != nullptr){
        if(containsForb(ui->customTagEdit->text())){
            QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
            return;
        }
        if(containsForb(ui->valueEdit->text())){
            QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
            return;
        }
		if(FT_->getMainPerson() != nullptr){
			std::string tag;
			if(ui->tagComboBox->count() - 1 == ui->tagComboBox->currentIndex()){
				tag = ui->customTagEdit->text().toStdString();
			}
			else{
				tag = ui->tagComboBox->currentText().toStdString();
			}
			std::string value = ui->valueEdit->text().toStdString();
			if(tag == "" || value == ""){
			    QMessageBox::critical(this, "Error", QString::fromStdString(error::EMPTY_BOXES));
			}
			else{
				if(ui->tagComboBox->count() - 1 == ui->tagComboBox->currentIndex()){
					FT_->getSettings()->addTag(tag);
				}
				if(newOne_){
                    auto [exists, oldValue] = FT_->getMainPerson()->existsTag(tag);
                    if(exists){
                        QMessageBox::StandardButton result;
                        std::stringstream ss;
                        ss << "Person already has this tag with value: " << oldValue << ". If you choose to save this one (" << value << ") the old one will be deleted.";
                        result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Save | QMessageBox::Cancel);
                        if (result != QMessageBox::Save) return;
                    }
				}
				FT_->getMainPerson()->addTag(tag, value);
				FT_->setUnsaved();
				accept();
			}
		}
	}
}

// =====================================================================
// AddFile
// =====================================================================

AddFileDialog::AddFileDialog(FileType type, const std::vector<std::string>& extensions, VirtualDrive* parentDrive, FamilyTree* FT, QWidget* parent)
	: QDialog(parent), FT_(FT), drive_(parentDrive), type_(type), ui(new Ui::AddFileDialog){
	ui->setupUi(this);
	QIcon fileIcon;
    ui->directoryEdit->setText(QString::fromStdString(parentDrive->getName()));
	switch (type){
        case MEDIA:
            fileIcon = QIcon(":/resources/media.svg");
            break;
        case NOTE:
            fileIcon = QIcon(":/resources/note.svg");
            break;
        case GENERAL_FILE:
        default:
            fileIcon = QIcon(":/resources/file.svg");
            break;
	}
	for(auto&& [id, file] : FT_->getFiles(type)){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, Qt::DisplayRole, QVariant::fromValue(id));
		item->setText(1, QString::fromStdString(file->getFilename()));
		item->setIcon(0, fileIcon);
		ui->fileList->addTopLevelItem(item);
	}
	for(int i = 0; i < ui->fileList->columnCount() - 1; ++i)
        ui->fileList->resizeColumnToContents(i);
	ui->extensions->addItem(QString::fromStdString(addfile::NONE));
	for(auto&& ext : extensions){
		ui->extensions->addItem(QString::fromStdString(ext));
	}
	QStringList header;
	header << "ID" << "Name";
	ui->fileList->setHeaderLabels(header);
	connect(ui->fileList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(openFile(QTreeWidgetItem*)));
	connect(ui->chooseFileButton, SIGNAL(clicked()), this, SLOT(copyFile()));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(createOrChooseFile()));
	connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshTabsWithFiles()));
	connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshProjectView()));
}

AddFileDialog::~AddFileDialog(){
	delete ui;
}

void AddFileDialog::copyFile(){
    auto filePath = QFileDialog::getOpenFileName(this,tr("Choose file to copy."), QDir::currentPath());
    ui->copyFilePath->setText(filePath);
    ui->fileList->clearSelection();
    ui->nameEdit->setText("");
}

void AddFileDialog::createOrChooseFile(){
    std::string name = ui->nameEdit->text().toStdString();
    std::string copyFile = ui->copyFilePath->text().toStdString();
    if(ui->tabWidget->currentIndex() == 0){ // from tree
        if(ui->fileList->currentItem() == nullptr){
            QMessageBox::critical(this, "Error", QString::fromStdString(error::NO_CHOSEN_FILE));
            return;
        }
        QTreeWidgetItem* item = ui->fileList->currentItem();
        size_t id = item->data(0, Qt::DisplayRole).toULongLong();
        drive_->addFile(id);
        FT_->setUnsaved();
        accept();
    }
    else if (ui->tabWidget->currentIndex() == 1){ // copy file
        size_t id = FT_->copyFile(copyFile, type_);
        if(!id){
            QMessageBox::critical(this, "Error", "Given file could not be copied.");
            ui->copyFilePath->selectAll();
            return;
        }
        else{
            drive_->addFile(id);
            FT_->setUnsaved();
            accept();
        }
    }
    else{ // create new file
        if(name == ""){
            QMessageBox::critical(this, "Error", QString::fromStdString(error::EMPTY_BOXES));
            return;
        }
        if(containsForb(ui->nameEdit->text())){
            QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
            return;
        }
        if(ui->extensions->currentIndex() != 0){ // None extension
            name += ui->extensions->currentText().toStdString();
        }
        size_t id = FT_->createEmptyFile(name, type_);
        drive_->addFile(id);
        FT_->setUnsaved();
        accept();
    }
}

void AddFileDialog::openFile(QTreeWidgetItem* item){
    size_t id = item->data(0, Qt::DisplayRole).toULongLong();
    std::string fileUrl;
    FT_->getUrlFile(id, fileUrl, type_);
    MainWindow::openExternalFile(fileUrl);
}

// =====================================================================
// RelationTemplateDialog
// =====================================================================

RelationTemplateDialog::RelationTemplateDialog(FamilyTree* FT, QWidget* parent)
   : QDialog(parent), addingNew_(false), FT_(FT), ui(new Ui::RelationTemplateDialog){
    ui->setupUi(this);
    refreshListWidget();
    clear();
    for(auto&& trait : trait::AllTraits)
        ui->trait->addItem(QString::fromStdString(trait::str(trait)));
    setEnabledWidgets(false);
    QStringList header;
    header << "ID" << "Title" << "Trait" << "Description";
    ui->templateWidget->setHeaderLabels(header);
    connect(ui->templateWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(showTemplate(QTreeWidgetItem*)));
    connect(ui->addNewButton, SIGNAL(clicked()), this, SLOT(addNewTemplate()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveTemplate()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeTemplate()));
    connect(ui->saveButton, SIGNAL(clicked()), parent, SLOT(refreshRelationTab()));
    connect(ui->saveButton, SIGNAL(clicked()), parent, SLOT(refreshProjectView()));
    connect(ui->removeButton, SIGNAL(clicked()), parent, SLOT(refreshRelationTab()));
    connect(ui->removeButton, SIGNAL(clicked()), parent, SLOT(refreshProjectView()));
    connect(ui->removeButton, SIGNAL(clicked()), parent, SLOT(refreshGraphics()));
    connect(ui->addDefaults, SIGNAL(clicked()), this, SLOT(addDefaults()));
}

RelationTemplateDialog::~RelationTemplateDialog(){
    delete ui;
}

void RelationTemplateDialog::addDefaults(){
    FT_->getSettings()->addDefaultRelations();
    refreshListWidget();
    FT_->setUnsaved();
}

void RelationTemplateDialog::addNewTemplate(){
    if(!addingNew_){
        ui->addNewButton->setText(QString::fromStdString(button::CANCEL));
        setEnabledWidgets(true);
        clear();
    }
    else{
        ui->addNewButton->setText(QString::fromStdString(button::ADD_NEW));
        setEnabledWidgets(false);
        clear();
    }
    addingNew_ = !addingNew_;
}

void RelationTemplateDialog::clear(){
    ui->titleEdit->setText("");
    ui->descriptionEdit->setText("");
    ui->trait->setCurrentIndex(0);
    ui->firstNameEdit->setText("");
    ui->secondNameEdit->setText("");
    ui->generationDifference->setValue(0);
    ui->templateWidget->clearSelection();
}

void RelationTemplateDialog::refreshListWidget(){
    ui->templateWidget->clear();
    for(auto&& [id, templ] : FT_->getSettings()->getRelationTemplates()){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, Qt::DisplayRole, QVariant::fromValue(id));
        item->setText(1, QString::fromStdString(templ->getTitle()));
        item->setText(2, QString::fromStdString(trait::str(templ->getTrait())));
        item->setText(3, QString::fromStdString(templ->getDescription()));
        item->setIcon(0, QIcon(":/resources/relation.svg"));
        ui->templateWidget->addTopLevelItem(item);
    }
    for(int i = 0; i < ui->templateWidget->columnCount() - 1; ++i)
        ui->templateWidget->resizeColumnToContents(i);

}

void RelationTemplateDialog::removeTemplate(){
    QTreeWidgetItem* item = ui->templateWidget->currentItem();
    if(item == nullptr){
        return;
    }
    size_t id = item->data(0, Qt::DisplayRole).toULongLong();
    size_t nr = FT_->templatesBasedOnRelationTemplate(id);
    QMessageBox::StandardButton result;
    std::stringstream ss;
    ss << "Template: " << item->text(1).toStdString() << " and all relations based on this relation template (count: " << nr << ") are going to be removed. Are you sure?";
    result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        FT_->removeRelationTemplate(id);
        refreshListWidget();
        clear();
        setEnabledWidgets(false);
    }
    FT_->setUnsaved();
}

void RelationTemplateDialog::saveTemplate(){
    bool empty_boxes = ui->titleEdit->text().isEmpty()
        || ui->descriptionEdit->toPlainText().isEmpty()
        || ui->firstNameEdit->text().isEmpty()
        || ui->secondNameEdit->text().isEmpty();
    if(empty_boxes){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::EMPTY_BOXES));
        return;
    }
    bool forb = containsForb(ui->titleEdit->text()) || containsForb(ui->descriptionEdit->toPlainText())
        || containsForb(ui->firstNameEdit->text()) || containsForb(ui->secondNameEdit->text());
    if(forb){
            QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
            return;
    }
    if(!addingNew_ && ui->templateWidget->currentItem() != nullptr){
        size_t id = ui->templateWidget->currentItem()->data(0, Qt::DisplayRole).toULongLong();
        auto optRel = FT_->getSettings()->getRelationTemplate(id);
        if(!optRel){
            FT_->log("Nonexisting template");
            return;
        }
        saveTemplate((*optRel));
    }
    else{
        RelationTemplate rel;
        saveTemplate(&rel);
        bool added = FT_->getSettings()->addRelationTemplate(rel);
        if(!added){
            QMessageBox::critical(this, "Error", "Template with this name already exists.");
            return;
        }
        ui->addNewButton->setText(QString::fromStdString(button::ADD_NEW));
        addingNew_ = false;
    }
    clear();
    setEnabledWidgets(false);
    refreshListWidget();
}

void RelationTemplateDialog::saveTemplate(RelationTemplate* rel){
    rel->setTitle() = ui->titleEdit->text().toStdString();
    rel->setDescription() = ui->descriptionEdit->toPlainText().toStdString();
    rel->setTrait(Trait(ui->trait->currentIndex()));
    rel->setFirstName() = ui->firstNameEdit->text().toStdString();
    rel->setSecondName() = ui->secondNameEdit->text().toStdString();
    rel->setGenerationDifference(ui->generationDifference->value());
    FT_->setUnsaved();
}

void RelationTemplateDialog::setEnabledWidgets(bool enabled){
    ui->titleEdit->setEnabled(enabled);
    ui->descriptionEdit->setEnabled(enabled);
    ui->trait->setEnabled(enabled);
    ui->firstNameEdit->setEnabled(enabled);
    ui->secondNameEdit->setEnabled(enabled);
    ui->generationDifference->setEnabled(enabled);
    ui->templateWidget->clearSelection();
    ui->saveButton->setEnabled(enabled);
    ui->removeButton->setEnabled(enabled);
}

void RelationTemplateDialog::showTemplate(QTreeWidgetItem* item){
    setEnabledWidgets(true);
    ui->templateWidget->setCurrentItem(item);
    ui->addNewButton->setText(QString::fromStdString(button::ADD_NEW));
    addingNew_ = false;
    size_t id = item->data(0, Qt::DisplayRole).toULongLong();
    auto optRel = FT_->getSettings()->getRelationTemplate(id);
    if(!optRel){
        FT_->log("Nonexisting template");
        return;
    }
    auto rel = (*optRel);
    ui->titleEdit->setText(QString::fromStdString(rel->getTitle()));
    ui->descriptionEdit->setText(QString::fromStdString(rel->getDescription()));
    ui->trait->setCurrentIndex(rel->getTrait());
    ui->firstNameEdit->setText(QString::fromStdString(rel->getFirstName()));
    ui->secondNameEdit->setText(QString::fromStdString(rel->getSecondName()));
    ui->generationDifference->setValue(rel->getGenerationDifference());
}

// =====================================================================
// EventTampletsDialog
// =====================================================================

EventTamplatesDialog::EventTamplatesDialog(FamilyTree* FT, QWidget* parent)
   : QDialog(parent), addingNew_(false), FT_(FT), ui(new Ui::EventTamplatesDialog){
    ui->setupUi(this);
    refreshListWidget();
    clear();
    setEnabledWidgets(false);
    QStringList header;
    header << "ID" << "Title" << "Description";
    ui->templateWidget->setHeaderLabels(header);
    connect(ui->templateWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(showTemplate(QTreeWidgetItem*)));
    connect(ui->addNewButton, SIGNAL(clicked()), this, SLOT(addNewTemplate()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveTemplate()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveTemplate()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeTemplate()));
    connect(ui->addRoleButton, SIGNAL(clicked()), this, SLOT(addNewRole()));
    connect(ui->removeRoleButton, SIGNAL(clicked()), this, SLOT(removeRole()));
    connect(ui->saveButton, SIGNAL(clicked()), parent, SLOT(refreshEventTab()));
    connect(ui->saveButton, SIGNAL(clicked()), parent, SLOT(refreshProjectView()));
    connect(ui->removeButton, SIGNAL(clicked()), parent, SLOT(refreshEventTab()));
    connect(ui->removeButton, SIGNAL(clicked()), parent, SLOT(refreshProjectView()));
    connect(ui->addDefaults, SIGNAL(clicked()), this, SLOT(addDefaults()));
}

EventTamplatesDialog::~EventTamplatesDialog(){
    delete ui;
}

void EventTamplatesDialog::addDefaults(){
    FT_->getSettings()->addDefaultEvents();
    FT_->setUnsaved();
    refreshListWidget();
}

void EventTamplatesDialog::addNewRole(){
    if(ui->roleEdit->text().isEmpty()){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::EMPTY_BOXES));
        return;
    }
    if(ui->roleEdit->text().toStdString() == events::NO_ROLE){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::NO_ROLE));
        return;
    }
    QListWidgetItem* item = new QListWidgetItem();
    ui->roleWidget->addItem(item);
    item->setText(ui->roleEdit->text());
    ui->roleEdit->setText("");
}

void EventTamplatesDialog::addNewTemplate(){
    if(!addingNew_){
        ui->addNewButton->setText(QString::fromStdString(button::CANCEL));
        setEnabledWidgets(true);
        clear();
    }
    else{
        ui->addNewButton->setText(QString::fromStdString(button::ADD_NEW));
        setEnabledWidgets(false);
        clear();
    }
    addingNew_ = !addingNew_;
}

void EventTamplatesDialog::clear(){
    ui->titleEdit->setText("");
    ui->descriptionEdit->setText("");
    ui->templateWidget->clearSelection();
    ui->roleWidget->clear();
    ui->roleEdit->setText("");
    ui->containsDate->setChecked(false);
    ui->containsPlace->setChecked(false);
    ui->hasMorePeople->setChecked(false);
}

void EventTamplatesDialog::refreshListWidget(){
    ui->templateWidget->clear();
    for(auto&& [id, templ] : FT_->getSettings()->getEventTemplates()){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, Qt::DisplayRole, QVariant::fromValue(id));
        item->setText(1, QString::fromStdString(templ->getTitle()));
        item->setText(2, QString::fromStdString(templ->getDescription()));
        item->setIcon(0, QIcon(":/resources/event.svg"));
        ui->templateWidget->addTopLevelItem(item);
    }
    for(int i = 0; i < ui->templateWidget->columnCount() - 1; ++i)
        ui->templateWidget->resizeColumnToContents(i);

}

void EventTamplatesDialog::removeRole(){
    if(ui->roleWidget->currentItem() == nullptr) return;
    delete ui->roleWidget->currentItem();
}

void EventTamplatesDialog::removeTemplate(){
    QTreeWidgetItem * item = ui->templateWidget->currentItem();
    if(item == nullptr) return;
    size_t id = item->data(0, Qt::DisplayRole).toULongLong();
    size_t nr = FT_->templatesBasedOnEventTemplate(id);
    QMessageBox::StandardButton result;
    std::stringstream ss;
    ss << "Template: " << item->text(0).toStdString() << " and all events based on this event template (count: " << nr << ") are going to be removed. Are you sure?";
    result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        FT_->removeEventTemplate(id);
        refreshListWidget();
        clear();
        setEnabledWidgets(false);
    }
    FT_->setUnsaved();
}

void EventTamplatesDialog::saveTemplate(){
    bool empty_boxes = ui->titleEdit->text().isEmpty() || ui->descriptionEdit->toPlainText().isEmpty();
    if(empty_boxes){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::EMPTY_BOXES));
        return;
    }
    bool forb = containsForb(ui->titleEdit->text()) || containsForb(ui->descriptionEdit->toPlainText());
    if(forb){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
        return;
    }
    if(!addingNew_ && ui->templateWidget->currentItem() != nullptr){
        QTreeWidgetItem* item = ui->templateWidget->currentItem() ;
        size_t id = item->data(0, Qt::DisplayRole).toULongLong();
        auto optTempl = FT_->getSettings()->getEventTemplate(id);
        if(!optTempl) return;
        saveTemplate(*optTempl);
    }
    else{
        EventTemplate event;
        saveTemplate(&event);
        bool added = FT_->getSettings()->addEventTemplate(event);
        if(!added){
            QMessageBox::critical(this, "Error", "Template with this name already exists.");
            return;
        }
        ui->addNewButton->setText(QString::fromStdString(button::ADD_NEW));
        addingNew_ = false;
    }
    clear();
    refreshListWidget();
    setEnabledWidgets(false);
}

void EventTamplatesDialog::saveTemplate(EventTemplate* event){
    event->setTitle() = ui->titleEdit->text().toStdString();
    event->setDescription() = ui->descriptionEdit->toPlainText().toStdString();
    event->setContainsDate(ui->containsDate->isChecked());
    event->setContainsPlace(ui->containsPlace->isChecked());
    event->setMorePeopleInvolved(ui->hasMorePeople->isChecked());
    std::vector<std::string> roles;
    for(auto it = 0; it < ui->roleWidget->count(); ++it){
        auto item = ui->roleWidget->item(it);
        roles.push_back(item->text().toStdString());
    }
    event->setRoles(roles);
    FT_->setUnsaved();
    FT_->updateEventsWithTemplate(event->getId());
}

void EventTamplatesDialog::setEnabledWidgets(bool enabled){
    ui->titleEdit->setEnabled(enabled);
    ui->descriptionEdit->setEnabled(enabled);
    ui->templateWidget->clearSelection();
    ui->roleWidget->clear();
    ui->roleWidget->setEnabled(enabled);
    ui->roleEdit->setEnabled(enabled);
    ui->containsDate->setEnabled(enabled);
    ui->containsPlace->setEnabled(enabled);
    ui->hasMorePeople->setEnabled(enabled);
    ui->addRoleButton->setEnabled(enabled);
    ui->removeRoleButton->setEnabled(enabled);
    ui->saveButton->setEnabled(enabled);
    ui->removeButton->setEnabled(enabled);
}

void EventTamplatesDialog::showTemplate(QTreeWidgetItem* item){
    size_t id = item->data(0, Qt::DisplayRole).toULongLong();
    auto optTempl = FT_->getSettings()->getEventTemplate(id);
    if(!optTempl) return;
    auto event = *optTempl;
    ui->titleEdit->setText(QString::fromStdString(event->getTitle()));
    ui->descriptionEdit->setText(QString::fromStdString(event->getDescription()));
    ui->hasMorePeople->setChecked(event->hasMorePeopleInvolved());
    ui->containsDate->setChecked(event->containsDate());
    ui->containsPlace->setChecked(event->containsPlace());
    ui->roleWidget->clear();
    setEnabledWidgets(true);
    ui->templateWidget->setCurrentItem(item);
    if(event->hasRoles()){
        for(auto&& role : event->getRoles())
            ui->roleWidget->addItem(QString::fromStdString(role));
    }
}

// =====================================================================
// RelationDialog
// =====================================================================

RelationDialog::RelationDialog(FamilyTree* FT, QWidget* parent, Relation* rel)
    : QDialog(parent), FT_(FT), person1_(nullptr), person2_(nullptr), rel_(rel), ui(new Ui::RelationDialog){
    ui->setupUi(this);
    for(auto&& [id, templ] : FT_->getSettings()->getRelationTemplates()){
        QVariant var = QVariant::fromValue(id);
        ui->templatesComboBox->addItem(QString::fromStdString(templ->getTitle()), var);
        if(rel != nullptr && rel_->getTemplate() == id)
            ui->templatesComboBox->setCurrentIndex(ui->templatesComboBox->count() - 1);
    }
    if(rel != nullptr){
        auto optPerson1 = FT_->getPerson(rel->getFirstPerson());
        auto optPerson2 = FT_->getPerson(rel->getSecondPerson());
        if(optPerson1){
            person1_ = (*optPerson1);
            ui->person1Text->setText(QString::fromStdString(person1_->str()));
        }
        if(optPerson2){
            person2_ = (*optPerson2);
            ui->person2Text->setText(QString::fromStdString(person2_->str()));
        }
    }
    else{
        saveFirstPerson_ = true;
        savePerson(FT_->getMainPerson());
    }
    ui->person1Label->setText("");
    ui->person2Label->setText("");
    ui->generationDifference->setText("");
    showTemplateDetails();
    connect(ui->switchButton, SIGNAL(clicked()), this, SLOT(switchPersons()));
    connect(ui->templatesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showTemplateDetails()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveRelation()));
    connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshRelationTab()));
    connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshProjectView()));
    connect(ui->person1Button, SIGNAL(clicked()), this, SLOT(chooseFirstPerson()));
    connect(ui->person2Button, SIGNAL(clicked()), this, SLOT(chooseSecondPerson()));
    connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshGraphics()));
}

RelationDialog::~RelationDialog(){
    delete ui;
}

void RelationDialog::chooseFirstPerson(){
    ChoosePersonDialog* chpd = new ChoosePersonDialog(FT_, person1_, this);
    saveFirstPerson_ = true;
    chpd->show();
}

void RelationDialog::chooseSecondPerson(){
    ChoosePersonDialog* chpd = new ChoosePersonDialog(FT_, person1_, this);
    saveFirstPerson_ = false;
    chpd->show();
}

void RelationDialog::savePerson(Person* person){
    if(saveFirstPerson_){
        person1_ = person;
        if(person1_ == nullptr) return;
        ui->person1Text->setText(QString::fromStdString(person1_->str()));
    }
    else{
        person2_ = person;
        if(person2_ == nullptr) return;
        ui->person2Text->setText(QString::fromStdString(person2_->str()));
    }
}

void RelationDialog::saveRelation(){
    size_t tempId = ui->templatesComboBox->currentData().toULongLong();
    bool newOne = false;
    if(person1_ == nullptr || person2_ == nullptr){
        QMessageBox::critical(this, "Error", "There is missing person.");
        return;
    }
    if(person1_ == person2_){
        QMessageBox::StandardButton result;
        std::stringstream ss;
        ss << "You are about to create a relation with one person only. Do you want to proceed?";
        result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::No) return;
    }
    if(rel_ == nullptr){
        rel_ = FT_->addRelation();
        newOne = true;
    }
    FT_->setRelation(rel_->getId(), person1_->getId(), person2_->getId(), tempId);
    FT_->setUnsaved();
    auto suggestions = FT_->getRelationSuggestions(rel_->getId());
    if(newOne && suggestions.size() > 0){
        SuggestionsDialog* sd = new SuggestionsDialog(FT_, suggestions);
        sd->show();
    }
    accept();
}

void RelationDialog::showTemplateDetails(){
    size_t id = ui->templatesComboBox->currentData().toULongLong();
    auto optTempl = FT_->getSettings()->getRelationTemplate(id);
    if(!optTempl) return;
    auto templ = *optTempl;
    ui->descriptionText->setText(QString::fromStdString(templ->getDescription()));
    ui->traitText->setText(QString::fromStdString(trait::str(templ->getTrait())));
    ui->person1Label->setText(QString::fromStdString(templ->getFirstName()));
    ui->person2Label->setText(QString::fromStdString(templ->getSecondName()));
    ui->generationDifference->setText(QString::fromStdString("Gen diff: " + std::to_string(templ->getGenerationDifference())));
}

void RelationDialog::switchPersons(){
    auto holder = person1_;
    person1_ = person2_;
    person2_ = holder;
    if(person1_ != nullptr)
        ui->person1Text->setText(QString::fromStdString(person1_->str()));
    else
        ui->person1Text->setText("");
    if(person2_ != nullptr)
        ui->person2Text->setText(QString::fromStdString(person2_->str()));
    else
        ui->person2Text->setText("");
}

// =====================================================================
// EventDialog
// =====================================================================

EventDialog::EventDialog(FamilyTree* FT, QWidget* parent, Event* event)
   : QDialog(parent), event_(event), FT_(FT), main_(FT_->getMainPerson()), ui(new Ui::EventDialog){
    ui->setupUi(this);
    for(auto&& [id, ev] : FT_->getSettings()->getEventTemplates()){
        auto index = QVariant::fromValue(id);
        ui->templatesComboBox->addItem(QString::fromStdString(ev->getTitle()), index);
        if(event_ != nullptr && event_->getTemplate() == id)
            ui->templatesComboBox->setCurrentIndex(ui->templatesComboBox->count() - 1);
    }
    QStringList list;
    list << "Role" << "Person";
    if(event_ != nullptr) date_ = event_->getDate();
    ui->roleList->setHeaderLabels(list);
    showTemplate();
    connect(ui->addRoleButton, SIGNAL(clicked()), this, SLOT(addEmptyRole()));
    connect(ui->removeRoleButton, SIGNAL(clicked()), this, SLOT(removeEmptyRole()));
    connect(ui->roleList, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(refreshPersons(QTreeWidgetItem*)));
    connect(ui->removePersonButton, SIGNAL(clicked()), this, SLOT(removePerson()));
    connect(ui->choosePersonButton, SIGNAL(clicked()), this, SLOT(choosePerson()));
    connect(ui->templatesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showTemplate()));
    connect(ui->customDate, SIGNAL(clicked()), this, SLOT(openCustomDate()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveEvent()));
    connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshEventTab()));
    connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshProjectView()));
}

EventDialog::~EventDialog(){
    delete ui;
}

void EventDialog::addEmptyRole(){
    ui->roleList->addTopLevelItem(new EventRoleItem(events::NO_ROLE));
    ui->roleList->resizeColumnToContents(0);
}

void EventDialog::choosePerson(){
    if(ui->roleList->currentItem() == nullptr) return;
    auto eventItem = dynamic_cast<EventRoleItem*>(ui->roleList->currentItem());
    auto optPerson = FT_->getPerson(eventItem->getId());
    if(!optPerson){
        ChoosePersonDialog* cpd = new ChoosePersonDialog(FT_, nullptr, this);
        cpd->show();
    }
    else{
        ChoosePersonDialog* cpd = new ChoosePersonDialog(FT_, *optPerson, this);
        cpd->show();
    }
}

void EventDialog::openCustomDate(){
    DateDialog* dd = new DateDialog(FT_, &date_, this);
    dd->show();
}

void EventDialog::refreshDates(){
    if(ui->templatesComboBox->count() == 0) return;
    size_t id = ui->templatesComboBox->currentData().toULongLong();
    auto optTempl = FT_->getSettings()->getEventTemplate(id);
    if(!optTempl) return;
    auto templ = *optTempl;
    if(!templ->containsDate()) ui->customDateText->setText("");
    else ui->customDateText->setText(QString::fromStdString(date_.str()));
}

void EventDialog::refreshPersons(QTreeWidgetItem* item){
    auto eventItem = dynamic_cast<EventRoleItem*>(item);
    if(eventItem->getId() == 0)
        ui->personText->setText("");
    else
        ui->personText->setText(item->text(1));
}

void EventDialog::removeEmptyRole(){
    if(ui->roleList->currentItem() == nullptr) return;
    if(ui->roleList->currentItem()->text(0).toStdString() == events::NO_ROLE)
        delete ui->roleList->currentItem();
}

void EventDialog::removePerson(){
    if(ui->roleList->currentItem() == nullptr) return;
    EventRoleItem* eri = dynamic_cast<EventRoleItem*>(ui->roleList->currentItem());
    eri->setPerson("");
}

void EventDialog::saveEvent(){
    size_t templId = ui->templatesComboBox->currentData().toULongLong();
    auto optTempl = FT_->getSettings()->getEventTemplate(templId);
    if(!optTempl) return;
    auto templ = *optTempl;
    if(containsForb(ui->textEdit->toPlainText()) || (containsForb(ui->placeText->text()) && templ->containsPlace())){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
        return;
    }
    if(event_ == nullptr){
        event_ = FT_->addEvent();
        if(!templ->hasRoles() && !templ->hasMorePeopleInvolved()){
            // Only main person is related to this event
            event_->addPerson(main_->getId());
            main_->addEvent(event_->getId());
        }
    }
    if(templ->containsDate()){
        event_->setDate() = date_;
    }
    event_->setTemplate(templId);

    event_->setText(ui->textEdit->toPlainText().toStdString());
    if(templ->containsPlace())
        event_->setPlace(ui->placeText->text().toStdString());
    if(templ->hasRoles() || templ->hasMorePeopleInvolved()){
        for(auto&& [role, id] : event_->getPersons()){
            auto optPerson = FT_->getPerson(id);
            if(optPerson)
                (*optPerson)->removeEvent(event_->getId());
        }
        event_->clearPersons();
        for(auto it = 0; it < ui->roleList->topLevelItemCount(); ++it){
            auto item = ui->roleList->topLevelItem(it);
            auto eventItem = dynamic_cast<EventRoleItem*>(item);
            if(eventItem->getId() == 0) continue;
            auto optPerson = FT_->getPerson(eventItem->getId());
            if(!optPerson){
                std::stringstream ss;
                ss << "Person with id " << eventItem->getId() << " is not in a database. It was removed from the event. If there was one person left, the entire event was removed.";
                QMessageBox::critical(this, "Error", QString::fromStdString(ss.str()));
                bool emptyEvent = event_->removePerson(eventItem->getId());
                if(emptyEvent){
                    FT_->removeEvent(event_->getId());
                    reject();
                    return;
                }
                continue;
            }
            if(eventItem->getId() != 0){
                event_->addPerson(eventItem->getId(), eventItem->text(0).toStdString());
                (*optPerson)->addEvent(event_->getId());
            }
        }
        if(event_->getPersons().size() == 0){
            std::stringstream ss;
            ss << "No person was assigned to this event. Thus it would be lost.";
            QMessageBox::critical(this, "Error", QString::fromStdString(ss.str()));
            FT_->removeEvent(event_->getId());
            reject();
            return;
        }
    }
    FT_->setUnsaved();
    accept();
}

void EventDialog::savePerson(Person* person){
    if(ui->roleList->currentItem() == nullptr || person == nullptr) return;
    auto eventItem = dynamic_cast<EventRoleItem*>(ui->roleList->currentItem());
    eventItem->setPerson(QString::fromStdString(person->str()), person->getId());
    if(eventItem->getId() == 0)
        ui->personText->setText("");
    else
        ui->personText->setText(eventItem->text(1));
}

void EventDialog::showEventDetails(){
    if(event_ == nullptr) return;
    ui->textEdit->setText(QString::fromStdString(event_->getText()));
    ui->placeText->setText(QString::fromStdString(event_->getPlace()));
    refreshDates();
    for(auto&& [role, personId] : event_->getPersons()){
        auto optPerson = FT_->getPerson(personId);
        if(!optPerson){
            std::stringstream ss;
            ss << "Person with id " << personId << " is not in a database. It was removed from the event. If there was one person left, the entire event was removed.";
            QMessageBox::critical(this, "Error", QString::fromStdString(ss.str()));
            bool emptyEvent = event_->removePerson(personId);
            if(emptyEvent){
                FT_->removeEvent(event_->getId());
                reject();
            }
            continue;
        }
        if(role == events::NO_ROLE){
            auto roleItem = new EventRoleItem(role);
            roleItem->setPerson(QString::fromStdString((*optPerson)->getFullName()), personId);
            ui->roleList->addTopLevelItem(roleItem);
        }
        else{
            auto items = ui->roleList->findItems(QString::fromStdString(role), Qt::MatchExactly);
            for(auto&& item : items){
                auto roleItem = dynamic_cast<EventRoleItem*>(item);
                if(roleItem->getId() == 0){
                    roleItem->setPerson(QString::fromStdString((*optPerson)->str()), personId);
                    break;
                }
            }
        }
    }
    ui->roleList->resizeColumnToContents(0);
}


void EventDialog::showTemplate(){
    if(ui->templatesComboBox->count() == 0) return;
    size_t id = ui->templatesComboBox->currentData().toULongLong();
    auto optTempl = FT_->getSettings()->getEventTemplate(id);
    if(!optTempl) return;
    auto templ = *optTempl;
    ui->descriptionText->setText(QString::fromStdString(templ->getDescription()));
    ui->customDate->setEnabled(templ->containsDate());
    refreshDates();
    ui->placeText->setEnabled(templ->containsPlace());
    ui->addRoleButton->setEnabled(templ->hasMorePeopleInvolved());
    ui->removeRoleButton->setEnabled(templ->hasMorePeopleInvolved());
    ui->roleList->setEnabled(templ->hasRoles() || templ->hasMorePeopleInvolved());
    ui->personText->setEnabled(templ->hasRoles() || templ->hasMorePeopleInvolved());
    ui->choosePersonButton->setEnabled(templ->hasRoles() || templ->hasMorePeopleInvolved());
    ui->removePersonButton->setEnabled(templ->hasRoles() || templ->hasMorePeopleInvolved());
    ui->roleList->clear();
    if(templ->hasRoles()){
        for(auto&& role : templ->getRoles()){
            EventRoleItem* eri = new EventRoleItem(role);
            ui->roleList->addTopLevelItem(eri);
        }
    }
    if(event_ != nullptr && event_->getTemplate() == id)
        showEventDetails();
    ui->roleList->resizeColumnToContents(0);
}

// =====================================================================
// HelpDialog
// =====================================================================

HelpDialog::HelpDialog(FamilyTree* FT, QWidget* parent)
    : QDialog(parent), ui(new Ui::HelpDialog){
    FT_ = FT;
    ui->setupUi(this);
    ui->help->setChecked(!FT_->showHelpOnStartup());
    connect(ui->help, SIGNAL(toggled(bool)), this, SLOT(openHelp(bool)));
}

HelpDialog::~HelpDialog(){
    delete ui;
}

void HelpDialog::openHelp(bool help){
    FT_->enableHelpOnStartup(!help);
}

// =====================================================================
// ChoosePersonDialog
// =====================================================================

ChoosePersonDialog::ChoosePersonDialog(FamilyTree* FT, Person* person, QWidget* parent)
    : QDialog(parent), FT_(FT), person_(person), ui(new Ui::ChoosePersonDialog){
    ui->setupUi(this);
    for(auto&& [id, upPerson] : FT_->getPersons()){
		if(!upPerson) continue;
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, Qt::DisplayRole, QVariant::fromValue(id));
        item->setText(1, QString::fromStdString(upPerson->str()));
        item->setText(2, QString::fromStdString(upPerson->birthDate()->str()));
        if(!upPerson->isAlive()) item->setText(3, QString::fromStdString(upPerson->deathDate()->str()));
        switch(upPerson->getGender()){
            case Male:
                item->setIcon(1, QIcon(":/resources/man.svg"));
                break;
            case Female:
                item->setIcon(1, QIcon(":/resources/woman.svg"));
                break;
            case Other:
            default:
                item->setIcon(1, QIcon(":/resources/other.svg"));
                break;
        }
        ui->treeWidget->addTopLevelItem(item);
        if(person_ != nullptr && person_->getId() == id){
            ui->treeWidget->setCurrentItem(item);
        }
    }
    for(int i = 0; i < ui->treeWidget->columnCount() - 1; ++i)
        ui->treeWidget->resizeColumnToContents(i);
    QStringList list;
    list << "ID" << "Name" << "Birth date" << "Death date";
    ui->treeWidget->setHeaderLabels(list);
    connect(ui->findText, SIGNAL(returnPressed()), this, SLOT(filterPersons()));
    connect(ui->findOk, SIGNAL(clicked()), this, SLOT(filterPersons()));
    connect(ui->findClear, SIGNAL(clicked()), this, SLOT(clearFilter()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveSelectedPerson()));
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(saveSelectedPerson()));
    connect(this, SIGNAL(savePerson(Person*)), parent, SLOT(savePerson(Person*)));
}

ChoosePersonDialog::~ChoosePersonDialog(){
    delete ui;
}

void ChoosePersonDialog::clearFilter(){
    ui->findText->setText("");
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
        ui->treeWidget->topLevelItem(i)->setHidden(false);
}

void ChoosePersonDialog::filterPersons(){
    auto widgetList = ui->treeWidget->findItems(ui->findText->text(), Qt::MatchContains, 1);
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
        ui->treeWidget->topLevelItem(i)->setHidden(true);
    for(auto&& widget : widgetList)
        widget->setHidden(false);
}

void ChoosePersonDialog::saveSelectedPerson(){
    if(ui->treeWidget->currentItem() == nullptr){
        QMessageBox::critical(this, "Error", "No person was selected.");
        return;
    }
    size_t id = ui->treeWidget->currentItem()->data(0, Qt::DisplayRole).toULongLong();
    auto optPerson = FT_->getPerson(id);
    if(!optPerson){
        QMessageBox::critical(this, "Error", "Selected person is not in the databse.");
        return;
    }
    emit savePerson((*optPerson));
    accept();
}

// =====================================================================
// VisualSettings
// =====================================================================

VisualSettings::VisualSettings(QColor probandColor, QColor promotedColor, QColor standardColor, QColor highlighted,
                                 int sizeX, int sizeY, QFont font, QPen* pen, QPen* textPen, int borderRadius, QWidget* parent)
    : QDialog(parent), borderRadius_(borderRadius), font_(font), highlightedColor_(highlighted), pen_(*pen), probandColor_(probandColor), promotedColor_(promotedColor),
    sizeX_(sizeX), sizeY_(sizeY), standardColor_(standardColor), textPen_(*textPen), ui(new Ui::VisualSettings){
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
	ui->width->setValue(sizeX_);
	ui->height->setValue(sizeY_);
	ui->font->setCurrentFont(font_);
	ui->fontSize->setValue(font_.pointSize());
	ui->textEdit->setText("Name of a person");
	ui->lineNumber->setValue(3);
	ui->lineWidth->setValue(pen_.width());
	ui->rounding->setValue(borderRadius_);
    refresh();
    connect(ui->probantColor, SIGNAL(clicked()), this, SLOT(probandColor()));
    connect(ui->standardColor, SIGNAL(clicked()), this, SLOT(standardColor()));
    connect(ui->promotedColor, SIGNAL(clicked()), this, SLOT(promotedColor()));
    connect(ui->highlightedColor, SIGNAL(clicked()), this, SLOT(highlightedColor()));
    connect(ui->height, SIGNAL(valueChanged(int)), this, SLOT(refresh()));
    connect(ui->width, SIGNAL(valueChanged(int)), this, SLOT(refresh()));
    connect(ui->font, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(refresh()));
    connect(ui->fontSize, SIGNAL(valueChanged(int)), this, SLOT(refresh()));
    connect(ui->lineNumber, SIGNAL(valueChanged(int)), this, SLOT(refresh()));
    connect(ui->textEdit, SIGNAL(editingFinished()), this, SLOT(refresh()));
    connect(this, SIGNAL(saveSettings(QColor, QColor, QColor, QColor, int, int, QFont, QColor, QColor, int, int)),
     parent, SLOT(saveGraphicsSetting(QColor, QColor, QColor, QColor, int, int, QFont, QColor, QColor, int, int)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveAndExit()));
    connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(refreshGraphics()));
    connect(ui->lineWidth, SIGNAL(valueChanged(int)), this, SLOT(refresh()));
    connect(ui->rounding, SIGNAL(valueChanged(int)), this, SLOT(refresh()));
    connect(ui->textColor, SIGNAL(clicked()), this, SLOT(textColor()));
    connect(ui->lineColor, SIGNAL(clicked()), this, SLOT(lineColor()));
}

VisualSettings::~VisualSettings(){
    delete scene;
    delete ui;
}

void VisualSettings::highlightedColor(){
    QColorDialog colorDialog(highlightedColor_, this);
    QColor color = colorDialog.getColor();
    if (color.isValid())
        highlightedColor_ = color;
    refresh();
}

void VisualSettings::lineColor(){
    QColorDialog colorDialog(pen_.color(), this);
    QColor color = colorDialog.getColor();
    if (color.isValid())
        pen_.setColor(color);
    refresh();
}

void VisualSettings::probandColor(){
    QColorDialog colorDialog(probandColor_, this);
    QColor color = colorDialog.getColor();
    if (color.isValid())
        probandColor_ = color;
    refresh();
}

void VisualSettings::refresh(){
    int gap = 10;
    scene->clear();
    sizeX_ = ui->width->value();
    sizeY_ = ui->height->value();
    font_ = ui->font->currentFont();
    font_.setPointSize(ui->fontSize->value());
    pen_.setWidth(ui->lineWidth->value());
    borderRadius_ = ui->rounding->value();
    {
        QPalette palette = ui->lineColor->palette();
        palette.setColor(QPalette::Button, pen_.color());
        ui->lineColor->setPalette(palette);
    }
    {
        QPalette palette = ui->textColor->palette();
        palette.setColor(QPalette::Button, textPen_.color());
        ui->textColor->setPalette(palette);
    }
    scene->setSceneRect(0, 0, sizeX_ + 2 * gap, 4*(sizeY_ + 2 * gap));
    {
        QStringList lines;
        lines << "Proband Color";
        for(int i = 0; i < ui->lineNumber->value(); ++i)
            lines << ui->textEdit->text();
        PersonsGraphicsItem* item = new PersonsGraphicsItem(lines, sizeX_, sizeY_, nullptr, gap, gap, probandColor_, highlightedColor_, font_,
                                                            this, &pen_, &textPen_, borderRadius_);
        scene->addItem(item);
        QPalette palette = ui->standardColor->palette();
        palette.setColor(QPalette::Button, standardColor_);
        ui->standardColor->setPalette(palette);
    }
    {
        QStringList lines;
        lines << "Special Color";
        for(int i = 0; i < ui->lineNumber->value(); ++i)
            lines << ui->textEdit->text();
        PersonsGraphicsItem* item = new PersonsGraphicsItem(lines, sizeX_, sizeY_, nullptr, gap, 2*gap + sizeY_, promotedColor_, highlightedColor_, font_,
                                                            this, &pen_, &textPen_, borderRadius_);
        scene->addItem(item);
        QPalette palette = ui->promotedColor->palette();
        palette.setColor(QPalette::Button, promotedColor_);
        ui->promotedColor->setPalette(palette);
    }
    {
        QStringList lines;
        lines << "Standard Color";
        for(int i = 0; i < ui->lineNumber->value(); ++i)
            lines << ui->textEdit->text();
        PersonsGraphicsItem* item = new PersonsGraphicsItem(lines, sizeX_, sizeY_, nullptr, gap, 3* gap + 2* sizeY_, standardColor_, highlightedColor_, font_,
                                                            this, &pen_, &textPen_, borderRadius_);
        scene->addItem(item);
        QPalette palette = ui->probantColor->palette();
        palette.setColor(QPalette::Button, probandColor_);
        ui->probantColor->setPalette(palette);
    }
    {
        QStringList lines;
        lines << "Highlighted";
        for(int i = 0; i < ui->lineNumber->value(); ++i)
            lines << ui->textEdit->text();
        PersonsGraphicsItem* item = new PersonsGraphicsItem(lines, sizeX_, sizeY_, nullptr, gap, 4* gap + 3* sizeY_, highlightedColor_, highlightedColor_, font_,
                                                            this, &pen_, &textPen_, borderRadius_);
        scene->addItem(item);
        QPalette palette = ui->probantColor->palette();
        palette.setColor(QPalette::Button, highlightedColor_);
        ui->highlightedColor->setPalette(palette);
    }
}

void VisualSettings::promotedColor(){
    QColorDialog colorDialog(promotedColor_, this);
    QColor color = colorDialog.getColor();
    if (color.isValid())
        promotedColor_ = color;
    refresh();
}

void VisualSettings::saveAndExit(){
    emit saveSettings(probandColor_, promotedColor_, standardColor_, highlightedColor_, sizeX_, sizeY_, font_, pen_.color(), textPen_.color(), pen_.width(), borderRadius_);
    accept();
}

void VisualSettings::standardColor(){
    QColorDialog colorDialog(standardColor_, this);
    QColor color = colorDialog.getColor();
    if (color.isValid())
        standardColor_ = color;
    refresh();
}

void VisualSettings::textColor(){
    QColorDialog colorDialog(textPen_.color(), this);
    QColor color = colorDialog.getColor();
    if (color.isValid())
        textPen_.setColor(color);
    refresh();
}

// =====================================================================
// ExportTemplatesDialog
// =====================================================================

ExportTemplatesDialog::ExportTemplatesDialog(FamilyTree* FT, QWidget* parent)
    : QDialog(parent), FT_(FT), ui(new Ui::ExportTemplatesDialog){
    ui->setupUi(this);
    for(auto&& [id, templ] : FT_->getSettings()->getEventTemplates()){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, Qt::DisplayRole, QVariant::fromValue(id));
        item->setText(1, QString::fromStdString(templ->getTitle()));
        item->setText(3, QString::fromStdString(templ->getDescription()));
        item->setIcon(0, QIcon(":/resources/event.svg"));
        item->setData(1, Qt::UserRole, QVariant::fromValue(true));
        ui->templateWidget->addTopLevelItem(item);
    }
    for(auto&& [id, templ] : FT_->getSettings()->getRelationTemplates()){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, Qt::DisplayRole, QVariant::fromValue(id));
        item->setText(1, QString::fromStdString(templ->getTitle()));
        item->setText(2, QString::fromStdString(trait::str(templ->getTrait())));
        item->setText(3, QString::fromStdString(templ->getDescription()));
        item->setIcon(0, QIcon(":/resources/relation.svg"));
        item->setData(1, Qt::UserRole, QVariant::fromValue(false));
        ui->templateWidget->addTopLevelItem(item);
    }
    QStringList header;
	header << "ID" << "Title" << "Trait" << "Description";
	ui->templateWidget->setHeaderLabels(header);
    for(int i = 0; i < ui->templateWidget->columnCount() - 1; ++i)
        ui->templateWidget->resizeColumnToContents(i);
    connect(ui->selectAll, SIGNAL(clicked()), ui->templateWidget, SLOT(selectAll()));
    connect(ui->unselectAll, SIGNAL(clicked()), ui->templateWidget, SLOT(clearSelection()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(exportTemplates()));
}

ExportTemplatesDialog::~ExportTemplatesDialog(){
    delete ui;
}

void ExportTemplatesDialog::exportTemplates(){
    auto selected = ui->templateWidget->selectedItems();
    if(selected.size() == 0) return;
    std::vector<size_t> relTemplates;
    std::vector<size_t> eventTemplates;
    for(auto&& item : selected){
        size_t id = item->data(0, Qt::DisplayRole).toULongLong();
        if(item->data(1, Qt::UserRole).toBool()){
            eventTemplates.push_back(id);
        }
        else{
            relTemplates.push_back(id);
        }
    }
    QString filename = QFileDialog::getSaveFileName(this, tr("Select or make new file"), QDir::currentPath(), tr("JSON (*.json)"));
    if(filename == "") return;
    FT_->exportTemplates(eventTemplates, relTemplates, filename.toStdString());
    accept();
}


// =====================================================================
// ExportProjectDialog
// =====================================================================

ExportProjectDialog::ExportProjectDialog(FamilyTree* FT, QWidget* parent)
    : QDialog(parent), FT_(FT), ui(new Ui::ExportProjectDialog){
    ui->setupUi(this);
    connect(ui->persons, SIGNAL(clicked()), this, SLOT(checkPerson()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(exportProject()));
}

ExportProjectDialog::~ExportProjectDialog(){
    delete ui;
}

void ExportProjectDialog::checkPerson(){
    ui->persons->setChecked(true);
}

void ExportProjectDialog::exportProject(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Select or make new file"), QDir::currentPath(), tr("JSON (*.json)"));
    if(filename == "") return;
    FT_->exportProject(filename.toStdString(), ui->events->isChecked(), ui->relations->isChecked());
    accept();
}

// =====================================================================
// PictureExport
// =====================================================================

PictureExport::PictureExport(bool closeDiagram, QWidget* parent)
    : QDialog(parent), closeDiagram_(closeDiagram), ui(new Ui::PictureExport){
    ui->setupUi(this);
    if(closeDiagram) ui->type->setText("Close family diagram:");
    else ui->type->setText("Family tree diagram:");
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(exportDiagram()));
}

PictureExport::~PictureExport(){
    delete ui;
}

void PictureExport::exportDiagram(){
    MainWindow* mw = dynamic_cast<MainWindow*>(parent());
    bool success = false;
    if(closeDiagram_) success = mw->exportCloseFamilyDiagram(ui->scale->value());
    else success = mw->exportTreeDiagram(ui->scale->value());
    if(success) accept();
}

// =====================================================================
// HtmlExport
// =====================================================================

HtmlExport::HtmlExport(FamilyTree* FT, bool all, int up, int down, QWidget* parent)
    : QDialog(parent), FT_(FT), ui(new Ui::HtmlExport){
    ui->setupUi(this);
    if(all) ui->type->setText("Pages for all persons:");
    else ui->type->setText("Page for proband only:");
    connect(ui->allOlder, SIGNAL(toggled(bool)), this, SLOT(allAboveGenerations(bool)));
    connect(ui->allYounger, SIGNAL(toggled(bool)), this, SLOT(allBelowGenerations(bool)));
    if(up == -1) ui->allYounger->setChecked(true);
    else ui->olderBarrier->setValue(up);
    if(down == -1) ui->allOlder->setChecked(true);
    else ui->youngerBarrier->setValue(down);
    if(all) connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(exportAllHtml()));
    else connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(exportMainHtml()));
}

HtmlExport::~HtmlExport(){
    delete ui;
}

void HtmlExport::allAboveGenerations(bool all){
    ui->olderBarrier->setEnabled(!all);
}

void HtmlExport::allBelowGenerations(bool all){
    ui->youngerBarrier->setEnabled(!all);
}

void HtmlExport::exportAllHtml(){
    if(FT_ == nullptr){
        reject();
        return;
    }
    Person* before = FT_->getMainPerson();
    MainWindow* main = dynamic_cast<MainWindow*>(parent());
    int up = ui->allOlder->isChecked() ? -1 : ui->olderBarrier->value();
    int down = ui->allYounger->isChecked() ? -1 : ui->youngerBarrier->value();
    auto [n_up, n_down] = main->setNumberOfGenerations(up, down);
    QString dirPath = QFileDialog::getExistingDirectory(this,tr("Choose an output directory"), QDir::currentPath());
    QProgressDialog progressDialog("Exporting webpages..", "Cancel export", 0, FT_->getPersons().size(), this);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.show();
    progressDialog.setValue(0);
    FT_->printHtml(dirPath.toStdString());
    std::string resDir = FT_->createResourcesDir(dirPath.toStdString());
    int index = 0;
    for(auto&& [id, person] : FT_->getPersons()){
        progressDialog.setValue(index++);
        if(progressDialog.wasCanceled()){
            FT_->setMainPerson(before);
            main->setNumberOfGenerations(n_up, n_down);
            main->refreshGraphics();
            reject();
            return;
        }
        FT_->setMainPerson(person.get());
        main->refreshGraphics();
        {
            std::stringstream ss;
            ss << resDir << html::PREFIX_CLOSE << FT_->getMainPerson()->getId() << ".png";
            main->exportCloseFamilyDiagram(ui->scaleClose->value(), ss.str());
        }
        {
            std::stringstream ss;
            ss << resDir << html::PREFIX_FAMILY_TREE << FT_->getMainPerson()->getId() << ".png";
            main->exportTreeDiagram(ui->scaleTree->value(), ss.str());
        }
    }
    FT_->setMainPerson(before);
    main->setNumberOfGenerations(n_up, n_down);
    main->refreshGraphics();
    progressDialog.close();
}

void HtmlExport::exportMainHtml(){
    if(FT_ == nullptr || FT_->getMainPerson() == nullptr){
        reject();
        return;
    }
    MainWindow* main = dynamic_cast<MainWindow*>(parent());
    int up = ui->allOlder->isChecked() ? -1 : ui->olderBarrier->value();
    int down = ui->allYounger->isChecked() ? -1 : ui->youngerBarrier->value();
    auto [n_up, n_down] = main->setNumberOfGenerations(up, down);
    QString dirPath = QFileDialog::getExistingDirectory(this,tr("Choose an output directory"), QDir::currentPath());
    FT_->printMainPersonHtml(dirPath.toStdString());
    std::string resDir = FT_->createResourcesDir(dirPath.toStdString());
    {
        std::stringstream ss;
        ss << resDir << html::PREFIX_CLOSE << FT_->getMainPerson()->getId() << ".png";
        main->exportCloseFamilyDiagram(ui->scaleClose->value(), ss.str());
    }
    {
        std::stringstream ss;
        ss << resDir << html::PREFIX_FAMILY_TREE << FT_->getMainPerson()->getId() << ".png";
        main->exportTreeDiagram(ui->scaleTree->value(), ss.str());
    }
    main->setNumberOfGenerations(n_up, n_down);
    main->refreshGraphics();
}

// =====================================================================
// SuggestionsDialog
// =====================================================================

SuggestionsDialog::SuggestionsDialog(FamilyTree* FT, std::vector<RelationSuggestion> suggestions, QWidget* parent)
    : QDialog(parent), FT_(FT), ui(new Ui::SuggestionsDialog){
    ui->setupUi(this);
    QStringList header;
    header << "First name" << "First person" << "Second name" << "Second person" << "Trait" << "Relation";
    ui->suggestionList->setHeaderLabels(header);
    for(auto&& [id, templ] : FT_->getSettings()->getRelationTemplates()){
        switch(templ->getTrait()){
            case Partnership:
                partnerships.push_back(templ.get());
                break;
            case Motherhood:
                motherhoods.push_back(templ.get());
                break;
            case Fatherhood:
                fatherhoods.push_back(templ.get());
                break;
            case Sibling:
                siblings.push_back(templ.get());
            default:
                break;
        }
    }
    for(auto&& rs : suggestions){
        switch(rs.trait){
            case Partnership:
                if(partnerships.size() > 0){
                    RelationItem* item = new RelationItem(FT_, rs, partnerships[0]);
                    ui->suggestionList->addTopLevelItem(item);
                }
                break;
            case Motherhood:
                if(motherhoods.size() > 0){
                    RelationItem* item = new RelationItem(FT_, rs, motherhoods[0]);
                    ui->suggestionList->addTopLevelItem(item);
                }
                break;
            case Fatherhood:
                if(fatherhoods.size() > 0){
                    RelationItem* item = new RelationItem(FT_, rs, fatherhoods[0]);
                    ui->suggestionList->addTopLevelItem(item);
                }
                break;
            case Sibling:
                if(siblings.size() > 0){
                    RelationItem* item = new RelationItem(FT_, rs, siblings[0]);
                    ui->suggestionList->addTopLevelItem(item);
                }
                break;
            default:
                break;
        }
    }
    ui->firstPerson->setText("");
    ui->secondPerson->setText("");
    for(int i = 0; i < ui->suggestionList->columnCount() - 1; ++i)
        ui->suggestionList->resizeColumnToContents(i);
    connect(ui->suggestionList, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(selectedSuggestion(QTreeWidgetItem*)));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeSuggestion()));
    connect(ui->swapButton, SIGNAL(clicked()), this, SLOT(swapPersons()));
    connect(ui->relations, SIGNAL(currentIndexChanged(int)), this, SLOT(switchedTemplate()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(save()));
}


SuggestionsDialog::~SuggestionsDialog(){
    delete ui;
}

void SuggestionsDialog::removeSuggestion(){
    if(ui->suggestionList->currentItem() == nullptr) return;
    delete ui->suggestionList->currentItem();
}

void SuggestionsDialog::save(){
    for(int i = 0; i < ui->suggestionList->topLevelItemCount(); ++i){
        RelationItem* relItem = dynamic_cast<RelationItem*>(ui->suggestionList->topLevelItem(i));
        Relation* newRel = FT_->addRelation();
        Relation* rel = relItem->getRelation();
        FT_->setRelation(newRel->getId(), rel->getFirstPerson(), rel->getSecondPerson(), rel->getTemplate());
    }
    accept();
}

void SuggestionsDialog::selectedSuggestion(QTreeWidgetItem* item){
    RelationItem* relItem = dynamic_cast<RelationItem*>(item);
    auto rel = relItem->getRelation();
    size_t relTempl = rel->getTemplate();
    ui->relations->clear();
    std::vector<RelationTemplate*>* container;
    switch(rel->getTrait()){
        case Motherhood:
            container = &motherhoods;
            break;
        case Fatherhood:
            container = &fatherhoods;
            break;
        case Partnership:
            container = &partnerships;
            break;
        case Sibling:
            container = &siblings;
            break;
        default:
            return;
    }
    int index = 0;
    for(auto&& templ : *container){
        ui->relations->addItem(QString::fromStdString(templ->getTitle()), QVariant::fromValue(templ->getId()));
        if(relTempl == templ->getId()){
            ui->relations->setCurrentIndex(index);
            ui->firstPerson->setText(QString::fromStdString(templ->getFirstName()));
            ui->secondPerson->setText(QString::fromStdString(templ->getSecondName()));
            ui->description->setText(QString::fromStdString(templ->getDescription()));
        }
        ++index;
    }
    auto optLocked = FT_->getPerson(rel->getFirstPerson());
    auto optSecond = FT_->getPerson(rel->getSecondPerson());
    if(!optLocked || !optSecond) return;
    ui->firstPersonEdit->setText(QString::fromStdString((*optLocked)->str()));
    ui->secondPersonEdit->setText(QString::fromStdString((*optSecond)->str()));
    relItem->refreshTexts();
}

void SuggestionsDialog::swapPersons(){
    if(ui->suggestionList->currentItem() == nullptr) return;
    RelationItem* relItem = dynamic_cast<RelationItem*>(ui->suggestionList->currentItem());
    auto rel = relItem->getRelation();
    rel->setPersons(rel->getSecondPerson(), rel->getFirstPerson());
    auto optLocked = FT_->getPerson(rel->getFirstPerson());
    auto optSecond = FT_->getPerson(rel->getSecondPerson());
    if(!optLocked || !optSecond) return;
    ui->firstPersonEdit->setText(QString::fromStdString((*optLocked)->str()));
    ui->secondPersonEdit->setText(QString::fromStdString((*optSecond)->str()));
    relItem->refreshTexts();
}

void SuggestionsDialog::switchedTemplate(){
    if(ui->suggestionList->currentItem() == nullptr) return;
    RelationItem* relItem = dynamic_cast<RelationItem*>(ui->suggestionList->currentItem());
    auto rel = relItem->getRelation();
    size_t id = ui->relations->currentData().toULongLong();
    auto relTempl = FT_->getSettings()->getRelationTemplate(id);
    if(!relTempl) return;
    rel->setTemplate(id);
    auto templ = *relTempl;
    ui->firstPerson->setText(QString::fromStdString(templ->getFirstName()));
    ui->secondPerson->setText(QString::fromStdString(templ->getSecondName()));
    ui->description->setText(QString::fromStdString(templ->getDescription()));
    relItem->refreshTexts();
}
