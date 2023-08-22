/// @file mainwindow_slots.cpp Implementation of all slots of the main window. For other methods see mainwindow.cpp.
#include "mainwindow.h"

void MainWindow::addNewEvent(){
    EventDialog* ed = new EventDialog(&FT, this);
	ed->show();
}

void MainWindow::addNewFile(){
    auto item = dynamic_cast<FileTreeItem*>(ui->fileTreeWidget->currentItem());
    std::vector<std::string> ext {".pdf"};
    addGeneralFile(GENERAL_FILE, ext, item);
}

void MainWindow::addNewFileFolder(){
    addGeneralFolder(GENERAL_FILE);
}

void MainWindow::addNewMedia(){
    auto item = dynamic_cast<FileTreeItem*>(ui->mediaTreeWidget->currentItem());
    std::vector<std::string> ext {".jpg", ".png", ".bmp", ".mp4", ".avi", ".mp3", ".flac", ".ogg"};
    addGeneralFile(MEDIA, ext, item);
}

void MainWindow::addNewMediaFolder(){
    addGeneralFolder(MEDIA);
}

void MainWindow::addNewNote(){
    auto item = dynamic_cast<FileTreeItem*>(ui->noteTreeWidget->currentItem());
    std::vector<std::string> ext {".txt", ".md", ".rtf", ".html"};
    addGeneralFile(NOTE, ext, item);
}

void MainWindow::addNewNoteFolder(){
    addGeneralFolder(NOTE);
}

void MainWindow::addNewPerson(){
    savePersonsInfo();
	Person* p = FT.addPerson();
	refreshUi();
	ProjectItem* item = new ProjectItem(PROJECT_PERSON, p->getId());
	item->setData(0, Qt::DisplayRole, QVariant::fromValue(p->getId()));
	ui->projectView->addTopLevelItem(item);
    initializeProjectSubView(item, p);
    item->setText(1, QString::fromStdString(p->str()));
    switch(p->getGender()){
        case Male:
            item->setIcon(0, QIcon(":/resources/man.svg"));
            break;
        case Female:
            item->setIcon(0, QIcon(":/resources/woman.svg"));
            break;
        case Other:
        default:
            item->setIcon(0, QIcon(":/resources/other.svg"));
            break;
    }
    item->setText(2, QString::fromStdString(p->getBirthDate().str()));
    if(!p->isAlive()) item->setText(3, QString::fromStdString(p->getDeathDate().str()));
    refreshProjectView();
    filterProjectItems();
    FT.setUnsaved();
}

void MainWindow::addNewRelation(){
    RelationDialog* rd = new RelationDialog(&FT, this);
	rd->show();
}

void MainWindow::allAboveGenerations(bool selected){
    ui->generationsNumberAbove->setEnabled(!selected);
    if(selected) genSizeUp_ = -1;
    else genSizeUp_ = ui->generationsNumberAbove->value();
    FT.getSettings()->setAppSettings().genSizeUp = -1;
    drawFamilyTree();
}

void MainWindow::allBelowGenerations(bool selected){
    ui->generationsNumberBelow->setEnabled(!selected);
    if(selected) genSizeDown_ = -1;
    else genSizeDown_ = ui->generationsNumberBelow->value();
    FT.getSettings()->setAppSettings().genSizeDown = -1;
    drawFamilyTree();
}

void MainWindow::changeGenerationsDown(int value){
    genSizeDown_ = value;
    FT.getSettings()->setAppSettings().genSizeDown = genSizeDown_;
    drawFamilyTree();
}

void MainWindow::changeGenerationsUp(int value){
    genSizeUp_ = value;
    FT.getSettings()->setAppSettings().genSizeUp = genSizeUp_;
    drawFamilyTree();
}

void MainWindow::checkStoreRefreshPersonsInfo(){
    if(FT.getMainPerson() == nullptr) return;
    bool forb = false;
    if(containsForb(ui->titleEditF->text())) forb = true;
    if(containsForb(ui->titleEditA->text())) forb = true;
    if(containsForb(ui->nameEdit->text())) forb = true;
    if(containsForb(ui->surnameEdit->text())) forb = true;
    if(containsForb(ui->maidenNameEdit->text())) forb = true;
    if(containsForb(ui->placeOfBirthEdit->text())) forb = true;
    if(!ui->isAlive->isChecked() && containsForb(ui->placeOfDeathEdit->text())) forb = true;
    if(forb){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
        return;
    }
    savePersonsInfo();
    ui->currentPerson->setText(QString::fromStdString(FT.getMainPerson()->str()));
    refreshProjectView();
    refreshGraphics();
}

void MainWindow::clearProjectFilter(){
    ui->findEdit->setText("");
    for(int i = 0; i < ui->projectView->topLevelItemCount(); ++i)
        ui->projectView->topLevelItem(i)->setHidden(false);
}

void MainWindow::clearProjectPaths(){
    FT.clearProjectPaths();
    auto actions = ui->menuRecent_trees->actions();
    for(auto&& action : actions){
        if(action != ui->actionClear_paths)
            ui->menuRecent_trees->removeAction(action);
    }
    for(auto&& root : FT.getProjectPaths()){
        QAction* action = ui->menuRecent_trees->addAction(QString::fromStdString(root));
        connect(action, SIGNAL(triggered()), this, SLOT(openExistingProject()));
    }
}

void MainWindow::disableFileFolderPrompt(){
    enableGeneralFolderPrompt(false, GENERAL_FILE);
}

void MainWindow::disableMediaFolderPrompt(){
  enableGeneralFolderPrompt(false, MEDIA);
}

void MainWindow::disableNoteFolderPrompt(){
   enableGeneralFolderPrompt(false, NOTE);
}

void MainWindow::editEvent(QTreeWidgetItem* item){
    auto id = item->data(0, Qt::UserRole).toULongLong();
    editEvent(id);
}

void MainWindow::editRelation(QTreeWidgetItem* item){
    auto id = item->data(0, Qt::UserRole).toULongLong();
    editRelation(id);
}

void MainWindow::editTag(QTreeWidgetItem* item){
	if(FT.getMainPerson() == nullptr) return;
	QString tag;
	QString value;
	if(item != nullptr){
		tag = item->text(0);
		value = item->text(1);
	}
	TagChooserDialog* tc = new TagChooserDialog(&FT, tag, value, this);
	tc->show();
}

void MainWindow::enableFileFolderPrompt(){
    enableGeneralFolderPrompt(true, GENERAL_FILE);
}

void MainWindow::enableMediaFolderPrompt(){
    enableGeneralFolderPrompt(true, MEDIA);
}

void MainWindow::enableNoteFolderPrompt(){
    enableGeneralFolderPrompt(true, NOTE);
}

void MainWindow::exportCloseFamilyDiagramDialog(){
    if(FT.getMainPerson() == nullptr) return;
    PictureExport* pe = new PictureExport(true, this);
    pe->show();
}

void MainWindow::exportHtml(){
    if(FT.getMainPerson() == nullptr) return;
    HtmlExport* he = new HtmlExport(&FT, true, genSizeUp_, genSizeDown_, this);
    he->show();
}

void MainWindow::exportMainHtml(){
    if(FT.getMainPerson() == nullptr) return;
    HtmlExport* he = new HtmlExport(&FT, false, genSizeUp_, genSizeDown_, this);
    he->show();
}

void MainWindow::exportProject(){
    ExportProjectDialog* epd = new ExportProjectDialog(&FT, this);
    epd->show();
}

void MainWindow::exportTemplates(){
    ExportTemplatesDialog* etd = new ExportTemplatesDialog(&FT, this);
    etd->show();
}

void MainWindow::exportTreeDiagramDialog(){
    if(FT.getMainPerson() == nullptr) return;
    PictureExport* pe = new PictureExport(false, this);
    pe->show();
}

void MainWindow::filterProjectItems(){
    auto items = ui->projectView->findItems(ui->findEdit->text(), Qt::MatchContains, 1);
    auto all_items = ui->projectView->findItems("", Qt::MatchContains);
    for(int i = 0; i < ui->projectView->topLevelItemCount(); ++i)
        ui->projectView->topLevelItem(i)->setHidden(true);
    for(auto&& item : items)
        item->setHidden(false);
}

void MainWindow::findPerson(){
	ui->mainTabWidget->setCurrentIndex(0);
    ui->findEdit->setFocus();
}

void MainWindow::fitTreeInView(){
    treeView->fitInView(treeScene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::importProject(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose file to import"), QDir::currentPath(), tr("JSON (*.json)"));
    if(filename == "") return;
    bool success = FT.importProject(filename.toStdString());
    if(!success){
        QMessageBox::critical(this, "Error", "Import was unsuccesful.");
    }
    refreshUi();
    ui->projectView->clear();
    initializeProjectView();
}

void MainWindow::importRootDir(){
    if(!FT.isDirectorySet()){
        QMessageBox::critical(this, "Error", "First save this project to set the root directory.");
        return;
    }
    QString dirPath = QFileDialog::getExistingDirectory(this,tr("Choose family tree project to import"), QDir::currentPath());
    if(dirPath == "") return;
    bool success = FT.importRootDir(dirPath.toStdString());
    if(!success){
        QMessageBox::critical(this, "Error", "Import was unsuccesful.");
    }
    refreshUi();
    ui->projectView->clear();
    initializeProjectView();
}

void MainWindow::importTemplates(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose file to import"), QDir::currentPath(), tr("JSON (*.json)"));
    if(filename == "") return;
    auto [success, imported] = FT.importTemplates(filename.toStdString());
    if(!success){
        QMessageBox::critical(this, "Error", "Import was unsuccesful.");
    }
    else{
        std::stringstream sstrem;
        sstrem << "Added new " << imported << " templates to the database.";
        QMessageBox::information(this, "Information", QString::fromStdString(sstrem.str()), QMessageBox::Yes);
    }
}

void MainWindow::interractWithProjectItem(QTreeWidgetItem* item){
    ProjectItem* project_item = dynamic_cast<ProjectItem*>(item);
    std::string fileUrl;
    switch(project_item->type()){
        case PROJECT_PERSON:
            savePersonsInfo();
            FT.setMainPerson(project_item->getId());
            refreshUi();
            break;
        case PROJECT_FILE:
            FT.getUrlFile(project_item->getId(), fileUrl, GENERAL_FILE);
            openExternalFile(fileUrl);
            break;
        case PROJECT_MEDIA:
            FT.getUrlFile(project_item->getId(), fileUrl, MEDIA);
            openExternalFile(fileUrl);
            break;
        case PROJECT_NOTE:
            FT.getUrlFile(project_item->getId(), fileUrl, NOTE);
            openExternalFile(fileUrl);
            break;
        case PROJECT_RELATIONS:
            editRelation(project_item->getId());
            break;
        case PROJECT_EVENTS:
            editEvent(project_item->getId());
            break;
        case PROJECT_GROUP:
        default:
            break;
    }
}

void MainWindow::openBirthDateDialog(){
	if(FT.getMainPerson() == nullptr) return;
	openCustomDateDialog(FT.getMainPerson()->birthDate());
}

void MainWindow::openDeathDateDialog(){
	if(FT.getMainPerson() == nullptr) return;
	openCustomDateDialog(FT.getMainPerson()->deathDate());
}

void MainWindow::openEventTemplatesDialog(){
    EventTamplatesDialog* etd = new EventTamplatesDialog(&FT, this);
    etd->show();
}

void MainWindow::openExistingProject(){
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        openGeneralProject(action->text().toStdString());
    }
}

void MainWindow::openGraphicSettings(){
    VisualSettings* gs = new VisualSettings(probandColor_, promotedColor_, standardColor_, highlightedColor_, sizeX_, sizeY_, font_, &linePen_, &textPen_, borderRadius_, this);
    gs->show();
}

void MainWindow::openNewFamilyTreeProject(){
    QString dirPath = QFileDialog::getExistingDirectory(this,tr("Choose existing family tree project"), QDir::currentPath());
    std::string dir = dirPath.toStdString();
	if(dir != ""){
        bool added = FT.writeConfig(dir);
        if(added){
            QAction* action = ui->menuRecent_trees->addAction(dirPath);
            connect(action, SIGNAL(triggered()), this, SLOT(openExistingProject()));
        }
        openGeneralProject(dir);
	}
}

void MainWindow::openRelationTemplatesDialog(){
    RelationTemplateDialog* rtd = new RelationTemplateDialog(&FT, this);
    rtd->show();
}

void MainWindow::openSelectedFile(QTreeWidgetItem* item){
	openGeneralFile(item, GENERAL_FILE);
}

void MainWindow::openSelectedMedia(QTreeWidgetItem* item){
    openGeneralFile(item, MEDIA);
}

void MainWindow::openSelectedNote(QTreeWidgetItem* item){
    openGeneralFile(item, NOTE);
}

void MainWindow::promoteRelation(){
    auto item = ui->olderGenerationWidget->currentItem();
    if(item == nullptr)
        item = ui->sameGenerationWidget->currentItem();
    if(item == nullptr)
        item = ui->youngerGenerationWidget->currentItem();
    if(item == nullptr)
        return;
    auto id = item->data(0, Qt::UserRole).toULongLong();
    FT.promoteRelation(id);
    refreshRelationTab();
    refreshGraphics();
    FT.setUnsaved();
}

void MainWindow::refreshDates(){
    if(FT.getMainPerson() == nullptr) return;
    Person* main = FT.getMainPerson();
	ui->customBirthText->setText(QString::fromStdString(main->getBirthDate().str()));
	if(!main->isAlive())
		ui->customDeathText->setText(QString::fromStdString(main->getDeathDate().str()));
	else
        ui->customDeathText->setText("");
    checkStoreRefreshPersonsInfo();
}

void MainWindow::refreshEventTab(){
    if(FT.getMainPerson() == nullptr) return;
    ui->eventWidget->clear();
    for(auto&& eventId : FT.getMainPerson()->getEvents()){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        auto event = FT.getEvent(eventId);
        if(event){
            auto optTeml = FT.getSettings()->getEventTemplate((*event)->getTemplate());
            if(!optTeml) continue;
            item->setText(0, QString::fromStdString((*event)->str()));
            item->setText(2, QString::fromStdString((*event)->getText()));
            if((*optTeml)->containsDate()) item->setText(1, QString::fromStdString((*event)->getDate().str()));
            else item->setText(1, "-");
            item->setData(0, Qt::UserRole, QVariant::fromValue(eventId));
            item->setIcon(0, QIcon(":/resources/event.svg"));
            ui->eventWidget->addTopLevelItem(item);
        }
        else{
            std::stringstream ss;
            ss << "Event couldn't be showed due to the absence of the event.";
            FT.log(ss.str());
            delete item;
        }
    }
    ui->eventWidget->resizeColumnToContents(0);
    ui->eventWidget->resizeColumnToContents(1);
}

void MainWindow::refreshGraphics(){
    drawCloseFamily();
    drawFamilyTree();
}

void MainWindow::refreshProjectView(){
    for(int i = 0; i < ui->projectView->topLevelItemCount(); ++i){
        ProjectItem* item = dynamic_cast<ProjectItem*>(ui->projectView->topLevelItem(i));
        size_t id = item->data(0, Qt::DisplayRole).toULongLong();
        auto optPerson = FT.getPerson(id);
        if(!optPerson){
            delete item;
            continue;
        }
        item->setData(0, Qt::DisplayRole, QVariant::fromValue((*optPerson)->getId()));
        item->setText(1, QString::fromStdString((*optPerson)->str()));
        switch((*optPerson)->getGender()){
            case Male:
                item->setIcon(0, QIcon(":/resources/man.svg"));
                break;
            case Female:
                item->setIcon(0, QIcon(":/resources/woman.svg"));
                break;
            case Other:
            default:
                item->setIcon(0, QIcon(":/resources/other.svg"));
                break;
        }
        item->setText(2, QString::fromStdString((*optPerson)->getBirthDate().str()));
        if(!(*optPerson)->isAlive()) item->setText(3, QString::fromStdString((*optPerson)->getDeathDate().str()));
        else item->setText(3, "");
        for(int j = 0; j < item->childCount(); ++j){
            auto project_item = dynamic_cast<ProjectItem*>(item->child(j));
            if(project_item->text(1) == "Files")
                refreshProjectFilesView(project_item, (*optPerson));
            else if(project_item->text(1) == "Media")
                refreshProjectMediaView(project_item, (*optPerson));
            else if(project_item->text(1) == "Notes")
                refreshProjectNoteView(project_item, (*optPerson));
            else if(project_item->text(1) == "Relations")
                refreshProjectRelationsView(project_item, (*optPerson));
            else if(project_item->text(1) == "Events")
                refreshProjectEventsView(project_item, (*optPerson));
        }
        if(item->childCount() == 0)
            initializeProjectSubView(item, (*optPerson));
    }
    for(int i = 0; i < ui->projectView->columnCount() - 1; ++i)
        ui->projectView->resizeColumnToContents(i);
}

void MainWindow::refreshRelationTab(){
    if(FT.getMainPerson() == nullptr) return;
    size_t mainPersonId = FT.getMainPerson()->getId();
    ui->olderGenerationWidget->clear();
    ui->sameGenerationWidget->clear();
    ui->youngerGenerationWidget->clear();
    QTreeWidget* widget;
    for(auto&& relId : FT.getMainPerson()->getRelations()){
        auto optRel = FT.getRelation(relId);
        if(!optRel) continue;
        Relation* rel = (*optRel);
        if(rel->isOlderGeneration(mainPersonId))
            widget = ui->youngerGenerationWidget;
        else if(rel->isYoungerGeneration(mainPersonId))
            widget = ui->olderGenerationWidget;
        else
            widget = ui->sameGenerationWidget;
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, Qt::UserRole, QVariant::fromValue(relId));
        widget->addTopLevelItem(item);
        item->setIcon(0, QIcon(":/resources/relation.svg"));
        item->setText(0, QString::fromStdString(rel->getTheOtherPersonName(mainPersonId)));
        auto optPerson = FT.getPerson(rel->getTheOtherPerson(mainPersonId));
        if(!optPerson){
            delete item;
            continue;
        }
        item->setText(1, QString::fromStdString((*optPerson)->getFullName()));
        if(FT.getMainPerson()->getFather() == relId || FT.getMainPerson()->getMother() == relId || FT.getMainPerson()->getPartner() == relId)
            item->setIcon(0, QIcon(":/resources/promotedRelation.svg"));
        widget->resizeColumnToContents(0);
    }
}

void MainWindow::refreshTabsWithFiles(){
    if(FT.getMainPerson() == nullptr) return;
	showGeneralFiles(ui->fileTreeWidget, FT.getMainPerson()->getFilesRootPointer(GENERAL_FILE), GENERAL_FILE);
	showGeneralFiles(ui->mediaTreeWidget, FT.getMainPerson()->getFilesRootPointer(MEDIA), MEDIA);
	showGeneralFiles(ui->noteTreeWidget, FT.getMainPerson()->getFilesRootPointer(NOTE), NOTE);
}

void MainWindow::refreshTagsList(){
	if(FT.getMainPerson() == nullptr) return;
	ui->tagWidget->clear();
	for(auto&& [tag, value] : FT.getMainPerson()->getTags()){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(tag));
        item->setText(1, QString::fromStdString(value));
		ui->tagWidget->addTopLevelItem(item);
	}
    for(int i = 0; i < ui->tagWidget->columnCount() - 1; ++i)
        ui->tagWidget->resizeColumnToContents(i);
}

void MainWindow::removeEvent(){
    QTreeWidgetItem* item = ui->eventWidget->currentItem();
    if(item == nullptr) return;
    auto optEvent = FT.getEvent(item->data(0, Qt::UserRole).toULongLong());
    if(!optEvent) return;
    QMessageBox::StandardButton result;
    std::stringstream ss;
    ss << "Following event: " << (*optEvent)->str() << " will be deleted. Are you sure?";
    result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes)
        FT.removeEvent(item->data(0, Qt::UserRole).toULongLong());
    refreshProjectView();
    refreshEventTab();
    FT.setUnsaved();
}

void MainWindow::removeFileFolder(){
    removeGeneralFolder(GENERAL_FILE);
}

void MainWindow::removeMediaFolder(){
    removeGeneralFolder(MEDIA);
}

void MainWindow::removeNoteFolder(){
    removeGeneralFolder(NOTE);
}

void MainWindow::removePerson(){
    if(FT.getMainPerson() == nullptr) return;
    size_t id = FT.getMainPerson()->getId();
    QMessageBox::StandardButton result;
    std::stringstream ss;
    ss << "Current person: " << FT.getMainPerson()->str() << " will be removed. Are you sure?";
    result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        FT.removePerson();
        FT.setUnsaved();
        ui->infoTab->setEnabled(false);
        for(int i = 0; i < ui->projectView->topLevelItemCount(); ++i){
            QTreeWidgetItem* item = ui->projectView->topLevelItem(i);
            auto idItem = item->data(0, Qt::DisplayRole).toULongLong();
            if(idItem == id){
                delete item;
                return;
            }
        }
    }
}

void MainWindow::removeRelation(){
    auto item = ui->olderGenerationWidget->currentItem();
    if(item == nullptr)
        item = ui->sameGenerationWidget->currentItem();
    if(item == nullptr)
        item = ui->youngerGenerationWidget->currentItem();
    if(item == nullptr) return;
    QMessageBox::StandardButton result;
    std::stringstream ss;
    ss << "Relation: " << item->text(0).toStdString() << " will be removed. Are you sure?";
    result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        FT.removeRelation(item->data(0, Qt::UserRole).toULongLong());
        refreshRelationTab();
        refreshProjectView();
        refreshGraphics();
        FT.setUnsaved();
    }
}

void MainWindow::removeTag(){
	if(FT.getMainPerson() == nullptr){
		return;
	}
	QTreeWidgetItem* item = ui->tagWidget->currentItem();
	if(item == nullptr){
		return;
	}
	QMessageBox::StandardButton result;
    std::stringstream ss;
    ss << "Tag: " << item->text(0).toStdString() << " with value: " << item->text(1).toStdString() << " will be removed. Are you sure?";
    result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        FT.getMainPerson()->removeTag(item->text(0).toStdString());
        refreshTagsList();
        FT.setUnsaved();
    }
}

void MainWindow::renameFile(){
    renameGeneralFileItem(GENERAL_FILE);
}

void MainWindow::renameMedia(){
    renameGeneralFileItem(MEDIA);
}

void MainWindow::renameNote(){
    renameGeneralFileItem(NOTE);
}

bool MainWindow::saveDatabase(){
	savePersonsInfo();
	auto files = FT.getOrphanFiles();
	if(files.size() > 0){
        std::stringstream ss;
        ss << "These files will be permanently deleted: ";
        bool first = true;
        for (auto&& file : files){
            if(!first) ss << ", ";
            ss << file;
            first = false;
        }
        ss << ". Do you want to save the database with losing these files?";
        auto result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::No) return false;
	}
	bool success = FT.storeDatabase();
	if(!success){
        QString dirPath = QFileDialog::getExistingDirectory(this,tr("Choose root directory for your project"), QDir::currentPath());
        std::string dir = dirPath.toStdString();
        if(dir != ""){
            bool added = FT.writeConfig(dir);
            if(added){
                QAction* action = ui->menuRecent_trees->addAction(QString::fromStdString(dir));
                connect(action, SIGNAL(triggered()), this, SLOT(openExistingProject()));
            }
            FT.createDatabase(dir);
        }
	}
	return true;
}

void MainWindow::saveGraphicsSetting(QColor probandColor, QColor specialColor, QColor standardColor, QColor highlightedColor, int sizeX, int sizeY, QFont font,
                                     QColor lineColor, QColor textColor, int lineWidth, int rounding){
    probandColor_ = probandColor;
    promotedColor_ = specialColor;
    standardColor_ = standardColor;
    highlightedColor_ = highlightedColor;
    linePen_.setColor(lineColor);
    linePen_.setWidth(lineWidth);
    textPen_.setColor(textColor);
    borderRadius_ = rounding;
    sizeX_ = sizeX;
    sizeY_  = sizeY;
    font_ = font;
    FT.getSettings()->setAppSettings().probandColor.blue = probandColor_.blue();
    FT.getSettings()->setAppSettings().probandColor.red = probandColor_.red();
    FT.getSettings()->setAppSettings().probandColor.green = probandColor_.green();
    FT.getSettings()->setAppSettings().promoted.blue = promotedColor_.blue();
    FT.getSettings()->setAppSettings().promoted.red = promotedColor_.red();
    FT.getSettings()->setAppSettings().promoted.green = promotedColor_.green();
    FT.getSettings()->setAppSettings().standard.blue = standardColor_.blue();
    FT.getSettings()->setAppSettings().standard.red = standardColor_.red();
    FT.getSettings()->setAppSettings().standard.green = standardColor_.green();
    FT.getSettings()->setAppSettings().highlightedColor.blue = highlightedColor_.blue();
    FT.getSettings()->setAppSettings().highlightedColor.red = highlightedColor_.red();
    FT.getSettings()->setAppSettings().highlightedColor.green = highlightedColor_.green();
    FT.getSettings()->setAppSettings().sizeX = sizeX_;
    FT.getSettings()->setAppSettings().sizeY = sizeY_;
    FT.getSettings()->setAppSettings().fontFamily = font_.family().toStdString();
    FT.getSettings()->setAppSettings().fontSize = font_.pointSize();
    FT.getSettings()->setAppSettings().textColor.blue = textColor.blue();
    FT.getSettings()->setAppSettings().textColor.red = textColor.red();
    FT.getSettings()->setAppSettings().textColor.green = textColor.green();
    FT.getSettings()->setAppSettings().lineColor.blue = lineColor.blue();
    FT.getSettings()->setAppSettings().lineColor.red = lineColor.red();
    FT.getSettings()->setAppSettings().lineColor.green = lineColor.green();
    FT.getSettings()->setAppSettings().lineWidth = lineWidth;
    FT.getSettings()->setAppSettings().radius = rounding;
    FT.setUnsaved();
}

void MainWindow::selectOlderGenerationOnly(){
    ui->sameGenerationWidget->clearSelection();
    ui->youngerGenerationWidget->clearSelection();
}

void MainWindow::selectSameGenerationOnly(){
    ui->olderGenerationWidget->clearSelection();
    ui->youngerGenerationWidget->clearSelection();
}

void MainWindow::selectYoungerGenerationOnly(){
    ui->olderGenerationWidget->clearSelection();
    ui->sameGenerationWidget->clearSelection();
}

void MainWindow::setEnabledDeathAtributes(bool disabled){
    ui->dateOfDeathLabel->setEnabled(!disabled);
    ui->customDeathDate->setEnabled(!disabled);
    ui->placeOfDeathLabel->setEnabled(!disabled);
    ui->placeOfDeathEdit->setEnabled(!disabled);
}

void MainWindow::setEventTabVisible(){
     for(auto it = 0; it < ui->infoTab->count(); ++it){
        if(ui->infoTab->tabText(it) == "Events"){
            ui->infoTab->setTabVisible(it, ui->actionShow_Events->isChecked());
            break;
        }
    }
    FT.getSettings()->setAppSettings().showEventTab = ui->actionShow_Events->isChecked();
}

void MainWindow::setFileTabVisible(){
     for(auto it = 0; it < ui->infoTab->count(); ++it){
        if(ui->infoTab->tabText(it) == "Files"){
            ui->infoTab->setTabVisible(it, ui->actionShow_Files->isChecked());
            break;
        }
    }
    FT.getSettings()->setAppSettings().showFileTab = ui->actionShow_Files->isChecked();
}

void MainWindow::setGeneralTabVisible(){
    for(auto it = 0; it < ui->infoTab->count(); ++it){
        if(ui->infoTab->tabText(it) == "General"){
            ui->infoTab->setTabVisible(it, ui->actionShow_General->isChecked());
            break;
        }
    }
    FT.getSettings()->setAppSettings().showGeneralTab = ui->actionShow_General->isChecked();
}

void MainWindow::setMainPerson(size_t personId){
    FT.setMainPerson(personId);
    refreshUi();
}

void MainWindow::setMediaTabVisible(){
    for(auto it = 0; it < ui->infoTab->count(); ++it){
        if(ui->infoTab->tabText(it) == "Media"){
            ui->infoTab->setTabVisible(it, ui->actionShow_Media->isChecked());
            break;
        }
    }
    FT.getSettings()->setAppSettings().showMediaTab = ui->actionShow_Media->isChecked();
}

void MainWindow::setNoteTabVisible(){
    for(auto it = 0; it < ui->infoTab->count(); ++it){
        if(ui->infoTab->tabText(it) == "Notes"){
            ui->infoTab->setTabVisible(it, ui->actionShow_Notes->isChecked());
            break;
        }
    }
    FT.getSettings()->setAppSettings().showNoteTab = ui->actionShow_Notes->isChecked();
}

void MainWindow::setRelationTabVisible(){
    for(auto it = 0; it < ui->infoTab->count(); ++it){
        if(ui->infoTab->tabText(it) == "Relations"){
            ui->infoTab->setTabVisible(it, ui->actionShow_Relations->isChecked());
            break;
        }
    }
    FT.getSettings()->setAppSettings().showRelationTab = ui->actionShow_Relations->isChecked();
}

void MainWindow::setTabEast(){
	ui->actionSouth->setChecked(false);
	ui->actionWest->setChecked(false);
	ui->actionNorth->setChecked(false);
	ui->actionEast->setChecked(true);
	ui->infoTab->setTabPosition(QTabWidget::East);
	FT.getSettings()->setAppSettings().tabPosition = East;
}

void MainWindow::setTabNorth(){
	ui->actionSouth->setChecked(false);
	ui->actionWest->setChecked(false);
	ui->actionEast->setChecked(false);
	ui->actionNorth->setChecked(true);
	ui->infoTab->setTabPosition(QTabWidget::North);
	FT.getSettings()->setAppSettings().tabPosition = North;
}

void MainWindow::setTabSouth(){
    ui->actionSouth->setChecked(true);
	ui->actionWest->setChecked(false);
	ui->actionEast->setChecked(false);
	ui->actionNorth->setChecked(false);
	ui->infoTab->setTabPosition(QTabWidget::South);
	FT.getSettings()->setAppSettings().tabPosition = South;
}

void MainWindow::setTabWest(){
	ui->actionSouth->setChecked(false);
	ui->actionWest->setChecked(true);
	ui->actionEast->setChecked(false);
	ui->actionNorth->setChecked(false);
	ui->infoTab->setTabPosition(QTabWidget::West);
	FT.getSettings()->setAppSettings().tabPosition = West;
}

void MainWindow::showHelp(){
    HelpDialog* td = new HelpDialog(&FT, this);
    td->show();
}

void MainWindow::showRenameFile(){
    enableGeneralFolderRenamePrompt(GENERAL_FILE);
}

void MainWindow::showRenameMedia(){
    enableGeneralFolderRenamePrompt(MEDIA);
}

void MainWindow::showRenameNote(){
    enableGeneralFolderRenamePrompt(NOTE);
}

void MainWindow::splitterChanged(){
    auto sizes = ui->splitter->sizes();
    FT.getSettings()->setAppSettings().splitterPositionOne = sizes[0];
    FT.getSettings()->setAppSettings().splitterPositionTwo = sizes[1];
    FT.setUnsaved();
}

void MainWindow::switchNewTreeProject(){
    if(!FT.isSaved()){
        QMessageBox::StandardButton result;
        std::stringstream ss;
        ss << "There are some changes that are not saved. Do you want to save them?";
        result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (result == QMessageBox::Save){
            bool saved = saveDatabase();
            if(!saved) return;
        }
        else if (result == QMessageBox::Cancel)
            return;
    }
    FT.clear();
    FT.getSettings()->setAppSettings().setDefault();
    loadSettings();
    clearUi();
}

void MainWindow::zoomIn(){
    double scaleFactor = 1.2;
    treeView->scale(scaleFactor, scaleFactor);
}

void MainWindow::zoomOut(){
    double scaleFactor = 1 / 1.2;
    treeView->scale(scaleFactor, scaleFactor);
}
