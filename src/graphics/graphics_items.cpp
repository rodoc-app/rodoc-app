/// @file graphics_items.cpp This files contains implementation of all graphics items, that are inherited from Qt framework.
#include "graphics_items.h"

bool containsForb(const QString& value){
    char forb[] = {'<', '>', '\"'};
    for(auto&& c : forb)
        if(value.contains(c)) return true;
    return false;
}

// =====================================================================
// FileTreeItem
// =====================================================================

FileTreeItem::FileTreeItem(VirtualDrive* drive, FamilyTree* FT, const QIcon& icon, FileType type)
   : drive_(drive), fileId_(0), FT_(FT), isDir_(true), type_(type){
    setIcon(0, QIcon(":/resources/folder.svg"));
    setText(0, QString::fromStdString(drive->getName()));
    for(auto it = drive->getSubdrives().begin(); it != drive->getSubdrives().end(); ++it){
        VirtualDrive* subdrive = (*it).get();
        addChild(new FileTreeItem(subdrive, FT, icon, type));
    }
    for(auto&& fileId : drive->getFiles())
        addChild(new FileTreeItem(fileId, FT_, icon, type));
}

FileTreeItem::FileTreeItem(size_t id, FamilyTree* FT, const QIcon& icon, FileType type)
   : drive_(nullptr), fileId_(id), FT_(FT), isDir_(false), type_(type){
    setIcon(0, icon);
    auto optFile = FT->getFile(id, type);
    if(optFile){
        setText(0, QString::fromStdString(((*optFile)->getFilename())));
        setIcon(0, icon);
    }
    else setText(0, "Missing file.");
}

void FileTreeItem::addFile(size_t id, const QIcon& icon){
    if(!isDir_){
        auto parentFile = dynamic_cast<FileTreeItem*>(parent());
        parentFile->addFile(id, icon);
        return;
    }
    drive_->addFile(id);
    addChild(new FileTreeItem(id, FT_, icon, type_));
}

void FileTreeItem::addSubDrive(const QString& folder){
    if(!isDir_){
        auto parentFile = dynamic_cast<FileTreeItem*>(parent());
        parentFile->addSubDrive(folder);
        return;
    }
    auto subdrive = drive_->addSubdrive(folder.toStdString());
    QIcon icon;
    switch(type_){
        case MEDIA:
            icon = QIcon(":/resources/media.svg");
            break;
        case NOTE:
            icon = QIcon(":/resources/note.svg");
            break;
        case GENERAL_FILE:
        default:
            icon = QIcon(":/resources/file.svg");
            break;
    }
    addChild(new FileTreeItem(subdrive, FT_, icon, type_));
    FT_->setUnsaved();
}

VirtualDrive* FileTreeItem::getDrive(){
    return drive_;
}

size_t FileTreeItem::getId(){
	return fileId_;
}

bool FileTreeItem::isDirectory(){
	return isDir_;
}

void FileTreeItem::removeFile(size_t id){
    drive_->removeFile(id);
}

bool FileTreeItem::removeFolder(){
    if(!isDir_){
        auto parentItem = dynamic_cast<FileTreeItem*>(parent());
        parentItem->removeFile(fileId_);
        return true;
    }
    for(int i = 0; i < childCount();){
        auto childItem = dynamic_cast<FileTreeItem*>(child(i));
        if(childItem->isDirectory()){
            drive_->remove(childItem->text(0).toStdString());
        }
        else{
            drive_->removeFile(childItem->getId());
        }
        delete childItem;
    }
    const auto parentItem = dynamic_cast<FileTreeItem*>(parent());
    if(parentItem == nullptr) return false;
    parentItem->getDrive()->remove(text(0).toStdString());
    return true;
}

bool FileTreeItem::rename(const QString& name){
    if(!isDir_) return true;
    drive_->rename(name.toStdString());
    setText(0, name);
    return false;
}

// =====================================================================
// ProjectItem
// =====================================================================

ProjectItem::ProjectItem(ProjectItemType type, size_t id) : id_(id), type_(type){}

size_t ProjectItem::getId() const{
    return id_;
}

ProjectItemType ProjectItem::type() const{
    return type_;
}

// =====================================================================
// EventRoleItem
// =====================================================================

EventRoleItem::EventRoleItem(const std::string& role, size_t id) : id_(id){
    setText(0, QString::fromStdString(role));
}

size_t EventRoleItem::getId(){
    return id_;
}

void EventRoleItem::setPerson(const QString& name, size_t id){
    setText(1, name);
    id_ = id;
}

// =====================================================================
// PersonsGraphicsItem
// =====================================================================

PersonsGraphicsItem::PersonsGraphicsItem(const QStringList& lines, int sizex, int sizey, Person* p, qreal x, qreal y,
                                         QColor color, QColor highlighted, QFont font, QWidget* parent, QPen* pen, QPen* textPen, int borderRadius)
  : borderRadius_(borderRadius), color_(color), font_(font), highlighted_(highlighted), hovered_(false), pen_(pen),
  person_(p), sizex_(sizex), sizey_(sizey), textLines_(lines), textPen_(textPen), x_(x), y_(y){
    setPos(x_, y_);
    setAcceptHoverEvents(true);
    bold_text_ = lines.length();
    if(person_ != nullptr){
        textLines_ << QString::fromStdString(person_->str()) << QString::fromStdString("Birth: " + person_->birthDate()->str());
        if(!person_->isAlive()) textLines_ << QString::fromStdString("Death: " + person_->deathDate()->str());
    }
    if(p != nullptr) connect(this, SIGNAL(itemClicked(size_t)), parent, SLOT(setMainPerson(size_t)));
}

PersonsGraphicsItem::~PersonsGraphicsItem(){
    disconnect(this, SIGNAL(itemClicked(size_t)), 0, 0);
}

QRectF PersonsGraphicsItem::boundingRect() const{
    return QRectF(0, 0, sizex_, sizey_);
}

void PersonsGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    painter->setPen(*pen_);
    QBrush brush;
    if (hovered_) brush = QBrush(highlighted_);
    else brush = QBrush(color_);
    painter->setBrush(brush);
    painter->drawRoundedRect(rect, borderRadius_, borderRadius_);
    painter->setFont(font_);
    qreal lineHeight = boundingRect().height() / (textLines_.size() + 1);
    qreal middle = lineHeight / 2;
    painter->setPen(*textPen_);
    for (int i = 0; i < textLines_.size(); ++i) {
        QRectF textRect = boundingRect();
        if(bold_text_ == i){
            font_.setBold(true);
            painter->setFont(font_);
        }
        textRect.setY(i * lineHeight + middle);
        painter->drawText(textRect, Qt::AlignHCenter, textLines_[i]);
        if(bold_text_ == i){
            font_.setBold(false);
            painter->setFont(font_);
        }
    }
}

void PersonsGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event){
    Q_UNUSED(event);
    if(person_ == nullptr) return;
    emit itemClicked(person_->getId());
    QCoreApplication::processEvents();
}

QPainterPath PersonsGraphicsItem::shape() const{
    QPainterPath path;
    path.addRoundedRect(boundingRect(), 10, 10);
    return path;
}

void PersonsGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    hovered_ = true;
    update(boundingRect());
}

void PersonsGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    hovered_ = false;
    update(boundingRect());
}

// =====================================================================
// CellGraphicsItem
// =====================================================================

CellGraphicsItem::CellGraphicsItem(int sizex, int sizey, qreal x, qreal y, QPen* pen, int borderRadius)
  : borderRadius_(borderRadius), pen_(pen), sizex_(sizex), sizey_(sizey), x_(x), y_(y){
    setPos(x_, y_);
}

QRectF CellGraphicsItem::boundingRect() const{
    return QRectF(0, 0, sizex_, sizey_);
}

void CellGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    painter->setPen(*pen_);
    QBrush brush = QBrush(QColor(Qt::transparent));
    painter->setBrush(brush);
    painter->drawRoundedRect(rect, borderRadius_, borderRadius_);
}

QPainterPath CellGraphicsItem::shape() const{
    QPainterPath path;
    path.addRoundedRect(boundingRect(), 10, 10);
    return path;
}

// =====================================================================
// GraphicsView
// =====================================================================

GraphicsView::GraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent){
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setMouseTracking(true);
    setAcceptDrops(false);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void GraphicsView::wheelEvent(QWheelEvent* event){
    double scaleFactor = 1.05;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    }
    else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

// =====================================================================
// RelationItem
// =====================================================================

RelationItem::RelationItem(FamilyTree* FT, const RelationSuggestion& suggestion, RelationTemplate* templ)
  : FT_(FT), relation_(FT->getSettings()){
    auto optLocked = FT->getPerson(suggestion.lockedPerson);
    auto optSecond = FT->getPerson(suggestion.suggestedPerson);
    if(!optLocked || !optSecond) return;
    relation_.setTemplate(templ->getId());
    if(suggestion.symetrical){
        relation_.setPersons((*optLocked)->getId(), (*optSecond)->getId());
    }
    else if(suggestion.parent){
        if(templ->getGenerationDifference() < 0)
            relation_.setPersons((*optLocked)->getId(), (*optSecond)->getId());
        else
            relation_.setPersons((*optSecond)->getId(), (*optLocked)->getId());
    }
    else{
        if(templ->getGenerationDifference() > 0)
            relation_.setPersons((*optLocked)->getId(), (*optSecond)->getId());
        else
            relation_.setPersons((*optSecond)->getId(), (*optLocked)->getId());
    }
    refreshTexts();
}

Relation* RelationItem::getRelation(){
    return &relation_;
}

void RelationItem::refreshTexts(){
    auto optLocked = FT_->getPerson(relation_.getFirstPerson());
    auto optSecond = FT_->getPerson(relation_.getSecondPerson());
    if(!optLocked || !optSecond) return;
    setText(0, QString::fromStdString(relation_.getTheOtherPersonName((*optSecond)->getId())));
    setText(1, QString::fromStdString((*optLocked)->str()));
    setText(2, QString::fromStdString(relation_.getTheOtherPersonName((*optLocked)->getId())));
    setText(3, QString::fromStdString((*optSecond)->str()));
    setText(4, QString::fromStdString(trait::str(relation_.getTrait())));
    setText(5, QString::fromStdString(relation_.getTitle()));
}
