/// @file mainwindow.cpp Implementation of all standard methods. For slots see mainwindow_slots.cpp.
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), scene(new QGraphicsScene()), treeScene(new QGraphicsScene()), treeView(new GraphicsView(treeScene, this)), ui(new Ui::MainWindow){
	ui->setupUi(this);
    for(auto&& root : FT.getProjectPaths()){
        QAction* action = ui->menuRecent_trees->addAction(QString::fromStdString(root));
        connect(action, SIGNAL(triggered()), this, SLOT(openExistingProject()));
    }
    if(FT.showHelpOnStartup())
        showHelp();
    treeView->setScene(treeScene);
	setUiElements();
	FT.getSettings()->setAppSettings().setDefault();
	loadSettings();
    ui->treeGraphics->setScene(scene);
    ui->treeViewLayout->addWidget(treeView);
    refreshUi();
    connectAllSlots();
}

MainWindow::~MainWindow(){
	delete ui;
	delete scene;
	delete treeScene;
	delete treeView;
}

void MainWindow::addGeneralFile(FileType type, const std::vector<std::string>& extensions, FileTreeItem* item){
    if(item == nullptr){
        QMessageBox::critical(this, "Error", "First select file or folder where will the new file be.");
        return;
    }
    if(!FT.isDirectorySet()){
        QMessageBox::critical(this, "Error", "Root directory for database have to be set firstly. To do so save this project by pressing this option in menu or pressing Ctrl+S.");
        return;
    }
    if(!item->isDirectory())
        item = dynamic_cast<FileTreeItem*>(item->parent());
    auto drive = item->getDrive();
    AddFileDialog* af = new AddFileDialog(type, extensions, drive, &FT, this);
    af->show();
}

void MainWindow::addGeneralFolder(FileType type){
    QTreeWidget* widget;
    QLineEdit* edit;
    switch(type){
        case MEDIA:
            widget = ui->mediaTreeWidget;
            edit = ui->folderMediaEdit;
            break;
        case NOTE:
            widget = ui->noteTreeWidget;
            edit = ui->folderNoteEdit;
            break;
        case GENERAL_FILE:
        default:
            widget = ui->fileTreeWidget;
            edit = ui->folderFileEdit;
            break;
    }
    auto item = widget->currentItem();
    if(item == nullptr){
        QMessageBox::critical(this, "Error", "Select first the destination for the new folder.");
        return;
    }
	if(edit->text().isEmpty()){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::EMPTY_BOXES));
		return;
	}
	if(containsForb(edit->text())){
	    QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
		edit->selectAll();
		return;
    }
	enableGeneralFolderPrompt(false, type);
	FileTreeItem* fileItem = dynamic_cast<FileTreeItem*>(item);
	fileItem->addSubDrive(edit->text());
	refreshTabsWithFiles();
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(!FT.isSaved()){
        QMessageBox::StandardButton result;
        std::stringstream ss;
        ss << "There are some changes that are not saved. Do you want to save them?";
        result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (result == QMessageBox::Save){
            bool saved = saveDatabase();
            if(!saved){
                event->ignore();
                return;
            }
        }
        else if(result == QMessageBox::Cancel){
            event->ignore();
            return;
        }
    }
    event->accept();
}

void MainWindow::clearUi(){
    ui->projectView->clear();
    ui->eventWidget->clear();
    scene->clear();
    ui->currentPerson->setText("");
    ui->titleEditF->setText("");
	ui->titleEditA->setText("");
	ui->nameEdit->setText("");
	ui->surnameEdit->setText("");
	ui->maidenNameEdit->setText("");
	disableFileFolderPrompt();
    disableMediaFolderPrompt();
    disableNoteFolderPrompt();
    ui->fileTreeWidget->clear();
	ui->mediaTreeWidget->clear();
    ui->noteTreeWidget->clear();
    ui->tagWidget->clear();
    ui->olderGenerationWidget->clear();
    ui->sameGenerationWidget->clear();
    ui->youngerGenerationWidget->clear();
    ui->infoTab->setEnabled(false);
    refreshGraphics();
}

void MainWindow::connectAllSlots(){
    // Personal basic info
	connect(ui->isAlive, SIGNAL (toggled(bool)), this, SLOT(setEnabledDeathAtributes(bool)));
	connect(ui->customBirthDate, SIGNAL (clicked()), this, SLOT(openBirthDateDialog()));
	connect(ui->customDeathDate, SIGNAL (clicked()), this, SLOT(openDeathDateDialog()));
	// Tags
	connect(ui->addTagButton, SIGNAL(clicked()), this, SLOT(editTag()));
	connect(ui->tagWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editTag(QTreeWidgetItem*)));
	connect(ui->removeTagButton, SIGNAL(clicked()), this, SLOT(removeTag()));
	// Templates
	connect(ui->actionRelation_templates_editor, SIGNAL(triggered()), this, SLOT(openRelationTemplatesDialog()));
    connect(ui->actionEvent_templates_editor, SIGNAL(triggered()), this, SLOT(openEventTemplatesDialog()));
	// Settings
	connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveDatabase()));
	connect(ui->actionNorth, SIGNAL(triggered()), this, SLOT(setTabNorth()));
	connect(ui->actionSouth, SIGNAL(triggered()), this, SLOT(setTabSouth()));
	connect(ui->actionEast, SIGNAL(triggered()), this, SLOT(setTabEast()));
	connect(ui->actionWest, SIGNAL(triggered()), this, SLOT(setTabWest()));
	connect(ui->actionGraphics_settings, SIGNAL(triggered()), this, SLOT(openGraphicSettings()));
	// Menu
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openNewFamilyTreeProject()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(switchNewTreeProject()));
    connect(ui->actionClear_paths, SIGNAL(triggered()), this, SLOT(clearProjectPaths()));
    // Person Buttons
    connect(ui->addPerson, SIGNAL(clicked()), this, SLOT(addNewPerson()));
    connect(ui->removePerson, SIGNAL(clicked()), this, SLOT(removePerson()));
	// File Tree
	connect(ui->fileTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(openSelectedFile(QTreeWidgetItem*)));
	connect(ui->addFolderFileButton, SIGNAL(clicked()), this, SLOT(enableFileFolderPrompt()));
	connect(ui->folderFileEdit, SIGNAL(returnPressed()), this, SLOT(addNewFileFolder()));
	connect(ui->cancelFolderFileButton, SIGNAL(clicked()), this, SLOT(disableFileFolderPrompt()));
	connect(ui->okFolderFileButton, SIGNAL(clicked()), this, SLOT(addNewFileFolder()));
	connect(ui->folderFileEdit, SIGNAL(inputRejected()), this, SLOT(disableFileFolderPrompt()));
	connect(ui->addFileButton, SIGNAL(clicked()), this, SLOT(addNewFile()));
	connect(ui->removeFileButton, SIGNAL(clicked()), this, SLOT(removeFileFolder()));
	connect(ui->cancelFolderFileRename, SIGNAL(clicked()), this, SLOT(disableFileFolderPrompt()));
	connect(ui->renameFileButton, SIGNAL(clicked()), this, SLOT(showRenameFile()));
	connect(ui->fileRenameEdit, SIGNAL(returnPressed()), this, SLOT(renameFile()));
	connect(ui->okFolderFileRename, SIGNAL(clicked()), this, SLOT(renameFile()));
	// Media Tree
	connect(ui->mediaTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(openSelectedMedia(QTreeWidgetItem*)));
	connect(ui->addFolderMediaButton, SIGNAL(clicked()), this, SLOT(enableMediaFolderPrompt()));
	connect(ui->folderMediaEdit, SIGNAL(returnPressed()), this, SLOT(addNewMediaFolder()));
	connect(ui->cancelFolderMediaButton, SIGNAL(clicked()), this, SLOT(disableMediaFolderPrompt()));
	connect(ui->folderMediaEdit, SIGNAL(inputRejected()), this, SLOT(disableMediaFolderPrompt()));
	connect(ui->okFolderMediaButton, SIGNAL(clicked()), this, SLOT(addNewMediaFolder()));
	connect(ui->addMediaButton, SIGNAL(clicked()), this, SLOT(addNewMedia()));
	connect(ui->removeMediaButton, SIGNAL(clicked()), this, SLOT(removeMediaFolder()));
	connect(ui->cancelFolderMediaRename, SIGNAL(clicked()), this, SLOT(disableMediaFolderPrompt()));
	connect(ui->renameMediaButton, SIGNAL(clicked()), this, SLOT(showRenameMedia()));
	connect(ui->mediaRenameEdit, SIGNAL(returnPressed()), this, SLOT(renameMedia()));
	connect(ui->okFolderMediaRename, SIGNAL(clicked()), this, SLOT(renameMedia()));
	// Note Tree
	connect(ui->noteTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(openSelectedNote(QTreeWidgetItem*)));
	connect(ui->addFolderNoteButton, SIGNAL(clicked()), this, SLOT(enableNoteFolderPrompt()));
	connect(ui->folderNoteEdit, SIGNAL(returnPressed()), this, SLOT(addNewNoteFolder()));
	connect(ui->cancelFolderNoteButton, SIGNAL(clicked()), this, SLOT(disableNoteFolderPrompt()));
	connect(ui->folderNoteEdit, SIGNAL(inputRejected()), this, SLOT(disableNoteFolderPrompt()));
	connect(ui->okFolderNoteButton, SIGNAL(clicked()), this, SLOT(addNewNoteFolder()));
	connect(ui->addNoteButton, SIGNAL(clicked()), this, SLOT(addNewNote()));
	connect(ui->removeNoteButton, SIGNAL(clicked()), this, SLOT(removeNoteFolder()));
	connect(ui->cancelFolderNoteRename, SIGNAL(clicked()), this, SLOT(disableNoteFolderPrompt()));
	connect(ui->renameNoteButton, SIGNAL(clicked()), this, SLOT(showRenameNote()));
	connect(ui->noteRenameEdit, SIGNAL(returnPressed()), this, SLOT(renameNote()));
	connect(ui->okFolderNoteRename, SIGNAL(clicked()), this, SLOT(renameNote()));
	// Project view
	connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(findPerson()));
	connect(ui->findEdit, SIGNAL(returnPressed()), this, SLOT(filterProjectItems()));
	connect(ui->findProjectView, SIGNAL(clicked()), this, SLOT(filterProjectItems()));
	connect(ui->clearProjectFilter, SIGNAL(clicked()), this, SLOT(clearProjectFilter()));
	connect(ui->showNotes, SIGNAL(clicked()), this, SLOT(refreshProjectView()));
	connect(ui->showMedia, SIGNAL(clicked()), this, SLOT(refreshProjectView()));
	connect(ui->showFiles, SIGNAL(clicked()), this, SLOT(refreshProjectView()));
	connect(ui->showEvents, SIGNAL(clicked()), this, SLOT(refreshProjectView()));
	connect(ui->showRelations, SIGNAL(clicked()), this, SLOT(refreshProjectView()));
	connect(ui->projectView, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(interractWithProjectItem(QTreeWidgetItem*)));
	connect(ui->expandAll, SIGNAL(clicked()), ui->projectView, SLOT(expandAll()));
	connect(ui->collapseAll, SIGNAL(clicked()), ui-> projectView, SLOT(collapseAll()));
	// Relations.
	connect(ui->addRelationButton, SIGNAL(clicked()), this, SLOT(addNewRelation()));
	connect(ui->olderGenerationWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editRelation(QTreeWidgetItem*)));
	connect(ui->sameGenerationWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editRelation(QTreeWidgetItem*)));
	connect(ui->youngerGenerationWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editRelation(QTreeWidgetItem*)));
	connect(ui->olderGenerationWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(selectOlderGenerationOnly()));
	connect(ui->sameGenerationWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(selectSameGenerationOnly()));
	connect(ui->youngerGenerationWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(selectYoungerGenerationOnly()));
	connect(ui->removeRelationButton, SIGNAL(clicked()), this, SLOT(removeRelation()));
	connect(ui->promoteButton, SIGNAL(clicked()), this, SLOT(promoteRelation()));
	// Events.
	connect(ui->addEvent, SIGNAL(clicked()), this, SLOT(addNewEvent()));
	connect(ui->eventWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editEvent(QTreeWidgetItem*)));
	connect(ui->removeEvent, SIGNAL(clicked()), this, SLOT(removeEvent()));
	// Tabs
	connect(ui->actionShow_General, SIGNAL(triggered()), this, SLOT(setGeneralTabVisible()));
	connect(ui->actionShow_Media, SIGNAL(triggered()), this, SLOT(setMediaTabVisible()));
	connect(ui->actionShow_Files, SIGNAL(triggered()), this, SLOT(setFileTabVisible()));
	connect(ui->actionShow_Notes, SIGNAL(triggered()), this, SLOT(setNoteTabVisible()));
	connect(ui->actionShow_Events, SIGNAL(triggered()), this, SLOT(setEventTabVisible()));
	connect(ui->actionShow_Relations, SIGNAL(triggered()), this, SLOT(setRelationTabVisible()));
	// help
	connect(ui->actionShow_help, SIGNAL(triggered()), this, SLOT(showHelp()));
	// Update persons info
	connect(ui->titleEditA, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
	connect(ui->titleEditF, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->nameEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->surnameEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->maidenNameEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->placeOfBirthEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->placeOfDeathEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->isAlive, SIGNAL (toggled(bool)), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->genderEdit, SIGNAL(activated(int)), this, SLOT(checkStoreRefreshPersonsInfo()));
    // Export
    connect(ui->actionWebpage_for_a_person, SIGNAL(triggered()), this, SLOT(exportMainHtml()));
    connect(ui->actionWebpage_for_all_persons, SIGNAL(triggered()), this, SLOT(exportHtml()));
    connect(ui->actionPicture_of_close_family, SIGNAL(triggered()), this, SLOT(exportCloseFamilyDiagramDialog()));
    connect(ui->actionFamily_tree_diagram, SIGNAL(triggered()), this, SLOT(exportTreeDiagramDialog()));
    connect(ui->actionRodoc_configuration, SIGNAL(triggered()), this, SLOT(exportTemplates()));
    connect(ui->actionRodoc_project, SIGNAL(triggered()), this, SLOT(exportProject()));
    // Import
    connect(ui->actionRodoc_project_import, SIGNAL(triggered()), this, SLOT(importProject()));
    connect(ui->actionRodoc_templates, SIGNAL(triggered()), this, SLOT(importTemplates()));
    connect(ui->actionRoot_directory, SIGNAL(triggered()), this, SLOT(importRootDir()));
    // Tree view related
    connect(ui->centerTreeView, SIGNAL(clicked()), this, SLOT(fitTreeInView()));
    connect(ui->zoomInTreeView, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(ui->zoomOutTreeView, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(ui->generationsNumberAbove, SIGNAL(valueChanged(int)), this, SLOT(changeGenerationsUp(int)));
    connect(ui->generationsNumberBelow, SIGNAL(valueChanged(int)), this, SLOT(changeGenerationsDown(int)));
    connect(ui->allGenerationsAbove, SIGNAL(toggled(bool)), this, SLOT(allAboveGenerations(bool)));
    connect(ui->allGenerationsBelow, SIGNAL(toggled(bool)), this, SLOT(allBelowGenerations(bool)));
    // State of window
    connect(ui->splitter, SIGNAL(splitterMoved(int, int)), this, SLOT(splitterChanged()));
}


void MainWindow::drawCloseContainer(const std::vector<Relation*>& container,
                                    qreal sizeX, qreal sizeY, qreal topLeftX, qreal topLeftY,
                                    qreal lineX1, qreal lineY1, qreal lineX2, qreal lineY2,
                                    size_t partner, size_t mother, size_t father, int gap,
                                    bool movex, bool movey){
    if(container.size() > 0){
        CellGraphicsItem* sameRels = new CellGraphicsItem(sizeX, sizeY, topLeftX, topLeftY, &linePen_, borderRadius_);
        QGraphicsLineItem* lineItem = new QGraphicsLineItem();
        lineItem->setPen(linePen_);
        lineItem->setLine(lineX1, lineY1, lineX2, lineY2);
        scene->addItem(lineItem);
        scene->addItem(sameRels);
        int index = 0;
        QColor color;
        for(auto&& rel : container){
            if(rel->getId() == partner || rel->getId() == father || rel->getId() == mother) color = promotedColor_;
            else color = standardColor_;
            QStringList lines;
            auto optPerson = FT.getPerson(rel->getTheOtherPerson(FT.getMainPerson()->getId()));
            if(!optPerson){
                continue;
            }
            Person* current = *optPerson;
            lines << QString::fromStdString(rel->getTheOtherPersonName(FT.getMainPerson()->getId()));
            qreal itemTopLeftX = topLeftX + ((sizeX_ + gap) * index * movex) + gap;
            qreal itemTopLeftY = topLeftY + ((sizeY_ + gap) * index * movey) + gap;
            PersonsGraphicsItem* person = new PersonsGraphicsItem(lines, sizeX_, sizeY_, current, itemTopLeftX, itemTopLeftY, color, highlightedColor_, font_,
                                                                  this, &linePen_, &textPen_, borderRadius_);
            scene->addItem(person);
            ++index;
        }
    }
}

void MainWindow::drawCloseFamily(){
    scene->clear();
    if(FT.getMainPerson() == nullptr) return;
    Person* main = FT.getMainPerson();
    QStringList textLines;
    std::vector<Relation*> same;
    std::vector<Relation*> younger;
    std::vector<Relation*> older;
    size_t father = main->getFather();
    size_t mother = main->getMother();
    size_t partner = main->getPartner();
    for(auto&& relId : main->getRelations()){
        auto optRel = FT.getRelation(relId);
        if(optRel){
            Relation* rel = *optRel;
            if(rel->isSameGenration()){
                same.push_back(rel);
            }
            else if(rel->isYoungerGeneration(main->getId())){
                older.push_back(rel);
            }
            else{
                younger.push_back(rel);
            }
        }
    }
    int gap = 10;
    int minimum = 30;
    qreal sizeLeft1 = (older.size() * (sizeX_ + gap) + gap) / 2;
    qreal sizeLeft2 = (younger.size() * (sizeX_ + gap) + gap) / 2;
    qreal sizeLeft = sizeLeft1 < sizeLeft2 ? sizeLeft2 : sizeLeft1;
    qreal sizeUpDown = (same.size() * (sizeY_ + gap)) / 2;
    qreal canvasSizeX = (sizeX_ + 2 * gap + sizeLeft) * 2;
    qreal canvasSizeY = (2 * sizeY_ + 2 * gap + sizeUpDown) * 2;
    scene->setSceneRect(0, 0, canvasSizeX, canvasSizeY);
    qreal middleBoxX = (canvasSizeX - sizeX_) / 2;
    qreal middleBoxY = (canvasSizeY - sizeY_) / 2;
    PersonsGraphicsItem* person = new PersonsGraphicsItem(textLines, sizeX_, sizeY_, main, middleBoxX, middleBoxY, probandColor_, highlightedColor_, font_,
                                                          this, &linePen_, &textPen_, borderRadius_);
    {
        qreal sizeX = sizeX_ + 2 * gap;
        qreal sizeY = (same.size() * (sizeY_ + gap)) + gap;
        qreal topLeftX = middleBoxX - sizeX - minimum;
        qreal topLeftY = middleBoxY + (sizeY_ - sizeY) / 2;
        qreal lineX1 = middleBoxX;
        qreal lineY1 = middleBoxY + sizeY_ / 2;
        qreal lineX2 = topLeftX + sizeX;
        qreal lineY2 = middleBoxY + sizeY_ / 2;
        drawCloseContainer(same, sizeX, sizeY, topLeftX, topLeftY, lineX1, lineY1, lineX2, lineY2, partner, mother, father, gap, false, true);
    }
    {
        qreal sizeX = (older.size() * (sizeX_ + gap)) + gap;
        qreal sizeY = sizeY_ + 2 * gap;
        qreal topLeftX = middleBoxX + (sizeX_ - sizeX) / 2;
        qreal topLeftY = middleBoxY - sizeY - (sizeUpDown == 0 ? minimum : sizeUpDown);
        qreal lineX1 = middleBoxX + sizeX_ / 2;
        qreal lineY1 = middleBoxY;
        qreal lineX2 = middleBoxX + sizeX_ / 2;
        qreal lineY2 = topLeftY + sizeY;
        drawCloseContainer(older, sizeX, sizeY, topLeftX, topLeftY, lineX1, lineY1, lineX2, lineY2, partner, mother, father, gap, true, false);
    }
    {
        qreal sizeX = (younger.size() * (sizeX_ + gap)) + gap;
        qreal sizeY = sizeY_ + 2 * gap;
        qreal topLeftX = middleBoxX + (sizeX_ - sizeX) / 2;
        qreal topLeftY = middleBoxY + sizeY_ + (sizeUpDown == 0 ? minimum : sizeUpDown);;
        qreal lineX1 = middleBoxX + sizeX_ / 2;
        qreal lineY1 = middleBoxY + sizeY_;
        qreal lineX2 = middleBoxX + sizeX_ / 2;
        qreal lineY2 = topLeftY;
        drawCloseContainer(younger, sizeX, sizeY, topLeftX, topLeftY, lineX1, lineY1, lineX2, lineY2, partner, mother, father, gap, true, false);
    }
    scene->addItem(person);
    auto rect = scene->itemsBoundingRect();
    rect.adjust(-gap, -gap, gap, gap);
    scene->setSceneRect(rect);
}

void MainWindow::drawGeneralGenerations(Person* p, qreal size, qreal midx, qreal midy, int horizontalGap, int verticalGap, size_t barrier,
                                        const std::vector<size_t>& container, bool up,
                                        std::function<std::pair<size_t, size_t>(Person*, size_t, size_t)> numberFunc,
                                        std::function<void(Person*, qreal, qreal, qreal, int, int, size_t)> drawFunc){
    std::vector<size_t> widths;
    size_t total = 0;
    for(auto&& relId : container){
        auto optRel = FT.getRelation(relId);
        if(!optRel || barrier == 0) continue;
        Relation* r = (*optRel);
        if(r->isParent(p->getId()) || up){
            auto optPerson = FT.getPerson(r->getTheOtherPerson(p->getId()));
            if(optPerson){
                auto [width, depth] = numberFunc(*optPerson, 0, barrier - 1);
                widths.push_back(width);
                total += width;
            }
        }
    }
    size_t index = 0;
    qreal posx = midx - size / 2;
    for(auto&& relId : container){
        auto optRel = FT.getRelation(relId);
        if(!optRel || barrier == 0) continue;
        Relation* r = (*optRel);
        if(r->isParent(p->getId()) || up){
            auto optPerson = FT.getPerson(r->getTheOtherPerson(p->getId()));
            if(optPerson){
                double div = (double) widths[index]/total;
                qreal currentSize = div * size;
                qreal currentMidX = posx + currentSize / 2;
                qreal currentTopX = currentMidX - sizeX_ / 2;
                qreal currentTopY = up ? midy - sizeY_ - verticalGap : midy + sizeY_ + verticalGap;
                drawFunc(*optPerson, currentSize, currentMidX, currentTopY, horizontalGap, verticalGap, barrier - 1);
                QGraphicsLineItem* lineItem = new QGraphicsLineItem();
                lineItem->setPen(linePen_);
                qreal lineY1 = up ? midy - verticalGap : currentTopY;
                qreal lineY2 = up ? midy : midy + sizeY_;
                lineItem->setLine(currentTopX + sizeX_ / 2, lineY1, midx, lineY2);
                treeScene->addItem(lineItem);
                QStringList lines;
                lines << QString::fromStdString(r->getTheOtherPersonName(p->getId()));
                PersonsGraphicsItem* item = new PersonsGraphicsItem(lines, sizeX_, sizeY_, (*optPerson), currentTopX, currentTopY, standardColor_, highlightedColor_, font_, this,
                                                                    &linePen_, & textPen_, borderRadius_);
                ++index;
                posx += currentSize;
                treeScene->addItem(item);
            }
        }
    }
}

void MainWindow::drawOlderGenerations(Person* p, qreal size, qreal midx, qreal midy, int horizontalGap, int verticalGap, size_t barrier){
    std::vector<size_t> cont;
    if(p->getFather() != 0) cont.push_back(p->getFather());
    if(p->getMother() != 0) cont.push_back(p->getMother());
    drawGeneralGenerations(p, size, midx, midy, horizontalGap, verticalGap, barrier, cont, true,
        [this](Person* p, size_t soFar, size_t barrier){
                           return this->olderGenerationNumber(p, soFar, barrier);
                           },
        [this](Person* p, qreal size, qreal midx, qreal midy, int horizontalGap, int verticalGap, size_t barrier){
            this->drawOlderGenerations(p, size, midx, midy, horizontalGap, verticalGap, barrier);
            });
}

void MainWindow::drawYoungerGenerations(Person* p, qreal size, qreal midx, qreal midy, int horizontalGap, int verticalGap, size_t barrier){
    drawGeneralGenerations(p, size, midx, midy, horizontalGap, verticalGap, barrier, p->getRelations(), false,
        [this](Person* p, size_t soFar, size_t barrier){
                           return this->youngerGenerationNumber(p, soFar, barrier);
                           },
        [this](Person* p, qreal size, qreal midx, qreal midy, int horizontalGap, int verticalGap, size_t barrier){
            this->drawYoungerGenerations(p, size, midx, midy, horizontalGap, verticalGap, barrier);
            });
}

void MainWindow::drawPartner(qreal canvasMidX, qreal canvasMidY, int horizontalGap){
    if(FT.getMainPerson()->getPartner() != 0){
        auto optRel = FT.getRelation(FT.getMainPerson()->getPartner());
        if(optRel){
            auto optPerson = FT.getPerson((*optRel)->getTheOtherPerson(FT.getMainPerson()->getId()));
            if(optPerson){
                QStringList lines;
                lines << QString::fromStdString((*optRel)->getTheOtherPersonName(FT.getMainPerson()->getId()));
                PersonsGraphicsItem* item;
                qreal xTop = canvasMidX - horizontalGap - sizeX_ - sizeX_ / 2;
                qreal yTop = canvasMidY;
                item = new PersonsGraphicsItem(lines, sizeX_, sizeY_, (*optPerson), xTop, yTop, promotedColor_, highlightedColor_, font_, this, &linePen_, &textPen_, borderRadius_);
                QGraphicsLineItem* lineItem = new QGraphicsLineItem();
                lineItem->setPen(linePen_);
                lineItem->setLine(xTop + sizeX_, yTop + sizeY_ / 2, xTop + sizeX_ + horizontalGap, yTop + sizeY_ / 2);
                treeScene->addItem(item);
                treeScene->addItem(lineItem);
            }
        }
    }
}

void MainWindow::drawFamilyTree(){
    treeScene->clear();
    int horizontalGap = 10;
    int verticalGap = 30;
    if(FT.getMainPerson() == nullptr) return;
    size_t barrierUp = genSizeUp_ == -1 ? FT.getPersons().size() : genSizeUp_;
    size_t barrierDown = genSizeDown_ == -1 ? FT.getPersons().size() : genSizeDown_;

    auto [topWidth, height] = olderGenerationNumber(FT.getMainPerson(), 0, barrierUp);
    size_t realTopWidth = topWidth * (sizeX_ + horizontalGap);

    auto [botWidth, depth] = youngerGenerationNumber(FT.getMainPerson(), 0, barrierDown);
    auto realBotWidth = botWidth * (sizeX_ + horizontalGap);

    size_t width = realTopWidth < realBotWidth ? realBotWidth : realTopWidth;
    size_t totalHeight = depth + height + 1;
    qreal canvasSizeX = width;
    qreal canvasSizeY = totalHeight * (2 * (verticalGap) + sizeY_);
    treeScene->setSceneRect(0, 0, canvasSizeX, canvasSizeY);
    qreal canvasMidX = (canvasSizeX) / 2;
    qreal canvasMidY = (canvasSizeY) / 2;
    drawYoungerGenerations(FT.getMainPerson(), realBotWidth, canvasMidX, canvasMidY, horizontalGap, verticalGap, barrierDown);
    drawPartner(canvasMidX, canvasMidY, horizontalGap);
    drawOlderGenerations(FT.getMainPerson(), realTopWidth, canvasMidX, canvasMidY, horizontalGap, verticalGap, barrierUp);

    QStringList lines;
    PersonsGraphicsItem* item = new PersonsGraphicsItem(lines, sizeX_, sizeY_, FT.getMainPerson(), canvasMidX - sizeX_ / 2, canvasMidY, probandColor_, highlightedColor_, font_,
                                                        this, &linePen_, &textPen_, borderRadius_);
    treeScene->addItem(item);

    QRectF rect = treeScene->itemsBoundingRect();
    rect.adjust(- 2*horizontalGap, - 2*verticalGap, 2*horizontalGap, 2*verticalGap);
    treeScene->setSceneRect(rect);
}

void MainWindow::editEvent(size_t id){
    auto event = FT.getEvent(id);
    if(event){
        EventDialog* ed = new EventDialog(&FT, this, *event);
        ed->show();
    }
    else{
        std::stringstream ss;
        ss << "Editing event was not possible due to the absence of given event.";
        FT.log(ss.str());
    }
}

void MainWindow::editRelation(size_t id){
    auto optRel = FT.getRelation(id);
    if(optRel){
        RelationDialog* rd = new RelationDialog(&FT, this, (*optRel));
        rd->show();
    }
    else{
        std::stringstream ss;
        ss << "Editing relation was not possible due to the absence of given relation.";
        FT.log(ss.str());
    }
}

void MainWindow::enableGeneralFolderPrompt(bool visible, FileType type){
    QStackedWidget* widget;
    QLineEdit* folderEdit;
    switch(type){
        case NOTE:
            folderEdit = ui->folderNoteEdit;
            widget = ui->noteStack;
            break;
        case MEDIA:
            folderEdit = ui->folderMediaEdit;
            widget = ui->mediaStack;
            break;
        case GENERAL_FILE:
        default:
            folderEdit = ui->folderFileEdit;
            widget = ui->fileStack;
            break;
    }
    if(visible){
        widget->setCurrentIndex(1);
        folderEdit->setFocus();
        folderEdit->selectAll();
    }
    else{
        widget->setCurrentIndex(0);
    }
}

void MainWindow::enableGeneralFolderRenamePrompt(FileType type){
    QStackedWidget* widget;
    QLineEdit* folderEdit;
    switch(type){
        case NOTE:
            folderEdit = ui->noteRenameEdit;
            widget = ui->noteStack;
            break;
        case MEDIA:
            folderEdit = ui->mediaRenameEdit;
            widget = ui->mediaStack;
            break;
        case GENERAL_FILE:
        default:
            folderEdit = ui->fileRenameEdit;
            widget = ui->fileStack;
            break;
    }
    widget->setCurrentIndex(2);
    folderEdit->setFocus();
    folderEdit->selectAll();
}

bool MainWindow::exportCloseFamilyDiagram(double scale, const std::string& filename){
    QString filepath = QString::fromStdString(filename);
    if(filename == "")
        filepath = QFileDialog::getSaveFileName(this, tr("Select or make new file"), QDir::currentPath(), tr("PNG (*.png)"));
    return exportGeneralDiagram(filepath, scene, scale);
}

bool MainWindow::exportGeneralDiagram(const QString& filename, QGraphicsScene* gscene, double scale){
    if(filename == "") return false;
    QSize size = gscene->sceneRect().size().toSize();
    size.rheight() *= scale;
    size.rwidth() *= scale;
    QImage image(size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    gscene->render(&painter);
    painter.end();
    QImageWriter imgWriter(filename);
    imgWriter.setFormat("png");
    imgWriter.setQuality(100);
    imgWriter.write(image);
    return true;
}

bool MainWindow::exportTreeDiagram(double scale, const std::string& filename){
    QString filepath = QString::fromStdString(filename);
    if(filename == "")
        filepath = QFileDialog::getSaveFileName(this, tr("Select or make new file"), QDir::currentPath(), tr("PNG (*.png)"));
    return exportGeneralDiagram(filepath, treeScene, scale);
}

std::pair<size_t, size_t> MainWindow::generalGenerationNumber(Person* p, size_t soFar, size_t barrier,
                                                              const std::vector<size_t>& container, bool up,
                                                              std::function<std::pair<size_t, size_t>(Person*, size_t, size_t)> func){
    size_t totalWidth = 0;
    size_t totalDepth = soFar;
    for(auto&& relId : container){
        auto optRel = FT.getRelation(relId);
        if(!optRel || barrier == 0) continue;
        Relation* r = (*optRel);
        if(r->isParent(p->getId()) || up){
            auto optPerson = FT.getPerson(r->getTheOtherPerson(p->getId()));
            if(optPerson){
                auto [width, depth] = func(*optPerson, soFar + 1, barrier - 1);
                totalWidth += width;
                totalDepth = depth > totalDepth ? depth : totalDepth;
            }
        }
    }
    if(totalWidth == 0) totalWidth = 1;
    return {totalWidth, totalDepth};
}

void MainWindow::initializeProjectView(){
    ui->projectView->clear();
    for(auto&& [id, person] : FT.getPersons()){
        ProjectItem* item = new ProjectItem(PROJECT_PERSON, id);
        item->setData(0, Qt::DisplayRole, QVariant::fromValue(id));
        item->setText(1, QString::fromStdString(person->str()));
        switch(person->getGender()){
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
        item->setText(2, QString::fromStdString(person->birthDate()->str()));
        if(!person->isAlive()) item->setText(3, QString::fromStdString(person->deathDate()->str()));
        ui->projectView->addTopLevelItem(item);
        initializeProjectSubView(item, person.get());
    }
    for(int i = 0; i < ui->projectView->columnCount() - 1; ++i)
        ui->projectView->resizeColumnToContents(i);

}

void MainWindow::initializeProjectSubView(ProjectItem* parent, Person* person){
    ProjectItem* fileGroup = new ProjectItem(PROJECT_GROUP);
    refreshProjectFilesView(fileGroup, person);
    parent->addChild(fileGroup);

    ProjectItem* mediaGroup = new ProjectItem(PROJECT_GROUP);
    refreshProjectMediaView(mediaGroup, person);
    parent->addChild(mediaGroup);

    ProjectItem* noteGroup = new ProjectItem(PROJECT_GROUP);
    refreshProjectNoteView(noteGroup, person);
    parent->addChild(noteGroup);

    ProjectItem* eventGroup = new ProjectItem(PROJECT_GROUP);
    refreshProjectEventsView(eventGroup, person);
    parent->addChild(eventGroup);

    ProjectItem* relGroup = new ProjectItem(PROJECT_GROUP);
    refreshProjectRelationsView(relGroup, person);
    parent->addChild(relGroup);
}

void MainWindow::loadSettings(){
    switch(FT.getSettings()->getAppSettings().tabPosition){
        case North:
            setTabNorth();
            break;
        case East:
            setTabEast();
            break;
        case West:
            setTabWest();
            break;
        case South:
            setTabSouth();
            break;
    }
    ui->actionShow_Relations->setChecked(FT.getSettings()->getAppSettings().showRelationTab);
    ui->actionShow_Events->setChecked(FT.getSettings()->getAppSettings().showEventTab);
    ui->actionShow_General->setChecked(FT.getSettings()->getAppSettings().showGeneralTab);
    ui->actionShow_Notes->setChecked(FT.getSettings()->getAppSettings().showNoteTab);
    ui->actionShow_Media->setChecked(FT.getSettings()->getAppSettings().showMediaTab);
    ui->actionShow_Files->setChecked(FT.getSettings()->getAppSettings().showFileTab);
    setGeneralTabVisible();
    setRelationTabVisible();
    setEventTabVisible();
    setFileTabVisible();
    setMediaTabVisible();
    setNoteTabVisible();
    Color color = FT.getSettings()->getAppSettings().probandColor;
    probandColor_ = QColor(color.red, color.green, color.blue);
    color = FT.getSettings()->getAppSettings().promoted;
    promotedColor_ = QColor(color.red, color.green, color.blue);
    color = FT.getSettings()->getAppSettings().standard;
    standardColor_ = QColor(color.red, color.green, color.blue);
    color = FT.getSettings()->getAppSettings().highlightedColor;
    highlightedColor_ = QColor(color.red, color.green, color.blue);
    font_ = QFont(QString::fromStdString(FT.getSettings()->getAppSettings().fontFamily), FT.getSettings()->getAppSettings().fontSize);
    sizeX_ = FT.getSettings()->getAppSettings().sizeX;
    sizeY_ = FT.getSettings()->getAppSettings().sizeY;
    genSizeUp_ = FT.getSettings()->getAppSettings().genSizeUp;
    if(genSizeUp_ == -1){
        ui->generationsNumberAbove->setValue(0);
        ui->allGenerationsAbove->setChecked(true);
    }
    else ui->generationsNumberAbove->setValue(genSizeUp_);
    genSizeDown_ = FT.getSettings()->getAppSettings().genSizeDown;
    if(genSizeDown_ == -1){
        ui->generationsNumberBelow->setValue(0);
        ui->allGenerationsBelow->setChecked(true);
    }
    else ui->generationsNumberBelow->setValue(genSizeDown_);
    QList<int> sizes;
    sizes << FT.getSettings()->getAppSettings().splitterPositionOne;
    sizes << FT.getSettings()->getAppSettings().splitterPositionTwo;
    ui->splitter->setSizes(sizes);
    color = FT.getSettings()->getAppSettings().textColor;
    textPen_ = QPen(QColor(color.red, color.green, color.blue));
    color = FT.getSettings()->getAppSettings().lineColor;
    linePen_ = QPen(QColor(color.red, color.green, color.blue));
    linePen_.setWidth(FT.getSettings()->getAppSettings().lineWidth);
    borderRadius_ = FT.getSettings()->getAppSettings().radius;
}

std::pair<size_t, size_t> MainWindow::olderGenerationNumber(Person* p, int soFar, size_t barrier){
    std::vector<size_t> cont;
    if(p->getFather() != 0) cont.push_back(p->getFather());
    if(p->getMother() != 0) cont.push_back(p->getMother());
    return generalGenerationNumber(p, soFar, barrier, cont, true, [this](Person* p, size_t soFar, size_t barrier){
                                   return this->olderGenerationNumber(p, soFar, barrier);
                                   });
}

void MainWindow::openCustomDateDialog(WrappedDate* date){
	DateDialog* dateDialog = new DateDialog(&FT, date, this);
	dateDialog->show();
}

void MainWindow::openExternalFile(const std::string& fileURL){
    bool success = QDesktopServices::openUrl(QUrl(QString::fromStdString(fileURL)));
	if(!success){
        #ifdef _WIN32
            std::system(("start " + fileURL).c_str());
        #else
            std::system(("xdg-open " + fileURL).c_str());
        #endif
	}
}

void MainWindow::openGeneralFile(QTreeWidgetItem* item, FileType type){
    auto temp = dynamic_cast<FileTreeItem*>(item);
	if(temp->isDirectory()){
		return;
	}
	std::string fileUrl;
	FT.getUrlFile(temp->getId(), fileUrl, type);
    openExternalFile(fileUrl);
}

void MainWindow::openGeneralProject(const std::string& dirPath){
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
    std::string errorMessage;
    std::string backupFile;
    FT.clear();
    clearUi();
    auto [success, backup] = FT.openDatabase(dirPath, errorMessage, backupFile);
    if(backup){
        QMessageBox::StandardButton result;
        std::stringstream ss;
        ss << "There was found a backup file: " << backupFile << ". Do you want to restore it? If not, the backup will be deleted.";
        result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes) {
            success = FT.restoreBackup(backupFile, errorMessage);
        }
        else{
            FT.removeBackup();
        }
    }
    if(!success){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::CORRUPTED_DATABASE) + QString::fromStdString(errorMessage));
        return;
    }
    auto [problem, str] = FT.checkFileConsistence();
    if(problem) QMessageBox::warning(this, "Warning", QString::fromStdString(str), QMessageBox::Ok);
    loadSettings();
    refreshUi();
    initializeProjectView();
}

void MainWindow::refreshPersonsInfoTab(){
    Person* main = FT.getMainPerson();
    if(main == nullptr) return;
    disconnect(ui->titleEditA, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
	disconnect(ui->titleEditF, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    disconnect(ui->nameEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    disconnect(ui->surnameEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    disconnect(ui->maidenNameEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    disconnect(ui->placeOfBirthEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    disconnect(ui->placeOfDeathEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    disconnect(ui->isAlive, SIGNAL (toggled(bool)), this, SLOT(checkStoreRefreshPersonsInfo()));
    disconnect(ui->genderEdit, SIGNAL(activated(int)), this, SLOT(checkStoreRefreshPersonsInfo()));
    ui->currentPerson->setText(QString::fromStdString(main->str()));
    ui->titleEditF->setText(QString::fromStdString(main->getFrontTitle()));
	ui->titleEditA->setText(QString::fromStdString(main->getAfterTitle()));
	ui->nameEdit->setText(QString::fromStdString(main->getName()));
	ui->surnameEdit->setText(QString::fromStdString(main->getSurname()));
	ui->maidenNameEdit->setText(QString::fromStdString(main->getMaidenName()));
	ui->genderEdit->setCurrentIndex(main->getGender());
	ui->placeOfBirthEdit->setText(QString::fromStdString(main->getBirthPlace()));
	ui->isAlive->setChecked(main->isAlive());
    setEnabledDeathAtributes(main->isAlive());
	ui->placeOfDeathEdit->setText(QString::fromStdString(main->getDeathPlace()));
    refreshDates();
    connect(ui->titleEditA, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
	connect(ui->titleEditF, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->nameEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->surnameEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->maidenNameEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->placeOfBirthEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->placeOfDeathEdit, SIGNAL(editingFinished()), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->isAlive, SIGNAL (toggled(bool)), this, SLOT(checkStoreRefreshPersonsInfo()));
    connect(ui->genderEdit, SIGNAL(activated(int)), this, SLOT(checkStoreRefreshPersonsInfo()));
}

void MainWindow::refreshUi(){
    disableFileFolderPrompt();
    disableMediaFolderPrompt();
    disableNoteFolderPrompt();
    ui->fileTreeWidget->clear();
	ui->mediaTreeWidget->clear();
    ui->noteTreeWidget->clear();
    if(FT.getMainPerson() == nullptr){
        ui->infoTab->setEnabled(false);
		return;
	}
	FT.getSettings()->setGlobalMainPerson(FT.getMainPerson()->getId());
	ui->infoTab->setEnabled(true);
	refreshPersonsInfoTab();
	refreshGraphics();
	refreshTabsWithFiles();
	refreshTagsList();
	refreshProjectView();
    refreshRelationTab();
	refreshEventTab();
}

void MainWindow::refreshProjectEventsView(ProjectItem* item, Person* person){
    if(!ui->showEvents->isChecked()) item->setHidden(true);
    else item->setHidden(false);
    item->setIcon(0, QIcon(":/resources/event.svg"));
    item->setText(1, "Events");
    for(int i = 0; i < item->childCount(); )
        delete item->child(i);
    for(auto&& id : person->getEvents()){
        ProjectItem* subitem = new ProjectItem(PROJECT_EVENTS, id);
        subitem->setText(0, QString::fromStdString(std::to_string(id)));
        auto optEvent = FT.getEvent(id);
        if(!optEvent){
            delete subitem;
            continue;
        }
        subitem->setText(1, QString::fromStdString((*optEvent)->str()));
        subitem->setIcon(0, QIcon(":/resources/event.svg"));
        item->addChild(subitem);
    }
}

void MainWindow::refreshProjectGeneralFilesView(const QIcon& containerIcon, const QString& containerName, const QIcon& fileIcon, FileType type, ProjectItem* item, Person* person){
    ProjectItemType pType;
    QCheckBox* box;
    switch(type){
        case MEDIA:
            pType = PROJECT_MEDIA;
            box = ui->showMedia;
            break;
        case NOTE:
            pType = PROJECT_NOTE;
            box = ui->showNotes;
            break;
        case GENERAL_FILE:
        default:
            pType = PROJECT_FILE;
            box = ui->showFiles;
            break;
    }
    if(!box->isChecked()) item->setHidden(true);
    else item->setHidden(false);
    item->setIcon(0, containerIcon);
    item->setText(1, containerName);
    for(int i = 0; i < item->childCount(); )
        delete item->child(i);
    std::vector<size_t> files;
    person->getFilesRoot(type).usedFiles(files);
    for(auto&& id : files){
        ProjectItem* subitem = new ProjectItem(pType, id);
        subitem->setText(0, QString::fromStdString(std::to_string(id)));
        auto optFile = FT.getFile(id, type);
        if(!optFile){
            delete subitem;
            continue;
        }
        subitem->setText(1, QString::fromStdString((*optFile)->getFilename()));
        subitem->setIcon(0, fileIcon);
        item->addChild(subitem);
    }
}

void MainWindow::refreshProjectFilesView(ProjectItem* item, Person* person){
    refreshProjectGeneralFilesView(QIcon(":/resources/folder.svg"), "Files", QIcon(":/resources/file.svg"), GENERAL_FILE, item, person);
}


void MainWindow::refreshProjectMediaView(ProjectItem* item, Person* person){
    refreshProjectGeneralFilesView(QIcon(":/resources/mediaFolder.svg"), "Media", QIcon(":/resources/media.svg"), MEDIA, item, person);
}

void MainWindow::refreshProjectNoteView(ProjectItem* item, Person* person){
    refreshProjectGeneralFilesView(QIcon(":/resources/folder.svg"), "Notes", QIcon(":/resources/note.svg"), NOTE, item, person);
}

void MainWindow::refreshProjectRelationsView(ProjectItem* item, Person* person){
    if(!ui->showRelations->isChecked()) item->setHidden(true);
    else item->setHidden(false);
    item->setIcon(0, QIcon(":/resources/relation.svg"));
    item->setText(1, "Relations");
    for(int i = 0; i < item->childCount(); )
        delete item->child(i);
    for(auto&& id : person->getRelations()){
        ProjectItem* subitem = new ProjectItem(PROJECT_RELATIONS, id);
        subitem->setText(0, QString::fromStdString(std::to_string(id)));
        auto optRelation = FT.getRelation(id);
        if(!optRelation){
            delete subitem;
            continue;
        }
        size_t firstPerson = (*optRelation)->getFirstPerson();
        size_t secondPerson = (*optRelation)->getSecondPerson();
        auto optPers1 = FT.getPerson(firstPerson);
        auto optPers2 = FT.getPerson(secondPerson);
        if(!optPers1 || !optPers2){
            delete subitem;
            continue;
        }
        subitem->setText(1, QString::fromStdString((*optRelation)->str((*optPers1)->str(), (*optPers2)->str())));
        subitem->setIcon(0, QIcon(":/resources/relation.svg"));
        item->addChild(subitem);
    }
}

void MainWindow::removeGeneralFolder(FileType type){
    QTreeWidget* widget;
    switch(type){
        case MEDIA:
            widget = ui->mediaTreeWidget;
            break;
        case NOTE:
            widget = ui->noteTreeWidget;
            break;
        case GENERAL_FILE:
        default:
            widget = ui->fileTreeWidget;
            break;
    }
	auto item = widget->currentItem();
	if(item == nullptr || FT.getMainPerson() == nullptr) return;
	QMessageBox::StandardButton result;
    std::stringstream ss;
    ss << "Following folder: " << item->text(0).toStdString() << " and all of its subfolders and files will be deleted from a person (it will still be present in the database). Are you sure?";
    result = QMessageBox::warning(this, "Warning", QString::fromStdString(ss.str()), QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        auto fileItem = dynamic_cast<FileTreeItem*>(item);
        bool removeCurrent = fileItem->removeFolder();
        if(removeCurrent) delete fileItem;
    }
    FT.setUnsaved();
    refreshProjectView();
}

void MainWindow::renameGeneralFileItem(FileType type){
    QTreeWidget* widget;
    QLineEdit* folderEdit;
    switch(type){
        case NOTE:
            folderEdit = ui->noteRenameEdit;
            widget = ui->noteTreeWidget;
            break;
        case MEDIA:
            folderEdit = ui->mediaRenameEdit;
            widget = ui->mediaTreeWidget;
            break;
        case GENERAL_FILE:
        default:
            folderEdit = ui->fileRenameEdit;
            widget = ui->fileTreeWidget;
            break;
    }
    if(folderEdit->text().isEmpty()){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::EMPTY_BOXES));
		return;
    }
    if(containsForb(folderEdit->text())){
        QMessageBox::critical(this, "Error", QString::fromStdString(error::FORBIDDEN_CHARS));
		folderEdit->selectAll();
		return;
    }
	auto item = widget->currentItem();
	if(FT.getMainPerson() == nullptr) return;
	if(item == nullptr){
        QMessageBox::critical(this, "Error", "Select a folder or file which will be renamed.");
        return;
	}
    auto file_item = dynamic_cast<FileTreeItem*>(item);
    bool fileOnly = file_item->rename(folderEdit->text());
    if(fileOnly){
        FT.renameFile(file_item->getId(), folderEdit->text().toStdString(), type);
        file_item->setText(0, folderEdit->text());
    }
    enableGeneralFolderPrompt(false, type);
    refreshTabsWithFiles();
    FT.setUnsaved();
}

std::pair<int, int> MainWindow::setNumberOfGenerations(int up, int down){
    int tmpUp = genSizeUp_;
    int tmpDown = genSizeDown_;
    genSizeDown_ = down;
    genSizeUp_ = up;
    refreshGraphics();
    return {tmpUp, tmpDown};
}

void MainWindow::savePersonsInfo(){
    if(FT.getMainPerson() == nullptr) return;
    Person* main = FT.getMainPerson();
    if(main->getFrontTitle() != ui->titleEditF->text().toStdString()){
        main->setFrontTitle(ui->titleEditF->text().toStdString());
        FT.setUnsaved();
    }
    if(main->getAfterTitle() != ui->titleEditA->text().toStdString()){
        main->setAfterTitle(ui->titleEditA->text().toStdString());
        FT.setUnsaved();
    }
    if(main->getName() != ui->nameEdit->text().toStdString()){
        main->setName() = ui->nameEdit->text().toStdString();
        FT.setUnsaved();
    }
    if(main->getSurname() != ui->surnameEdit->text().toStdString()){
        main->setSurname() = ui->surnameEdit->text().toStdString();
        FT.setUnsaved();
    }
    if(main->getMaidenName() != ui->maidenNameEdit->text().toStdString()){
        main->setMaidenName() = ui->maidenNameEdit->text().toStdString();
        FT.setUnsaved();
    }
    if(main->getGender() != Gender(ui->genderEdit->currentIndex())){
        main->setGender(Gender(ui->genderEdit->currentIndex()));
        FT.setUnsaved();
    }
    if(main->getBirthPlace() != ui->placeOfBirthEdit->text().toStdString()){
        main->setBirthPlace() = ui->placeOfBirthEdit->text().toStdString();
        FT.setUnsaved();
    }
    if(main->isAlive() != ui->isAlive->isChecked()){
        main->setLives(ui->isAlive->isChecked());
        FT.setUnsaved();
    }
    if(!main->isAlive() && main->getDeathPlace() != ui->placeOfDeathEdit->text().toStdString()){
        main->setDeathPlace() = ui->placeOfDeathEdit->text().toStdString();
        FT.setUnsaved();
    }
}

void MainWindow::setUiElements(){
    for(auto&& g : gender::AllGenders){
        ui->genderEdit->addItem(QString::fromStdString(gender::str(g)));
    }
    QStringList tagHeader;
    tagHeader << "Tag" << "Value";
    ui->tagWidget->setHeaderLabels(tagHeader);
    QStringList personHeader;
    personHeader << "ID" << "Name" << "Birth date" << "Death date";
    ui->projectView->setHeaderLabels(personHeader);
    QStringList relationHeader;
    relationHeader << "Relation" << "Other person";
    ui->olderGenerationWidget->setHeaderLabels(relationHeader);
    ui->sameGenerationWidget->setHeaderLabels(relationHeader);
    ui->youngerGenerationWidget->setHeaderLabels(relationHeader);
    QStringList eventHeader;
    eventHeader << "Event" << "Date" << "Note";
    ui->eventWidget->setHeaderLabels(eventHeader);
}

void MainWindow::showGeneralFiles(QTreeWidget* widget, VirtualDrive* root, FileType type){
    QIcon icon;
    switch(type){
        case MEDIA:
            icon = QIcon(":/resources/media.svg");
            break;
        case NOTE:
            icon = QIcon(":/resources/note.svg");
            break;
        default:
            icon = QIcon(":/resources/file.svg");
            break;
    }
    widget->clear();
    widget->addTopLevelItem(new FileTreeItem(root, &FT, icon, type));
    widget->expandAll();
}

std::pair<size_t, size_t> MainWindow::youngerGenerationNumber(Person* p, size_t soFar, size_t barrier){
    return generalGenerationNumber(p, soFar, barrier, p->getRelations(), false, [this](Person* p, size_t soFar, size_t barrier){
                                   return this->youngerGenerationNumber(p, soFar, barrier);
                                   });
}
